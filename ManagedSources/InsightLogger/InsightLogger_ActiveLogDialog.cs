using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using InsightSheath.Debugging.Process;
using InsightSheath;
using System.Runtime.CompilerServices;
using InsightSheath.Debugging;

namespace InsightLogger
{
    /*
     * The control ListBoxLogEvent's item collection contains generated log entries that are also stored in LogCollector
     * 
     * When change the display format, we go thru and update variable that controls the output of the ToString() routine.
     * Afterwords we reassign the item to its slot to retrigger a call to get the new text to display.
     * 
     * IMPORTANT! Code assumes that ONLY LogEntries are going to be in the ListBoxLogEvent item collection. It's 
     * likely GOING TO BREAK if that's not true.
     * 
     */
    /// <summary>
    /// Displays the log of active debug processes to the user
    /// </summary>
    public partial class InsightLogger_ActiveLogDialog : Form
    {
        /// <summary>
        /// Current log display format for the listbox items. To update that list use <see cref="SetDisplayLogFormat(LogEntry.StringOutFormat)"/>
        /// </summary>
        LogEntry.StringOutFormat DisplayLogFormat = LogEntry.StringOutFormat.PassThru;
        #region general log and gui function
        /// <summary>
        /// Set the display log format and trigger update on the list items.
        /// </summary>
        /// <param name="displayLogFormat"></param>
        void SetDisplayLogFormat(LogEntry.StringOutFormat displayLogFormat)
        {
            DisplayLogFormat = displayLogFormat;
            for (int step = 0; step < ListBoxLogEvent.Items.Count; step++)
            {
                LogEntry Unpack = (LogEntry)ListBoxLogEvent.Items[step];
                //((LogEntry) = ChooseLogFormat.DisplayedLogFormat;
                Unpack.Format = DisplayLogFormat;
                ListBoxLogEvent.Items[step] = ListBoxLogEvent.Items[step];
            }
        }

        /// <summary>
        /// Export all entries in the listbox log display to this stream
        /// </summary>
        /// <param name="Target">where to write</param>
        void ExportLog(Stream Target)
        {
            ExportLogFormat(Target, LogHandler.CurrentFormat);
        }

        /// <summary>
        /// Export all entries in the listbox log display to this stream with the specific format
        /// </summary>
        /// <param name="Target">where to write</param>
        /// <param name="For">format</param>
        void ExportLogFormat(Stream Target, LogFormat For)
        {
            CommonLogFormatExportor(Target, For, ListBoxLogEvent.Items.GetEnumerator());
        }


        /// <summary>
        /// Write the Selected Log entries in the listbox to the target
        /// </summary>
        /// <param name="Target">where to write</param>
        void ExportSelectedLog(Stream Target)
        {
            ExportSelectedLogFormat(Target, LogHandler.CurrentFormat);
        }

        /// <summary>
        /// Write the Selected Log entries in the listbox to the target with the specific logformat
        /// </summary>
        /// <param name="Target">where to write</param>
        /// <param name="For">format</param>
        void ExportSelectedLogFormat(Stream Target, LogFormat For)
        {
            CommonLogFormatExportor(Target, For, ListBoxLogEvent.SelectedItems.GetEnumerator());
        }


        /// <summary>
        /// THe Varous ExportXXXXLog converge here
        /// </summary>
        /// <param name="Target">where to write</param>
        /// <param name="For">format</param>
        /// <param name="Stuff">enumerator of what to write. IMPORTANT MUST BE direct enumoration of a collection of <see cref="LogEntry"/> </param>
        void CommonLogFormatExportor(Stream Target, LogFormat For, System.Collections.IEnumerator? Stuff)
        {
            LogHandler.ExportSelectedLogViaEnumerator(Target, For, Stuff);
        }




