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
namespace InsightLogger
{
    public partial class LogRunner_DetoursForm : Form
    {
        public string GeneralDllListLocation;
        public string TelementryDllListLocation;
        /// <summary>
        /// List of Dll. key = name such as IoDeviceTracking.   value = C:\\IoDeviceTracking32.dll
        /// </summary>
        Dictionary<string, string> TelemetryDllList = new Dictionary<string, string>();
        Dictionary<string, string> GenereralDllList = new Dictionary<string, string>();

        void AddDllUpdateList(string Friendly, string DllLocation, CheckedListBox TargetBox, Dictionary<string, string> TargetList)
        {
            if (TargetList.ContainsKey(Friendly))
            {
                TargetList[Friendly] = DllLocation;
            }
            else
            {
                TargetList.Add(Friendly, DllLocation);
            }
            if (!TargetBox.Items.Contains(Friendly))
            {
                TargetBox.Items.Add(Friendly);
            }
        }


        void AddTelemetryDll(string Friendly, string TelementryDll)
        {
            AddDllUpdateList(Friendly, TelementryDll, CheckedListBoxTelemetryDlls, TelemetryDllList);
        }
        void AddGeneralDll(string Friendly, string Location)
        {
            AddDllUpdateList(Friendly, Location, CheckedListBoxAdditionDLLs, GenereralDllList);
        }
        enum KnownTeleDLL
        {
            IoDeviceTracking
        };

        public LogRunner_DetoursForm()
        {
            InitializeComponent();
        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private string DropQuotes(string target)
        {
            target = target.Trim();
            if (target.StartsWith("\""))
            {
                target = target.Substring(1);
            }
            if (target.EndsWith("\""))
            {
                target = target.Substring(0, target.Length - 1);
            }
            return target;
        }
        private void LoadDllList(CheckedListBox target, string FileLocation, Dictionary<string, string> container)
        {
            string[] Lines;

            try
            {
                Lines = File.ReadAllLines(FileLocation);

                for (int step = 0; step < Lines.Length; step++)
                {
                    string l = Lines[step].Trim();
                    int EqualLoc = l.IndexOf('=');
                    if ((EqualLoc != -1) && (EqualLoc > 0))
                    {
                        string Friendly = l.Substring(0, EqualLoc);
                        string Path = l.Substring(EqualLoc + 1);
                        Path = DropQuotes(Path);
                        if (File.Exists(Path))
                        {
                            container.Add(Friendly, Path);
                        }
                    }
                }
            }
            catch (IOException err)
            {
                MessageBox.Show(string.Format("Unable to load a dll list. Reason\" {0} \"", err.Message));
            }
            finally
            {

            }
            target.Items.Clear();
            if (container.Count > 0)
            {
                foreach (string s in container.Keys)
                {
                    target.Items.Add(s);
                }
            }

        }
        private void LogRunner_DetoursForm_Load(object sender, EventArgs e)
        {

        }

        private void ButtonOk_Click(object sender, EventArgs e)
        {

        }

        private void LogRunner_DetoursForm_Shown(object sender, EventArgs e)
        {
            LoadDllList(CheckedListBoxAdditionDLLs, GeneralDllListLocation, GenereralDllList);
            LoadDllList(CheckedListBoxTelemetryDlls, TelementryDllListLocation, TelemetryDllList);
        }

        private void OpenFileDialogSelectGenericDetour_FileOk(object sender, CancelEventArgs e)
        {
            AddGeneralDll(Path.GetFileName(OpenFileDialogSelectGenericDetour.FileName), OpenFileDialogSelectGenericDetour.FileName);

        }

        private void ButtonAddNew_Click(object sender, EventArgs e)
        {
            OpenFileDialogSelectGenericDetour.ShowDialog(this);
        }

        private void ButtonClearNewDllList_Click(object sender, EventArgs e)
        {

            

        }
    }
}
