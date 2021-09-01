
namespace FileSandBox_GUI
{
    partial class MainWindow
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
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.chooseTargetToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.debugEventsToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.debugEventsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tDBToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.advancedCustomizeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helperDllSettingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.existingProcessToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
            this.newProcessToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.debugEventsToolStripMenuItem,
            this.helperDllSettingsToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(800, 28);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.chooseTargetToolStripMenuItem,
            this.debugEventsToolStripMenuItem1});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(46, 24);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // chooseTargetToolStripMenuItem
            // 
            this.chooseTargetToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.existingProcessToolStripMenuItem,
            this.toolStripMenuItem2,
            this.newProcessToolStripMenuItem});
            this.chooseTargetToolStripMenuItem.Name = "chooseTargetToolStripMenuItem";
            this.chooseTargetToolStripMenuItem.Size = new System.Drawing.Size(224, 26);
            this.chooseTargetToolStripMenuItem.Text = "Choose Targets";
            // 
            // debugEventsToolStripMenuItem1
            // 
            this.debugEventsToolStripMenuItem1.Name = "debugEventsToolStripMenuItem1";
            this.debugEventsToolStripMenuItem1.Size = new System.Drawing.Size(224, 26);
            this.debugEventsToolStripMenuItem1.Text = "Debug Events";
            // 
            // debugEventsToolStripMenuItem
            // 
            this.debugEventsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tDBToolStripMenuItem,
            this.toolStripMenuItem1,
            this.advancedCustomizeToolStripMenuItem});
            this.debugEventsToolStripMenuItem.Name = "debugEventsToolStripMenuItem";
            this.debugEventsToolStripMenuItem.Size = new System.Drawing.Size(114, 24);
            this.debugEventsToolStripMenuItem.Text = "Debug Events";
            // 
            // tDBToolStripMenuItem
            // 
            this.tDBToolStripMenuItem.Name = "tDBToolStripMenuItem";
            this.tDBToolStripMenuItem.Size = new System.Drawing.Size(240, 26);
            this.tDBToolStripMenuItem.Text = "TDB";
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(237, 6);
            // 
            // advancedCustomizeToolStripMenuItem
            // 
            this.advancedCustomizeToolStripMenuItem.Name = "advancedCustomizeToolStripMenuItem";
            this.advancedCustomizeToolStripMenuItem.Size = new System.Drawing.Size(240, 26);
            this.advancedCustomizeToolStripMenuItem.Text = "Advanced Customize...";
            // 
            // helperDllSettingsToolStripMenuItem
            // 
            this.helperDllSettingsToolStripMenuItem.Name = "helperDllSettingsToolStripMenuItem";
            this.helperDllSettingsToolStripMenuItem.Size = new System.Drawing.Size(148, 24);
            this.helperDllSettingsToolStripMenuItem.Text = "Helper Dll Settings";
            // 
            // existingProcessToolStripMenuItem
            // 
            this.existingProcessToolStripMenuItem.Name = "existingProcessToolStripMenuItem";
            this.existingProcessToolStripMenuItem.Size = new System.Drawing.Size(224, 26);
            this.existingProcessToolStripMenuItem.Text = "Existing Process";
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(221, 6);
            // 
            // newProcessToolStripMenuItem
            // 
            this.newProcessToolStripMenuItem.Name = "newProcessToolStripMenuItem";
            this.newProcessToolStripMenuItem.Size = new System.Drawing.Size(224, 26);
            this.newProcessToolStripMenuItem.Text = "New Process";
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainWindow";
            this.Text = "FileSandBox GUI";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem chooseTargetToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem debugEventsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem tDBToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem advancedCustomizeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helperDllSettingsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem debugEventsToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem existingProcessToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem newProcessToolStripMenuItem;
    }
}

