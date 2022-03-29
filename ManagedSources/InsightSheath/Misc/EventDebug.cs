using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using InsightSheath.Wrappers;

namespace InsightSheath.Misc
{
    /// <summary>
    /// Implements an event debugger system for processing events. You will need to link a way to call <see cref="EventDebugAssist.FireEvents(DebugEvent)"/> in order to actually make to the thing work
    /// </summary>
    public class EventDebugAssist
    {
        
        /// <summary>
        /// 
        /// </summary>
        /// <param name="Ptr"></param>
        public delegate void CreateProcessEventCallback(DebugEvent Ptr);
        /// <summary>
        /// 
        /// </summary>
        /// <param name="Ptr"></param>
        public delegate void CreateThreadEventCallback(DebugEvent Ptr);
        /// <summary>
        /// 
        /// </summary>
        /// <param name="Ptr"></param>
        public delegate void ExceptionDebugEventCallback(DebugEvent Ptr);
        /// <summary>
        /// 
        /// </summary>
        /// <param name="Ptr"></param>
        public delegate void ExitProcessDebugEventCallback(DebugEvent Ptr);
        /// <summary>
        /// 
        /// </summary>
        /// <param name="Ptr"></param>
        public delegate void ExitThreadDebugEventCallback(DebugEvent Ptr);
        /// <summary>
        /// 
        /// </summary>
        /// <param name="Ptr"></param>
        public delegate void LoadDllDebugEventCallback(DebugEvent Ptr);
        /// <summary>
        /// 
        /// </summary>
        /// <param name="Ptr"></param>
        public delegate void OutputDebugStringCallback(DebugEvent Ptr);
        /// <summary>
        /// 
        /// </summary>
        /// <param name="Ptr"></param>
        public delegate void RipCallback(DebugEvent Ptr);
        /// <summary>
        /// 
        /// </summary>
        /// <param name="Ptr"></param>
        public delegate void UnloadDebugEventCallback(DebugEvent Ptr);
        /// <summary>
        /// 
        /// </summary>
        /// <param name="Ptr"></param>
        public delegate void GeneralDebugEventCallback(DebugEvent Ptr);

        /// <summary>
        /// Subscribe to get CreateProcess Events 
        /// </summary>
        public event CreateProcessEventCallback CreateProcessEvent;
        /// <summary>
        /// Subsrubte to get CreateThread 
        /// </summary>
        public event CreateThreadEventCallback CreateThreadEvent;
        /// <summary>
        /// Subscribe to get Exceptions
        /// </summary>
        public event ExceptionDebugEventCallback ExceptionEvent;
        /// <summary>
        /// Subscribe to get ExitProess
        /// </summary>
        public event ExitProcessDebugEventCallback ExitProcessEvent;
        /// <summary>
        /// Subscvribe to get ExitThread
        /// </summary>
        public event ExitThreadDebugEventCallback ExitThreadEvent;
        /// <summary>
        /// Subscribe to load Dll events
        /// </summary>
        public event LoadDllDebugEventCallback LoadDllEvent;
        /// <summary>
        /// Subsvrube to get output debug strings
        /// </summary>
        public event OutputDebugStringCallback OutputDebugStringEvent;
        /// <summary>
        /// 
        /// </summary>
        public event RipCallback RipEvent;
        /// <summary>
        /// Subscvribe to get RIp events
        /// </summary>
        public event UnloadDebugEventCallback UnloadDllEvent;
        /// <summary>
        /// Subscribe to catch all the other invents. IMPORTANT!.  This is called before the others!
        /// </summary>
        public event GeneralDebugEventCallback GeneralEvent;

        /// <summary>
        /// Your code will need to actual call this to trigger getting events.
        /// </summary>
        /// <param name="Ptr"></param>
        public virtual void FireEvents(DebugEvent Ptr)
        {
            if (Ptr != null)
            {
                GeneralEvent?.Invoke(Ptr);
                switch (Ptr.EventType)
                {
                    case DebugEventType.CreateProcessEvent:
                        {
                            CreateProcessEvent?.Invoke(Ptr);
                            break;
                        }
                    case DebugEventType.CreateTheadEvent:
                        {
                            CreateThreadEvent?.Invoke(Ptr);
                            break;
                        }
                    case DebugEventType.ExceptionEvent:
                        {
                            ExceptionEvent?.Invoke(Ptr);
                            break;
                        }
                    case DebugEventType.ExitProcessEvent:
                        {
                            ExitProcessEvent?.Invoke(Ptr);
                            break;
                        }
                    case DebugEventType.ExitThreadEvent:
                        {
                            ExitThreadEvent.Invoke(Ptr);
                            break;
                        }
                    case DebugEventType.LoadDllEvent:
                        {
                            LoadDllEvent?.Invoke(Ptr);
                            break;
                        }
                    case DebugEventType.OutputDebugString:
                        {
                            OutputDebugStringEvent?.Invoke(Ptr);
                            break;
                        }
                    case DebugEventType.RipEvent:
                        {
                            RipEvent?.Invoke(Ptr);
                            break;
                        }
                    case DebugEventType.UnloadDllEvent:
                        {
                            break;
                        }
                    default:
                        {
                            GeneralEvent?.Invoke(Ptr);
                            break;
                        }
                }
            }
        }
    }
}
