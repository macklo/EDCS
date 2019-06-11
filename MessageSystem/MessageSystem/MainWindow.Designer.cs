namespace MessageSystem
{
    partial class MainWindow
    {
        /// <summary>
        /// Wymagana zmienna projektanta.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Wyczyść wszystkie używane zasoby.
        /// </summary>
        /// <param name="disposing">prawda, jeżeli zarządzane zasoby powinny zostać zlikwidowane; Fałsz w przeciwnym wypadku.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Kod generowany przez Projektanta formularzy systemu Windows

        /// <summary>
        /// Metoda wymagana do obsługi projektanta — nie należy modyfikować
        /// jej zawartości w edytorze kodu.
        /// </summary>
        private void InitializeComponent()
        {
            this.sendMessageButton = new System.Windows.Forms.Button();
            this.chatTextBox = new System.Windows.Forms.RichTextBox();
            this.messageTextBox = new System.Windows.Forms.RichTextBox();
            this.userListBox = new System.Windows.Forms.ListBox();
            this.addUserButton = new System.Windows.Forms.Button();
            this.deleteUserButton = new System.Windows.Forms.Button();
            this.exportUsersButton = new System.Windows.Forms.Button();
            this.importUsersButton = new System.Windows.Forms.Button();
            this.alertCheckBox = new System.Windows.Forms.CheckBox();
            this.checkConnectionButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // sendMessageButton
            // 
            this.sendMessageButton.Location = new System.Drawing.Point(531, 415);
            this.sendMessageButton.Name = "sendMessageButton";
            this.sendMessageButton.Size = new System.Drawing.Size(87, 23);
            this.sendMessageButton.TabIndex = 1;
            this.sendMessageButton.Text = "Send Message";
            this.sendMessageButton.UseVisualStyleBackColor = true;
            this.sendMessageButton.Click += new System.EventHandler(this.sendMessageButton_Click);
            // 
            // chatTextBox
            // 
            this.chatTextBox.Location = new System.Drawing.Point(12, 12);
            this.chatTextBox.Name = "chatTextBox";
            this.chatTextBox.Size = new System.Drawing.Size(402, 394);
            this.chatTextBox.TabIndex = 3;
            this.chatTextBox.Text = "";
            this.chatTextBox.TextChanged += new System.EventHandler(this.chatTextBox_TextChanged);
            // 
            // messageTextBox
            // 
            this.messageTextBox.Location = new System.Drawing.Point(12, 415);
            this.messageTextBox.Name = "messageTextBox";
            this.messageTextBox.Size = new System.Drawing.Size(513, 23);
            this.messageTextBox.TabIndex = 4;
            this.messageTextBox.Text = "";
            this.messageTextBox.TextChanged += new System.EventHandler(this.messageTextBox_TextChanged);
            this.messageTextBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.messageTextBox_KeyDown);
            this.messageTextBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.messageTextBox_KeyPress);
            // 
            // userListBox
            // 
            this.userListBox.FormattingEnabled = true;
            this.userListBox.Location = new System.Drawing.Point(457, 12);
            this.userListBox.Name = "userListBox";
            this.userListBox.Size = new System.Drawing.Size(161, 394);
            this.userListBox.TabIndex = 5;
            this.userListBox.SelectedIndexChanged += new System.EventHandler(this.userListBox_SelectedIndexChanged);
            // 
            // addUserButton
            // 
            this.addUserButton.Location = new System.Drawing.Point(671, 12);
            this.addUserButton.Name = "addUserButton";
            this.addUserButton.Size = new System.Drawing.Size(75, 23);
            this.addUserButton.TabIndex = 6;
            this.addUserButton.Text = "Add User";
            this.addUserButton.UseVisualStyleBackColor = true;
            this.addUserButton.Click += new System.EventHandler(this.addUserButton_Click);
            // 
            // deleteUserButton
            // 
            this.deleteUserButton.Location = new System.Drawing.Point(671, 41);
            this.deleteUserButton.Name = "deleteUserButton";
            this.deleteUserButton.Size = new System.Drawing.Size(75, 23);
            this.deleteUserButton.TabIndex = 7;
            this.deleteUserButton.Text = "Delete User";
            this.deleteUserButton.UseVisualStyleBackColor = true;
            this.deleteUserButton.Click += new System.EventHandler(this.deleteUserButton_Click);
            // 
            // exportUsersButton
            // 
            this.exportUsersButton.Location = new System.Drawing.Point(671, 182);
            this.exportUsersButton.Name = "exportUsersButton";
            this.exportUsersButton.Size = new System.Drawing.Size(75, 23);
            this.exportUsersButton.TabIndex = 8;
            this.exportUsersButton.Text = "Export Users";
            this.exportUsersButton.UseVisualStyleBackColor = true;
            this.exportUsersButton.Click += new System.EventHandler(this.exportUsersButton_Click);
            // 
            // importUsersButton
            // 
            this.importUsersButton.Location = new System.Drawing.Point(671, 211);
            this.importUsersButton.Name = "importUsersButton";
            this.importUsersButton.Size = new System.Drawing.Size(75, 23);
            this.importUsersButton.TabIndex = 9;
            this.importUsersButton.Text = "Import Users";
            this.importUsersButton.UseVisualStyleBackColor = true;
            this.importUsersButton.Click += new System.EventHandler(this.importUsersButton_Click);
            // 
            // alertCheckBox
            // 
            this.alertCheckBox.AutoSize = true;
            this.alertCheckBox.Location = new System.Drawing.Point(649, 420);
            this.alertCheckBox.Name = "alertCheckBox";
            this.alertCheckBox.Size = new System.Drawing.Size(93, 17);
            this.alertCheckBox.TabIndex = 10;
            this.alertCheckBox.Text = "Alert Message";
            this.alertCheckBox.UseVisualStyleBackColor = true;
            // 
            // checkConnectionButton
            // 
            this.checkConnectionButton.Location = new System.Drawing.Point(671, 98);
            this.checkConnectionButton.Name = "checkConnectionButton";
            this.checkConnectionButton.Size = new System.Drawing.Size(75, 40);
            this.checkConnectionButton.TabIndex = 11;
            this.checkConnectionButton.Text = "Check Connection";
            this.checkConnectionButton.UseVisualStyleBackColor = true;
            this.checkConnectionButton.Click += new System.EventHandler(this.checkConnectionButton_Click);
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.checkConnectionButton);
            this.Controls.Add(this.alertCheckBox);
            this.Controls.Add(this.importUsersButton);
            this.Controls.Add(this.exportUsersButton);
            this.Controls.Add(this.deleteUserButton);
            this.Controls.Add(this.addUserButton);
            this.Controls.Add(this.userListBox);
            this.Controls.Add(this.messageTextBox);
            this.Controls.Add(this.chatTextBox);
            this.Controls.Add(this.sendMessageButton);
            this.Name = "MainWindow";
            this.Text = "MessageSystem";
            this.Load += new System.EventHandler(this.MainWindow_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Button sendMessageButton;
        private System.Windows.Forms.RichTextBox chatTextBox;
        private System.Windows.Forms.RichTextBox messageTextBox;
        private System.Windows.Forms.ListBox userListBox;
        private System.Windows.Forms.Button addUserButton;
        private System.Windows.Forms.Button deleteUserButton;
        private System.Windows.Forms.Button exportUsersButton;
        private System.Windows.Forms.Button importUsersButton;
        private System.Windows.Forms.CheckBox alertCheckBox;
        private System.Windows.Forms.Button checkConnectionButton;
    }
}

