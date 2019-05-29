using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MessageSystem
{
    public partial class Form1 : Form
    {
        SimpleServer server;
        Client client;
        ServerWindow serverWindow;
        public Form1()
        {
            InitializeComponent();
            server = new SimpleServer();
            client = new Client();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            serverWindow = new ServerWindow();
            serverWindow.Show();
        }

        private void sendMessageButton_Click(object sender, EventArgs e)
        {
            Thread t1 = new Thread(client.StartClient);
            t1.Start();
        }
    }
}