        #endregion
        /// <summary>
        /// Dialog to let use pick a different log format to display as
        /// </summary>
        readonly ChooseLogFormat ChooseLogFormat = new ChooseLogFormat();
        /// <summary>
        /// The log we work with and display to user
        /// </summary>
        public readonly LogCollector LogHandler = new LogCollector();
        /// <summary>
        /// Deleage to extend export format. Used by routines such as <see cref="SaveAllLogAsTextFile(string)"/>
        /// </summary>
        /// <param name="Line"></param>
        /// <param name="Output"></param>
        public delegate void LogWriteHandler(string Line, FileStream Output);
        /// <summary>
        /// Save all entries as unicode text to here
        /// </summary>
        /// <param name="Location"></param>
        public void SaveAllLogAsTextFile(string Location)
        {
            void WriteUnicodeText(string Line, FileStream Target)
            {
                byte[] B = Encoding.UTF8.GetBytes(Line);
                Target.Write(B, 0, B.Length);
            }
            SaveAllLog(Location, WriteUnicodeText);
        }

        /// <summary>
        /// Save all entries by calling HowToWrite to the location
        /// </summary>
        /// <param name="Location"></param>
        /// <param name="HowToWrite">This is called to write. You can write in any format you want.</param>
        public void SaveAllLog(string Location, LogWriteHandler HowToWrite)
        {
            using (var Fn = File.OpenWrite(Location))
            {
                StringBuilder DumpThis = new();
                foreach (var l in ListBoxLogEvent.Items)
                {
                    DumpThis.AppendLine(l.ToString());
                }
                HowToWrite(DumpThis.ToString(), Fn);
            }
        }
        public InsightLogger_ActiveLogDialog()
        {
            InitializeComponent();
        }

        public InsightProcess LaunchThis = InsightProcess.CreateInstance();


        public string TargetOnGoingLogLocation;

        private void PostLogThing(string msg)
        {
            PostLogThingMsg(msg);
        }

        private void PostLogThing(DebugEvent Ev)
        {
            PostLogThingDebugEvent(Ev);
        }

        /// <summary>
        /// Add this message to the log with the datetime stamp of now in ticks.
        /// </summary>
        /// <param name="msg">string to add</param>
        private void PostLogThingMsg(string msg)
        {
            PostLogThingMsgTimeStamp(DateTime.Now.Ticks.ToString(), msg);
        }


        /// <summary>
        /// Add this message to the log with the index string of Timestamp
        /// </summary>
        /// <param name="Timestamp">string to index the entry. </param>
        /// <param name="msg">msg to add</param>
        /// <remarks>We're updating both our <see cref="LogHandler"/> and the <see cref="ListBoxLogEvent"/></remarks>
        private void PostLogThingMsgTimeStamp(string Timestamp, string msg)
        {
            var Entry = new LogEntry(msg, ChooseLogFormat.DisplayedLogFormat);
            ListBoxLogEvent.Items.Add(Entry);
            this.LogHandler.AddLog(Timestamp, Entry);
        }

        /// <summary>
        /// Add a copy of this debug event to the log with the datetime stamp of now
        /// </summary>
        /// <param name="Instance">debug event to add. </param>
        /// <remarks>Note that this routine makes its own copy of the struct</remarks>
        private void PostLogThingDebugEvent(DebugEvent Instance)
        {
            PostLogThingDebugEventMsg(DateTime.Now.Ticks.ToString(), Instance);
        }

        /// <summary>
        /// Add a copy of this debug event to the log with  the index string of Timestamp
        /// </summary>
        /// <param name="Timestamp">string to index the entry. </param>
        /// <param name="Instance">debug event to add. </param>
        /// <remarks>Note that this routine makes its own copy of the struct</remarks>
        public void PostLogThingDebugEventMsg(string Timestamp, DebugEvent Instance)
        {
            var Entry = new LogEntry(Instance, ChooseLogFormat.DisplayedLogFormat);
            ListBoxLogEvent.Items.Add(Entry);
            LogHandler.AddLog(Timestamp, Entry);
        }


