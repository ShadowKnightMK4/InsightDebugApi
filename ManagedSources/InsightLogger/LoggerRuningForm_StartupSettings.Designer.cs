namespace InsightLogger
{
    partial class LoggerRunForm_StartupSettings
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LoggerRunForm_StartupSettings));
            groupBox1 = new System.Windows.Forms.GroupBox();
            ButtonSelectEnvTextFile = new System.Windows.Forms.Button();
            TextBoxEnvTextFile = new System.Windows.Forms.TextBox();
            label1 = new System.Windows.Forms.Label();
            RadioButtonInheritEnv = new System.Windows.Forms.RadioButton();
            RadioButtonExplicitEnv = new System.Windows.Forms.RadioButton();
            OpenFileDialogSelectEnvFile = new System.Windows.Forms.OpenFileDialog();
            TextBoxProcessArguments = new System.Windows.Forms.TextBox();
            label2 = new System.Windows.Forms.Label();
            label3 = new System.Windows.Forms.Label();
            TextBoxProcessStartingDir = new System.Windows.Forms.TextBox();
            ButtonSelectStartingDir = new System.Windows.Forms.Button();
            FolderBrowserDialogSelectProcessStartDir = new System.Windows.Forms.FolderBrowserDialog();
            ButtonOK = new System.Windows.Forms.Button();
            ButtonCancel = new System.Windows.Forms.Button();
            textBox1 = new System.Windows.Forms.TextBox();
            label4 = new System.Windows.Forms.Label();
            groupBox2 = new System.Windows.Forms.GroupBox();
            checkBox1 = new System.Windows.Forms.CheckBox();
            checkBox2 = new System.Windows.Forms.CheckBox();
            groupBox1.SuspendLayout();
            groupBox2.SuspendLayout();
            SuspendLayout();
            // 
            // groupBox1
            // 
            groupBox1.Controls.Add(ButtonSelectEnvTextFile);
            groupBox1.Controls.Add(TextBoxEnvTextFile);
            groupBox1.Controls.Add(label1);
            groupBox1.Controls.Add(RadioButtonInheritEnv);
            groupBox1.Controls.Add(RadioButtonExplicitEnv);
            groupBox1.Location = new System.Drawing.Point(12, 32);
            groupBox1.Name = "groupBox1";
            groupBox1.Size = new System.Drawing.Size(494, 369);
            groupBox1.TabIndex = 0;
            groupBox1.TabStop = false;
            groupBox1.Text = "Environment Variables";
            // 
            // ButtonSelectEnvTextFile
            // 
            ButtonSelectEnvTextFile.Location = new System.Drawing.Point(396, 300);
            ButtonSelectEnvTextFile.Name = "ButtonSelectEnvTextFile";
            ButtonSelectEnvTextFile.Size = new System.Drawing.Size(65, 34);
            ButtonSelectEnvTextFile.TabIndex = 4;
            ButtonSelectEnvTextFile.Text = "...";
            ButtonSelectEnvTextFile.UseVisualStyleBackColor = true;
            ButtonSelectEnvTextFile.Click += ButtonSelectEnvTextFile_Click;
            // 
            // TextBoxEnvTextFile
            // 
            TextBoxEnvTextFile.Location = new System.Drawing.Point(15, 300);
            TextBoxEnvTextFile.Name = "TextBoxEnvTextFile";
            TextBoxEnvTextFile.Size = new System.Drawing.Size(375, 31);
            TextBoxEnvTextFile.TabIndex = 3;
            TextBoxEnvTextFile.Validating += TextBoxEnvTextFile_Validating;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new System.Drawing.Point(15, 29);
            label1.Name = "label1";
            label1.Size = new System.Drawing.Size(375, 175);
            label1.TabIndex = 2;
            label1.Text = resources.GetString("label1.Text");
            // 
            // RadioButtonInheritEnv
            // 
            RadioButtonInheritEnv.AutoSize = true;
            RadioButtonInheritEnv.Location = new System.Drawing.Point(15, 251);
            RadioButtonInheritEnv.Name = "RadioButtonInheritEnv";
            RadioButtonInheritEnv.Size = new System.Drawing.Size(318, 29);
            RadioButtonInheritEnv.TabIndex = 1;
            RadioButtonInheritEnv.TabStop = true;
            RadioButtonInheritEnv.Text = "Inherit Logger Enviroment Variables";
            RadioButtonInheritEnv.UseVisualStyleBackColor = true;
            // 
            // RadioButtonExplicitEnv
            // 
            RadioButtonExplicitEnv.AutoSize = true;
            RadioButtonExplicitEnv.Location = new System.Drawing.Point(15, 216);
            RadioButtonExplicitEnv.Name = "RadioButtonExplicitEnv";
            RadioButtonExplicitEnv.Size = new System.Drawing.Size(275, 29);
            RadioButtonExplicitEnv.TabIndex = 0;
            RadioButtonExplicitEnv.TabStop = true;
            RadioButtonExplicitEnv.Text = "Only Explicit Defined Variables";
            RadioButtonExplicitEnv.UseVisualStyleBackColor = true;
            // 
            // OpenFileDialogSelectEnvFile
            // 
            OpenFileDialogSelectEnvFile.DefaultExt = "*.TXT";
            OpenFileDialogSelectEnvFile.Filter = "Unicode Text Files|*.txt|All Files|*.txt";
            OpenFileDialogSelectEnvFile.FileOk += OpenFileDialogSelectEnvFile_FileOk;
            // 
            // TextBoxProcessArguments
            // 
            TextBoxProcessArguments.Location = new System.Drawing.Point(525, 72);
            TextBoxProcessArguments.Name = "TextBoxProcessArguments";
            TextBoxProcessArguments.Size = new System.Drawing.Size(519, 31);
            TextBoxProcessArguments.TabIndex = 1;
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new System.Drawing.Point(525, 44);
            label2.Name = "label2";
            label2.Size = new System.Drawing.Size(316, 25);
            label2.TabIndex = 2;
            label2.Text = "Specify your Process's arguments here";
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new System.Drawing.Point(512, 113);
            label3.Name = "label3";
            label3.Size = new System.Drawing.Size(417, 25);
            label3.TabIndex = 3;
            label3.Text = "Indicate your Process's initial starting directory here";
            // 
            // TextBoxProcessStartingDir
            // 
            TextBoxProcessStartingDir.Location = new System.Drawing.Point(523, 141);
            TextBoxProcessStartingDir.Name = "TextBoxProcessStartingDir";
            TextBoxProcessStartingDir.Size = new System.Drawing.Size(521, 31);
            TextBoxProcessStartingDir.TabIndex = 4;
            // 
            // ButtonSelectStartingDir
            // 
            ButtonSelectStartingDir.Location = new System.Drawing.Point(1050, 141);
            ButtonSelectStartingDir.Name = "ButtonSelectStartingDir";
            ButtonSelectStartingDir.Size = new System.Drawing.Size(60, 34);
            ButtonSelectStartingDir.TabIndex = 5;
            ButtonSelectStartingDir.Text = "...";
            ButtonSelectStartingDir.UseVisualStyleBackColor = true;
            ButtonSelectStartingDir.Click += ButtonSelectStartingDir_Click;
            // 
            // FolderBrowserDialogSelectProcessStartDir
            // 
            FolderBrowserDialogSelectProcessStartDir.InitialDirectory = "C:\\Windows\\system32";
            // 
            // ButtonOK
            // 
            ButtonOK.Location = new System.Drawing.Point(952, 404);
            ButtonOK.Name = "ButtonOK";
            ButtonOK.Size = new System.Drawing.Size(112, 34);
            ButtonOK.TabIndex = 6;
            ButtonOK.Text = "Ok";
            ButtonOK.UseVisualStyleBackColor = true;
            ButtonOK.Click += ButtonOK_Click;
            // 
            // ButtonCancel
            // 
            ButtonCancel.Location = new System.Drawing.Point(817, 404);
            ButtonCancel.Name = "ButtonCancel";
            ButtonCancel.Size = new System.Drawing.Size(112, 34);
            ButtonCancel.TabIndex = 7;
            ButtonCancel.Text = "Cancel";
            ButtonCancel.UseVisualStyleBackColor = true;
            ButtonCancel.Click += ButtonCancel_Click;
            // 
            // textBox1
            // 
            textBox1.Location = new System.Drawing.Point(19, 70);
            textBox1.Name = "textBox1";
            textBox1.Size = new System.Drawing.Size(264, 31);
            textBox1.TabIndex = 8;
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new System.Drawing.Point(19, 33);
            label4.Name = "label4";
            label4.Size = new System.Drawing.Size(264, 25);
            label4.TabIndex = 9;
            label4.Text = "Process Creation Settings Value ";
            // 
            // groupBox2
            // 
            groupBox2.Controls.Add(checkBox2);
            groupBox2.Controls.Add(checkBox1);
            groupBox2.Controls.Add(label4);
            groupBox2.Controls.Add(textBox1);
            groupBox2.Location = new System.Drawing.Point(525, 190);
            groupBox2.Name = "groupBox2";
            groupBox2.Size = new System.Drawing.Size(511, 114);
            groupBox2.TabIndex = 10;
            groupBox2.TabStop = false;
            groupBox2.Text = "Programmer/Dev";
            // 
            // checkBox1
            // 
            checkBox1.AutoSize = true;
            checkBox1.Location = new System.Drawing.Point(309, 29);
            checkBox1.Name = "checkBox1";
            checkBox1.Size = new System.Drawing.Size(144, 29);
            checkBox1.TabIndex = 10;
            checkBox1.Text = "Debug Mode";
            checkBox1.UseVisualStyleBackColor = true;
            // 
            // checkBox2
            // 
            checkBox2.AutoSize = true;
            checkBox2.Location = new System.Drawing.Point(309, 55);
            checkBox2.Name = "checkBox2";
            checkBox2.Size = new System.Drawing.Size(126, 29);
            checkBox2.TabIndex = 11;
            checkBox2.Text = "Debug Priv";
            checkBox2.UseVisualStyleBackColor = true;
            // 
            // LoggerRunForm_StartupSettings
            // 
            AutoScaleDimensions = new System.Drawing.SizeF(10F, 25F);
            AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            ClientSize = new System.Drawing.Size(1122, 450);
            Controls.Add(groupBox2);
            Controls.Add(ButtonCancel);
            Controls.Add(ButtonOK);
            Controls.Add(ButtonSelectStartingDir);
            Controls.Add(TextBoxProcessStartingDir);
            Controls.Add(label3);
            Controls.Add(label2);
            Controls.Add(TextBoxProcessArguments);
            Controls.Add(groupBox1);
            Name = "LoggerRunForm_StartupSettings";
            Text = "Insight Logger - Customize Startup Settings";
            groupBox1.ResumeLayout(false);
            groupBox1.PerformLayout();
            groupBox2.ResumeLayout(false);
            groupBox2.PerformLayout();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton RadioButtonInheritEnv;
        private System.Windows.Forms.RadioButton RadioButtonExplicitEnv;
        private System.Windows.Forms.Button ButtonSelectEnvTextFile;
        private System.Windows.Forms.TextBox TextBoxEnvTextFile;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.OpenFileDialog OpenFileDialogSelectEnvFile;
        private System.Windows.Forms.TextBox TextBoxProcessArguments;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox TextBoxProcessStartingDir;
        private System.Windows.Forms.Button ButtonSelectStartingDir;
        private System.Windows.Forms.FolderBrowserDialog FolderBrowserDialogSelectProcessStartDir;
        private System.Windows.Forms.Button ButtonOK;
        private System.Windows.Forms.Button ButtonCancel;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.CheckBox checkBox2;
        private System.Windows.Forms.CheckBox checkBox1;
    }
}