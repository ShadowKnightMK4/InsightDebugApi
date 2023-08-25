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
    public partial class NewProcessForm : Form
    {
        public NewProcessForm()
        {
            InitializeComponent();
        }

        void AssignOpenDialog(OpenFileDialog UseMe, TextBox outtext)
        {
            var Result = UseMe.ShowDialog();
            if (Result == DialogResult.OK)
            {
                outtext.Text = UseMe.FileName;
            }

        }
        OpenFileDialog ChooseTextEnvFile = new OpenFileDialog();
        OpenFileDialog ChooseSpawnProcess = new OpenFileDialog();
        private void tabPage1_Click(object sender, EventArgs e)
        {

        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        private void ButtonChooseEnvText_Click(object sender, EventArgs e)
        {

            AssignOpenDialog(ChooseTextEnvFile, TextBoxTargetEnvFile);
        }

        private void ButtonChooseProcessToSpawn_Click(object sender, EventArgs e)
        {
            AssignOpenDialog(ChooseSpawnProcess, TextBoxTargetProcess);
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {

        }
    }
}
