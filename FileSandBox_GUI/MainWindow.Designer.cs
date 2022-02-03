
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
            this.existingProcessToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
            this.newProcessToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem5 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.debugEventsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tDBToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.allToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.allExceptHelperDllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem4 = new System.Windows.Forms.ToolStripSeparator();
            this.createProcessEventToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.createThreadEventToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.generalExceptionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.generalToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helperDLLCommunicationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitProcessEventToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitThreadEventToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.outputDebugStringEventToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ripEventToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.unloadDllDebugEventToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.advancedCustomizeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helperDllSettingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.chooseWhichDetoursDllsToLoadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
            this.whichDisableToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.windowsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showHelpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem6 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.debugEventsToolStripMenuItem,
            this.helperDllSettingsToolStripMenuItem,
            this.windowsToolStripMenuItem,
            this.helpToolStripMenuItem});
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
            this.toolStripMenuItem5,
            this.exitToolStripMenuItem});
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
            // existingProcessToolStripMenuItem
            // 
            this.existingProcessToolStripMenuItem.Name = "existingProcessToolStripMenuItem";
            this.existingProcessToolStripMenuItem.Size = new System.Drawing.Size(224, 26);
            this.existingProcessToolStripMenuItem.Text = "Existing Process";
            this.existingProcessToolStripMenuItem.Click += new System.EventHandler(this.existingProcessToolStripMenuItem_Click);
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
            this.newProcessToolStripMenuItem.Click += new System.EventHandler(this.newProcessToolStripMenuItem_Click);
            // 
            // toolStripMenuItem5
            // 
            this.toolStripMenuItem5.Name = "toolStripMenuItem5";
            this.toolStripMenuItem5.Size = new System.Drawing.Size(221, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(224, 26);
            this.exitToolStripMenuItem.Text = "Exit";
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
            this.tDBToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.allToolStripMenuItem,
            this.allExceptHelperDllToolStripMenuItem,
            this.toolStripMenuItem4,
            this.createProcessEventToolStripMenuItem,
            this.createThreadEventToolStripMenuItem,
            this.generalExceptionsToolStripMenuItem,
            this.exitProcessEventToolStripMenuItem,
            this.exitThreadEventToolStripMenuItem,
            this.outputDebugStringEventToolStripMenuItem,
            this.ripEventToolStripMenuItem,
            this.unloadDllDebugEventToolStripMenuItem});
            this.tDBToolStripMenuItem.Name = "tDBToolStripMenuItem";
            this.tDBToolStripMenuItem.Size = new System.Drawing.Size(240, 26);
            this.tDBToolStripMenuItem.Text = "Logging";
            // 
            // allToolStripMenuItem
            // 
            this.allToolStripMenuItem.CheckOnClick = true;
            this.allToolStripMenuItem.Name = "allToolStripMenuItem";
            this.allToolStripMenuItem.Size = new System.Drawing.Size(270, 26);
            this.allToolStripMenuItem.Text = "All";
            // 
            // allExceptHelperDllToolStripMenuItem
            // 
            this.allExceptHelperDllToolStripMenuItem.Name = "allExceptHelperDllToolStripMenuItem";
            this.allExceptHelperDllToolStripMenuItem.Size = new System.Drawing.Size(270, 26);
            this.allExceptHelperDllToolStripMenuItem.Text = "All Except Helper Dll";
            this.allExceptHelperDllToolStripMenuItem.Click += new System.EventHandler(this.allExceptHelperDllToolStripMenuItem_Click);
            // 
            // toolStripMenuItem4
            // 
            this.toolStripMenuItem4.Name = "toolStripMenuItem4";
            this.toolStripMenuItem4.Size = new System.Drawing.Size(267, 6);
            // 
            // createProcessEventToolStripMenuItem
            // 
            this.createProcessEventToolStripMenuItem.CheckOnClick = true;
            this.createProcessEventToolStripMenuItem.Name = "createProcessEventToolStripMenuItem";
            this.createProcessEventToolStripMenuItem.Size = new System.Drawing.Size(270, 26);
            this.createProcessEventToolStripMenuItem.Text = "Create Process Event";
            // 
            // createThreadEventToolStripMenuItem
            // 
            this.createThreadEventToolStripMenuItem.CheckOnClick = true;
            this.createThreadEventToolStripMenuItem.Name = "createThreadEventToolStripMenuItem";
            this.createThreadEventToolStripMenuItem.Size = new System.Drawing.Size(270, 26);
            this.createThreadEventToolStripMenuItem.Text = "Create Thread Event";
            // 
            // generalExceptionsToolStripMenuItem
            // 
            this.generalExceptionsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.generalToolStripMenuItem,
            this.helperDLLCommunicationToolStripMenuItem});
            this.generalExceptionsToolStripMenuItem.Name = "generalExceptionsToolStripMenuItem";
            this.generalExceptionsToolStripMenuItem.Size = new System.Drawing.Size(270, 26);
            this.generalExceptionsToolStripMenuItem.Text = "Exceptions Events";
            // 
            // generalToolStripMenuItem
            // 
            this.generalToolStripMenuItem.CheckOnClick = true;
            this.generalToolStripMenuItem.Name = "generalToolStripMenuItem";
            this.generalToolStripMenuItem.Size = new System.Drawing.Size(271, 26);
            this.generalToolStripMenuItem.Text = "General";
            // 
            // helperDLLCommunicationToolStripMenuItem
            // 
            this.helperDLLCommunicationToolStripMenuItem.CheckOnClick = true;
            this.helperDLLCommunicationToolStripMenuItem.Name = "helperDLLCommunicationToolStripMenuItem";
            this.helperDLLCommunicationToolStripMenuItem.Size = new System.Drawing.Size(271, 26);
            this.helperDLLCommunicationToolStripMenuItem.Text = "HelperDLL Communication";
            // 
            // exitProcessEventToolStripMenuItem
            // 
            this.exitProcessEventToolStripMenuItem.CheckOnClick = true;
            this.exitProcessEventToolStripMenuItem.Name = "exitProcessEventToolStripMenuItem";
            this.exitProcessEventToolStripMenuItem.Size = new System.Drawing.Size(270, 26);
            this.exitProcessEventToolStripMenuItem.Text = "Exit Process Event";
            // 
            // exitThreadEventToolStripMenuItem
            // 
            this.exitThreadEventToolStripMenuItem.CheckOnClick = true;
            this.exitThreadEventToolStripMenuItem.Name = "exitThreadEventToolStripMenuItem";
            this.exitThreadEventToolStripMenuItem.Size = new System.Drawing.Size(270, 26);
            this.exitThreadEventToolStripMenuItem.Text = "Exit Thread Event";
            // 
            // outputDebugStringEventToolStripMenuItem
            // 
            this.outputDebugStringEventToolStripMenuItem.CheckOnClick = true;
            this.outputDebugStringEventToolStripMenuItem.Name = "outputDebugStringEventToolStripMenuItem";
            this.outputDebugStringEventToolStripMenuItem.Size = new System.Drawing.Size(270, 26);
            this.outputDebugStringEventToolStripMenuItem.Text = "Output Debug String Event";
            // 
            // ripEventToolStripMenuItem
            // 
            this.ripEventToolStripMenuItem.CheckOnClick = true;
            this.ripEventToolStripMenuItem.Name = "ripEventToolStripMenuItem";
            this.ripEventToolStripMenuItem.Size = new System.Drawing.Size(270, 26);
            this.ripEventToolStripMenuItem.Text = "Rip Event";
            // 
            // unloadDllDebugEventToolStripMenuItem
            // 
            this.unloadDllDebugEventToolStripMenuItem.CheckOnClick = true;
            this.unloadDllDebugEventToolStripMenuItem.Name = "unloadDllDebugEventToolStripMenuItem";
            this.unloadDllDebugEventToolStripMenuItem.Size = new System.Drawing.Size(270, 26);
            this.unloadDllDebugEventToolStripMenuItem.Text = "Unload Dll Debug Event";
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
            this.helperDllSettingsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.chooseWhichDetoursDllsToLoadToolStripMenuItem,
            this.toolStripMenuItem3,
            this.whichDisableToolStripMenuItem});
            this.helperDllSettingsToolStripMenuItem.Name = "helperDllSettingsToolStripMenuItem";
            this.helperDllSettingsToolStripMenuItem.Size = new System.Drawing.Size(148, 24);
            this.helperDllSettingsToolStripMenuItem.Text = "Helper Dll Settings";
            // 
            // chooseWhichDetoursDllsToLoadToolStripMenuItem
            // 
            this.chooseWhichDetoursDllsToLoadToolStripMenuItem.Name = "chooseWhichDetoursDllsToLoadToolStripMenuItem";
            this.chooseWhichDetoursDllsToLoadToolStripMenuItem.Size = new System.Drawing.Size(326, 26);
            this.chooseWhichDetoursDllsToLoadToolStripMenuItem.Text = "Choose Which Detours Dlls to Load";
            // 
            // toolStripMenuItem3
            // 
            this.toolStripMenuItem3.Name = "toolStripMenuItem3";
            this.toolStripMenuItem3.Size = new System.Drawing.Size(323, 6);
            // 
            // whichDisableToolStripMenuItem
            // 
            this.whichDisableToolStripMenuItem.Name = "whichDisableToolStripMenuItem";
            this.whichDisableToolStripMenuItem.Size = new System.Drawing.Size(326, 26);
            this.whichDisableToolStripMenuItem.Text = "Quick Disable";
            // 
            // windowsToolStripMenuItem
            // 
            this.windowsToolStripMenuItem.Name = "windowsToolStripMenuItem";
            this.windowsToolStripMenuItem.Size = new System.Drawing.Size(84, 24);
            this.windowsToolStripMenuItem.Text = "Windows";
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.showHelpToolStripMenuItem,
            this.toolStripMenuItem6,
            this.exitToolStripMenuItem1});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(55, 24);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // showHelpToolStripMenuItem
            // 
            this.showHelpToolStripMenuItem.Name = "showHelpToolStripMenuItem";
            this.showHelpToolStripMenuItem.Size = new System.Drawing.Size(224, 26);
            this.showHelpToolStripMenuItem.Text = "Show Help";
            // 
            // toolStripMenuItem6
            // 
            this.toolStripMenuItem6.Name = "toolStripMenuItem6";
            this.toolStripMenuItem6.Size = new System.Drawing.Size(221, 6);
            // 
            // exitToolStripMenuItem1
            // 
            this.exitToolStripMenuItem1.Name = "exitToolStripMenuItem1";
            this.exitToolStripMenuItem1.Size = new System.Drawing.Size(224, 26);
            this.exitToolStripMenuItem1.Text = "About FileSandBox";
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
        private System.Windows.Forms.ToolStripMenuItem existingProcessToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem newProcessToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem allToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem4;
        private System.Windows.Forms.ToolStripMenuItem createProcessEventToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem createThreadEventToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem generalExceptionsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem generalToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helperDLLCommunicationToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitProcessEventToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitThreadEventToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem outputDebugStringEventToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ripEventToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem unloadDllDebugEventToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem allExceptHelperDllToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem chooseWhichDetoursDllsToLoadToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
        private System.Windows.Forms.ToolStripMenuItem whichDisableToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem5;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem windowsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showHelpToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem6;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem1;
    }
}

