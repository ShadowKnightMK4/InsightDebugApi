﻿using System;
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

namespace InsightLogger
{
    public partial class InsightLogger_ActiveLogDialog : Form
    {
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
            ListBoxLogEvent.Items.Add(msg);
        }
        /// <summary>
        /// Add an event to the log. Write it the ongoing file if existant.
        /// </summary>
        /// <param name="msg"></param>
        public void PostLogEvent(string msg)
        {
            if (!ListBoxLogEvent.InvokeRequired)
            {
                PostLogThing(msg);
            }
            else
            {
                ListBoxLogEvent.Invoke(PostLogThing, msg);
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

        private void TimerPulseDebugEventKick_Tick(object sender, EventArgs e)
        {

            LaunchThis.PulseDebugEventThead();
        }


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

        private void ButtonSaveAll_Click(object sender, EventArgs e)
        {
            SaveFileDialogAllEntriesExport.ShowDialog(this);
        }

        private void SaveFileDialogAllEntriesExport_FileOk(object sender, CancelEventArgs e)
        {
            SaveAllLogAsTextFile(SaveFileDialogAllEntriesExport.FileName);
        }
    }
}