namespace InsightLogger
{
    partial class LoggerRunDialog
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
            components = new System.ComponentModel.Container();
            ComboBoxTarget = new System.Windows.Forms.ComboBox();
            LabelOpen = new System.Windows.Forms.Label();
            ButtonOK = new System.Windows.Forms.Button();
            ButtonCancel = new System.Windows.Forms.Button();
            ButtonBrowse = new System.Windows.Forms.Button();
            LabelSelectDirections = new System.Windows.Forms.Label();
            ButtonCustomizeEnv = new System.Windows.Forms.Button();
            ContextMenuStripSpecialSettings = new System.Windows.Forms.ContextMenuStrip(components);
            ToolStripMenuItemLogAdjustShow = new System.Windows.Forms.ToolStripMenuItem();
            ToolStripMenuItemEnvEdit = new System.Windows.Forms.ToolStripMenuItem();
            ToolStripMenuItemDetourDllSetting = new System.Windows.Forms.ToolStripMenuItem();
            exceptionSettingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            OpenFileDialogSelectExe = new System.Windows.Forms.OpenFileDialog();
            ContextMenuStripSpecialSettings.SuspendLayout();
            SuspendLayout();
            // 
            // ComboBoxTarget
            // 
            ComboBoxTarget.FormattingEnabled = true;
            ComboBoxTarget.Items.AddRange(new object[] { "C:\\Windows\\System32\\cmd.exe" });
            ComboBoxTarget.Location = new System.Drawing.Point(77, 136);
            ComboBoxTarget.Name = "ComboBoxTarget";
            ComboBoxTarget.Size = new System.Drawing.Size(407, 33);
            ComboBoxTarget.TabIndex = 0;
            // 
            // LabelOpen
            // 
            LabelOpen.AutoSize = true;
            LabelOpen.Location = new System.Drawing.Point(12, 139);
            LabelOpen.Name = "LabelOpen";
            LabelOpen.Size = new System.Drawing.Size(56, 25);
            LabelOpen.TabIndex = 1;
            LabelOpen.Text = "&Open";
            // 
            // ButtonOK
            // 
            ButtonOK.Location = new System.Drawing.Point(136, 203);
            ButtonOK.Name = "ButtonOK";
            ButtonOK.Size = new System.Drawing.Size(112, 34);
            ButtonOK.TabIndex = 2;
            ButtonOK.Text = "OK";
            ButtonOK.UseVisualStyleBackColor = true;
            ButtonOK.Click += ButtonOK_Click;
            // 
            // ButtonCancel
            // 
            ButtonCancel.Location = new System.Drawing.Point(254, 203);
            ButtonCancel.Name = "ButtonCancel";
            ButtonCancel.Size = new System.Drawing.Size(112, 34);
            ButtonCancel.TabIndex = 3;
            ButtonCancel.Text = "Cancel";
            ButtonCancel.UseVisualStyleBackColor = true;
            ButtonCancel.Click += ButtonCancel_Click;
            // 
            // ButtonBrowse
            // 
            ButtonBrowse.Location = new System.Drawing.Point(372, 203);
            ButtonBrowse.Name = "ButtonBrowse";
            ButtonBrowse.Size = new System.Drawing.Size(112, 34);
            ButtonBrowse.TabIndex = 4;
            ButtonBrowse.Text = "&Browse...";
            ButtonBrowse.UseVisualStyleBackColor = true;
            ButtonBrowse.Click += button1_Click;
            // 
            // LabelSelectDirections
            // 
            LabelSelectDirections.AutoSize = true;
            LabelSelectDirections.Location = new System.Drawing.Point(48, 21);
            LabelSelectDirections.Name = "LabelSelectDirections";
            LabelSelectDirections.Size = new System.Drawing.Size(436, 75);
            LabelSelectDirections.TabIndex = 5;
            LabelSelectDirections.Text = "Type the Exact path of a Windows Program to Run.    \r\nYou may Browse for Something.\r\nUse the >> to Customize Log\r\n";
            // 
            // ButtonCustomizeEnv
            // 
            ButtonCustomizeEnv.Location = new System.Drawing.Point(490, 12);
            ButtonCustomizeEnv.Name = "ButtonCustomizeEnv";
            ButtonCustomizeEnv.Size = new System.Drawing.Size(48, 34);
            ButtonCustomizeEnv.TabIndex = 6;
            ButtonCustomizeEnv.Text = ">>";
            ButtonCustomizeEnv.UseVisualStyleBackColor = true;
            ButtonCustomizeEnv.Click += ButtonCustomizeEnv_Click;
            // 
            // ContextMenuStripSpecialSettings
            // 
            ContextMenuStripSpecialSettings.ImageScalingSize = new System.Drawing.Size(24, 24);
            ContextMenuStripSpecialSettings.Items.AddRange(new System.Windows.Forms.ToolStripItem[] { ToolStripMenuItemLogAdjustShow, ToolStripMenuItemEnvEdit, ToolStripMenuItemDetourDllSetting, exceptionSettingsToolStripMenuItem });
            ContextMenuStripSpecialSettings.Name = "ContextMenuStripSpecialSettings";
            ContextMenuStripSpecialSettings.Size = new System.Drawing.Size(319, 132);
            ContextMenuStripSpecialSettings.Opening += ContextMenuStripSpecialSettings_Opening;
            // 
            // ToolStripMenuItemLogAdjustShow
            // 
            ToolStripMenuItemLogAdjustShow.Name = "ToolStripMenuItemLogAdjustShow";
            ToolStripMenuItemLogAdjustShow.Size = new System.Drawing.Size(318, 32);
            ToolStripMenuItemLogAdjustShow.Text = "Adjust What to Log...";
            // 
            // ToolStripMenuItemEnvEdit
            // 
            ToolStripMenuItemEnvEdit.Name = "ToolStripMenuItemEnvEdit";
            ToolStripMenuItemEnvEdit.Size = new System.Drawing.Size(318, 32);
            ToolStripMenuItemEnvEdit.Text = "Define Starting Settings...";
            ToolStripMenuItemEnvEdit.Click += ToolStripMenuItemEnvEdit_Click;
            // 
            // ToolStripMenuItemDetourDllSetting
            // 
            ToolStripMenuItemDetourDllSetting.Name = "ToolStripMenuItemDetourDllSetting";
            ToolStripMenuItemDetourDllSetting.Size = new System.Drawing.Size(318, 32);
            ToolStripMenuItemDetourDllSetting.Text = "Detour Dll Injection Settings...";
            ToolStripMenuItemDetourDllSetting.Click += ToolStripMenuItemDetourDllSetting_Click;
            // 
            // exceptionSettingsToolStripMenuItem
            // 
            exceptionSettingsToolStripMenuItem.Name = "exceptionSettingsToolStripMenuItem";
            exceptionSettingsToolStripMenuItem.Size = new System.Drawing.Size(318, 32);
            exceptionSettingsToolStripMenuItem.Text = "Exception Settings";
            exceptionSettingsToolStripMenuItem.Click += exceptionSettingsToolStripMenuItem_Click;
            // 
            // OpenFileDialogSelectExe
            // 
            OpenFileDialogSelectExe.Filter = "Windows Programs|*.exe;|All Files|*.*";
            OpenFileDialogSelectExe.InitialDirectory = "C:\\Windows\\system32";
            OpenFileDialogSelectExe.RestoreDirectory = true;
            OpenFileDialogSelectExe.ShowHiddenFiles = true;
            OpenFileDialogSelectExe.FileOk += OpenFileDialogSelectExe_FileOk;
            // 
            // LoggerRunDialog
            // 
            AutoScaleDimensions = new System.Drawing.SizeF(10F, 25F);
            AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            ClientSize = new System.Drawing.Size(545, 274);
            Controls.Add(ButtonCustomizeEnv);
            Controls.Add(LabelSelectDirections);
            Controls.Add(ButtonBrowse);
            Controls.Add(ButtonCancel);
            Controls.Add(ButtonOK);
            Controls.Add(LabelOpen);
            Controls.Add(ComboBoxTarget);
            Name = "LoggerRunDialog";
            Text = "Insight Logger - Process Launcher";
            Load += LoggerRunDialog_Load;
            ContextMenuStripSpecialSettings.ResumeLayout(false);
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private System.Windows.Forms.ComboBox ComboBoxTarget;
        private System.Windows.Forms.Label LabelOpen;
        private System.Windows.Forms.Button ButtonOK;
        private System.Windows.Forms.Button ButtonCancel;
        private System.Windows.Forms.Button ButtonBrowse;
        private System.Windows.Forms.Label LabelSelectDirections;
        private System.Windows.Forms.Button ButtonCustomizeEnv;
        private System.Windows.Forms.ContextMenuStrip ContextMenuStripSpecialSettings;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemLogAdjustShow;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemEnvEdit;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemDetourDllSetting;
        private System.Windows.Forms.OpenFileDialog OpenFileDialogSelectExe;
        private System.Windows.Forms.ToolStripMenuItem exceptionSettingsToolStripMenuItem;
    }
}