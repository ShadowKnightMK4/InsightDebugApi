using InsightSheath.Debugging;
using InsightSheath.Debugging.Process;
using InsightSheath.Debugging.Thread;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Diagnostics;
using System.Reflection;
using System.Runtime.InteropServices;
using Windows.System;
using Windows.UI.WindowManagement;

namespace MangedUnitTests
{
    [TestClass]
    [TestCategory("Sheath Unit Tests")]
    public class NativeAndNETPairing
    {
        [DllImport("kernel32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
        static extern bool SetDllDirectoryW(string Path);
        static string BuildVarient = "Debug";
        static string CodeVarient = "x64";
        static string BasePath = "C:\\Users\\Thoma\\source\\repos\\InsightAPI\\UnitTests\\MangedUnitTests";

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
            string relpath = BasePath;
            relpath = Path.GetDirectoryName(relpath);
            relpath = Path.GetDirectoryName(relpath);
            relpath += "\\Code\\Native\\InsightApi\\";
            relpath += BuildVarient + "\\" + CodeVarient + "";
            //if (!SetDllDirectoryW(relpath))
            if (!SetDllDirectoryW(@"C:\Users\Thoma\source\repos\InsightAPI\Code\Native\InsightApi\Debug\x64"))
            {
                throw new InternalTestFailureException("SetDllDirectoW did not work. You can manually copy the required InsightAPI.dll to the direct the unit test compiles too or adjust the basepath variable in the unit tests for the sheath");
            }
            else
            {
                Console.WriteLine("SetDll Worked. InsightApi.DLL should be loaded by the Sheath Dll when needed.");
            }
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
                test.DetourMustSucceed = true;

                test.DetourMustSucceed = false;


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


                for (int step =0;step < 3000;step++)
                {
                    Test.SetExplicitEnviromentValue("test" + step.ToString(), step.ToString());
                    TestINput["test" + step.ToString()] = step.ToString();
                }
                after_allot1 = Process.GetCurrentProcess().PrivateMemorySize64; 

                for (int step = 0; step < 3000;step++)
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