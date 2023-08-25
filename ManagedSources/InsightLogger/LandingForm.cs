using System.Windows.Forms;

namespace InsightLogger
{
    public partial class LandingForm : Form
    {
        public LandingForm()
        {
            InitializeComponent();
        }

        NewProcessForm NewProcess = new NewProcessForm();
        LoggingForm LoggingForm = new LoggingForm();
        private void spawnToolStripMenuItem_Click(object sender, System.EventArgs e)
        {
            NewProcess.ShowDialog();
        }

        private void logSettingsToolStripMenuItem_Click(object sender, System.EventArgs e)
        {
            LoggingForm.ShowDialog();
        }

        private void processToolStripMenuItem_Click(object sender, System.EventArgs e)
        {

        }
    }
}