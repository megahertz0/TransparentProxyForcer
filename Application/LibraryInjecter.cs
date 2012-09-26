using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.ServiceProcess;
using System.Text;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;

namespace TransparentProxyForcer
{
    public partial class LibraryInjecter : ServiceBase
    {

        public LibraryInjecter()
        {
            InitializeComponent();
        }

        protected override void OnStart(string[] args)
        {
            installHook(true);
        }

        public static String installHook()
        {
            return installHook(false);
        }

        public static String installHook(bool pernament) {
            String path = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            String log = path + "\\injecter.txt";
            string[] keys = { "\\KnownDlls32", "\\KnownDlls" };
            foreach(string key in keys) {
                Native.OBJECT_ATTRIBUTES section = new Native.OBJECT_ATTRIBUTES(key + "\\WS2_32.dll", Native.OBJ_CASE_INSENSITIVE);
                IntPtr ptr;
                Native.NtStatus status = Native.ZwOpenSection(out ptr, Native.SECTION_ALL_ACCESS, ref section);
                if (status == 0) {
                    File.AppendAllText(log, "Removing: " + section.ObjectName + "\r\n");                
                    status = Native.ZwMakeTemporaryObject(ptr);
                    if (status != 0) {
                        File.AppendAllText(log, "Could not remove: " + section.ObjectName + " - error " 
                            + String.Format("0x{0:X}", status) + ")\r\n");
                    }
                    status = Native.ZwClose(ptr);
                    if (status != 0) {
                        File.AppendAllText(log, "Could not close: " + section.ObjectName + " - error "
                            + String.Format("0x{0:X}", status) + ")\r\n");
                    }
                }
            }
            IntPtr pPath;
            if (Native.SHGetKnownFolderPath(Native.KnownFolder.Windows, 0, IntPtr.Zero, out pPath) == 0) {
                string s = Marshal.PtrToStringUni(pPath);
                Marshal.FreeCoTaskMem(pPath);
                Native.CreateSymbolicLink(s + "\\system32\\ws2_32_org.dll", s + "\\system32\\ws2_32.dll", Native.SYMBOLIC_LINK_FLAG.File);
                Native.CreateSymbolicLink(s + "\\SysWOW64\\ws2_32_org.dll", s + "\\SysWOW64\\ws2_32.dll", Native.SYMBOLIC_LINK_FLAG.File);
            }
            Native.OBJECT_ATTRIBUTES dll32 = new Native.OBJECT_ATTRIBUTES("\\DosDevices\\" + path + "\\ws2_32.dll",
                Native.OBJ_CASE_INSENSITIVE);
            Native.OBJECT_ATTRIBUTES dll64 = new Native.OBJECT_ATTRIBUTES("\\DosDevices\\" + path + "\\x64\\ws2_32.dll",
                Native.OBJ_CASE_INSENSITIVE);
            IntPtr ws2_32, ws2_64;
            Native.IO_STATUS_BLOCK iosb32 = new Native.IO_STATUS_BLOCK(), iosb64 = new Native.IO_STATUS_BLOCK();
            Native.NtStatus statDll32 = Native.ZwOpenFile(out ws2_32, Native.FILE_GENERIC_READ | Native.FILE_GENERIC_WRITE | Native.FILE_GENERIC_EXECUTE,
                ref dll32, ref iosb32, Native.FILE_SHARE_READ | Native.FILE_SHARE_WRITE, 0);
            Native.NtStatus statDll64 = Native.ZwOpenFile(out ws2_64, Native.FILE_GENERIC_READ | Native.FILE_GENERIC_WRITE | Native.FILE_GENERIC_EXECUTE,
                ref dll64, ref iosb64, Native.FILE_SHARE_READ | Native.FILE_SHARE_WRITE, 0);
            if (statDll32 != 0) {
                File.AppendAllText(log, "Could not open: " + dll32.ObjectName + ", assuming uninstallation (error "
                    + String.Format("0x{0:X}", statDll32) + ")\r\n");
            } else {
                File.AppendAllText(log, "Opened: " + dll32.ObjectName + "\r\n");
                Native.OBJECT_ATTRIBUTES secDir32 = new Native.OBJECT_ATTRIBUTES("\\KnownDlls32", Native.OBJ_CASE_INSENSITIVE);
                Native.OBJECT_ATTRIBUTES secDir64 = new Native.OBJECT_ATTRIBUTES("\\KnownDlls", Native.OBJ_CASE_INSENSITIVE);
                IntPtr dir32, dir64;
                Native.NtStatus statSec32 = Native.ZwOpenDirectoryObject(out dir32, Native.DIRECTORY_ALL_ACCESS, ref secDir32);
                Native.NtStatus statSec64 = Native.ZwOpenDirectoryObject(out dir64, Native.DIRECTORY_ALL_ACCESS, ref secDir64);
                if (statSec64 != 0) {
                    File.AppendAllText(log, "Could not open: " + secDir64.ObjectName + " (error "
                        + String.Format("0x{0:X}", statSec64) + ")\r\n");
                    File.AppendAllText(log, "FAIL!\r\n");
                    if (statDll32 == 0) {
                        Native.ZwClose(ws2_32);
                    }
                    if (statDll64 == 0) {
                        Native.ZwClose(ws2_64);
                    }
                    return "Could not open section: " + secDir64.ObjectName + " (error " + Enum.GetName(typeof(Native.NtStatus), statSec64);
                }
                if (statSec32 != 0) {
                    File.AppendAllText(log, "Could not open: " + secDir32.ObjectName + " (error "
                        + String.Format("0x{0:X}", statSec32) + ")\r\n");
                    dir32 = dir64;
                    secDir32 = secDir64;
                    dir64 = IntPtr.Zero;
                } else {
                    if (statDll64 != 0) {
                        File.AppendAllText(log, "Could not open: " + dll64.ObjectName + " (error "
                            + String.Format("0x{0:X}", statDll64) + ")\r\n");
                        File.AppendAllText(log, "FAIL!\r\n");
                        if (statDll32 == 0) {
                            Native.ZwClose(ws2_32);
                        }
                        if (statDll64 == 0) {
                            Native.ZwClose(ws2_64);
                        }
                        return "Could not open dll: " + dll64.ObjectName + " (error " + Enum.GetName(typeof(Native.NtStatus), statDll64);
                    }
                }
                if (dir64 != IntPtr.Zero) {
                    File.AppendAllText(log, "Going to install: " + dll64.ObjectName + " on "
                        + secDir64.ObjectName + "\r\n");
                    Native.OBJECT_ATTRIBUTES secFile64 = new Native.OBJECT_ATTRIBUTES(secDir64.ObjectName + "\\WS2_32.dll", 0);
                    IntPtr file64;
                    long zero64 = 0;
                    Native.NtStatus statFile64 = Native.ZwCreateSection(out file64, Native.SECTION_ALL_ACCESS, ref secFile64, ref zero64,
                        Native.PAGE_EXECUTE_READWRITE, Native.SEC_IMAGE, ws2_64);
                    if (statFile64 != 0) {
                        File.AppendAllText(log, "Could not install: " + secFile64.ObjectName + " (error "
                            + String.Format("0x{0:X}", statFile64) + ")\r\n");
                    } else {
                        if (pernament) {
                            Native.NtStatus status = Native.ZwMakePermanentObject(file64);
                            if (status != 0)
                            {
                                File.AppendAllText(log, "Could not make permanent: " + secFile64.ObjectName + " (error "
                                    + String.Format("0x{0:X}", status) + ")\r\n");
                            }
                        }
                    }
                }
                File.AppendAllText(log, "Going to install: " + dll32.ObjectName + " on "
                    + secDir32.ObjectName + "\r\n");
                Native.OBJECT_ATTRIBUTES secFile32 = new Native.OBJECT_ATTRIBUTES(secDir32.ObjectName + "\\WS2_32.dll", 0);
                IntPtr file32;
                long zero32 = 0;
                Native.NtStatus statFile32 = Native.ZwCreateSection(out file32, Native.SECTION_ALL_ACCESS, ref secFile32, ref zero32,
                    Native.PAGE_EXECUTE_READWRITE, Native.SEC_IMAGE, ws2_32);
                if (statFile32 != 0) {
                    File.AppendAllText(log, "Could not install: " + secFile32.ObjectName + " (error "
                        + String.Format("0x{0:X}", statFile32) + ")\r\n");
                } else {
                    if (pernament) {
                        Native.NtStatus status = Native.ZwMakePermanentObject(file32);
                        if (status != 0)
                        {
                            File.AppendAllText(log, "Could not make permanent: " + secFile32.ObjectName + " (error "
                                + String.Format("0x{0:X}", status) + ")\r\n");
                        }
                    }
                }
            }
            if (statDll32 == 0) {
                Native.ZwClose(ws2_32);
            }
            if (statDll64 == 0) {
                Native.ZwClose(ws2_64);
            }
            return null;
        }
    }
}
