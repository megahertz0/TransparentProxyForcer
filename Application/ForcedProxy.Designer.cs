namespace TransparentProxyForcer
{
    partial class ForcedProxy
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.programsPanels = new System.Windows.Forms.Panel();
            this.programs = new System.Windows.Forms.CheckedListBox();
            this.settingsPanel = new System.Windows.Forms.Panel();
            this.install = new System.Windows.Forms.Button();
            this.save = new System.Windows.Forms.Button();
            this.ip = new System.Windows.Forms.TextBox();
            this.label = new System.Windows.Forms.Label();
            this.timer = new System.Windows.Forms.Timer(this.components);
            this.persistent = new System.Windows.Forms.CheckBox();
            this.programsPanels.SuspendLayout();
            this.settingsPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // programsPanels
            // 
            this.programsPanels.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.programsPanels.Controls.Add(this.programs);
            this.programsPanels.Location = new System.Drawing.Point(12, 80);
            this.programsPanels.Name = "programsPanels";
            this.programsPanels.Size = new System.Drawing.Size(257, 170);
            this.programsPanels.TabIndex = 2;
            // 
            // programs
            // 
            this.programs.Dock = System.Windows.Forms.DockStyle.Fill;
            this.programs.Location = new System.Drawing.Point(0, 0);
            this.programs.Name = "programs";
            this.programs.Size = new System.Drawing.Size(257, 170);
            this.programs.TabIndex = 3;
            this.programs.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.programs_ItemCheck);
            // 
            // settingsPanel
            // 
            this.settingsPanel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.settingsPanel.Controls.Add(this.install);
            this.settingsPanel.Controls.Add(this.save);
            this.settingsPanel.Controls.Add(this.ip);
            this.settingsPanel.Location = new System.Drawing.Point(12, 12);
            this.settingsPanel.Name = "settingsPanel";
            this.settingsPanel.Size = new System.Drawing.Size(260, 29);
            this.settingsPanel.TabIndex = 3;
            // 
            // install
            // 
            this.install.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.install.Location = new System.Drawing.Point(177, 3);
            this.install.Name = "install";
            this.install.Size = new System.Drawing.Size(80, 23);
            this.install.TabIndex = 2;
            this.install.Text = "Install";
            this.install.UseVisualStyleBackColor = true;
            this.install.Click += new System.EventHandler(this.install_Click);
            // 
            // save
            // 
            this.save.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.save.Enabled = false;
            this.save.Location = new System.Drawing.Point(91, 3);
            this.save.Name = "save";
            this.save.Size = new System.Drawing.Size(80, 23);
            this.save.TabIndex = 1;
            this.save.Text = "Save IP";
            this.save.UseVisualStyleBackColor = true;
            this.save.Click += new System.EventHandler(this.save_Click);
            // 
            // ip
            // 
            this.ip.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ip.Location = new System.Drawing.Point(3, 5);
            this.ip.Name = "ip";
            this.ip.Size = new System.Drawing.Size(82, 20);
            this.ip.TabIndex = 0;
            this.ip.Text = "127.0.0.1";
            this.ip.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.ip.TextChanged += new System.EventHandler(this.ip_TextChanged);
            // 
            // label
            // 
            this.label.AutoSize = true;
            this.label.Location = new System.Drawing.Point(9, 64);
            this.label.Name = "label";
            this.label.Size = new System.Drawing.Size(224, 13);
            this.label.TabIndex = 4;
            this.label.Text = "Force HTTP(S) proxing for following programs:";
            // 
            // timer
            // 
            this.timer.Enabled = true;
            this.timer.Interval = 1000;
            this.timer.Tick += new System.EventHandler(this.timer_Tick);
            // 
            // persistent
            // 
            this.persistent.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.persistent.AutoSize = true;
            this.persistent.Location = new System.Drawing.Point(43, 44);
            this.persistent.Name = "persistent";
            this.persistent.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.persistent.Size = new System.Drawing.Size(226, 17);
            this.persistent.TabIndex = 5;
            this.persistent.Text = "Mantain the hook after exiting this program";
            this.persistent.UseVisualStyleBackColor = true;
            // 
            // ForcedProxy
            // 
            this.ClientSize = new System.Drawing.Size(284, 262);
            this.Controls.Add(this.persistent);
            this.Controls.Add(this.label);
            this.Controls.Add(this.settingsPanel);
            this.Controls.Add(this.programsPanels);
            this.MinimumSize = new System.Drawing.Size(300, 300);
            this.Name = "ForcedProxy";
            this.Text = "Forced Proxy Configuration";
            this.programsPanels.ResumeLayout(false);
            this.settingsPanel.ResumeLayout(false);
            this.settingsPanel.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel programsPanels;
        private System.Windows.Forms.CheckedListBox programs;
        private System.Windows.Forms.Panel settingsPanel;
        private System.Windows.Forms.Button save;
        private System.Windows.Forms.TextBox ip;
        private System.Windows.Forms.Button install;
        private System.Windows.Forms.Label label;
        private System.Windows.Forms.Timer timer;
        private System.Windows.Forms.CheckBox persistent;
    }
}