        /// <summary>
        /// Add an event to the log. Write it the ongoing file if existant.
        /// </summary>
        /// <param name="msg"></param>
        public void PostLogEvent(DebugEvent Instance)
        {
            if (!ListBoxLogEvent.InvokeRequired)
            {
                PostLogThingDebugEvent(Instance);
            }
            else
            {
                ListBoxLogEvent.Invoke(PostLogThingDebugEvent, Instance);
            }
        }


        /// <summary>
        /// Add an event to the log. Write it the ongoing file if existant.
        /// </summary>
        /// <param name="msg"></param>
        public void PostLogEvent(string msg)
        {
            if (!ListBoxLogEvent.InvokeRequired)
            {
                PostLogThingMsg(msg);
            }
            else
            {
                ListBoxLogEvent.Invoke(PostLogThingMsg, msg);
            }
            {
                try
                {
                    using (var tmpHandle = File.Open(TargetOnGoingLogLocation, FileMode.OpenOrCreate, FileAccess.ReadWrite))
                    {
                        var Bytes = Encoding.UTF8.GetBytes(msg);
                        tmpHandle.Write(Bytes, 0, Bytes.Length);
                    }
                }
                catch (ArgumentException)
                {
                    // its fine.
                }
            }
        }

        private void InsightLogger_ActiveLogDialog_Load(object sender, EventArgs e)
        {

        }

        /// <summary>
        /// The tick event juts checks if the pause event processing flag is set and calls <see cref="InsightProcess.PulseDebugEventThead"/> if it's clear
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void TimerPulseDebugEventKick_Tick(object sender, EventArgs e)
        {
            if (!this.CheckBoxStopNextEvent.Checked)
                LaunchThis.PulseDebugEventThead();
        }


        /// <summary>
        /// Update the tick wait time and reset to safe files if output of range
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void vScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {


            try
            {
                int p = int.Parse(TextBoxEventLogTimerMilli.Text);
                if (e.OldValue < e.NewValue)
                {
                    p -= 10;
                }
                else
                {
                    p += 10;
                }



                if (p <= 1)
                    p = 1;
                TextBoxEventLogTimerMilli.Text = p.ToString();
                TimerPulseDebugEventKick.Interval = p;
            }
            catch (FormatException)
            {
                TextBoxEventLogTimerMilli.Text = TimerPulseDebugEventKick.Interval.ToString();
            }

        }

        /// <summary>
        /// init form. Begin the tick rate value from the textbox 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void InsightLogger_ActiveLogDialog_VisibleChanged(object sender, EventArgs e)
        {
            if (Visible)
            {
                try
                {
                    int p = int.Parse(TextBoxEventLogTimerMilli.Text);
                    TimerPulseDebugEventKick.Interval = p;
                }
                catch (FormatException)
                {
                    TextBoxEventLogTimerMilli.Text = TimerPulseDebugEventKick.Interval.ToString();
                }
            }
        }

        /// <summary>
        /// Attempt to convert the tickrate textbox to a number and cancel event if it cant
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void TextBoxEventLogTimerMilli_Validating(object sender, CancelEventArgs e)
        {
            try
            {
                int p = int.Parse(TextBoxEventLogTimerMilli.Text);
                TimerPulseDebugEventKick.Interval = p;
            }
            catch (FormatException)
            {
                e.Cancel = true;
            }
        }

        private void TextBoxEventLogTimerMilli_TextChanged(object sender, EventArgs e)
        {

        }

        /// <summary>
        /// Show the dialog to select target to save the entire log
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ButtonSaveAll_Click(object sender, EventArgs e)
        {
            SaveFileDialogAllEntriesExport.ShowDialog(this);
        }

        /// <summary>
        /// Event processor for the dialog to save the entire log
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void SaveFileDialogAllEntriesExport_FileOk(object sender, CancelEventArgs e)
        {
            //SaveAllLogAsTextFile(SaveFileDialogAllEntriesExport.FileName);
            using (Stream s = File.OpenWrite(SaveFileDialogAllEntriesExport.FileName))
            {
                this.LogHandler.ExportLog(s);
            }
        }

