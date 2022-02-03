
namespace FileSandBox_Forms
{
    partial class InsightDebuggerMainView
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
            this.newProcessToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.openFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveFileAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.eventsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.createProcessToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.exitProcessToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitThreadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadDllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.unloadDllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.debugStripToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ripEventToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
            this.helpDllEventsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openingCreatingAFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openingCreatingRegistryStuffToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.creatingAProcessToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.eventsToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(800, 33);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newProcessToolStripMenuItem,
            this.toolStripMenuItem1,
            this.openFileToolStripMenuItem,
            this.saveFileAsToolStripMenuItem,
            this.toolStripMenuItem2,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(54, 29);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // newProcessToolStripMenuItem
            // 
            this.newProcessToolStripMenuItem.Name = "newProcessToolStripMenuItem";
            this.newProcessToolStripMenuItem.Size = new System.Drawing.Size(270, 34);
            this.newProcessToolStripMenuItem.Text = "New Process";
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(267, 6);
            // 
            // openFileToolStripMenuItem
            // 
            this.openFileToolStripMenuItem.Name = "openFileToolStripMenuItem";
            this.openFileToolStripMenuItem.Size = new System.Drawing.Size(270, 34);
            this.openFileToolStripMenuItem.Text = "Open File";
            // 
            // saveFileAsToolStripMenuItem
            // 
            this.saveFileAsToolStripMenuItem.Name = "saveFileAsToolStripMenuItem";
            this.saveFileAsToolStripMenuItem.Size = new System.Drawing.Size(270, 34);
            this.saveFileAsToolStripMenuItem.Text = "Save File As";
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(267, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(270, 34);
            this.exitToolStripMenuItem.Text = "Exit";
            // 
            // eventsToolStripMenuItem
            // 
            this.eventsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.createProcessToolStripMenuItem,
            this.exitToolStripMenuItem1,
            this.exitProcessToolStripMenuItem,
            this.exitThreadToolStripMenuItem,
            this.loadDllToolStripMenuItem,
            this.unloadDllToolStripMenuItem,
            this.debugStripToolStripMenuItem,
            this.ripEventToolStripMenuItem,
            this.toolStripMenuItem3,
            this.helpDllEventsToolStripMenuItem});
            this.eventsToolStripMenuItem.Name = "eventsToolStripMenuItem";
            this.eventsToolStripMenuItem.Size = new System.Drawing.Size(79, 29);
            this.eventsToolStripMenuItem.Text = "Events";
            // 
            // createProcessToolStripMenuItem
            // 
            this.createProcessToolStripMenuItem.CheckOnClick = true;
            this.createProcessToolStripMenuItem.Name = "createProcessToolStripMenuItem";
            this.createProcessToolStripMenuItem.Size = new System.Drawing.Size(270, 34);
            this.createProcessToolStripMenuItem.Text = "Create Process ";
            // 
            // exitToolStripMenuItem1
            // 
            this.exitToolStripMenuItem1.CheckOnClick = true;
            this.exitToolStripMenuItem1.Name = "exitToolStripMenuItem1";
            this.exitToolStripMenuItem1.Size = new System.Drawing.Size(270, 34);
            this.exitToolStripMenuItem1.Text = "Create Thread";
            // 
            // exitProcessToolStripMenuItem
            // 
            this.exitProcessToolStripMenuItem.CheckOnClick = true;
            this.exitProcessToolStripMenuItem.Name = "exitProcessToolStripMenuItem";
            this.exitProcessToolStripMenuItem.Size = new System.Drawing.Size(270, 34);
            this.exitProcessToolStripMenuItem.Text = "Exit Process";
            // 
            // exitThreadToolStripMenuItem
            // 
            this.exitThreadToolStripMenuItem.CheckOnClick = true;
            this.exitThreadToolStripMenuItem.Name = "exitThreadToolStripMenuItem";
            this.exitThreadToolStripMenuItem.Size = new System.Drawing.Size(270, 34);
            this.exitThreadToolStripMenuItem.Text = "Exit Thread";
            // 
            // loadDllToolStripMenuItem
            // 
            this.loadDllToolStripMenuItem.CheckOnClick = true;
            this.loadDllToolStripMenuItem.Name = "loadDllToolStripMenuItem";
            this.loadDllToolStripMenuItem.Size = new System.Drawing.Size(270, 34);
            this.loadDllToolStripMenuItem.Text = "Load Dll";
            // 
            // unloadDllToolStripMenuItem
            // 
            this.unloadDllToolStripMenuItem.CheckOnClick = true;
            this.unloadDllToolStripMenuItem.Name = "unloadDllToolStripMenuItem";
            this.unloadDllToolStripMenuItem.Size = new System.Drawing.Size(270, 34);
            this.unloadDllToolStripMenuItem.Text = "Unload Dll";
            // 
            // debugStripToolStripMenuItem
            // 
            this.debugStripToolStripMenuItem.CheckOnClick = true;
            this.debugStripToolStripMenuItem.Name = "debugStripToolStripMenuItem";
            this.debugStripToolStripMenuItem.Size = new System.Drawing.Size(270, 34);
            this.debugStripToolStripMenuItem.Text = "Debug String";
            // 
            // ripEventToolStripMenuItem
            // 
            this.ripEventToolStripMenuItem.CheckOnClick = true;
            this.ripEventToolStripMenuItem.Name = "ripEventToolStripMenuItem";
            this.ripEventToolStripMenuItem.Size = new System.Drawing.Size(270, 34);
            this.ripEventToolStripMenuItem.Text = "Rip Event";
            // 
            // toolStripMenuItem3
            // 
            this.toolStripMenuItem3.Name = "toolStripMenuItem3";
            this.toolStripMenuItem3.Size = new System.Drawing.Size(267, 6);
            // 
            // helpDllEventsToolStripMenuItem
            // 
            this.helpDllEventsToolStripMenuItem.CheckOnClick = true;
            this.helpDllEventsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openingCreatingAFileToolStripMenuItem,
            this.openingCreatingRegistryStuffToolStripMenuItem,
            this.creatingAProcessToolStripMenuItem});
            this.helpDllEventsToolStripMenuItem.Name = "helpDllEventsToolStripMenuItem";
            this.helpDllEventsToolStripMenuItem.Size = new System.Drawing.Size(270, 34);
            this.helpDllEventsToolStripMenuItem.Text = "Helper Dll Events";
            // 
            // openingCreatingAFileToolStripMenuItem
            // 
            this.openingCreatingAFileToolStripMenuItem.Name = "openingCreatingAFileToolStripMenuItem";
            this.openingCreatingAFileToolStripMenuItem.Size = new System.Drawing.Size(366, 34);
            this.openingCreatingAFileToolStripMenuItem.Text = "Opening/Creating a File";
            // 
            // openingCreatingRegistryStuffToolStripMenuItem
            // 
            this.openingCreatingRegistryStuffToolStripMenuItem.Name = "openingCreatingRegistryStuffToolStripMenuItem";
            this.openingCreatingRegistryStuffToolStripMenuItem.Size = new System.Drawing.Size(366, 34);
            this.openingCreatingRegistryStuffToolStripMenuItem.Text = "Opening/Creating Registry Stuff";
            // 
            // creatingAProcessToolStripMenuItem
            // 
            this.creatingAProcessToolStripMenuItem.Name = "creatingAProcessToolStripMenuItem";
            this.creatingAProcessToolStripMenuItem.Size = new System.Drawing.Size(366, 34);
            this.creatingAProcessToolStripMenuItem.Text = "Creating a Process";
            // 
            // InsightDebuggerMainView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "InsightDebuggerMainView";
            this.Text = "Form1";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newProcessToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem openFileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveFileAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem eventsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem createProcessToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem exitProcessToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitThreadToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadDllToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem unloadDllToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem debugStripToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ripEventToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
        private System.Windows.Forms.ToolStripMenuItem helpDllEventsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openingCreatingAFileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openingCreatingRegistryStuffToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem creatingAProcessToolStripMenuItem;
    }
}

