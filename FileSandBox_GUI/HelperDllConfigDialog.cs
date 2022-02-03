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
    public partial class HelperDllConfigDialog : Form
    {
        public HelperDllConfigDialog()
        {
            InitializeComponent();
        }


        
        public string[] GetDllsToLoad
        {
            get
            {
                return DllsToLoad.ToArray();
            }
        }

        List<string> DllsToLoad;
        private void HelperDllConfigDialog_Load(object sender, EventArgs e)
        {
            Text = "Choose which Detours Dll to force spawned applications to Load";

        }
    }
}