        private void ButtonContinueDebugEvent_Click(object sender, EventArgs e)
        {
            if (TextBoxEventLogTimerMilli.Enabled == false)
            {
                TextBoxEventLogTimerMilli.Enabled = true;
                LaunchThis.PulseDebugEventThead();
                CheckBoxStopNextEvent.Checked = false;
            }

        }

        /// <summary>
        /// Show dialog to select a new *DISPLAY* log format. Note that this does not effect the exported log format
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ButtonReinterpretLog_Click(object sender, EventArgs e)
        {
            ChooseLogFormat.ShowDialog(this);
            if (ChooseLogFormat.DialogResult == DialogResult.OK)
            {
                SetDisplayLogFormat(ChooseLogFormat.DisplayedLogFormat);
            }
            return;
        }


        /// <summary>
        /// event file ok for saving selected events
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ButtonSelectedLogSave_Click(object sender, EventArgs e)
        {

            SaveFileDialogSelectedEvents.ShowDialog(this);


        }

        /// <summary>
        /// Save the selected events to the chosen file
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void SaveFileDialogSelectedEvents_FileOk(object sender, CancelEventArgs e)
        {
            using (Stream s = File.OpenWrite(SaveFileDialogSelectedEvents.FileName))
            {
                ExportSelectedLog(s);
            }
        }

        /// <summary>
        /// Set our tool top to show what listbox item we're hovering over
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ListBoxLogEvent_MouseMove(object sender, MouseEventArgs e)
        {
            var Index = ListBoxLogEvent.IndexFromPoint(e.Location);
            if ((Index >= 0) && (Index < ListBoxLogEvent.Items.Count))
            {
                //if (ListBoxLogEvent.SelectedItems.Contains(ListBoxLogEvent.Items[Index]))
                {
                    ToolTipLogEntryViewHint.SetToolTip(ListBoxLogEvent, ListBoxLogEvent.Items[Index].ToString());

                }
            }
            else
            {
                ToolTipLogEntryViewHint.RemoveAll();
            }
        }

        private void ListBoxLogEvent_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void chooseDisppToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        /// <summary>
        /// Event to set display log format to friendly english
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void chooseFriendlyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SetDisplayLogFormat(LogEntry.StringOutFormat.FriendlyEnglish);
        }

