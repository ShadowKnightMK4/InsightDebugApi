namespace InsightLogger
{
    partial class NewProcessForm
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
            TextBoxTargetProcess = new System.Windows.Forms.TextBox();
            ButtonChooseProcessToSpawn = new System.Windows.Forms.Button();
            TabControlProcessCreate = new System.Windows.Forms.TabControl();
            tabPage1 = new System.Windows.Forms.TabPage();
            checkedListBox1 = new System.Windows.Forms.CheckedListBox();
            tabPage2 = new System.Windows.Forms.TabPage();
            label6 = new System.Windows.Forms.Label();
            ButtonChooseEnvText = new System.Windows.Forms.Button();
            label3 = new System.Windows.Forms.Label();
            label2 = new System.Windows.Forms.Label();
            groupBox1 = new System.Windows.Forms.GroupBox();
            radioButton2 = new System.Windows.Forms.RadioButton();
            RadioButton = new System.Windows.Forms.RadioButton();
            TextBoxTargetEnvFile = new System.Windows.Forms.TextBox();
            tabPage3 = new System.Windows.Forms.TabPage();
            CheckBoxWantDebug = new System.Windows.Forms.CheckBox();
            tabPage4 = new System.Windows.Forms.TabPage();
            label5 = new System.Windows.Forms.Label();
            label4 = new System.Windows.Forms.Label();
            label1 = new System.Windows.Forms.Label();
            CheckBoxDebugPriv = new System.Windows.Forms.CheckBox();
            checkBox1 = new System.Windows.Forms.CheckBox();
            TabControlProcessCreate.SuspendLayout();
            tabPage1.SuspendLayout();
            tabPage2.SuspendLayout();
            groupBox1.SuspendLayout();
            tabPage3.SuspendLayout();
            tabPage4.SuspendLayout();
            SuspendLayout();
            // 
            // TextBoxTargetProcess
            // 
            TextBoxTargetProcess.Location = new System.Drawing.Point(25, 46);
            TextBoxTargetProcess.Name = "TextBoxTargetProcess";
            TextBoxTargetProcess.Size = new System.Drawing.Size(375, 31);
            TextBoxTargetProcess.TabIndex = 0;
            // 
            // ButtonChooseProcessToSpawn
            // 
            ButtonChooseProcessToSpawn.Location = new System.Drawing.Point(406, 46);
            ButtonChooseProcessToSpawn.Name = "ButtonChooseProcessToSpawn";
            ButtonChooseProcessToSpawn.Size = new System.Drawing.Size(57, 31);
            ButtonChooseProcessToSpawn.TabIndex = 1;
            ButtonChooseProcessToSpawn.Text = "...";
            ButtonChooseProcessToSpawn.UseVisualStyleBackColor = true;
            ButtonChooseProcessToSpawn.Click += ButtonChooseProcessToSpawn_Click;
            // 
            // TabControlProcessCreate
            // 
            TabControlProcessCreate.Controls.Add(tabPage1);
            TabControlProcessCreate.Controls.Add(tabPage2);
            TabControlProcessCreate.Controls.Add(tabPage3);
            TabControlProcessCreate.Controls.Add(tabPage4);
            TabControlProcessCreate.Location = new System.Drawing.Point(12, 101);
            TabControlProcessCreate.Name = "TabControlProcessCreate";
            TabControlProcessCreate.SelectedIndex = 0;
            TabControlProcessCreate.Size = new System.Drawing.Size(776, 295);
            TabControlProcessCreate.TabIndex = 2;
            // 
            // tabPage1
            // 
            tabPage1.Controls.Add(checkedListBox1);
            tabPage1.Location = new System.Drawing.Point(4, 34);
            tabPage1.Name = "tabPage1";
            tabPage1.Padding = new System.Windows.Forms.Padding(3);
            tabPage1.Size = new System.Drawing.Size(768, 257);
            tabPage1.TabIndex = 0;
            tabPage1.Text = "Creation Settings";
            tabPage1.UseVisualStyleBackColor = true;
            tabPage1.Click += tabPage1_Click;
            // 
            // checkedListBox1
            // 
            checkedListBox1.FormattingEnabled = true;
            checkedListBox1.Location = new System.Drawing.Point(21, 28);
            checkedListBox1.Name = "checkedListBox1";
            checkedListBox1.Size = new System.Drawing.Size(180, 144);
            checkedListBox1.TabIndex = 0;
            // 
            // tabPage2
            // 
            tabPage2.Controls.Add(label6);
            tabPage2.Controls.Add(ButtonChooseEnvText);
            tabPage2.Controls.Add(label3);
            tabPage2.Controls.Add(label2);
            tabPage2.Controls.Add(groupBox1);
            tabPage2.Controls.Add(TextBoxTargetEnvFile);
            tabPage2.Location = new System.Drawing.Point(4, 34);
            tabPage2.Name = "tabPage2";
            tabPage2.Padding = new System.Windows.Forms.Padding(3);
            tabPage2.Size = new System.Drawing.Size(768, 257);
            tabPage2.TabIndex = 1;
            tabPage2.Text = "Environment Settings";
            tabPage2.UseVisualStyleBackColor = true;
            // 
            // label6
            // 
            label6.AutoSize = true;
            label6.Location = new System.Drawing.Point(26, 75);
            label6.Name = "label6";
            label6.Size = new System.Drawing.Size(153, 25);
            label6.TabIndex = 6;
            label6.Text = "Format is \"A\"=\"B\"";
            // 
            // ButtonChooseEnvText
            // 
            ButtonChooseEnvText.Location = new System.Drawing.Point(243, 121);
            ButtonChooseEnvText.Name = "ButtonChooseEnvText";
            ButtonChooseEnvText.Size = new System.Drawing.Size(112, 34);
            ButtonChooseEnvText.TabIndex = 5;
            ButtonChooseEnvText.Text = "...";
            ButtonChooseEnvText.UseVisualStyleBackColor = true;
            ButtonChooseEnvText.Click += ButtonChooseEnvText_Click;
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new System.Drawing.Point(28, 47);
            label3.Name = "label3";
            label3.Size = new System.Drawing.Size(487, 25);
            label3.TabIndex = 4;
            label3.Text = "Any defined by your file will always override inherited values";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new System.Drawing.Point(25, 21);
            label2.Name = "label2";
            label2.Size = new System.Drawing.Size(389, 25);
            label2.TabIndex = 3;
            label2.Text = "Choose a file to load enviroment variables from";
            // 
            // groupBox1
            // 
            groupBox1.Controls.Add(radioButton2);
            groupBox1.Controls.Add(RadioButton);
            groupBox1.Location = new System.Drawing.Point(390, 101);
            groupBox1.Name = "groupBox1";
            groupBox1.Size = new System.Drawing.Size(300, 150);
            groupBox1.TabIndex = 2;
            groupBox1.TabStop = false;
            groupBox1.Text = "Choose Environent Variable Mode";
            groupBox1.Enter += groupBox1_Enter;
            // 
            // radioButton2
            // 
            radioButton2.AutoSize = true;
            radioButton2.Location = new System.Drawing.Point(18, 90);
            radioButton2.Name = "radioButton2";
            radioButton2.Size = new System.Drawing.Size(308, 29);
            radioButton2.TabIndex = 1;
            radioButton2.TabStop = true;
            radioButton2.Text = "Only Explicitly Defined Enviroment";
            radioButton2.UseVisualStyleBackColor = true;
            // 
            // RadioButton
            // 
            RadioButton.AutoSize = true;
            RadioButton.Location = new System.Drawing.Point(18, 45);
            RadioButton.Name = "RadioButton";
            RadioButton.Size = new System.Drawing.Size(257, 29);
            RadioButton.TabIndex = 0;
            RadioButton.TabStop = true;
            RadioButton.Text = "Inherit Process Environment";
            RadioButton.UseVisualStyleBackColor = true;
            RadioButton.CheckedChanged += radioButton1_CheckedChanged;
            // 
            // TextBoxTargetEnvFile
            // 
            TextBoxTargetEnvFile.Location = new System.Drawing.Point(25, 121);
            TextBoxTargetEnvFile.Name = "TextBoxTargetEnvFile";
            TextBoxTargetEnvFile.Size = new System.Drawing.Size(199, 31);
            TextBoxTargetEnvFile.TabIndex = 1;
            // 
            // tabPage3
            // 
            tabPage3.Controls.Add(checkBox1);
            tabPage3.Controls.Add(CheckBoxDebugPriv);
            tabPage3.Controls.Add(CheckBoxWantDebug);
            tabPage3.Location = new System.Drawing.Point(4, 34);
            tabPage3.Name = "tabPage3";
            tabPage3.Size = new System.Drawing.Size(768, 257);
            tabPage3.TabIndex = 2;
            tabPage3.Text = "Debug Settings";
            tabPage3.UseVisualStyleBackColor = true;
            // 
            // CheckBoxWantDebug
            // 
            CheckBoxWantDebug.AutoSize = true;
            CheckBoxWantDebug.Location = new System.Drawing.Point(44, 19);
            CheckBoxWantDebug.Name = "CheckBoxWantDebug";
            CheckBoxWantDebug.Size = new System.Drawing.Size(390, 29);
            CheckBoxWantDebug.TabIndex = 0;
            CheckBoxWantDebug.Text = "Enable Debug Mode for Direct Child Process";
            CheckBoxWantDebug.UseVisualStyleBackColor = true;
            // 
            // tabPage4
            // 
            tabPage4.Controls.Add(label5);
            tabPage4.Controls.Add(label4);
            tabPage4.Location = new System.Drawing.Point(4, 34);
            tabPage4.Name = "tabPage4";
            tabPage4.Size = new System.Drawing.Size(768, 257);
            tabPage4.TabIndex = 3;
            tabPage4.Text = "Telemetry DLL Settings";
            tabPage4.UseVisualStyleBackColor = true;
            // 
            // label5
            // 
            label5.AutoSize = true;
            label5.Location = new System.Drawing.Point(9, 75);
            label5.Name = "label5";
            label5.Size = new System.Drawing.Size(739, 25);
            label5.TabIndex = 1;
            label5.Text = "Choose which Tele Dlls to force the process to load. Note mixing x86/x64 may cause trouble";
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new System.Drawing.Point(31, 41);
            label4.Name = "label4";
            label4.Size = new System.Drawing.Size(608, 25);
            label4.TabIndex = 0;
            label4.Text = "A telemetry DLL is a DLL specially programmed to follow a detours method";
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new System.Drawing.Point(25, 9);
            label1.Name = "label1";
            label1.Size = new System.Drawing.Size(389, 25);
            label1.TabIndex = 3;
            label1.Text = "Choose a Target Windows Executable To Launch";
            // 
            // CheckBoxDebugPriv
            // 
            CheckBoxDebugPriv.AutoSize = true;
            CheckBoxDebugPriv.Location = new System.Drawing.Point(44, 89);
            CheckBoxDebugPriv.Name = "CheckBoxDebugPriv";
            CheckBoxDebugPriv.Size = new System.Drawing.Size(296, 29);
            CheckBoxDebugPriv.TabIndex = 1;
            CheckBoxDebugPriv.Text = "Enable Debug Priv for Debugger";
            CheckBoxDebugPriv.UseVisualStyleBackColor = true;
            // 
            // checkBox1
            // 
            checkBox1.AutoSize = true;
            checkBox1.Location = new System.Drawing.Point(44, 54);
            checkBox1.Name = "checkBox1";
            checkBox1.Size = new System.Drawing.Size(431, 29);
            checkBox1.TabIndex = 2;
            checkBox1.Text = "Enable Debug mode For All other Child Processes";
            checkBox1.UseVisualStyleBackColor = true;
            // 
            // NewProcessForm
            // 
            AutoScaleDimensions = new System.Drawing.SizeF(10F, 25F);
            AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            ClientSize = new System.Drawing.Size(800, 450);
            Controls.Add(label1);
            Controls.Add(TabControlProcessCreate);
            Controls.Add(ButtonChooseProcessToSpawn);
            Controls.Add(TextBoxTargetProcess);
            Name = "NewProcessForm";
            Text = "Spawn A Process";
            TabControlProcessCreate.ResumeLayout(false);
            tabPage1.ResumeLayout(false);
            tabPage2.ResumeLayout(false);
            tabPage2.PerformLayout();
            groupBox1.ResumeLayout(false);
            groupBox1.PerformLayout();
            tabPage3.ResumeLayout(false);
            tabPage3.PerformLayout();
            tabPage4.ResumeLayout(false);
            tabPage4.PerformLayout();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private System.Windows.Forms.TextBox TextBoxTargetProcess;
        private System.Windows.Forms.Button ButtonChooseProcessToSpawn;
        private System.Windows.Forms.TabControl TabControlProcessCreate;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.TabPage tabPage4;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckedListBox checkedListBox1;
        private System.Windows.Forms.TextBox TextBoxTargetEnvFile;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton RadioButton;
        private System.Windows.Forms.RadioButton radioButton2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button ButtonChooseEnvText;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.CheckBox CheckBoxWantDebug;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.CheckBox CheckBoxDebugPriv;
        private System.Windows.Forms.CheckBox checkBox1;
    }
}