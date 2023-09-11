using InsightSheath.Debugging;
using InsightSheath.Debugging.Process;
using InsightSheath.Wrappers;
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


        InsightLogger_ActiveLogDialog LogLoop = new InsightLogger_ActiveLogDialog();
        LogRunner_DetoursForm Detours = new LogRunner_DetoursForm();
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

        private void ToolStripMenuItemDetourDllSetting_Click(object sender, EventArgs e)
        {
            Detours.TelementryDllListLocation = @"C:\Dummy\InsightApiDemo\TestFile.txt";
            Detours.GeneralDllListLocation = @"C:\Dummy\InsightApiDemo\TestFile.txt";
            Detours.ShowDialog(this);
        }


        private void ProcessEnv(string SourceFile, InsightProcess target)
        {
            if (string.IsNullOrEmpty(SourceFile))
                return;
            try
            {
                string[] Lines = System.IO.File.ReadAllLines(SourceFile);
                foreach (string Line in Lines)
                {
                    int Equal = Line.IndexOf("=");
                    if (Equal > 0)
                    {
                        string Name = Line.Substring(0, Equal - 1);
                        string Val = Line.Substring(Equal + 1);
                        target.SetExplicitEnviromentValue(Name, Val);
                    }
                }
            }
            finally
            {

            }
        }


        int EventLogPoster(IntPtr DebugEventPtr, IntPtr ContStat, IntPtr WaitTime, IntPtr Custom)
        {
            // code on unmaanged side keeps its own copy of a debug event struct in its worker thread.
            DebugEvent Ev = new DebugEvent(DebugEventPtr);

            switch (Ev.EventType)
            {
                case DebugEventType.CreateProcessEvent:
                    {
                        //LogLoop.PostLogEvent("Welcome Process #" + Ev.ProcessID + " created from a file at " + Ev.GetDebugEventCreateProcessInfo().ImageName + "\r\n");
                        //  LogLoop.PostLogEvent("Welcome Process #" + Ev.ProcessID + "  " + Ev.GetDebugEventCreateProcessInfo().ToString());
                        LogLoop.PostLogEvent(Ev);
                        //LogLoop.LogHandler.AddLog(DateTime.Now.Ticks.ToString(), Ev);

                        Ev.ContinueDebugEvent(Ev.ProcessID, Ev.ThreadID, DebugContState.DebugContinueState);
                        break;
                    }
                case DebugEventType.CreateTheadEvent:
                    {
                        //LogLoop.PostLogEvent("Process #" + Ev.ProcessID + " has created a thread " + Ev.GetDebugEventCreateThreadInfo());
                        //LogLoop.PostLogEvent(Ev.GetDebugEventCreateThreadInfo().ToString() + ",");
                        LogLoop.PostLogEvent(Ev);
                        Ev.ContinueDebugEvent(Ev.ProcessID, Ev.ThreadID, DebugContState.DebugContinueState);
                        break;
                    }
                case DebugEventType.ExceptionEvent:
                    {
                        //LogLoop.PostLogEvent("Exception Occured in Process #" + Ev.ProcessID + " Value is " + Ev.GetDebugEventExceptionInfo().ExceptionCode.ToString());
                        LogLoop.PostLogEvent(Ev);
                        Ev.ContinueDebugEvent(Ev.ProcessID, Ev.ThreadID, DebugContState.DebugExceptionNotHandled);
                        break;
                    }

                case DebugEventType.ExitProcessEvent:
                    {
                        //LogLoop.PostLogEvent("Process # " + Ev.ProcessID + "Exited with a return code of " + Ev.GetEventExitProcessInfo().ExitCode + "\r\n");
                        LogLoop.PostLogEvent(Ev.GetEventExitProcessInfo().ToString() + ",");
                        LogLoop.PostLogEvent(Ev);
                        Ev.ContinueDebugEvent(Ev.ProcessID, Ev.ThreadID, DebugContState.DebugContinueState);
                        break;
                    }

                case DebugEventType.ExitThreadEvent:
                    {
                        //LogLoop.PostLogEvent("Process # " + Ev.ProcessID + "Had a thread id of " + Ev.ThreadID.ToString() + " exit  " + Ev.GetEventExitThreadInfo().ExitCode + "\r\n");
                        //LogLoop.PostLogEvent(Ev.GetEventExitThreadInfo().ToString() + ",");
                        LogLoop.PostLogEvent(Ev);
                        Ev.ContinueDebugEvent(Ev.ProcessID, Ev.ThreadID, DebugContState.DebugContinueState);
                        break;
                    }

                case DebugEventType.LoadDllEvent:
                    {
                        //LogLoop.PostLogEvent("Process # " + Ev.ProcessID + "Loaded a Dll from " + Ev.GetDebugEventLoadDll().ImageName + "\r\n");
                        //LogLoop.PostLogEvent(Ev.GetDebugEventLoadDll().ToString() + ",");
                        LogLoop.PostLogEvent(Ev);
                        Ev.ContinueDebugEvent(Ev.ProcessID, Ev.ThreadID, DebugContState.DebugContinueState);
                        break;
                    }
                case DebugEventType.OutputDebugString:
                    {
                        //   LogLoop.PostLogEvent("Process # " + Ev.ProcessID + "sent a debug string  \"" +Ev.GetDebugEventStringInfo().OutputString  +"\"");
                        //LogLoop.PostLogEvent(Ev.GetDebugEventStringInfo().ToString() + ",");
                        LogLoop.PostLogEvent(Ev);
                        Ev.ContinueDebugEvent(Ev.ProcessID, Ev.ThreadID, DebugContState.DebugContinueState);
                        break;
                    }
                case DebugEventType.UnloadDllEvent:
                    {
                        //LogLoop.PostLogEvent("Process # " + Ev.ProcessID + "Unloaded a Dll from this virtual memory location " + Ev.GetDebugEventUnloadDllInfo().BaseOfDll + "\r\n");
                        //LogLoop.PostLogEvent(Ev.GetDebugEventUnloadDllInfo().ToString() + ",");
                        LogLoop.PostLogEvent(Ev);
                        Ev.ContinueDebugEvent(Ev.ProcessID, Ev.ThreadID, DebugContState.DebugContinueState);
                        break;
                    }
                case DebugEventType.RipEvent:
                    {
                        //LogLoop.PostLogEvent("Process # " + Ev.ProcessID + "Had a RIP event trigger" + Ev.GetDebugEventRipInfo().Error + "\r\n");
                        //LogLoop.PostLogEvent(Ev.GetDebugEventRipInfo().ToString() + ",");
                        LogLoop.PostLogEvent(Ev);
                        Ev.ContinueDebugEvent(Ev.ProcessID, Ev.ThreadID, DebugContState.DebugContinueState);
                        break;
                    }


                default:
                    {
                        throw new Exception("You forgot something"); ;
                        LogLoop.PostLogEvent("Unknown Event in " + Ev.ProcessID.ToString() + "happebnd\r\n");
                        Ev.ContinueDebugEvent(Ev.ProcessID, Ev.ThreadID, DebugContState.DebugExceptionNotHandled);
                        break;
                    }
            }

            return 0;
        }
        private void ButtonOK_Click(object sender, EventArgs e)
        {
            try
            {
                int test = int.Parse(setting.TextBoxRawProcessCreationValue.Text);
                LogLoop.LaunchThis.CreationFlags = (InsightProcess.CreationFlagValues)test;
            }
            catch (FormatException)
            {
                LogLoop.LaunchThis.CreationFlags = 0;
            }


            if (setting.CheckBoxWantDebugMode.Checked)
            {
                LogLoop.LaunchThis.DebugMode = DebugModeType.EnableWorkerThread | DebugModeType.WorkerDropCallbackForNoEvents;
                LogLoop.LaunchThis.CreationFlags |= InsightProcess.CreationFlagValues.DebugProcess;
                LogLoop.LaunchThis.UserDebugCallRoutine = EventLogPoster;
            }
            else
            {
                LogLoop.LaunchThis.DebugMode = DebugModeType.Default;
            }

            if (setting.CheckBoxWantDebugPriv.Checked)
            {
                LogLoop.LaunchThis.RequestDebugPriv = true;
            }
            else
            {
                LogLoop.LaunchThis.RequestDebugPriv = false;
            }

            if (setting.RadioButtonExplicitEnv.Checked)
            {
                LogLoop.LaunchThis.InheritDefaultEnviroment = false;
            }
            else
            {
                if (setting.RadioButtonInheritEnv.Checked)
                {
                    LogLoop.LaunchThis.InheritDefaultEnviroment = true;
                }
            }


            ProcessEnv(setting.TextBoxEnvTextFile.Text, LogLoop.LaunchThis);
            LogLoop.LaunchThis.WorkingDirectory = setting.TextBoxProcessStartingDir.Text;
            LogLoop.LaunchThis.ProcessArguments = setting.TextBoxProcessArguments.Text;
            LogLoop.LaunchThis.ProcessName = ComboBoxTarget.Text;
            // ("Not obsulate but not good code. Verify User input aka Bobby tables")]


            LogLoop.PostLogEvent("Attempt to Start Process " + LogLoop.LaunchThis.ProcessName + " Returend PID of " + LogLoop.LaunchThis.SpawnProcess().ToString());
            if (LogLoop.LaunchThis.CreationFlags.HasFlag(InsightProcess.CreationFlagValues.DebugProcess))
            {
                LogLoop.PostLogEvent("Debug Mode \r\n");
            }
            LogLoop.ShowDialog(this);

        }

        private void LoggerRunDialog_Load(object sender, EventArgs e)
        {

        }
    }
}
