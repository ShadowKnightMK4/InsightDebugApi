namespace InsightLogger
{
    partial class InsightLogger_ActiveLogDialog
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
            ListBoxLogEvent = new System.Windows.Forms.ListBox();
            SaveFileDialogOnGoing = new System.Windows.Forms.SaveFileDialog();
            SaveFileDialogSelectedEvents = new System.Windows.Forms.SaveFileDialog();
            ButtonSelectedLogSave = new System.Windows.Forms.Button();
            ButtonSaveAll = new System.Windows.Forms.Button();
            CheckBoxCloseKillsSpawnedApp = new System.Windows.Forms.CheckBox();
            TimerPulseDebugEventKick = new System.Windows.Forms.Timer(components);
            vScrollBar1 = new System.Windows.Forms.VScrollBar();
            TextBoxEventLogTimerMilli = new System.Windows.Forms.TextBox();
            label1 = new System.Windows.Forms.Label();
            button1 = new System.Windows.Forms.Button();
            SaveFileDialogAllEntriesExport = new System.Windows.Forms.SaveFileDialog();
            CheckBoxStopNextEvent = new System.Windows.Forms.CheckBox();
            ButtonReinterpretLog = new System.Windows.Forms.Button();
            SuspendLayout();
            // 
            // ListBoxLogEvent
            // 
            ListBoxLogEvent.FormattingEnabled = true;
            ListBoxLogEvent.ItemHeight = 25;
            ListBoxLogEvent.Location = new System.Drawing.Point(-3, 48);
            ListBoxLogEvent.Name = "ListBoxLogEvent";
            ListBoxLogEvent.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            ListBoxLogEvent.Size = new System.Drawing.Size(1441, 554);
            ListBoxLogEvent.TabIndex = 0;
            // 
            // SaveFileDialogSelectedEvents
            // 
            SaveFileDialogSelectedEvents.FileOk += SaveFileDialogSelectedEvents_FileOk;
            // 
            // ButtonSelectedLogSave
            // 
            ButtonSelectedLogSave.Location = new System.Drawing.Point(1611, 23);
            ButtonSelectedLogSave.Name = "ButtonSelectedLogSave";
            ButtonSelectedLogSave.Size = new System.Drawing.Size(202, 34);
            ButtonSelectedLogSave.TabIndex = 1;
            ButtonSelectedLogSave.Text = "Export Selected Entries";
            ButtonSelectedLogSave.UseVisualStyleBackColor = true;
            ButtonSelectedLogSave.Click += ButtonSelectedLogSave_Click;
            // 
            // ButtonSaveAll
            // 
            ButtonSaveAll.Location = new System.Drawing.Point(1611, 63);
            ButtonSaveAll.Name = "ButtonSaveAll";
            ButtonSaveAll.Size = new System.Drawing.Size(202, 34);
            ButtonSaveAll.TabIndex = 2;
            ButtonSaveAll.Text = "Save All Entries Listed";
            ButtonSaveAll.UseVisualStyleBackColor = true;
            ButtonSaveAll.Click += ButtonSaveAll_Click;
            // 
            // CheckBoxCloseKillsSpawnedApp
            // 
            CheckBoxCloseKillsSpawnedApp.AutoSize = true;
            CheckBoxCloseKillsSpawnedApp.Location = new System.Drawing.Point(1476, 143);
            CheckBoxCloseKillsSpawnedApp.Name = "CheckBoxCloseKillsSpawnedApp";
            CheckBoxCloseKillsSpawnedApp.Size = new System.Drawing.Size(341, 54);
            CheckBoxCloseKillsSpawnedApp.TabIndex = 3;
            CheckBoxCloseKillsSpawnedApp.Text = "When Closing this Window, terminate \r\nthe launched app";
            CheckBoxCloseKillsSpawnedApp.UseVisualStyleBackColor = true;
            // 
            // TimerPulseDebugEventKick
            // 
            TimerPulseDebugEventKick.Enabled = true;
            TimerPulseDebugEventKick.Tick += TimerPulseDebugEventKick_Tick;
            // 
            // vScrollBar1
            // 
            vScrollBar1.Location = new System.Drawing.Point(1678, 228);
            vScrollBar1.Name = "vScrollBar1";
            vScrollBar1.Size = new System.Drawing.Size(34, 31);
            vScrollBar1.TabIndex = 4;
            vScrollBar1.Scroll += vScrollBar1_Scroll;
            // 
            // TextBoxEventLogTimerMilli
            // 
            TextBoxEventLogTimerMilli.Location = new System.Drawing.Point(1525, 228);
            TextBoxEventLogTimerMilli.Name = "TextBoxEventLogTimerMilli";
            TextBoxEventLogTimerMilli.Size = new System.Drawing.Size(150, 31);
            TextBoxEventLogTimerMilli.TabIndex = 5;
            TextBoxEventLogTimerMilli.TextChanged += TextBoxEventLogTimerMilli_TextChanged;
            TextBoxEventLogTimerMilli.Validating += TextBoxEventLogTimerMilli_Validating;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new System.Drawing.Point(1476, 200);
            label1.Name = "label1";
            label1.Size = new System.Drawing.Size(315, 25);
            label1.TabIndex = 6;
            label1.Text = "How quick to update debug event log";
            // 
            // button1
            // 
            button1.Location = new System.Drawing.Point(1611, 103);
            button1.Name = "button1";
            button1.Size = new System.Drawing.Size(206, 34);
            button1.TabIndex = 7;
            button1.Text = "Choose Ongoing Log";
            button1.UseVisualStyleBackColor = true;
            // 
            // SaveFileDialogAllEntriesExport
            // 
            SaveFileDialogAllEntriesExport.FileOk += SaveFileDialogAllEntriesExport_FileOk;
            // 
            // CheckBoxStopNextEvent
            // 
            CheckBoxStopNextEvent.AutoSize = true;
            CheckBoxStopNextEvent.Location = new System.Drawing.Point(1491, 265);
            CheckBoxStopNextEvent.Name = "CheckBoxStopNextEvent";
            CheckBoxStopNextEvent.Size = new System.Drawing.Size(221, 29);
            CheckBoxStopNextEvent.TabIndex = 8;
            CheckBoxStopNextEvent.Text = "Pause Event Processing";
            CheckBoxStopNextEvent.UseVisualStyleBackColor = true;
            // 
            // ButtonReinterpretLog
            // 
            ButtonReinterpretLog.Location = new System.Drawing.Point(2, 12);
            ButtonReinterpretLog.Name = "ButtonReinterpretLog";
            ButtonReinterpretLog.Size = new System.Drawing.Size(245, 34);
            ButtonReinterpretLog.TabIndex = 9;
            ButtonReinterpretLog.Text = "Change Display Format";
            ButtonReinterpretLog.UseVisualStyleBackColor = true;
            ButtonReinterpretLog.Click += ButtonReinterpretLog_Click;
            // 
            // InsightLogger_ActiveLogDialog
            // 
            AutoScaleDimensions = new System.Drawing.SizeF(10F, 25F);
            AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            ClientSize = new System.Drawing.Size(1825, 613);
            Controls.Add(ButtonReinterpretLog);
            Controls.Add(CheckBoxStopNextEvent);
            Controls.Add(button1);
            Controls.Add(label1);
            Controls.Add(TextBoxEventLogTimerMilli);
            Controls.Add(vScrollBar1);
            Controls.Add(CheckBoxCloseKillsSpawnedApp);
            Controls.Add(ButtonSaveAll);
            Controls.Add(ButtonSelectedLogSave);
            Controls.Add(ListBoxLogEvent);
            Name = "InsightLogger_ActiveLogDialog";
            Text = "InsightLogger - Active Log";
            Load += InsightLogger_ActiveLogDialog_Load;
            VisibleChanged += InsightLogger_ActiveLogDialog_VisibleChanged;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private System.Windows.Forms.ListBox ListBoxLogEvent;
        private System.Windows.Forms.SaveFileDialog SaveFileDialogOnGoing;
        private System.Windows.Forms.SaveFileDialog SaveFileDialogSelectedEvents;
        private System.Windows.Forms.Button ButtonSelectedLogSave;
        private System.Windows.Forms.Button ButtonSaveAll;
        private System.Windows.Forms.CheckBox CheckBoxCloseKillsSpawnedApp;
        private System.Windows.Forms.Timer TimerPulseDebugEventKick;
        private System.Windows.Forms.VScrollBar vScrollBar1;
        private System.Windows.Forms.TextBox TextBoxEventLogTimerMilli;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.SaveFileDialog SaveFileDialogAllEntriesExport;
        private System.Windows.Forms.CheckBox CheckBoxStopNextEvent;
        private System.Windows.Forms.Button ButtonReinterpretLog;
    }
}