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
    public partial class LoggerRunDialog : Form
    {
        public LoggerRunDialog()
        {
            InitializeComponent();
        }

        LoggerRunForm_StartupSettings setting = new LoggerRunForm_StartupSettings();
        LogRunner_ExceptionSettings ExceptionSettings = new LogRunner_ExceptionSettings();
        private void ButtonCustomizeEnv_Click(object sender, EventArgs e)
        {
            ContextMenuStripSpecialSettings.Show(MousePosition.X, MousePosition.Y);
        }

        private void OpenFileDialogSelectExe_FileOk(object sender, CancelEventArgs e)
        {
            ComboBoxTarget.Text = OpenFileDialogSelectExe.FileName;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            OpenFileDialogSelectExe.ShowDialog();
        }

        private void ButtonCancel_Click(object sender, EventArgs e)
        {
            var Result = MessageBox.Show("Quit?", "Quit Now?", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            if (Result == DialogResult.Yes)
            {
                Close();
            }

        }

        private void ContextMenuStripSpecialSettings_Opening(object sender, CancelEventArgs e)
        {

        }

        private void ToolStripMenuItemEnvEdit_Click(object sender, EventArgs e)
        {
            setting.ShowDialog(this);
        }

        private void exceptionSettingsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExceptionSettings.ShowDialog(this);
        }
    }
}
