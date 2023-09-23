using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace InsightLogger
{
    public partial class ChooseLogFormat : Form
    {
        public ChooseLogFormat()
        {
            InitializeComponent();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void ButtonOK_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
            Close();
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (comboBox1.SelectedIndex)
            {
                case 0:
                    DisplayedLogFormat = LogEntry.StringOutFormat.PassThru;
                    LabelExampleFormat.Text = "Your Visual log will contain what the built in converting to \r\nstring functions are";
                    break;
                case 1:
                    DisplayedLogFormat = LogEntry.StringOutFormat.JSON;
                    LabelExampleFormat.Text = "Your Visual log will look something like this:  {\t \"DebugEventType\": \"LoadDllEvent\",\r\n\t \"PID\": \"19376\",\r\n\t \"TID\": \"22116\",\r\n\t \"IsEventFrom32BitProcess\": \"False\",\r\n\t \"ImageName\": \"\\\\\\\\?\\\\C:\\\\Windows\\\\System32\\\\ucrtbase.dll\",\r\n\t \"DllBaseAddress\": \"7FFCCF4C0000\"\r\n}";
                    break;
                case 2:
                    DisplayedLogFormat = LogEntry.StringOutFormat.FriendlyEnglish;
                    LabelExampleFormat.Text = "Your Visual log will be friendly english \r\nbut not nessarily easy to parse.";
                    break;
                default:
                    throw new NotImplementedException();
            }
        }

        /// <summary>
        ///  on close with ok, this is set to what the user wanted to use
        /// </summary>
        public LogEntry.StringOutFormat DisplayedLogFormat = LogEntry.StringOutFormat.PassThru;
        private void ChooseLogFormat_Load(object sender, EventArgs e)
        {

        }

        private void ButtonCancel_Click(object sender, EventArgs e)
        {
            DisplayedLogFormat = (LogEntry.StringOutFormat)0;
            DialogResult = DialogResult.Cancel;
            Close();
        }

        private void LabelExampleFormat_Click(object sender, EventArgs e)
        {

        }
    }
}
