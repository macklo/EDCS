using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MessageSystem
{
    public partial class AddUserWindow : Form
    {
        private string userName;
        private string ipAddress;
        private MainWindow mainWindow;

        public AddUserWindow(MainWindow mainWindow)
        {
            InitializeComponent();
            this.mainWindow = mainWindow;
        }

        private void usernameTextBox_TextChanged(object sender, EventArgs e)
        {
            userName = usernameTextBox.Text;
        }

        private void addUserButton_Click(object sender, EventArgs e)
        {
            mainWindow.addClient(userName, ipAddress);
            this.DestroyHandle();
        }

        private void ipAddressTextBox_TextChanged(object sender, EventArgs e)
        {
            ipAddress = ipAddressTextBox.Text;
        }
    }
}
