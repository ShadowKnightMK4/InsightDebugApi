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
    public partial class MainWindow : Form
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        DebugProcessGuiLauncher SpawnNewProcessGui;
        private void allExceptHelperDllToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void existingProcessToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void newProcessToolStripMenuItem_Click(object sender, EventArgs e)
        {
           if (SpawnNewProcessGui == null)
            {
                SpawnNewProcessGui = new DebugProcessGuiLauncher();
            }
            
            {
                SpawnNewProcessGui.ResetGUI();
                DialogResult result = SpawnNewProcessGui.ShowDialog(this);
            }
        }
    }
}
