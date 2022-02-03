
namespace FileSandBox_GUI
{
    partial class DebugProcessGuiLauncher
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
            this.TextBoxTargetProcessToSpawn = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.ButtonChooseProcessShowDialog = new System.Windows.Forms.Button();
            this.ButtonChooseArgumentSourceFileButton = new System.Windows.Forms.Button();
            this.TextBoxArguments = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.CheckBoxEnableDebugPriv = new System.Windows.Forms.CheckBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.CheckBoxDirectArgument = new System.Windows.Forms.CheckBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.CheckBoxDebugTarget = new System.Windows.Forms.CheckBox();
            this.CheckBoxHostileTarget = new System.Windows.Forms.CheckBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.RadioButtonLeaveDebuggedAppRunning = new System.Windows.Forms.RadioButton();
            this.RadioButtonKillDebuggedAppOnExit = new System.Windows.Forms.RadioButton();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.label4 = new System.Windows.Forms.Label();
            this.linkLabel1 = new System.Windows.Forms.LinkLabel();
            this.label3 = new System.Windows.Forms.Label();
            this.ButtonLauchProcess = new System.Windows.Forms.Button();
            this.ButtonCancel = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.SuspendLayout();
            // 
            // TextBoxTargetProcessToSpawn
            // 
            this.TextBoxTargetProcessToSpawn.Location = new System.Drawing.Point(6, 46);
            this.TextBoxTargetProcessToSpawn.Name = "TextBoxTargetProcessToSpawn";
            this.TextBoxTargetProcessToSpawn.Size = new System.Drawing.Size(384, 27);
            this.TextBoxTargetProcessToSpawn.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(236, 20);
            this.label1.TabIndex = 1;
            this.label1.Text = "Choose the Target Program to Run";
            // 
            // ButtonChooseProcessShowDialog
            // 
            this.ButtonChooseProcessShowDialog.Location = new System.Drawing.Point(396, 46);
            this.ButtonChooseProcessShowDialog.Name = "ButtonChooseProcessShowDialog";
            this.ButtonChooseProcessShowDialog.Size = new System.Drawing.Size(81, 27);
            this.ButtonChooseProcessShowDialog.TabIndex = 2;
            this.ButtonChooseProcessShowDialog.Text = "...";
            this.ButtonChooseProcessShowDialog.UseVisualStyleBackColor = true;
            this.ButtonChooseProcessShowDialog.Click += new System.EventHandler(this.ButtonChooseProcessToSpawn_Click);
            // 
            // ButtonChooseArgumentSourceFileButton
            // 
            this.ButtonChooseArgumentSourceFileButton.Location = new System.Drawing.Point(396, 97);
            this.ButtonChooseArgumentSourceFileButton.Name = "ButtonChooseArgumentSourceFileButton";
            this.ButtonChooseArgumentSourceFileButton.Size = new System.Drawing.Size(81, 29);
            this.ButtonChooseArgumentSourceFileButton.TabIndex = 3;
            this.ButtonChooseArgumentSourceFileButton.Text = "From File";
            this.ButtonChooseArgumentSourceFileButton.UseVisualStyleBackColor = true;
            // 
            // TextBoxArguments
            // 
            this.TextBoxArguments.Location = new System.Drawing.Point(6, 99);
            this.TextBoxArguments.Name = "TextBoxArguments";
            this.TextBoxArguments.Size = new System.Drawing.Size(384, 27);
            this.TextBoxArguments.TabIndex = 4;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 76);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(273, 20);
            this.label2.TabIndex = 5;
            this.label2.Text = "Enter Arguments or load from a text file";
            // 
            // CheckBoxEnableDebugPriv
            // 
            this.CheckBoxEnableDebugPriv.AutoSize = true;
            this.CheckBoxEnableDebugPriv.Location = new System.Drawing.Point(6, 26);
            this.CheckBoxEnableDebugPriv.Name = "CheckBoxEnableDebugPriv";
            this.CheckBoxEnableDebugPriv.Size = new System.Drawing.Size(154, 24);
            this.CheckBoxEnableDebugPriv.TabIndex = 6;
            this.CheckBoxEnableDebugPriv.Text = "Ask for Debug Priv";
            this.CheckBoxEnableDebugPriv.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.CheckBoxDirectArgument);
            this.groupBox1.Controls.Add(this.TextBoxArguments);
            this.groupBox1.Controls.Add(this.ButtonChooseArgumentSourceFileButton);
            this.groupBox1.Controls.Add(this.ButtonChooseProcessShowDialog);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.TextBoxTargetProcessToSpawn);
            this.groupBox1.Location = new System.Drawing.Point(19, 91);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(502, 188);
            this.groupBox1.TabIndex = 7;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Target Program and Args";
            // 
            // CheckBoxDirectArgument
            // 
            this.CheckBoxDirectArgument.AutoSize = true;
            this.CheckBoxDirectArgument.Location = new System.Drawing.Point(6, 132);
            this.CheckBoxDirectArgument.Name = "CheckBoxDirectArgument";
            this.CheckBoxDirectArgument.Size = new System.Drawing.Size(184, 24);
            this.CheckBoxDirectArgument.TabIndex = 6;
            this.CheckBoxDirectArgument.Text = "Type Argument Instead";
            this.CheckBoxDirectArgument.UseVisualStyleBackColor = true;
            this.CheckBoxDirectArgument.CheckedChanged += new System.EventHandler(this.CheckBoxDirectArgument_CheckedChanged);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.CheckBoxDebugTarget);
            this.groupBox2.Controls.Add(this.CheckBoxHostileTarget);
            this.groupBox2.Controls.Add(this.groupBox3);
            this.groupBox2.Controls.Add(this.CheckBoxEnableDebugPriv);
            this.groupBox2.Location = new System.Drawing.Point(12, 285);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(509, 125);
            this.groupBox2.TabIndex = 8;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Debugger Settings";
            // 
            // CheckBoxDebugTarget
            // 
            this.CheckBoxDebugTarget.AutoSize = true;
            this.CheckBoxDebugTarget.Location = new System.Drawing.Point(7, 87);
            this.CheckBoxDebugTarget.Name = "CheckBoxDebugTarget";
            this.CheckBoxDebugTarget.Size = new System.Drawing.Size(121, 24);
            this.CheckBoxDebugTarget.TabIndex = 11;
            this.CheckBoxDebugTarget.Text = "Debug Target";
            this.CheckBoxDebugTarget.UseVisualStyleBackColor = true;
            this.CheckBoxDebugTarget.CheckedChanged += new System.EventHandler(this.CheckBoxDebugTarget_CheckedChanged);
            // 
            // CheckBoxHostileTarget
            // 
            this.CheckBoxHostileTarget.AutoSize = true;
            this.CheckBoxHostileTarget.Location = new System.Drawing.Point(6, 56);
            this.CheckBoxHostileTarget.Name = "CheckBoxHostileTarget";
            this.CheckBoxHostileTarget.Size = new System.Drawing.Size(123, 24);
            this.CheckBoxHostileTarget.TabIndex = 10;
            this.CheckBoxHostileTarget.Text = "Hostile Target";
            this.CheckBoxHostileTarget.UseVisualStyleBackColor = true;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.RadioButtonLeaveDebuggedAppRunning);
            this.groupBox3.Controls.Add(this.RadioButtonKillDebuggedAppOnExit);
            this.groupBox3.Location = new System.Drawing.Point(184, 26);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(251, 79);
            this.groupBox3.TabIndex = 9;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "When Debugger Quits (Effects All)";
            // 
            // RadioButtonLeaveDebuggedAppRunning
            // 
            this.RadioButtonLeaveDebuggedAppRunning.AutoSize = true;
            this.RadioButtonLeaveDebuggedAppRunning.Location = new System.Drawing.Point(17, 52);
            this.RadioButtonLeaveDebuggedAppRunning.Name = "RadioButtonLeaveDebuggedAppRunning";
            this.RadioButtonLeaveDebuggedAppRunning.Size = new System.Drawing.Size(208, 24);
            this.RadioButtonLeaveDebuggedAppRunning.TabIndex = 1;
            this.RadioButtonLeaveDebuggedAppRunning.Text = "Debugged App Stays Alive";
            this.RadioButtonLeaveDebuggedAppRunning.UseVisualStyleBackColor = true;
            // 
            // RadioButtonKillDebuggedAppOnExit
            // 
            this.RadioButtonKillDebuggedAppOnExit.AutoSize = true;
            this.RadioButtonKillDebuggedAppOnExit.Checked = true;
            this.RadioButtonKillDebuggedAppOnExit.Location = new System.Drawing.Point(17, 21);
            this.RadioButtonKillDebuggedAppOnExit.Name = "RadioButtonKillDebuggedAppOnExit";
            this.RadioButtonKillDebuggedAppOnExit.Size = new System.Drawing.Size(226, 24);
            this.RadioButtonKillDebuggedAppOnExit.TabIndex = 0;
            this.RadioButtonKillDebuggedAppOnExit.TabStop = true;
            this.RadioButtonKillDebuggedAppOnExit.Text = "Debugged App is Terminated";
            this.RadioButtonKillDebuggedAppOnExit.UseVisualStyleBackColor = true;
            this.RadioButtonKillDebuggedAppOnExit.CheckedChanged += new System.EventHandler(this.radioButton1_CheckedChanged);
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.label4);
            this.groupBox5.Controls.Add(this.linkLabel1);
            this.groupBox5.Controls.Add(this.label3);
            this.groupBox5.Location = new System.Drawing.Point(12, 427);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(506, 194);
            this.groupBox5.TabIndex = 10;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Detour Helper Dll Loading";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(7, 38);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(539, 20);
            this.label4.TabIndex = 2;
            this.label4.Text = "Should you choose a Dll that does not offer that, your program will not function." +
    "";
            // 
            // linkLabel1
            // 
            this.linkLabel1.AutoSize = true;
            this.linkLabel1.Location = new System.Drawing.Point(6, 66);
            this.linkLabel1.Name = "linkLabel1";
            this.linkLabel1.Size = new System.Drawing.Size(491, 20);
            this.linkLabel1.TabIndex = 1;
            this.linkLabel1.TabStop = true;
            this.linkLabel1.Text = "https://github.com/Microsoft/Detours/wiki/DetourCreateProcessWithDlls";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(3, 23);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(455, 20);
            this.label3.TabIndex = 0;
            this.label3.Text = "IMPORTANT. You Dll Should match how Detours Describes Loading.";
            // 
            // ButtonLauchProcess
            // 
            this.ButtonLauchProcess.Location = new System.Drawing.Point(415, 12);
            this.ButtonLauchProcess.Name = "ButtonLauchProcess";
            this.ButtonLauchProcess.Size = new System.Drawing.Size(94, 29);
            this.ButtonLauchProcess.TabIndex = 11;
            this.ButtonLauchProcess.Text = "Launch";
            this.ButtonLauchProcess.UseVisualStyleBackColor = true;
            // 
            // ButtonCancel
            // 
            this.ButtonCancel.Location = new System.Drawing.Point(415, 47);
            this.ButtonCancel.Name = "ButtonCancel";
            this.ButtonCancel.Size = new System.Drawing.Size(94, 29);
            this.ButtonCancel.TabIndex = 12;
            this.ButtonCancel.Text = "Cancel";
            this.ButtonCancel.UseVisualStyleBackColor = true;
            // 
            // DebugProcessGuiLauncher
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(549, 714);
            this.Controls.Add(this.ButtonCancel);
            this.Controls.Add(this.ButtonLauchProcess);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.Name = "DebugProcessGuiLauncher";
            this.ShowInTaskbar = false;
            this.Text = "Launch New Process to Debug / Monitor";
            this.Load += new System.EventHandler(this.DebugProcessGuiLauncher_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TextBox TextBoxTargetProcessToSpawn;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button ButtonChooseProcessShowDialog;
        private System.Windows.Forms.Button ButtonChooseArgumentSourceFileButton;
        private System.Windows.Forms.TextBox TextBoxArguments;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.CheckBox CheckBoxEnableDebugPriv;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.CheckBox CheckBoxHostileTarget;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.LinkLabel linkLabel1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.RadioButton RadioButtonLeaveDebuggedAppRunning;
        private System.Windows.Forms.RadioButton RadioButtonKillDebuggedAppOnExit;
        private System.Windows.Forms.CheckBox CheckBoxDirectArgument;
        private System.Windows.Forms.Button ButtonLauchProcess;
        private System.Windows.Forms.Button ButtonCancel;
        private System.Windows.Forms.CheckBox CheckBoxDebugTarget;
    }
}