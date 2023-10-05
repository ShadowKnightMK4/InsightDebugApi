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
            ToolTipLogEntryViewHint = new System.Windows.Forms.ToolTip(components);
            MenuStripActiveLogWindow = new System.Windows.Forms.MenuStrip();
            fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            closeThisToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            logSettingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            chooseDisppToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            chooseFriendlyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            chooseJSONToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            choosePassThruToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            exportSelectedLogToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            exportAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            exportAsJSONToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            exportAsSimpleEnglishToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            exportLogToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            exportAsPassThruToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            exportAsJSONToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            exportAsBasicEnglishToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
            debugEventToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            logCreateProcessToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            logCreateThreadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            logGeneralExceptionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            logExitProcessEventToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            logExitThreadEventToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            loadLoadDllEventToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            logDebugStringEventToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            logRipEventToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            loadUnloadDllEventToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
            logAllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            logNoneToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            debugEventsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            fullStopNextEventToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            toolStripMenuItem4 = new System.Windows.Forms.ToolStripSeparator();
            exceptionHandlingToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            secondChangeExceptionHandlingToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            SaveFileDialog_JSON_SelectedEntryExport = new System.Windows.Forms.SaveFileDialog();
            SaveFileDialog_SimpleEnglish_SelectedExport = new System.Windows.Forms.SaveFileDialog();
            SaveFileDialog_PassThru_SelectedExport = new System.Windows.Forms.SaveFileDialog();
            SaveFileDialog_PassThru_ExportAll = new System.Windows.Forms.SaveFileDialog();
            SaveFileDialog_SimpleEnglish_ExportAll = new System.Windows.Forms.SaveFileDialog();
            SaveFileDialog_JSON_ExportAll = new System.Windows.Forms.SaveFileDialog();
            MenuStripActiveLogWindow.SuspendLayout();
            SuspendLayout();
            // 
            // ListBoxLogEvent
            // 
            ListBoxLogEvent.FormattingEnabled = true;
            ListBoxLogEvent.ItemHeight = 25;
            ListBoxLogEvent.Location = new System.Drawing.Point(0, 77);
            ListBoxLogEvent.Name = "ListBoxLogEvent";
            ListBoxLogEvent.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            ListBoxLogEvent.Size = new System.Drawing.Size(1441, 554);
            ListBoxLogEvent.TabIndex = 0;
            ListBoxLogEvent.SelectedIndexChanged += ListBoxLogEvent_SelectedIndexChanged;
            ListBoxLogEvent.MouseMove += ListBoxLogEvent_MouseMove;
            // 
            // SaveFileDialogSelectedEvents
            // 
            SaveFileDialogSelectedEvents.FileOk += SaveFileDialogSelectedEvents_FileOk;
            // 
            // ButtonSelectedLogSave
            // 
            ButtonSelectedLogSave.Location = new System.Drawing.Point(1473, 63);
            ButtonSelectedLogSave.Name = "ButtonSelectedLogSave";
            ButtonSelectedLogSave.Size = new System.Drawing.Size(202, 34);
            ButtonSelectedLogSave.TabIndex = 1;
            ButtonSelectedLogSave.Text = "Export Selected Entries";
            ButtonSelectedLogSave.UseVisualStyleBackColor = true;
            ButtonSelectedLogSave.Click += ButtonSelectedLogSave_Click;
            // 
            // ButtonSaveAll
            // 
            ButtonSaveAll.Location = new System.Drawing.Point(1476, 103);
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
            button1.Location = new System.Drawing.Point(1476, 300);
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
            ButtonReinterpretLog.Location = new System.Drawing.Point(1447, 448);
            ButtonReinterpretLog.Name = "ButtonReinterpretLog";
            ButtonReinterpretLog.Size = new System.Drawing.Size(245, 34);
            ButtonReinterpretLog.TabIndex = 9;
            ButtonReinterpretLog.Text = "Change Display Format";
            ButtonReinterpretLog.UseVisualStyleBackColor = true;
            ButtonReinterpretLog.Click += ButtonReinterpretLog_Click;
            // 
            // MenuStripActiveLogWindow
            // 
            MenuStripActiveLogWindow.ImageScalingSize = new System.Drawing.Size(24, 24);
            MenuStripActiveLogWindow.Items.AddRange(new System.Windows.Forms.ToolStripItem[] { fileToolStripMenuItem, logSettingsToolStripMenuItem, debugEventsToolStripMenuItem });
            MenuStripActiveLogWindow.Location = new System.Drawing.Point(0, 0);
            MenuStripActiveLogWindow.Name = "MenuStripActiveLogWindow";
            MenuStripActiveLogWindow.Size = new System.Drawing.Size(1825, 33);
            MenuStripActiveLogWindow.TabIndex = 10;
            MenuStripActiveLogWindow.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] { closeThisToolStripMenuItem });
            fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            fileToolStripMenuItem.Size = new System.Drawing.Size(54, 29);
            fileToolStripMenuItem.Text = "&File";
            // 
            // closeThisToolStripMenuItem
            // 
            closeThisToolStripMenuItem.Name = "closeThisToolStripMenuItem";
            closeThisToolStripMenuItem.Size = new System.Drawing.Size(193, 34);
            closeThisToolStripMenuItem.Text = "Close This";
            // 
            // logSettingsToolStripMenuItem
            // 
            logSettingsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] { toolStripMenuItem1, chooseDisppToolStripMenuItem, exportSelectedLogToolStripMenuItem, exportLogToolStripMenuItem, toolStripMenuItem2, debugEventToolStripMenuItem });
            logSettingsToolStripMenuItem.Name = "logSettingsToolStripMenuItem";
            logSettingsToolStripMenuItem.Size = new System.Drawing.Size(155, 29);
            logSettingsToolStripMenuItem.Text = "Log Commands";
            logSettingsToolStripMenuItem.Click += logSettingsToolStripMenuItem_Click;
            // 
            // toolStripMenuItem1
            // 
            toolStripMenuItem1.Name = "toolStripMenuItem1";
            toolStripMenuItem1.Size = new System.Drawing.Size(331, 6);
            toolStripMenuItem1.Click += toolStripMenuItem1_Click;
            // 
            // chooseDisppToolStripMenuItem
            // 
            chooseDisppToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] { chooseFriendlyToolStripMenuItem, chooseJSONToolStripMenuItem, choosePassThruToolStripMenuItem });
            chooseDisppToolStripMenuItem.Name = "chooseDisppToolStripMenuItem";
            chooseDisppToolStripMenuItem.Size = new System.Drawing.Size(334, 34);
            chooseDisppToolStripMenuItem.Text = "Choose Display Log Format";
            chooseDisppToolStripMenuItem.Click += chooseDisppToolStripMenuItem_Click;
            // 
            // chooseFriendlyToolStripMenuItem
            // 
            chooseFriendlyToolStripMenuItem.Name = "chooseFriendlyToolStripMenuItem";
            chooseFriendlyToolStripMenuItem.Size = new System.Drawing.Size(253, 34);
            chooseFriendlyToolStripMenuItem.Text = "Choose Friendly";
            chooseFriendlyToolStripMenuItem.Click += chooseFriendlyToolStripMenuItem_Click;
            // 
            // chooseJSONToolStripMenuItem
            // 
            chooseJSONToolStripMenuItem.Name = "chooseJSONToolStripMenuItem";
            chooseJSONToolStripMenuItem.Size = new System.Drawing.Size(253, 34);
            chooseJSONToolStripMenuItem.Text = "Choose JSON";
            chooseJSONToolStripMenuItem.Click += chooseJSONToolStripMenuItem_Click;
            // 
            // choosePassThruToolStripMenuItem
            // 
            choosePassThruToolStripMenuItem.Name = "choosePassThruToolStripMenuItem";
            choosePassThruToolStripMenuItem.Size = new System.Drawing.Size(253, 34);
            choosePassThruToolStripMenuItem.Text = "Choose Pass Thru";
            choosePassThruToolStripMenuItem.Click += choosePassThruToolStripMenuItem_Click;
            // 
            // exportSelectedLogToolStripMenuItem
            // 
            exportSelectedLogToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] { exportAsToolStripMenuItem, exportAsJSONToolStripMenuItem1, exportAsSimpleEnglishToolStripMenuItem });
            exportSelectedLogToolStripMenuItem.Name = "exportSelectedLogToolStripMenuItem";
            exportSelectedLogToolStripMenuItem.Size = new System.Drawing.Size(334, 34);
            exportSelectedLogToolStripMenuItem.Text = "Export Selected Log";
            exportSelectedLogToolStripMenuItem.Click += exportSelectedLogToolStripMenuItem_Click;
            // 
            // exportAsToolStripMenuItem
            // 
            exportAsToolStripMenuItem.Name = "exportAsToolStripMenuItem";
            exportAsToolStripMenuItem.Size = new System.Drawing.Size(310, 34);
            exportAsToolStripMenuItem.Text = "Export As Pass Thru";
            exportAsToolStripMenuItem.Click += exportAsToolStripMenuItem_Click;
            // 
            // exportAsJSONToolStripMenuItem1
            // 
            exportAsJSONToolStripMenuItem1.Name = "exportAsJSONToolStripMenuItem1";
            exportAsJSONToolStripMenuItem1.Size = new System.Drawing.Size(310, 34);
            exportAsJSONToolStripMenuItem1.Text = "Export As JSON";
            exportAsJSONToolStripMenuItem1.Click += exportAsJSONToolStripMenuItem1_Click;
            // 
            // exportAsSimpleEnglishToolStripMenuItem
            // 
            exportAsSimpleEnglishToolStripMenuItem.Name = "exportAsSimpleEnglishToolStripMenuItem";
            exportAsSimpleEnglishToolStripMenuItem.Size = new System.Drawing.Size(310, 34);
            exportAsSimpleEnglishToolStripMenuItem.Text = "Export As Simple English";
            exportAsSimpleEnglishToolStripMenuItem.Click += exportAsSimpleEnglishToolStripMenuItem_Click;
            // 
            // exportLogToolStripMenuItem
            // 
            exportLogToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] { exportAsPassThruToolStripMenuItem, exportAsJSONToolStripMenuItem, exportAsBasicEnglishToolStripMenuItem });
            exportLogToolStripMenuItem.Name = "exportLogToolStripMenuItem";
            exportLogToolStripMenuItem.Size = new System.Drawing.Size(334, 34);
            exportLogToolStripMenuItem.Text = "Export Whole Log";
            // 
            // exportAsPassThruToolStripMenuItem
            // 
            exportAsPassThruToolStripMenuItem.Name = "exportAsPassThruToolStripMenuItem";
            exportAsPassThruToolStripMenuItem.Size = new System.Drawing.Size(292, 34);
            exportAsPassThruToolStripMenuItem.Text = "Export As Pass Thru";
            exportAsPassThruToolStripMenuItem.Click += exportAsPassThruToolStripMenuItem_Click;
            // 
            // exportAsJSONToolStripMenuItem
            // 
            exportAsJSONToolStripMenuItem.Name = "exportAsJSONToolStripMenuItem";
            exportAsJSONToolStripMenuItem.Size = new System.Drawing.Size(292, 34);
            exportAsJSONToolStripMenuItem.Text = "Export As JSON";
            exportAsJSONToolStripMenuItem.Click += exportAsJSONToolStripMenuItem_Click;
            // 
            // exportAsBasicEnglishToolStripMenuItem
            // 
            exportAsBasicEnglishToolStripMenuItem.Name = "exportAsBasicEnglishToolStripMenuItem";
            exportAsBasicEnglishToolStripMenuItem.Size = new System.Drawing.Size(292, 34);
            exportAsBasicEnglishToolStripMenuItem.Text = "Export as Basic English";
            exportAsBasicEnglishToolStripMenuItem.Click += exportAsBasicEnglishToolStripMenuItem_Click;
            // 
            // toolStripMenuItem2
            // 
            toolStripMenuItem2.Name = "toolStripMenuItem2";
            toolStripMenuItem2.Size = new System.Drawing.Size(331, 6);
            // 
            // debugEventToolStripMenuItem
            // 
            debugEventToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] { logCreateProcessToolStripMenuItem, logCreateThreadToolStripMenuItem, logGeneralExceptionsToolStripMenuItem, logExitProcessEventToolStripMenuItem, logExitThreadEventToolStripMenuItem, loadLoadDllEventToolStripMenuItem, logDebugStringEventToolStripMenuItem, logRipEventToolStripMenuItem, loadUnloadDllEventToolStripMenuItem, toolStripMenuItem3, logAllToolStripMenuItem, logNoneToolStripMenuItem });
            debugEventToolStripMenuItem.Name = "debugEventToolStripMenuItem";
            debugEventToolStripMenuItem.Size = new System.Drawing.Size(334, 34);
            debugEventToolStripMenuItem.Text = "Debug Event";
            debugEventToolStripMenuItem.DropDownOpening += debugEventToolStripMenuItem_DropDownOpening;
            // 
            // logCreateProcessToolStripMenuItem
            // 
            logCreateProcessToolStripMenuItem.CheckOnClick = true;
            logCreateProcessToolStripMenuItem.Name = "logCreateProcessToolStripMenuItem";
            logCreateProcessToolStripMenuItem.Size = new System.Drawing.Size(302, 34);
            logCreateProcessToolStripMenuItem.Text = "Log Create Process";
            // 
            // logCreateThreadToolStripMenuItem
            // 
            logCreateThreadToolStripMenuItem.CheckOnClick = true;
            logCreateThreadToolStripMenuItem.Name = "logCreateThreadToolStripMenuItem";
            logCreateThreadToolStripMenuItem.Size = new System.Drawing.Size(302, 34);
            logCreateThreadToolStripMenuItem.Text = "Log Create Thread";
            // 
            // logGeneralExceptionsToolStripMenuItem
            // 
            logGeneralExceptionsToolStripMenuItem.CheckOnClick = true;
            logGeneralExceptionsToolStripMenuItem.Name = "logGeneralExceptionsToolStripMenuItem";
            logGeneralExceptionsToolStripMenuItem.Size = new System.Drawing.Size(302, 34);
            logGeneralExceptionsToolStripMenuItem.Text = "Log General Exceptions";
            // 
            // logExitProcessEventToolStripMenuItem
            // 
            logExitProcessEventToolStripMenuItem.CheckOnClick = true;
            logExitProcessEventToolStripMenuItem.Name = "logExitProcessEventToolStripMenuItem";
            logExitProcessEventToolStripMenuItem.Size = new System.Drawing.Size(302, 34);
            logExitProcessEventToolStripMenuItem.Text = "Log Exit Process Event";
            // 
            // logExitThreadEventToolStripMenuItem
            // 
            logExitThreadEventToolStripMenuItem.CheckOnClick = true;
            logExitThreadEventToolStripMenuItem.Name = "logExitThreadEventToolStripMenuItem";
            logExitThreadEventToolStripMenuItem.Size = new System.Drawing.Size(302, 34);
            logExitThreadEventToolStripMenuItem.Text = "Log Exit Thread Event";
            // 
            // loadLoadDllEventToolStripMenuItem
            // 
            loadLoadDllEventToolStripMenuItem.CheckOnClick = true;
            loadLoadDllEventToolStripMenuItem.Name = "loadLoadDllEventToolStripMenuItem";
            loadLoadDllEventToolStripMenuItem.Size = new System.Drawing.Size(302, 34);
            loadLoadDllEventToolStripMenuItem.Text = "Log Load Dll Event";
            // 
            // logDebugStringEventToolStripMenuItem
            // 
            logDebugStringEventToolStripMenuItem.CheckOnClick = true;
            logDebugStringEventToolStripMenuItem.Name = "logDebugStringEventToolStripMenuItem";
            logDebugStringEventToolStripMenuItem.Size = new System.Drawing.Size(302, 34);
            logDebugStringEventToolStripMenuItem.Text = "Log Debug String Event";
            // 
            // logRipEventToolStripMenuItem
            // 
            logRipEventToolStripMenuItem.CheckOnClick = true;
            logRipEventToolStripMenuItem.Name = "logRipEventToolStripMenuItem";
            logRipEventToolStripMenuItem.Size = new System.Drawing.Size(302, 34);
            logRipEventToolStripMenuItem.Text = "Log Rip Event";
            // 
            // loadUnloadDllEventToolStripMenuItem
            // 
            loadUnloadDllEventToolStripMenuItem.CheckOnClick = true;
            loadUnloadDllEventToolStripMenuItem.Name = "loadUnloadDllEventToolStripMenuItem";
            loadUnloadDllEventToolStripMenuItem.Size = new System.Drawing.Size(302, 34);
            loadUnloadDllEventToolStripMenuItem.Text = "Load Unload Dll Event";
            // 
            // toolStripMenuItem3
            // 
            toolStripMenuItem3.Name = "toolStripMenuItem3";
            toolStripMenuItem3.Size = new System.Drawing.Size(299, 6);
            // 
            // logAllToolStripMenuItem
            // 
            logAllToolStripMenuItem.CheckOnClick = true;
            logAllToolStripMenuItem.Name = "logAllToolStripMenuItem";
            logAllToolStripMenuItem.Size = new System.Drawing.Size(302, 34);
            logAllToolStripMenuItem.Text = "Log All";
            logAllToolStripMenuItem.Click += logAllToolStripMenuItem_Click;
            // 
            // logNoneToolStripMenuItem
            // 
            logNoneToolStripMenuItem.CheckOnClick = true;
            logNoneToolStripMenuItem.Name = "logNoneToolStripMenuItem";
            logNoneToolStripMenuItem.Size = new System.Drawing.Size(302, 34);
            logNoneToolStripMenuItem.Text = "Log None";
            logNoneToolStripMenuItem.Click += logNoneToolStripMenuItem_Click;
            // 
            // debugEventsToolStripMenuItem
            // 
            debugEventsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] { fullStopNextEventToolStripMenuItem, toolStripMenuItem4, exceptionHandlingToolStripMenuItem, secondChangeExceptionHandlingToolStripMenuItem });
            debugEventsToolStripMenuItem.Name = "debugEventsToolStripMenuItem";
            debugEventsToolStripMenuItem.Size = new System.Drawing.Size(151, 29);
            debugEventsToolStripMenuItem.Text = "Debug Options";
            // 
            // fullStopNextEventToolStripMenuItem
            // 
            fullStopNextEventToolStripMenuItem.Name = "fullStopNextEventToolStripMenuItem";
            fullStopNextEventToolStripMenuItem.Size = new System.Drawing.Size(408, 34);
            fullStopNextEventToolStripMenuItem.Text = "Full Stop Next Event";
            // 
            // toolStripMenuItem4
            // 
            toolStripMenuItem4.Name = "toolStripMenuItem4";
            toolStripMenuItem4.Size = new System.Drawing.Size(405, 6);
            // 
            // exceptionHandlingToolStripMenuItem
            // 
            exceptionHandlingToolStripMenuItem.Name = "exceptionHandlingToolStripMenuItem";
            exceptionHandlingToolStripMenuItem.Size = new System.Drawing.Size(408, 34);
            exceptionHandlingToolStripMenuItem.Text = "First Chance Exception Handling ...";
            // 
            // secondChangeExceptionHandlingToolStripMenuItem
            // 
            secondChangeExceptionHandlingToolStripMenuItem.Name = "secondChangeExceptionHandlingToolStripMenuItem";
            secondChangeExceptionHandlingToolStripMenuItem.Size = new System.Drawing.Size(408, 34);
            secondChangeExceptionHandlingToolStripMenuItem.Text = "Second Change Exception Handling...";
            // 
            // SaveFileDialog_JSON_SelectedEntryExport
            // 
            SaveFileDialog_JSON_SelectedEntryExport.DefaultExt = "JSON";
            SaveFileDialog_JSON_SelectedEntryExport.Filter = "Text Files|*.txt|JSON Files|*.JSON|All Files|*.*";
            SaveFileDialog_JSON_SelectedEntryExport.Title = "Select where to export Selected Entries to in the JSON Format";
            SaveFileDialog_JSON_SelectedEntryExport.FileOk += SaveFileDialogJSON_SelectExport_FileOk;
            // 
            // SaveFileDialog_SimpleEnglish_SelectedExport
            // 
            SaveFileDialog_SimpleEnglish_SelectedExport.DefaultExt = "LOG";
            SaveFileDialog_SimpleEnglish_SelectedExport.Title = "Select where to export Selected Entries to in simple English";
            SaveFileDialog_SimpleEnglish_SelectedExport.FileOk += SaveFileDialog_ExportSelectedSimpleEnglish_FileOk;
            // 
            // SaveFileDialog_PassThru_SelectedExport
            // 
            SaveFileDialog_PassThru_SelectedExport.DefaultExt = "TXT";
            SaveFileDialog_PassThru_SelectedExport.Title = "Select Location to Save Selected Entries in Pass Thru - Raw String Representation";
            SaveFileDialog_PassThru_SelectedExport.FileOk += SaveFileDialog_PassThru_SelectedExport_FileOk;
            // 
            // SaveFileDialog_PassThru_ExportAll
            // 
            SaveFileDialog_PassThru_ExportAll.Title = "Select Location to Save All Entries in Pass Thru - Raw String Representation";
            SaveFileDialog_PassThru_ExportAll.FileOk += SaveFileDialog_PassThru_ExportAll_FileOk;
            // 
            // SaveFileDialog_SimpleEnglish_ExportAll
            // 
            SaveFileDialog_SimpleEnglish_ExportAll.Title = "Select where to export All Entries to in simple English";
            SaveFileDialog_SimpleEnglish_ExportAll.FileOk += SaveFileDialog_SimpleEnglish_ExportAll_FileOk;
            // 
            // SaveFileDialog_JSON_ExportAll
            // 
            SaveFileDialog_JSON_ExportAll.Title = "Select where to export All Entries to in the JSON Format";
            SaveFileDialog_JSON_ExportAll.FileOk += SaveFileDialog_JSON_ExportAll_FileOk;
            // 
            // InsightLogger_ActiveLogDialog
            // 
            AutoScaleDimensions = new System.Drawing.SizeF(10F, 25F);
            AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            ClientSize = new System.Drawing.Size(1825, 671);
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
            Controls.Add(MenuStripActiveLogWindow);
            MainMenuStrip = MenuStripActiveLogWindow;
            Name = "InsightLogger_ActiveLogDialog";
            Text = "InsightLogger - Active Log";
            Load += InsightLogger_ActiveLogDialog_Load;
            VisibleChanged += InsightLogger_ActiveLogDialog_VisibleChanged;
            MenuStripActiveLogWindow.ResumeLayout(false);
            MenuStripActiveLogWindow.PerformLayout();
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
        private System.Windows.Forms.ToolTip ToolTipLogEntryViewHint;
        private System.Windows.Forms.MenuStrip MenuStripActiveLogWindow;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem logSettingsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem chooseDisppToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem exportLogToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exportAsPassThruToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exportAsJSONToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exportAsBasicEnglishToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exportSelectedLogToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exportAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exportAsJSONToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem exportAsSimpleEnglishToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem chooseFriendlyToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem chooseJSONToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem choosePassThruToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem debugEventToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem logCreateProcessToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem logCreateThreadToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem logGeneralExceptionsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem logExitProcessEventToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem logExitThreadEventToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadLoadDllEventToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem closeThisToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem logDebugStringEventToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem logRipEventToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadUnloadDllEventToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
        private System.Windows.Forms.ToolStripMenuItem logAllToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem logNoneToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem debugEventsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fullStopNextEventToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem4;
        private System.Windows.Forms.ToolStripMenuItem exceptionHandlingToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem secondChangeExceptionHandlingToolStripMenuItem;
        private System.Windows.Forms.SaveFileDialog SaveFileDialog_JSON_SelectedEntryExport;
        private System.Windows.Forms.SaveFileDialog SaveFileDialog_SimpleEnglish_SelectedExport;
        private System.Windows.Forms.SaveFileDialog SaveFileDialog_PassThru_SelectedExport;
        private System.Windows.Forms.SaveFileDialog SaveFileDialog_PassThru_ExportAll;
        private System.Windows.Forms.SaveFileDialog SaveFileDialog_SimpleEnglish_ExportAll;
        private System.Windows.Forms.SaveFileDialog SaveFileDialog_JSON_ExportAll;
    }
}