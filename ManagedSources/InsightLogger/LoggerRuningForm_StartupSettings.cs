using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace InsightLogger
{
    public partial class LoggerRunForm_StartupSettings : Form
    {
        bool VerifyEnvFile(string name)
        {
            string[] Lines;
            try
            {
                Lines = System.IO.File.ReadAllLines(name);
                foreach (string line in Lines)
                {
                    string lline = line.Trim();
                    int index_result = lline.IndexOf('=');
                    if ((index_result == -1) || (index_result == 0))
                    {
                        return false;
                    }
                }
            }
            catch (UnauthorizedAccessException)
            {
                return false;
            }
            catch (IOException)
            {
                return false;
            }
            return true;
        }

        public LoggerRunForm_StartupSettings()
        {
            InitializeComponent();
        }

        private void ButtonSelectStartingDir_Click(object sender, EventArgs e)
        {

            FolderBrowserDialogSelectProcessStartDir.ShowDialog(this);
            this.TextBoxProcessStartingDir.Text = FolderBrowserDialogSelectProcessStartDir.SelectedPath;
        }

        private void ButtonSelectEnvTextFile_Click(object sender, EventArgs e)
        {
            OpenFileDialogSelectEnvFile.ShowDialog();

        }

        private void OpenFileDialogSelectEnvFile_FileOk(object sender, CancelEventArgs e)
        {
            TextBoxEnvTextFile.Text = OpenFileDialogSelectEnvFile.FileName;
        }

        private void ButtonOK_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
            Close();
        }

        private void ButtonCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            Close();
        }

        private void TextBoxEnvTextFile_Validating(object sender, CancelEventArgs e)
        {
            e.Cancel = !VerifyEnvFile(TextBoxEnvTextFile.Text);

            if (e.Cancel == true)
            {

            }
        }
    }
}
