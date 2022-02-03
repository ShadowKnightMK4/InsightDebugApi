using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FileSandBox_GUI
{
    public partial class DebugProcessGuiLauncher : Form
    {
        public DebugProcessGuiLauncher()
        {
            InitializeComponent();
        }

        OpenFileDialog ChooseProcessDialog = null;
        OpenFileDialog ChooseArgumentDialog = null;
        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {

        }

        /// <summary>
        /// Reset the dialog to a fresh window.
        /// </summary>
        public void ResetGUI()
        {


        }
        private void groupBox4_Enter(object sender, EventArgs e)
        {

        }

        private void DebugProcessGuiLauncher_Load(object sender, EventArgs e)
        {
            if (ChooseProcessDialog == null)
            {
                ChooseProcessDialog = new OpenFileDialog();
            }
            ChooseProcessDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Windows);
            ChooseProcessDialog.Filter = "(Executable Files)| *.exe| (All Files) | (*.*) ";
            ChooseProcessDialog.Title = "Specify a Process to Start";

            if (ChooseArgumentDialog == null)
            {
                ChooseArgumentDialog = new OpenFileDialog();
            }
            ChooseArgumentDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
            ChooseArgumentDialog.Filter = "(Text File)| *.txt| (All Files) | (*.*)";
            ChooseArgumentDialog.Title = "Specify a Unicode Text file containing the arguments for this process";
            
        }

        private void ButtonChooseProcessToSpawn_Click(object sender, EventArgs e)
        {
            var Result = this.ChooseProcessDialog.ShowDialog();
            if (Result == DialogResult.OK)
            {
                TextBoxTargetProcessToSpawn.Text = ChooseProcessDialog.FileName;
            }
        }

        private void CheckBoxDirectArgument_CheckedChanged(object sender, EventArgs e)
        {
            if (CheckBoxDirectArgument.Checked)
            {
                ButtonChooseArgumentSourceFileButton.Enabled = false;
            }
            else
            {
                ButtonChooseArgumentSourceFileButton.Enabled = true;
            }
        }

        private void CheckBoxDebugTarget_CheckedChanged(object sender, EventArgs e)
        {
            CheckBoxEnableDebugPriv.Enabled = CheckBoxHostileTarget.Enabled = CheckBoxDebugTarget.Checked;
        }
    }
}
