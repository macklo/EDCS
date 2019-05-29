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
    public partial class ServerWindow : Form
    {
        SimpleServer server;
        public ServerWindow()
        {
            InitializeComponent();
            server = new SimpleServer();
        }

        private void ServerWindow_Load(object sender, EventArgs e)
        {
            server.Start();
            Thread t = new Thread(server.Listen);
            t.Start();
        }

        public void setMessageBoxText(string msg)
        {
            messageTextBox.AppendText(msg);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            setMessageBoxText(server.data);
        }
    }
}
