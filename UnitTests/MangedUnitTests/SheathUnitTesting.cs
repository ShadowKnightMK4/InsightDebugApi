using InsightSheath.Debugging;
using InsightSheath.Debugging.Process;
using InsightSheath.Debugging.Thread;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Windows.ApplicationModel.VoiceCommands;
using Windows.Networking.NetworkOperators;
using Windows.Storage.Provider;
using Windows.System;
using Windows.UI.WindowManagement;

namespace MangedUnitTests
{
    /// <summary>
    /// This class is used to import and define the various routines that support the unit test.
    /// </summary>
    /// <remarks>Important!  <see cref="Win32DirectImport_UnitTest.BasePath"/> is hard coded current to where the unittest folder is on my machine. Change this to where you store it.</remarks>
    public static class Win32DirectImport_UnitTest
    {
        [DllImport("Kernelbase.dll", SetLastError = true, CharSet = CharSet.Unicode)]
        static extern bool CompareObjectHandles(nint a, nint b);
        [DllImport("kernel32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
        static extern bool SetDllDirectoryW(string Path);
        [DllImport("kernel32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
        static extern bool Wow64EnableWow64FsRedirection(bool state);
        static string BuildVarient = "Debug";
        static string CodeVarient = "x64";
        /// <summary>
        /// Point this to where the unit test solution is located.
        /// </summary>
        public static  string BasePath = "C:\\Users\\Thoma\\source\\repos\\InsightAPI\\UnitTests\\MangedUnitTests";


        /// <summary>
        /// Compare two handles via Native routine CompareObjectHandles.
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <returns></returns>
        /// <remarks>Will call Assert.Fail() with a message if the native routine is not found</remarks>
        public static bool CompareHandles(nint a, nint b)
        {
            try
            {
                return CompareObjectHandles(a, b);
            }
            catch (EntryPointNotFoundException)
            {
                Assert.Fail("Unit Test Running on a system that doesn't offer CompareObjectHandles() Win32 routine. A quick fix is swap the inport out with something that implements this.");
            }
            return false;
        }
        public static string AssertFileExists_JustWaitProcessMsg(string FileLocation, bool Use86)
        {
            string msg = FileLocation + " does not appear to exist. Ensure it's built first by choosing to build the ";
            if (Use86)
            {
                msg += "x86 ";
            }
            else
            {
                msg += "x64 ";
            }
            msg += "code version in the configration settings";
            return msg;
        }
        /// <summary>
        /// Test if the file assists and calls Assert.Fail with the fail message if not.
        /// </summary>
        /// <param name="file"></param>
        /// <param name="fail"></param> 
        public static void AssertFileExists(string file, string fail)
        {
            if (!File.Exists(file))
            {
                Assert.Fail(fail);
            }
        }
        /// <summary>
        /// Return a base derived from where the UnitTest Process compiles doo.
        /// </summary>
        /// <returns></returns>
        public static string GetJustWaitProcessLocation(bool Use86)
        {
            string ret = Win32DirectImport_UnitTest.BasePath;
            ret = Path.GetDirectoryName(ret);
            ret = Path.GetDirectoryName(ret);

            ret += "\\Code\\Testing\\Debug\\JustWait";

            if (Use86)
            {
                ret += "x86.exe";
            }
            else
            {
                ret += "x64.exe";
            }
            return ret;
        }

        public static bool Wow64FileRedirect(bool enable)
        {
            return Wow64EnableWow64FsRedirection((bool) enable);
        }
        /// <summary>
        /// Set the approproate insightapi.dll for x86 or x64 bit code.
        /// </summary>
        /// <exception cref="InternalTestFailureException"></exception>
        public static void SetInsightNativeDllDirectory()
        {
            if (nint.Size == 4)
            {
                CodeVarient = "x86";
            }
            else
            {
                CodeVarient = "x64";
            }
            string relpath = BasePath;
            relpath = Path.GetDirectoryName(relpath);
            relpath = Path.GetDirectoryName(relpath);
            relpath += "\\Code\\Native\\InsightApi\\";
            relpath += BuildVarient + "\\" + CodeVarient + "";
            //if (!SetDllDirectoryW(relpath))
            //if (!SetDllDirectoryW(@"C:\Users\Thoma\source\repos\InsightAPI\Code\Native\InsightApi\Debug\x64"))
            if (!SetDllDirectoryW(relpath))
            {
                throw new InternalTestFailureException("SetDllDirectoW did not work. You can manually copy the required InsightAPI.dll to the direct the unit test compiles too or adjust the basepath variable in the unit tests for the sheath");
            }
            else
            {
                Console.WriteLine("SetDll Worked. " + "for ( "+CodeVarient +")"  + " InsightApi.DLL should be loaded by the Sheath Dll when needed.");
            }
        }
    }

    [TestClass]
    public class InsightProcess_UnitTests_SpawnProcess_Tests
    {
        [TestInitialize]
        public void Init()
        {
            Win32DirectImport_UnitTest.SetInsightNativeDllDirectory();
        }


        [TestMethod]
        public void InsightProcess_Create_WithDebugAndWorkerThread()
        {
            bool CreateProcessOk = false;
            int StubCallbackSigCreateProcess(IntPtr DebugEvent, IntPtr ContStat, IntPtr WaitTime, IntPtr Custom)
            {
                DebugEvent debugEvent = new DebugEvent(DebugEvent);

                
                if (debugEvent.EventType == DebugEventType.ExceptionEvent)
                {
                    debugEvent.ContinueDebugEvent(debugEvent.ProcessID, debugEvent.ThreadID, DebugContState.DebugExceptionNotHandled);
                }
                else
                {
                    debugEvent.ContinueDebugEvent(debugEvent.ProcessID, debugEvent.ThreadID, DebugContState.DebugContinueState);
                    if (debugEvent.EventType == DebugEventType.CreateProcessEvent)
                    {
                        CreateProcessOk = true;
                    }
                }
                return 0;
            }
            Process a1 = null;
            InsightProcess Demo = null;
            try
            {
                Demo = InsightProcess.CreateInstance();
                Demo.ProcessName = Win32DirectImport_UnitTest.GetJustWaitProcessLocation(false);
                Demo.CreationFlags = InsightProcess.CreationFlagValues.DebugProcess;
                Demo.DebugMode = DebugModeType.WorkerDropCallbackForNoEvents | DebugModeType.EnableWorkerThread;

                Demo.UserDebugCallRoutine = StubCallbackSigCreateProcess;

                Demo.ProcessName = Win32DirectImport_UnitTest.GetJustWaitProcessLocation(false);

                Demo.ProcessArguments = "200";

                a1 = Process.GetProcessById(Demo.SpawnProcess());


                while (a1.HasExited == false)
                {
                    if (CreateProcessOk)
                    {
                        break;
                    }
                }
            }
            finally
            {
                Demo?.Dispose();
            }
        }
        static int StubCallback(IntPtr DebugEvent, IntPtr ContStat, IntPtr WaitTime, IntPtr Custom)
        {
            DebugEvent debugEvent = new DebugEvent(DebugEvent);

            if (debugEvent.EventType == DebugEventType.ExceptionEvent)
            {
                debugEvent.ContinueDebugEvent(debugEvent.ProcessID, debugEvent.ThreadID, DebugContState.DebugExceptionNotHandled);
            }
            else
            {
                debugEvent.ContinueDebugEvent(debugEvent.ProcessID, debugEvent.ThreadID, DebugContState.DebugContinueState);
            }
            return 0;
        }
        [TestMethod]
        void InsightProcess_CreateSuspendedTest_Common(bool AddDebug)
        {
           
            Process a1 = null;
            InsightProcess Demo = null;
            try
            {
                Demo = InsightProcess.CreateInstance();
                Demo.ProcessName = Win32DirectImport_UnitTest.GetJustWaitProcessLocation(false);
                Demo.CreationFlags = InsightProcess.CreationFlagValues.CreateSuspended;


                if (AddDebug)
                {
                    Demo.ProcessArguments = "200";
                    Demo.CreationFlags |= InsightProcess.CreationFlagValues.DebugOnlyThisProcess;
                    Demo.UserDebugCallRoutine = StubCallback;
                    Demo.DebugMode = DebugModeType.WorkerDropCallbackForNoEvents | DebugModeType.EnableWorkerThread;
                    Demo.RequestDebugPriv = true;
                    a1 = Process.GetProcessById(Demo.SpawnProcess());

                    Thread.Sleep(300);

                    if (a1.HasExited == true)
                    {
                        Assert.Fail("Expected process to have not exited due to being debugged and us not processing events. It exiting means CREATE_SUSPENDED flag did not work.");
                    }
                    a1.Kill();
                }
                else
                {
                    Demo.ProcessArguments = "200";
                    Demo.CreationFlags |= InsightProcess.CreationFlagValues.CreateSuspended;
                    

                    a1 = Process.GetProcessById(Demo.SpawnProcess());

                    Thread.Sleep(300);

                    if (a1.HasExited == true)
                    {
                        Assert.Fail("Expected process to have not exited due to being spawned as CREATE_SUSPENEDED and it waiting less time than this test did");
                    }
                    
                }



            }
            finally
            {
                if (a1 != null)
                {
                    if (!a1.HasExited)
                    {
                        a1.Kill();
                    }
                }
                Demo?.Dispose();
            }
        }

        [TestMethod]
        public void InsightProcess_CreateSuspendedTest_WithDebug()
        {
            InsightProcess_CreateSuspendedTest_Common(true);
        }

        [TestMethod]
        public void InsightProcess_CreateSuspendedTest_NoDebug()
        {
            InsightProcess_CreateSuspendedTest_Common(false);
        }

        [TestMethod]
        public void InsightProcess_JustSpawn_ComparePID_And_Name_And_Handle_With_DiagnosticProcess()
        {
            Process a1, a2;
            a1 = a2 = null;
            InsightProcess Demo = null;
            try
            {
                Demo = InsightProcess.CreateInstance();
                Assert.IsNotNull(Demo);


                Demo.ProcessName = Win32DirectImport_UnitTest.GetJustWaitProcessLocation(false);
                Win32DirectImport_UnitTest.AssertFileExists(Demo.ProcessName, Win32DirectImport_UnitTest.AssertFileExists_JustWaitProcessMsg(Demo.ProcessName, false));

                a1 = Process.GetProcessById(Demo.SpawnProcess());

                Assert.AreEqual((uint)a1.Id, (uint)Demo.GetMainProcessId());
                Assert.AreEqual(Path.GetFileNameWithoutExtension(a1.ProcessName), Path.GetFileNameWithoutExtension(Demo.ProcessName));

                Assert.IsTrue(Win32DirectImport_UnitTest.CompareHandles(a1.Handle, Demo.GetMainProcessHandle()));
                a1.Kill();
            }
            finally
            {
                Demo?.Dispose();
                a1?.Kill();
                a2?.Kill();
            }
        }
    }
    [TestClass]
    public class InsightProcess_UnitTests_Simple
    {
        /*
        * tests if we can set and clear individual environemnt values
        * 
        * fail spots:
        * Can't create an InsightProcess instance.
        * Getting a prev assigned evnironemnt value does not work.
        * Deleting a prev assigned environment value does not result in null when getting it
        *      Delete is tested by both Setting the assigned value to null AND calling
        *      RemoveExplicitEnvironmentValue() too
        */
        [TestCategory("Environment Variables")]
        [TestMethod]
        public void Env_AssignAndRemoval()
        {
            InsightProcess test = null;
            try
            {
                test = InsightProcess.CreateInstance();
                Assert.IsTrue(test != null);

                test.SetExplicitEnviromentValue("HELLO", "HOW");

                string testval = test.GetExplicitEnviromentValue("HELLO");
                Assert.IsTrue((testval != null) && (testval == "HOW"));

                test.RemoveExplicitEnvironmentValue("HELLO");

                string testremove = test.GetExplicitEnviromentValue("HELLO");
                Assert.IsNull(testremove);

                test.SetExplicitEnviromentValue("HELLO2", "HI");
                string testval2 = test.GetExplicitEnviromentValue("HELLO2");

                Assert.IsTrue((testval2 != null) && (testval2 == "HI"));

                test.SetExplicitEnviromentValue("HELLO2", null);

                string testremove2 = test.GetExplicitEnviromentValue("HELLO2");
                Assert.IsNull(testremove2);

            }
            finally
            {
                test?.Dispose();
            }
        }
        [TestMethod]
        public void EnvAssign_BruteBuilkAmount()
        {

            long start = Process.GetCurrentProcess().PrivateMemorySize64;
            long after_allot1;
            long after_allot_clear1;

            InsightProcess Test = null;
            try
            {
                Dictionary<string, string> TestINput = new Dictionary<string, string>();
                Dictionary<string, string> TestOutput = new Dictionary<string, string>();
                Dictionary<string, string> TestOutput2 = new Dictionary<string, string>();
                Test = InsightProcess.CreateInstance();
                Assert.IsTrue(Test != null);


                for (int step = 0; step < 3000; step++)
                {
                    Test.SetExplicitEnviromentValue("test" + step.ToString(), step.ToString());
                    TestINput["test" + step.ToString()] = step.ToString();
                }
                after_allot1 = Process.GetCurrentProcess().PrivateMemorySize64;

                for (int step = 0; step < 3000; step++)
                {
                    TestOutput["test" + step.ToString()] = Test.GetExplicitEnviromentValue("test" + step.ToString());
                }




                // thank you bing gpt
                bool areEqual = TestINput.OrderBy(kvp => kvp.Key)
                    .SequenceEqual(TestOutput.OrderBy(kvp => kvp.Key))
                    && TestINput.All(kvp => TestOutput.ContainsKey(kvp.Key) && TestOutput[kvp.Key] == kvp.Value);


                Assert.IsTrue(areEqual);


                Test.WipeExplicitEnviroment();
                after_allot_clear1 = Process.GetCurrentProcess().PrivateMemorySize64;

                // clearing the env should also decrease memory.
                Assert.IsTrue(after_allot_clear1 < after_allot1);


                for (int step = 0; step < 3000; step++)
                {
                    TestOutput2["test" + step.ToString()] = Test.GetExplicitEnviromentValue("test" + step.ToString());
                }

                // thank you bing gpt
                bool areEqualAfter = TestINput.OrderBy(kvp => kvp.Key)
                    .SequenceEqual(TestOutput2.OrderBy(kvp => kvp.Key))
                    && TestINput.All(kvp => TestOutput2.ContainsKey(kvp.Key) && TestOutput2[kvp.Key] == kvp.Value);

                Assert.IsFalse(areEqualAfter);
            }
            finally
            {
                Test?.Dispose();
            }
        }
        [TestMethod]
        public void InsightProcess_CanSetGet_WorkingDir()
        {
            const string WorkDir = "C:\\ExampleDir";
            InsightProcess Demo = null;
            try
            {

                Demo = InsightProcess.CreateInstance();
                Assert.IsNotNull(Demo);
                Demo.WorkingDirectory = WorkDir;

                Assert.AreEqual(Demo.WorkingDirectory, WorkDir);
            }
            finally
            {
                Demo?.Dispose();
            }
        }



        [TestMethod]
        public void InsightProcess_CanSetGet_UserCallback()
        {
            int TestRoutine(nint a, nint b, nint c, nint d)
            {
                return 0;
            }
            InsightProcess Demo = null;
            try
            {

                Demo = InsightProcess.CreateInstance();
                Assert.IsNotNull(Demo);
                var NoAssignDefaultTest = Demo.UserDebugCallRoutine;

                Assert.IsNull(NoAssignDefaultTest);

                Demo.UserDebugCallRoutine = TestRoutine;
                Assert.AreEqual(TestRoutine, Demo.UserDebugCallRoutine);
            }
            finally
            {
                Demo?.Dispose();
            }
        }


        [TestMethod]
        public void InsightProcess_CanSetGet_RequestDebugPriv()
        {
            
            InsightProcess Demo = null;
            try
            {
                Demo = InsightProcess.CreateInstance();
                Assert.IsNotNull(Demo);

                Demo.RequestDebugPriv = true;
                Assert.AreEqual(true, Demo.RequestDebugPriv);
            }
            finally
            {
                Demo?.Dispose();
            }
        }




        [TestMethod]
        public void InsightProcess_CanSetGet_ProcessArgs()
        {
            const string TestVal = "/P /Q /R /S /T /u";
            InsightProcess Demo = null;
            try
            {
                Demo = InsightProcess.CreateInstance();
                Assert.IsNotNull(Demo);

                Demo.ProcessArguments = TestVal;
                Assert.AreEqual(TestVal, Demo.ProcessArguments);
            }
            finally
            {
                Demo?.Dispose();
            }
        }


        [TestMethod]
        public void InsightProcess_CanSetGet_ProcessName()
        {
            const string TestVal = "Something.exe";
            InsightProcess Demo = null;
            try
            {
                Demo = InsightProcess.CreateInstance();
                Assert.IsNotNull(Demo);

                Demo.ProcessName = TestVal;
                Assert.AreEqual(TestVal, Demo.ProcessName);
            }
            finally
            {
                Demo?.Dispose();
            }
        }
            [TestMethod]
        public void InsightProcess_CanSetGet_InheritEnviroment()
        {
            InsightProcess Demo = null;
            try
            {
                Demo = InsightProcess.CreateInstance();
                Assert.IsNotNull(Demo);

                Demo.InheritDefaultEnviroment = true;
                Assert.IsTrue(Demo.InheritDefaultEnviroment);
            }
            finally
            {
                Demo?.Dispose();
            }

        }

            [TestMethod]
        public void InsightProcess_CanSetGet_ExtraFlag()
        {
            InsightProcess Demo = null;
            try
            {
                Demo = InsightProcess.CreateInstance();
                Assert.IsNotNull(Demo);

                Demo.ExtraFlags = InsightProcess.SpecialCaseFlags.CreateSuspended | InsightProcess.SpecialCaseFlags.DebugOnlyThis;
                Assert.IsTrue(Demo.ExtraFlags == (InsightProcess.SpecialCaseFlags.CreateSuspended | InsightProcess.SpecialCaseFlags.DebugOnlyThis));
            }
            finally
            {
                Demo?.Dispose();
            }

        }


        [TestMethod]
        public void InsightProcess_CanSetGet_EnableSymbolEngine()
        {
            InsightProcess Demo = null;
            try
            {
                Demo = InsightProcess.CreateInstance();
                Assert.IsNotNull(Demo);

                Demo.EnableSymbolEngine = true;
                Assert.IsTrue(Demo.EnableSymbolEngine);
            }
            finally
            {
                Demo?.Dispose();
            }

        }


        [TestMethod]
        public void InsightProcess_CanSetGet_DetourMustSucceed()
        {
            InsightProcess Demo = null;
            try
            {
                Demo = InsightProcess.CreateInstance();
                Assert.IsNotNull(Demo);

                
                Assert.IsFalse(Demo.DetourMustSucceed);
            }
            finally
            {
                Demo?.Dispose();
            }

        }


        [TestMethod]
        public void InsightProcess_CanSetGet_DebugMode()
        {
            InsightProcess Demo = null;
            try
            {
                Demo = InsightProcess.CreateInstance();
                Assert.IsNotNull(Demo);

                Demo.DebugMode = DebugModeType.EnableWorkerThread | DebugModeType.WorkerDropCallbackForNoEvents;
                Assert.IsTrue(Demo.DebugMode == (DebugModeType.EnableWorkerThread | DebugModeType.WorkerDropCallbackForNoEvents));

            }
            finally
            {
                Demo?.Dispose();
            }

        }



        [TestMethod]
        public void InsightProcess_CanSetGet_CreationFlags()
        {
            InsightProcess Demo = null;
            try
            {
                Demo = InsightProcess.CreateInstance();
                Assert.IsNotNull(Demo);

                Demo.CreationFlags = InsightProcess.CreationFlagValues.DebugProcess;
                Assert.IsTrue(Demo.CreationFlags == InsightProcess.CreationFlagValues.DebugProcess);
                Demo.CreationFlags = (InsightProcess.CreationFlagValues)uint.MaxValue;
                Assert.IsTrue(Demo.CreationFlags == (InsightProcess.CreationFlagValues)uint.MaxValue);

            }
            finally
            {
                Demo?.Dispose();
            }

        }



        [TestMethod]
        public void InsightProcess_CanMake()
        {
            InsightProcess Demo = null;
            try
            {
                Demo = InsightProcess.CreateInstance();
                Assert.IsNotNull(Demo);
            }
            finally
            {
                Demo?.Dispose();
            }

        }
        [TestInitialize]
        public void init()
        {
            Win32DirectImport_UnitTest.SetInsightNativeDllDirectory();
        }
    }
    /*
     * Class Design
     *          Class is C++ based with a C# wrapper.
     *          Class Containns an https://learn.microsoft.com/en-us/windows/win32/api/psapi/ns-psapi-process_memory_counters_ex PROCESS_MEMORY_COUNTERS_EX struct
     *          It reads from it.
     *          Consider adding Mini Process RAM Dump  MiniDumpWriteDump 
     *              https://learn.microsoft.com/en-us/windows/win32/api/minidumpapiset/nf-minidumpapiset-minidumpwritedump
     *          Code Notations
     *                  Curently builds to your debugger insight api class.
     *                  TODO:  Theoriical overflow  THis means x86 code may have a too small buffer to hold an x64 bit call too read.
     *                  
     * Testing Requirements as for class
     * Can we Create an Instance?
     *          based on a process Handle?
     *          based on a process ID?
     *  Can we duplicate an instead i.e. create an instance based of an existing copy?
     *      
     *  Reading
     *      Can We Read an x64 bit process as x86?
     *      Can We Read an x86 process as x64?
     *          
     *  Dependancys to test above
     *      need demo process to read from for x86 and x64. 
     *      Consider a direct call to GetProcessMemory() in C# to see if the code path works from InsightMemory to the C# sode
     *      
     *  Structs in C# to test with this
     *      ProcessMemoryCount64
     *      ProcessMemoryCount32.
     */
    [TestCategory("InsightMemoryTest")]
    [TestClass]
    public class InsightMemoryTests
    {
        [TestInitialize]
        public void Init()
        {
            Win32DirectImport_UnitTest.SetInsightNativeDllDirectory();
        }

        
        [TestMethod]
        public void TestCreateInstance()
        {
            InsightMemory Test=null;
            try
            {
                Test = InsightMemory.CreateInstance();
                Assert.IsNotNull(Test);
            }
            finally
            {
                Test?.Dispose();
            }
        }


        /// <summary>
        /// Tests if we can successfully read ProcessMemory32 (or ProcessMemory64 depending on what C# code we got) from an x86 process
        /// </summary>
        [TestMethod]
        public void TestReadMemoryx86()
        {
            TestReadMemoryCommon(true);
        }

        /// <summary>
        /// Tests if we can successfully read ProcessMemory32 (or ProcessMemory64 depending on what C# code we got) from an x64 process
        /// </summary>
        [TestMethod]
        public void TestReadMememryx64()
        {
            TestReadMemoryCommon(false);
        }
        public void TestReadMemoryCommon(bool Use86)
        {
            InsightMemory Test = null;
            ProcessStartInfo Target = new ProcessStartInfo();
            Target.FileName = Win32DirectImport_UnitTest.GetJustWaitProcessLocation(Use86);

            Win32DirectImport_UnitTest.AssertFileExists(Target.FileName, Win32DirectImport_UnitTest.AssertFileExists_JustWaitProcessMsg(Target.FileName, Use86));
            

            Process TryMe = Process.Start(Target);
            {
                Test =  InsightMemory.CreateInstance(TryMe.Handle);

                /* For C# code size, we're going to need know the size of the pointer to get meniful stuff.*/

                if (nint.Size == 8)
                {
                    var TestMarshal = Test.MemoryStatsBulk64;
                    Assert.IsTrue(TestMarshal.HasValue);
                    Assert.IsTrue(TestMarshal.Value.cb == ProcessMemoryCountConstants.Expected64BitSize);
                }
                else
                {
                    var TestMarshal = Test.MemoryStatsBulk32;
                    Assert.IsTrue(TestMarshal.HasValue);
                    Assert.IsTrue(TestMarshal.Value.cb == ProcessMemoryCountConstants.Expected86BitSize);
                }

                {
                    var TestMarshal = Test.MemoryStatsBulkGen;
                    Assert.IsTrue(TestMarshal.HasValue);
                    Assert.IsTrue(TestMarshal.Value.cb == ProcessMemoryCountConstants.Expected64BitSize);
                }
            }
        }
        [TestMethod]
        public void TestDuplicateFromExisting()
        {
            InsightMemory Dup = null;
            InsightMemory Test = null;
            try
            {
                Test = InsightMemory.CreateInstance();
                Assert.IsNotNull(Test);

                Assert.IsTrue(Test.SetTargetProcessViaHandle(Process.GetCurrentProcess().Handle));


                Dup = InsightMemory.DuplicateInstance(Test);

                Assert.IsNotNull(Dup);
                Assert.IsTrue(Dup.NativePointer != Test.NativePointer);
                Assert.IsTrue(Dup.GetTargetProcessID() == Test.GetTargetProcessID());

                
            }
            finally
            {
                Test?.Dispose();
                Dup?.Dispose();
            }
        }

        [TestMethod]
        public void TestAssignFromHandle()
        {
            InsightMemory Test = null;
            try
            {
                Test = InsightMemory.CreateInstance();
                Assert.IsNotNull(Test);

                Assert.IsTrue(Test.SetTargetProcessViaHandle(Process.GetCurrentProcess().Handle));
            }
            finally
            {
                Test?.Dispose();
            }
        }


        [TestMethod]
        public void TestAssignFromPID()
        {
            InsightMemory Test = null;
            try
            {
                Test = InsightMemory.CreateInstance();
                Assert.IsNotNull(Test);

                Assert.IsTrue(Test.SetTargetProcessViaProcessId(Process.GetCurrentProcess().Id));
            }
            finally
            {
                Test?.Dispose();
            }
        }
    }
    static class NativeAndNETPairing_ClassExtForTests
    {
        /// <summary>
        /// Unit Test version of <see cref="DebugEvent.GetDebugEventCreateProcessInfo"/>. Does not include the check for if it's the correct exception.
        /// </summary>
        /// <param name="ptr">ptr</param>
        /// <returns>returns a new instance of DebugEventCreateProcessInfo pointing to the same unmanaged pointer as DebugEvent</returns>
        public static DebugEventCreateProcessInfo GetUnitTestVersionCreateProcessInfo(this DebugEvent ptr)
        {
            ptr.AddRef();
            var ret = new DebugEventCreateProcessInfo(ptr.NativePointer, true, ptr.ReferenceCount);
            if (ret == null)
            {
                ptr.Release();
            }
            return ret;
        }

        /// <summary>
        /// Unit Test version of <see cref="DebugEvent.GetDebugEventCreateThreadInfo"/>. Does not include the check for if it's the correct exception.
        /// </summary>
        /// <param name="ptr">ptr</param>
        /// <returns>returns a new instance of DebugEventCreateThreadInfo pointing to the same unmanaged pointer as DebugEvent</returns>
        public static DebugEventCreateThreadInfo GetUnitTestVersionCreateThreadInfo(this DebugEvent ptr)
        {
            ptr.AddRef();
            var ret = new DebugEventCreateThreadInfo(ptr.NativePointer, true, ptr.ReferenceCount);
            if (ret == null)
            {
                ptr.Release();
            }
            return ret;
        }

        /// <summary>
        /// Unit Test version of <see cref="DebugEvent.GetDebugEventExceptionInfo"/>. Does not include the check for if it's the correct exception.
        /// </summary>
        /// <param name="ptr">ptr</param>
        /// <returns>returns a new instance of DebugEventExceptionInfo pointing to the same unmanaged pointer as DebugEvent</returns>
        public static DebugEventExceptionInfo GetUnitTestVersionExceptionInfo(this DebugEvent ptr)
        {
            ptr.AddRef();
            var ret = new DebugEventExceptionInfo(ptr.NativePointer, true, ptr.ReferenceCount);
            if (ret == null)
            {
                ptr.Release();
            }
            return ret;
        }

        /// <summary>
        /// Unit Test version of <see cref="DebugEvent.GetEventExitProcessInfo"/>. Does not include the check for if it's the correct exception.
        /// </summary>
        /// <param name="ptr">ptr</param>
        /// <returns>returns a new instance of DebugEventExitProcessInfo pointing to the same unmanaged pointer as DebugEvent</returns>
        public static DebugEventExitProcessInfo GetUnitTestVersionExitProcessInfo(this DebugEvent ptr)
        {
            ptr.AddRef();
            var ret = new DebugEventExitProcessInfo(ptr.NativePointer, true, ptr.ReferenceCount);
            if (ret == null)
            {
                ptr.Release();
            }
            return ret;
        }

        /// <summary>
        /// Unit Test version of <see cref="DebugEvent.GetEventExitThreadInfo"/>. Does not include the check for if it's the correct exception.
        /// </summary>
        /// <param name="ptr">ptr</param>
        /// <returns>returns a new instance of DebugEventExitThreadInfo pointing to the same unmanaged pointer as DebugEvent</returns>
        public static DebugEventExitThreadInfo GetUnitTestVersionExitThreadInfo(this DebugEvent ptr)
        {
            ptr.AddRef();
            var ret = new DebugEventExitThreadInfo(ptr.NativePointer, true, ptr.ReferenceCount);
            if (ret == null)
            {
                ptr.Release();
            }
            return ret;
        }

        /// <summary>
        /// Unit Test version of <see cref="DebugEvent.GetDebugEventLoadDll"/>. Does not include the check for if it's the correct exception.
        /// </summary>
        /// <param name="ptr">ptr</param>
        /// <returns>returns a new instance of DebugEventLoadDllInfo pointing to the same unmanaged pointer as DebugEvent</returns>
        public static DebugEventLoadDllInfo GetUnitTestVersionLoadDllInfo(this DebugEvent ptr)
        {
            ptr.AddRef();
            var ret = new DebugEventLoadDllInfo(ptr.NativePointer, true, ptr.ReferenceCount);
            if (ret == null)
            {
                ptr.Release();
            }
            return ret;
        }

        /// <summary>
        /// Unit Test version of <see cref="DebugEvent.GetDebugEventUnloadDllInfo"/>. Does not include the check for if it's the correct exception.
        /// </summary>
        /// <param name="ptr">ptr</param>
        /// <returns>returns a new instance of DebugEventUnloadDllInfo pointing to the same unmanaged pointer as DebugEvent</returns>
        public static DebugEventUnloadDllInfo GetUnitTestVersionUnLoadDllInfo(this DebugEvent ptr)
        {
            ptr.AddRef();
            var ret = new DebugEventUnloadDllInfo(ptr.NativePointer, true, ptr.ReferenceCount);
            if (ret == null)
            {
                ptr.Release();
            }
            return ret;
        }

        /// <summary>
        /// Unit Test version of <see cref="DebugEvent.GetDebugEventStringInfo"/>. Does not include the check for if it's the correct exception.
        /// </summary>
        /// <param name="ptr">ptr</param>
        /// <returns>returns a new instance of DebugEventStringInfo pointing to the same unmanaged pointer as DebugEvent</returns>
        public static DebugEventStringInfo GetUnitTestVersionStringInfo(this DebugEvent ptr)
        {
            ptr.AddRef();
            var ret = new DebugEventStringInfo(ptr.NativePointer, true, ptr.ReferenceCount);
            if (ret == null)
            {
                ptr.Release();
            }
            return ret;
        }

        /// <summary>
        /// Unit Test version of <see cref="DebugEvent.GetDebugEventRipInfo"/>. Does not include the check for if it's the correct exception.
        /// </summary>
        /// <param name="ptr">ptr</param>
        /// <returns>returns a new instance of DebugEventRipInfo pointing to the same unmanaged pointer as DebugEvent</returns>
        public static DebugEventRipInfo GetUnitTestVersionRipInfo(this DebugEvent ptr)
        {
            ptr.AddRef();
            var ret = new DebugEventRipInfo(ptr.NativePointer, true, ptr.ReferenceCount);
            if (ret == null)
            {
                ptr.Release();
            }
            return ret;
        }

    }
    [TestClass]
    [TestCategory("Sheath Unit Tests")]
    public class NativeAndNETPairing
    {
        
     
        [TestCategory("InsightProcess")]
        [TestMethod]
        public void InsightProcess_SetAndGetProcessName()
        {
            string Target = Directory.GetCurrentDirectory();
            InsightProcess test = null;
            try
            {
                test = InsightProcess.CreateInstance();
                Assert.IsTrue(test != null);
                test.ProcessName = Target;

                Assert.IsTrue(test.ProcessName == Target);
            }
            finally
            {
                test?.Dispose();
            }
        }



        [TestCategory("InsightProcess")]
        [TestMethod]
        public void InsightProcess_SetAndGetProcessArguments()
        {
            string Target = Directory.GetCurrentDirectory();
            InsightProcess test = null;
            try
            {
                test = InsightProcess.CreateInstance();
                Assert.IsTrue(test != null);
                test.ProcessArguments = Target;

                Assert.IsTrue(test.ProcessArguments == Target);
            }
            finally
            {
                test?.Dispose();
            }
        }



        [TestCategory("InsightProcess")]
        [TestMethod]
        public void InsightProcess_SetAndGetWorkingDir()
        {
            string Target = Directory.GetCurrentDirectory();
            InsightProcess test = null;
            try
            {
                test = InsightProcess.CreateInstance();
                Assert.IsTrue(test != null);
                test.WorkingDirectory = Target;

                Assert.IsTrue(test.WorkingDirectory == Target);
            }
            finally
            {
                test?.Dispose();
            }
        }

        [TestCategory("DebugEvent Structure")]
        [TestMethod]
        public void DebugEvent_Reference_Increase_And_Release()
        {


            
               
            DebugEvent Test = null;
            try
            {
                Test = DebugEvent.CreatePrivateStruct();
                Assert.IsNotNull(Test);

                

                DebugEventCreateProcessInfo a = Test.GetUnitTestVersionCreateProcessInfo();
                DebugEventCreateThreadInfo b = Test.GetUnitTestVersionCreateThreadInfo();
                DebugEventExceptionInfo c = Test.GetUnitTestVersionExceptionInfo();
                DebugEventExitProcessInfo e = Test.GetUnitTestVersionExitProcessInfo();
                DebugEventExitThreadInfo f = Test.GetUnitTestVersionExitThreadInfo();
                DebugEventLoadDllInfo g = Test.GetUnitTestVersionLoadDllInfo();
                DebugEventUnloadDllInfo h = Test.GetUnitTestVersionUnLoadDllInfo();
                DebugEventRipInfo i = Test.GetUnitTestVersionRipInfo();
                DebugEventStringInfo j = Test.GetUnitTestVersionStringInfo();
                
                       Assert.IsTrue(Test.ReferenceCounter == 10); // one for each event strut we got
                                                                   // they all point to the same native pointer
                                                                   // and can be released by the parent class
                                                                   // when this reference counter is 0 safely.





                a.Dispose();
                b.Dispose();
                c.Dispose();
                e.Dispose();
                f.Dispose();
                g.Dispose();
                h.Dispose();
                i.Dispose();
                j.Dispose();
                Test.Dispose();

                Assert.IsTrue(Test.ReferenceCounter == 0);
            }
            finally
            {
                // won't actually be needed if executio is nmormally followed. 
                Test?.Dispose();
            }
        }
        [TestCategory("DebugEvent Structure")]
        [TestMethod]
        public void DebugEvent_StructCreation()
        {
            DebugEvent Test = null;
            try
            {
                Test = DebugEvent.CreatePrivateStruct();
                Assert.IsNotNull(Test);

                
            }
            finally
            {
                Test?.Dispose();
            }
        }
        [TestCategory("Process Spawn")]
        [TestMethod]
        public void CanSpawnProcess()
        {
            InsightProcess test = InsightProcess.CreateInstance();
            try
            {
                Assert.IsNotNull(test);


                test.CreationFlags = InsightProcess.CreationFlagValues.CreateSuspended;
                test.ProcessName =  Environment.GetFolderPath( Environment.SpecialFolder.System ) + "\\notepad.exe";

                
                var id = test.SpawnProcess();

                var thread = test.GetMainThreadId();

                var threadclass = ThreadContext.CreateInstance(thread);
                threadclass.ResumeThread();
                Assert.IsTrue( id != 0);
                var man = Process.GetProcessById(id);

                man.Kill();
            }
            finally
            {
                test?.Dispose();
            }
        }
        [TestInitialize]
        public void Initialize()
        {
            Win32DirectImport_UnitTest.SetInsightNativeDllDirectory();
        }
        [TestCategory("Detour Setting Check")]
        [TestCategory("Memory Leak Check")]
        [TestMethod]
        public void InsightProcess_DetourSetting_Array()
        {
            InsightProcess test = null;
            Console.WriteLine("InsightProcess_DetourSetting_Array is intended to see if we have a memory leak in adding detour entries.  It may throw an execption regardless due to what I understand about shrink_to_fit().");
            try
            {
                Assert.IsNotNull(test);

                void populate(int amount)
                {
                    for (int step = 0; step < amount; step++)
                    {
                        test.AddDetoursDll("TestDetour" + step.ToString() + ".dll");
                    }
                }


                long start0 = Process.GetCurrentProcess().PrivateMemorySize64;

                populate(10000);
                long start1 = Process.GetCurrentProcess().PrivateMemorySize64;
                populate(10000);
                long start2 = Process.GetCurrentProcess().PrivateMemorySize64;
                populate(10000);
                long start3 = Process.GetCurrentProcess().PrivateMemorySize64;
                populate(10000);
                long start4 = Process.GetCurrentProcess().PrivateMemorySize64;
                populate(10000);
                long start5 = Process.GetCurrentProcess().PrivateMemorySize64;
                populate(10000);
                long start6 = Process.GetCurrentProcess().PrivateMemorySize64;
                populate(10000);


                test.ResetDetoursDllList();
                long testvalk = Process.GetCurrentProcess().PrivateMemorySize64;

                Assert.IsTrue(testvalk < start1);

                populate(10000);
                long start7 = Process.GetCurrentProcess().PrivateMemorySize64;
                populate(10000);
                long start8 = Process.GetCurrentProcess().PrivateMemorySize64;
                populate(10000);
                long start9 = Process.GetCurrentProcess().PrivateMemorySize64;
                populate(10000);
                long start10 = Process.GetCurrentProcess().PrivateMemorySize64;
                populate(10000);
                long start11 = Process.GetCurrentProcess().PrivateMemorySize64;
                populate(10000);
                long start12 = Process.GetCurrentProcess().PrivateMemorySize64;
                populate(10000);

                test.ResetDetoursDllList();

                Assert.IsTrue(Process.GetCurrentProcess().PrivateMemorySize64 < start1);
            }
            finally
            {
                test?.Dispose();
            }

        }

        [TestCategory("Detour Setting Check")]
        [TestMethod]
        public void InsightProcess_DetourSettingsTests_NonArray()
        {
            InsightProcess test = null;

            
            try
            {
                test = InsightProcess.CreateInstance();
                Assert.IsNotNull(test);
   


                Assert.IsTrue(test.DetourMustSucceed == false);

            }
            finally
            {
                test?.Dispose();
            }
        }

        [TestCategory("Flag Test for InsightProcess")]
        /*
         * Sets if we can set and get all flags in the class that aren't arrays
         */
        [TestMethod]
        public void InsightProcess_FlagTests()
        {
            int StDebugEventCallBackRoutine(IntPtr DebugEvent, IntPtr ContinueState, IntPtr WaitTimer, IntPtr CustomArg)
            {
                return 0;
            }
        InsightProcess test = null;
            try
            {
                test = InsightProcess.CreateInstance();

                test.CreationFlags = (InsightProcess.CreationFlagValues)uint.MaxValue;

                Assert.IsTrue((uint)test.CreationFlags == uint.MaxValue);

                test.DebugMode = DebugModeType.WorkerDropCallbackForNoEvents;

                Assert.IsTrue(test.DebugMode == DebugModeType.WorkerDropCallbackForNoEvents);


                test.EnableSymbolEngine = true;

                Assert.IsTrue(test.EnableSymbolEngine);

                test.CreationFlags = (InsightProcess.CreationFlagValues)uint.MaxValue;

                Assert.AreEqual(test.CreationFlags, (InsightProcess.CreationFlagValues)uint.MaxValue);

                test.CreationFlags = (InsightProcess.CreationFlagValues)uint.MaxValue;

                test.ExtraFlags = InsightProcess.SpecialCaseFlags.CreateSuspended | InsightProcess.SpecialCaseFlags.DebugOnlyThis;

                Assert.IsTrue(test.ExtraFlags == (InsightProcess.SpecialCaseFlags.CreateSuspended | InsightProcess.SpecialCaseFlags.DebugOnlyThis));

                test.InheritDefaultEnviroment = true;

                Assert.IsTrue(test.InheritDefaultEnviroment);

                test.ProcessArguments = "Test Arguments";

                Assert.IsTrue(string.Compare(test.ProcessArguments, "Test Arguments") == 0);

                test.ProcessName = "Test name.exe";
                Assert.IsTrue(string.Compare(test.ProcessName, "Test name.exe") == 0);

                test.RequestDebugPriv = true;
                Assert.IsTrue(test.RequestDebugPriv);

                
                test.UserDebugCallRoutine = StDebugEventCallBackRoutine;


                Assert.IsTrue(test.UserDebugCallRoutine == StDebugEventCallBackRoutine);

                

            }
            finally
            {
                test?.Dispose();
            }
        }
       
        /*
         * Tests if we can get and set 3000 env values correctly.
         * Tests if we can wipe the set values with one call and by doing so frees the memory
         * 
         * failure stuff:
         * Unable to load the InsightAPI.DLL
         * Unable to make an instance of InsightProcess.
         * Unable to set and then later recall  up to 3000 same enviromenbt values.
         * Recalled Enviroment Values differ than what was set
         * WipeEnviroemnetValue does not reduce privatememorysize64 in self
         * 
         */
        [TestCategory("Environment Variables")]
        [TestCategory("Memory Leak Tests")]
        [TestCategory("Passes single run")]
        [TestMethod]
       
        public void SetupCheck()
        {

            InsightProcess Test=null;

            try
            {
                Test = InsightProcess.CreateInstance();
                Assert.IsTrue(Test != null);
            }
            finally
            {
                Test?.Dispose();
            }
            
        }
    }

    
    public class SheathUnitTesting
    {
        [TestMethod]
        public void TestMethod1()
        {
        }
    }
}