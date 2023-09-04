namespace InsightLogger
{
    partial class LogRunner_DetoursForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LogRunner_DetoursForm));
            label1 = new System.Windows.Forms.Label();
            linkLabel1 = new System.Windows.Forms.LinkLabel();
            CheckedListBoxTelemetryDlls = new System.Windows.Forms.CheckedListBox();
            label2 = new System.Windows.Forms.Label();
            CheckedListBoxAdditionDLLs = new System.Windows.Forms.CheckedListBox();
            label3 = new System.Windows.Forms.Label();
            ButtonAddNew = new System.Windows.Forms.Button();
            ButtonClearGenericChecked = new System.Windows.Forms.Button();
            ButtonOk = new System.Windows.Forms.Button();
            ButtonCancel = new System.Windows.Forms.Button();
            OpenFileDialogSelectGenericDetour = new System.Windows.Forms.OpenFileDialog();
            SuspendLayout();
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new System.Drawing.Point(12, 9);
            label1.Name = "label1";
            label1.Size = new System.Drawing.Size(464, 100);
            label1.TabIndex = 0;
            label1.Text = resources.GetString("label1.Text");
            label1.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // linkLabel1
            // 
            linkLabel1.AutoSize = true;
            linkLabel1.Location = new System.Drawing.Point(12, 109);
            linkLabel1.Name = "linkLabel1";
            linkLabel1.Size = new System.Drawing.Size(590, 25);
            linkLabel1.TabIndex = 1;
            linkLabel1.TabStop = true;
            linkLabel1.Text = "https://github.com/Microsoft/Detours/wiki/DetourCreateProcessWithDlls";
            // 
            // CheckedListBoxTelemetryDlls
            // 
            CheckedListBoxTelemetryDlls.FormattingEnabled = true;
            CheckedListBoxTelemetryDlls.Items.AddRange(new object[] { "IoDeviceTracking" });
            CheckedListBoxTelemetryDlls.Location = new System.Drawing.Point(27, 180);
            CheckedListBoxTelemetryDlls.Name = "CheckedListBoxTelemetryDlls";
            CheckedListBoxTelemetryDlls.Size = new System.Drawing.Size(319, 144);
            CheckedListBoxTelemetryDlls.TabIndex = 2;
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new System.Drawing.Point(27, 152);
            label2.Name = "label2";
            label2.Size = new System.Drawing.Size(319, 25);
            label2.TabIndex = 3;
            label2.Text = "Telemetry DLL - Debug Mode Required";
            // 
            // CheckedListBoxAdditionDLLs
            // 
            CheckedListBoxAdditionDLLs.FormattingEnabled = true;
            CheckedListBoxAdditionDLLs.Location = new System.Drawing.Point(482, 180);
            CheckedListBoxAdditionDLLs.Name = "CheckedListBoxAdditionDLLs";
            CheckedListBoxAdditionDLLs.Size = new System.Drawing.Size(295, 144);
            CheckedListBoxAdditionDLLs.TabIndex = 4;
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new System.Drawing.Point(482, 154);
            label3.Name = "label3";
            label3.Size = new System.Drawing.Size(136, 25);
            label3.TabIndex = 5;
            label3.Text = "Additional DLLs";
            label3.Click += label3_Click;
            // 
            // ButtonAddNew
            // 
            ButtonAddNew.Location = new System.Drawing.Point(482, 330);
            ButtonAddNew.Name = "ButtonAddNew";
            ButtonAddNew.Size = new System.Drawing.Size(157, 34);
            ButtonAddNew.TabIndex = 6;
            ButtonAddNew.Text = "Add New DLL";
            ButtonAddNew.UseVisualStyleBackColor = true;
            ButtonAddNew.Click += ButtonAddNew_Click;
            // 
            // ButtonClearGenericChecked
            // 
            ButtonClearGenericChecked.Location = new System.Drawing.Point(646, 330);
            ButtonClearGenericChecked.Name = "ButtonClearGenericChecked";
            ButtonClearGenericChecked.Size = new System.Drawing.Size(112, 34);
            ButtonClearGenericChecked.TabIndex = 7;
            ButtonClearGenericChecked.Text = "Clear Selected";
            ButtonClearGenericChecked.UseVisualStyleBackColor = true;
            ButtonClearGenericChecked.Click += ButtonClearNewDllList_Click;
            // 
            // ButtonOk
            // 
            ButtonOk.Location = new System.Drawing.Point(711, 19);
            ButtonOk.Name = "ButtonOk";
            ButtonOk.Size = new System.Drawing.Size(112, 34);
            ButtonOk.TabIndex = 8;
            ButtonOk.Text = "OK";
            ButtonOk.UseVisualStyleBackColor = true;
            ButtonOk.Click += ButtonOk_Click;
            // 
            // ButtonCancel
            // 
            ButtonCancel.Location = new System.Drawing.Point(711, 59);
            ButtonCancel.Name = "ButtonCancel";
            ButtonCancel.Size = new System.Drawing.Size(112, 34);
            ButtonCancel.TabIndex = 9;
            ButtonCancel.Text = "Cancel";
            ButtonCancel.UseVisualStyleBackColor = true;
            // 
            // OpenFileDialogSelectGenericDetour
            // 
            OpenFileDialogSelectGenericDetour.Filter = "Dynamic Link Libraries Files|*.DLL|All Files|*.*";
            OpenFileDialogSelectGenericDetour.FileOk += OpenFileDialogSelectGenericDetour_FileOk;
            // 
            // LogRunner_DetoursForm
            // 
            AutoScaleDimensions = new System.Drawing.SizeF(10F, 25F);
            AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            ClientSize = new System.Drawing.Size(920, 450);
            Controls.Add(ButtonCancel);
            Controls.Add(ButtonOk);
            Controls.Add(ButtonClearGenericChecked);
            Controls.Add(ButtonAddNew);
            Controls.Add(label3);
            Controls.Add(CheckedListBoxAdditionDLLs);
            Controls.Add(label2);
            Controls.Add(CheckedListBoxTelemetryDlls);
            Controls.Add(linkLabel1);
            Controls.Add(label1);
            Name = "LogRunner_DetoursForm";
            Text = "Insight Log - Detours Settings";
            Load += LogRunner_DetoursForm_Load;
            Shown += LogRunner_DetoursForm_Shown;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.LinkLabel linkLabel1;
        private System.Windows.Forms.CheckedListBox CheckedListBoxTelemetryDlls;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.CheckedListBox CheckedListBoxAdditionDLLs;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button ButtonAddNew;
        private System.Windows.Forms.Button ButtonClearGenericChecked;
        private System.Windows.Forms.Button ButtonOk;
        private System.Windows.Forms.Button ButtonCancel;
        private System.Windows.Forms.OpenFileDialog OpenFileDialogSelectGenericDetour;
    }
}