        private void chooseJSONToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SetDisplayLogFormat(LogEntry.StringOutFormat.JSON);
        }

        private void choosePassThruToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SetDisplayLogFormat(LogEntry.StringOutFormat.PassThru);
        }

        private void logAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LogHandler.SetLogAllEvents();
        }

        private void toolStripMenuItem1_Click(object sender, EventArgs e)
        {

        }

        private void logSettingsToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void logNoneToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void debugEventToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
        {
            if (LogHandler.LogTheseEvents[DebugEventType.CreateThreadEvent] == true)
            {
                logCreateThreadToolStripMenuItem.Checked = true;
            }
            else
            {
                logCreateThreadToolStripMenuItem.Checked = false;
            }
            if (LogHandler.LogTheseEvents[DebugEventType.CreateProcessEvent] == true)
            {
                logCreateProcessToolStripMenuItem.Checked = true;
            }
            else
            {
                logCreateProcessToolStripMenuItem.Checked = false;
            }

            if (LogHandler.LogTheseEvents[DebugEventType.ExceptionEvent] == true)
            {

            }


            if (LogHandler.LogTheseEvents[DebugEventType.ExitProcessEvent] == true)
            {
                logExitProcessEventToolStripMenuItem.Checked = true;
            }
            else
            {
                logExitProcessEventToolStripMenuItem.Checked = false;
            }

            if (LogHandler.LogTheseEvents[DebugEventType.ExitThreadEvent] == true)
            {
                logExitThreadEventToolStripMenuItem.Checked = true;
            }
            else
            {
                logCreateThreadToolStripMenuItem.Checked = false;
            }


            if (LogHandler.LogTheseEvents[DebugEventType.LoadDllEvent] == true)
            {
                loadLoadDllEventToolStripMenuItem.Checked = true;
            }
            else
            {
                loadLoadDllEventToolStripMenuItem.Checked = false;
            }


            if (LogHandler.LogTheseEvents[DebugEventType.OutputDebugString] == true)
            {
                logDebugStringEventToolStripMenuItem.Checked = true;
            }
            else
            {
                logDebugStringEventToolStripMenuItem.Checked = false;
            }


            if (LogHandler.LogTheseEvents[DebugEventType.RipEvent] == true)
            {
                logDebugStringEventToolStripMenuItem.Checked = true;
            }
            else
            {
                logDebugStringEventToolStripMenuItem.Checked = false;
            }

            if (LogHandler.LogTheseEvents[DebugEventType.UnloadDllEvent] == true)
            {
                loadUnloadDllEventToolStripMenuItem.Checked = true;
            }
            else
            {
                loadUnloadDllEventToolStripMenuItem.Checked = false;
            }
        }

        private void exportAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog_PassThru_SelectedExport.ShowDialog(this);
        }

        private void exportAsJSONToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            SaveFileDialog_JSON_SelectedEntryExport.ShowDialog(this);
        }

        private void exportAsSimpleEnglishToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog_SimpleEnglish_SelectedExport.ShowDialog(this);
        }

        private void exportAsPassThruToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog_PassThru_ExportAll.ShowDialog(this);
        }

        private void exportSelectedLogToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void SaveFileDialogJSON_SelectExport_FileOk(object sender, CancelEventArgs e)
        {
            using (Stream s = File.Open(SaveFileDialog_JSON_SelectedEntryExport.FileName, FileMode.OpenOrCreate, FileAccess.Write))
            {
                ExportSelectedLogFormat(s, LogHandler.LogFormatJson);
            }
        }

        private void SaveFileDialog_ExportSelectedSimpleEnglish_FileOk(object sender, CancelEventArgs e)
        {
            using (Stream s = File.Open(SaveFileDialog_SimpleEnglish_SelectedExport.FileName, FileMode.OpenOrCreate, FileAccess.Write))
            {
                ExportSelectedLogFormat(s, new LogFormatFriendlyEnglish());
            }
        }

        private void SaveFileDialog_PassThruSelectExport_FileOk(object sender, CancelEventArgs e)
        {

        }

        private void SaveFileDialog_PassThru_ExportAll_FileOk(object sender, CancelEventArgs e)
        {
            using (Stream s = File.Open(SaveFileDialog_PassThru_ExportAll.FileName, FileMode.OpenOrCreate, FileAccess.Write))
            {
                ExportLogFormat(s, LogHandler.LogFormatPassThru);
            }
        }

        private void SaveFileDialog_SimpleEnglish_ExportAll_FileOk(object sender, CancelEventArgs e)
        {
            using (Stream s = File.Open(SaveFileDialog_SimpleEnglish_ExportAll.FileName, FileMode.OpenOrCreate, FileAccess.Write))
            {
                ExportLogFormat(s, new LogFormatFriendlyEnglish());
            }
        }

        private void SaveFileDialog_JSON_ExportAll_FileOk(object sender, CancelEventArgs e)
        {
            using (Stream s = File.Open(SaveFileDialog_JSON_ExportAll.FileName, FileMode.OpenOrCreate, FileAccess.Write))
            {
                ExportLogFormat(s, LogHandler.LogFormatJson);
            }
        }

        private void exportAsJSONToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog_JSON_ExportAll.ShowDialog(this);
        }

        private void exportAsBasicEnglishToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog_SimpleEnglish_ExportAll.ShowDialog(this);
        }

        private void SaveFileDialog_PassThru_SelectedExport_FileOk(object sender, CancelEventArgs e)
        {
            using (Stream s = File.Open(SaveFileDialog_PassThru_SelectedExport.FileName, FileMode.OpenOrCreate, FileAccess.Write))
            {
                ExportSelectedLogFormat(s, LogHandler.LogFormatPassThru);
            }
        }
    }
}
