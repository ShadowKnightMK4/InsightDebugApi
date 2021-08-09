



/*
* From https://processhacker.sourceforge.io/doc/ntpsapi_8h.html on 7/12/21
*   Most of these I'm not terribly sure of their meaning / use.     I needed the Function Definiation for CreateUserProcess to
* be able to hook to send to the FileSandbox Debugger part of the module
* */


  typedef struct _PS_ATTRIBUTE
  {
          ULONG Attribute;
          SIZE_T Size;
          union
              {
                  ULONG Value;
                  PVOID ValuePtr;
              };
          PSIZE_T ReturnLength;
      } PS_ATTRIBUTE, * PPS_ATTRIBUTE;
  typedef struct _PS_ATTRIBUTE_LIST
  {
          SIZE_T TotalLength;
          PS_ATTRIBUTE Attributes[1];
      } PS_ATTRIBUTE_LIST, * PPS_ATTRIBUTE_LIST;

  // windows-internals-book:"Chapter 5"
  typedef enum _PS_CREATE_STATE
  {
          PsCreateInitialState,
              PsCreateFailOnFileOpen,
              PsCreateFailOnSectionCreate,
              PsCreateFailExeFormat,
              PsCreateFailMachineMismatch,
              PsCreateFailExeName, // Debugger specified
              PsCreateSuccess,
              PsCreateMaximumStates
          } PS_CREATE_STATE;

typedef struct
{
    SIZE_T Size;
    PS_CREATE_STATE State;
    union
    {
         // PsCreateInitialState
                struct
                 {
                     union
                     {
                           ULONG InitFlags;
                           struct
                           {
                                   UCHAR WriteOutputOnExit : 1;
                                     UCHAR DetectManifest : 1;
                                     UCHAR IFEOSkipDebugger : 1;
                                     UCHAR IFEODoNotPropagateKeyState : 1;
                                     UCHAR SpareBits1 : 4;
                                     UCHAR SpareBits2 : 8;
                                     USHORT ProhibitedImageCharacteristics : 16;
                                 };
                         };
                     ACCESS_MASK AdditionalFileAccess;
                 } InitState;
    
                 // PsCreateFailOnSectionCreate
                 struct
                 {
                     HANDLE FileHandle;
                 } FailSection;
    
                 // PsCreateFailExeFormat
                 struct
                 {
                     USHORT DllCharacteristics;
                 } ExeFormat;
    
                 // PsCreateFailExeName
                 struct
                 {
                     HANDLE IFEOKey;
                 } ExeName;
    
                 // PsCreateSuccess
                 struct
                 {
                     union
                         {
                             ULONG OutputFlags;
                             struct
                                 {
                                     UCHAR ProtectedProcess : 1;
                                     UCHAR AddressSpaceOverride : 1;
                                     UCHAR DevOverrideEnabled : 1; // from Image File Execution Options
                                     UCHAR ManifestDetected : 1;
                                     UCHAR ProtectedProcessLight : 1;
                                     UCHAR SpareBits1 : 3;
                                     UCHAR SpareBits2 : 8;
                                     USHORT SpareBits3 : 16;
                                 };
                         };
                     HANDLE FileHandle;
                     HANDLE SectionHandle;
                     ULONGLONG UserProcessParametersNative;
                     ULONG UserProcessParametersWow64;
                     ULONG CurrentParameterFlags;
                     ULONGLONG PebAddressNative;
                     ULONG PebAddressWow64;
                     ULONGLONG ManifestAddress;
                     ULONG ManifestSize;
                 } SuccessState;
        };
 } PS_CREATE_INFO, * PPS_CREATE_INFO;


typedef NTSTATUS (NTAPI *NtCreateUserProcessPtr)(
    _Out_ PHANDLE ProcessHandle,
    _Out_ PHANDLE ThreadHandle,
    _In_ ACCESS_MASK ProcessDesiredAccess,
    _In_ ACCESS_MASK ThreadDesiredAccess,
    _In_opt_ POBJECT_ATTRIBUTES ProcessObjectAttributes,
    _In_opt_ POBJECT_ATTRIBUTES ThreadObjectAttributes,
    _In_ ULONG ProcessFlags, // PROCESS_CREATE_FLAGS_*
    _In_ ULONG ThreadFlags, // THREAD_CREATE_FLAGS_*
    _In_opt_ PRTL_USER_PROCESS_PARAMETERS ProcessParameters, // PRTL_USER_PROCESS_PARAMETERS
    _Inout_ PPS_CREATE_INFO CreateInfo,
    _In_opt_ PPS_ATTRIBUTE_LIST AttributeList);