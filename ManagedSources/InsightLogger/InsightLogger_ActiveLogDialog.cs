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
namespace InsightLogger
{
    public partial class InsightLogger_ActiveLogDialog : Form
    {
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
    }
}
