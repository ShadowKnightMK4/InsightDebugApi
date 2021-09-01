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
namespace FileSandBox_GUI
{
    public partial class SingleLogWindow : Form
    {
        public SingleLogWindow()
        {
            InitializeComponent();
        }

        public List<MemoryStream> Logs;
        private void SingleLogWindow_Load(object sender, EventArgs e)
        {
            Logs = new List<MemoryStream>();

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox1_Validating(object sender, CancelEventArgs e)
        {
            foreach (MemoryStream Log in Logs)
            {
                
            }
        }
    }
}
