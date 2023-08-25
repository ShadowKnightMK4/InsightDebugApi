namespace InsightLogger
{
    partial class LandingForm
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            menuStrip1 = new System.Windows.Forms.MenuStrip();
            fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            processToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            spawnToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            loggingToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            logSettingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            clearLogToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            saveLogAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
            exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            menuStrip1.SuspendLayout();
            SuspendLayout();
            // 
            // menuStrip1
            // 
            menuStrip1.ImageScalingSize = new System.Drawing.Size(24, 24);
            menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] { fileToolStripMenuItem, processToolStripMenuItem, loggingToolStripMenuItem });
            menuStrip1.Location = new System.Drawing.Point(0, 0);
            menuStrip1.Name = "menuStrip1";
            menuStrip1.Size = new System.Drawing.Size(800, 33);
            menuStrip1.TabIndex = 0;
            menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] { clearLogToolStripMenuItem, toolStripMenuItem1, saveLogAsToolStripMenuItem, toolStripMenuItem2, exitToolStripMenuItem });
            fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            fileToolStripMenuItem.Size = new System.Drawing.Size(54, 29);
            fileToolStripMenuItem.Text = "&File";
            // 
            // processToolStripMenuItem
            // 
            processToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] { spawnToolStripMenuItem });
            processToolStripMenuItem.Name = "processToolStripMenuItem";
            processToolStripMenuItem.Size = new System.Drawing.Size(88, 29);
            processToolStripMenuItem.Text = "Process";
            processToolStripMenuItem.Click += processToolStripMenuItem_Click;
            // 
            // spawnToolStripMenuItem
            // 
            spawnToolStripMenuItem.Name = "spawnToolStripMenuItem";
            spawnToolStripMenuItem.Size = new System.Drawing.Size(270, 34);
            spawnToolStripMenuItem.Text = "Spawn";
            spawnToolStripMenuItem.Click += spawnToolStripMenuItem_Click;
            // 
            // loggingToolStripMenuItem
            // 
            loggingToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] { logSettingsToolStripMenuItem });
            loggingToolStripMenuItem.Name = "loggingToolStripMenuItem";
            loggingToolStripMenuItem.Size = new System.Drawing.Size(94, 29);
            loggingToolStripMenuItem.Text = "Logging";
            // 
            // logSettingsToolStripMenuItem
            // 
            logSettingsToolStripMenuItem.Name = "logSettingsToolStripMenuItem";
            logSettingsToolStripMenuItem.Size = new System.Drawing.Size(270, 34);
            logSettingsToolStripMenuItem.Text = "Log Settings";
            logSettingsToolStripMenuItem.Click += logSettingsToolStripMenuItem_Click;
            // 
            // clearLogToolStripMenuItem
            // 
            clearLogToolStripMenuItem.Name = "clearLogToolStripMenuItem";
            clearLogToolStripMenuItem.Size = new System.Drawing.Size(270, 34);
            clearLogToolStripMenuItem.Text = "Clear Log";
            // 
            // toolStripMenuItem1
            // 
            toolStripMenuItem1.Name = "toolStripMenuItem1";
            toolStripMenuItem1.Size = new System.Drawing.Size(267, 6);
            // 
            // saveLogAsToolStripMenuItem
            // 
            saveLogAsToolStripMenuItem.Name = "saveLogAsToolStripMenuItem";
            saveLogAsToolStripMenuItem.Size = new System.Drawing.Size(270, 34);
            saveLogAsToolStripMenuItem.Text = "Save Log As";
            // 
            // toolStripMenuItem2
            // 
            toolStripMenuItem2.Name = "toolStripMenuItem2";
            toolStripMenuItem2.Size = new System.Drawing.Size(267, 6);
            // 
            // exitToolStripMenuItem
            // 
            exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            exitToolStripMenuItem.Size = new System.Drawing.Size(270, 34);
            exitToolStripMenuItem.Text = "Exit";
            // 
            // LandingForm
            // 
            AutoScaleDimensions = new System.Drawing.SizeF(10F, 25F);
            AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            ClientSize = new System.Drawing.Size(800, 450);
            Controls.Add(menuStrip1);
            MainMenuStrip = menuStrip1;
            Name = "LandingForm";
            Text = "Insight Logger";
            menuStrip1.ResumeLayout(false);
            menuStrip1.PerformLayout();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem processToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem spawnToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loggingToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem logSettingsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem clearLogToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem saveLogAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
    }
}