﻿using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Json;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace MessageSystem
{
    public partial class MainWindow : Form
    {
        Listener listener;
        Client client;
        //ServerWindow serverWindow;
        AddUserWindow addUserWindow;
        List<Contact> contacts;
        string ipAddress;
        private Contact currentContact;

        public MainWindow()
        {
            InitializeComponent();
            listener = new Listener();
            client = new Client(this);
            contacts = new List<Contact>();

            listener.Start(this);
            Thread t = new Thread(listener.Listen);
            t.Start();

            this.ipAddress = listener.ipAddress;
        }

        public void addClient(string userName, string ipAddress)
        {
            contacts.Add(new Contact(userName, ipAddress));

            List<string> str = contacts.Select(s => s.name).ToList();
            userListBox.DataSource = contacts.Select(s => s.name).ToList();
        }

        private void MainWindow_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            //serverWindow = new ServerWindow();
            //serverWindow.Show();
        }

        private void sendMessageButton_Click(object sender, EventArgs e)
        {
            if (currentContact.isConnected)
            {
                string receiverIpAddress = contacts.ElementAt(userListBox.SelectedIndex).ipAddress;
                Message msg = new Message(messageTextBox.Text, receiverIpAddress, ipAddress, alertCheckBox.Checked);
                string message = msg.getJsonString();
                Thread t1 = new Thread(() => client.sendMessage(message + "<EOF>", receiverIpAddress));
                t1.Start();
                addToChat("You: " + messageTextBox.Text);
            }
        }

        private void messageTextBox_TextChanged(object sender, EventArgs e)
        {
            //client.message = messageTextBox.Text + "<EOF>";
        }

        private void chatTextBox_TextChanged(object sender, EventArgs e)
        {

        }

        private void userListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (currentContact != null) { 
                currentContact.messages = chatTextBox.Text;
            }
            currentContact = contacts.ElementAt(userListBox.SelectedIndex);
            chatTextBox.Text = currentContact.messages;
            if (!currentContact.isConnected && !currentContact.tested)
            {
                currentContact.tested = true;
                sendTestMessage(currentContact.ipAddress);
            }

            if (!currentContact.isConnected)
            {
                sendMessageButton.Enabled = false;
            }
            else
            {
                sendMessageButton.Enabled = true;
            }
        }

        private void addUserButton_Click(object sender, EventArgs e)
        {
            addUserWindow = new AddUserWindow(this);
            addUserWindow.Show();
        }

        private void deleteUserButton_Click(object sender, EventArgs e)
        {
            int idx = userListBox.SelectedIndex;
            contacts.RemoveAt(idx);
            userListBox.DataSource = contacts.Select(s => s.name).ToList();
        }

        private void exportUsersButton_Click(object sender, EventArgs e)
        {
            FileStream fs = File.Create("./contacts.txt");

            DataContractJsonSerializer ds = new DataContractJsonSerializer(typeof(List<Contact>));
            ds.WriteObject(fs, contacts);

            fs.Close();
        }

        private void importUsersButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                FileStream fs = File.OpenRead(openFileDialog.FileName);
                DataContractJsonSerializer ds = new DataContractJsonSerializer(typeof(List<Contact>));
                contacts = ds.ReadObject(fs) as List<Contact>;

                userListBox.DataSource = contacts.Select(s => s.name).ToList();
                fs.Close();
            }
                
        }
        delegate void addToChatCallback(string text);

        public void addToChat(string message)
        {
            if (this.chatTextBox.InvokeRequired)
            {
                addToChatCallback d = new addToChatCallback(addToChat);
                this.Invoke(d, new object[] { message });
            } else
            {
                currentContact.messages = currentContact.messages + message + "\n";
                chatTextBox.Text = currentContact.messages;
            }
        }

        delegate void addMessageCallback(Message message, bool showUser);

        public void addMessage(Message message, bool showUser = true)
        {
            if (this.chatTextBox.InvokeRequired)
            {
                addMessageCallback d = new addMessageCallback(addMessage);
                this.Invoke(d, new object[] { message, showUser });
            }
            else
            {
                if (message.message != "")
                {
                    string senderIpAddress = message.senderAddress;
                    Contact user = contacts.Where(s => s.ipAddress.CompareTo(senderIpAddress) == 0).ToList().ElementAt(0);
                    string label = showUser ? user.name + ": " : "";
                    user.messages += label + message.message + "\n";
                    if (user == currentContact)
                    {
                        chatTextBox.Text = currentContact.messages;
                    }
                }
            }
        }

        public void addLocalMessage(string text, string receiverIpAddress)
        {
            addMessage(new Message(text, ipAddress, receiverIpAddress, false, false), false);
        }

        public void sendTestMessage(string receiverIpAddress)
        {
            addLocalMessage("Trying to connect", receiverIpAddress);
            Message msg = new Message("", receiverIpAddress, ipAddress, false, true);
            string message = msg.getJsonString();
            Thread t1 = new Thread(() => client.sendMessage(message + "<EOF>", receiverIpAddress, true));
            t1.Start();
        }

        public void recievedTestMessage(String msg)
        {
            MemoryStream ms = new MemoryStream(Encoding.ASCII.GetBytes(msg));
            DataContractJsonSerializer ds = new DataContractJsonSerializer(typeof(Message));

            Message msgObject = ds.ReadObject(ms) as Message;

            string senderIpAddress = msgObject.senderAddress;

            Contact user = contacts.Where(s => s.ipAddress.CompareTo(senderIpAddress) == 0).ToList().ElementAt(0);
            user.isConnected = true;

            addLocalMessage("Connected", senderIpAddress);
        }
    }
}
