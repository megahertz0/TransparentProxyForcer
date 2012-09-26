using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Net;
using Microsoft.Win32;
using System.Configuration.Install;
using System.Reflection;
using System.ServiceProcess;
using System.IO;
using System.Security.Principal;
using System.Security.AccessControl;

namespace TransparentProxyForcer
{
    public partial class ForcedProxy : Form
    {
        public ForcedProxy()
        {
            InitializeComponent();            
            RegistryKey key;
            key = Registry.LocalMachine.CreateSubKey("Software\\Klatt\\Forced Proxy");
            SecurityIdentifier everybodyId = new SecurityIdentifier(WellKnownSidType.WorldSid, null);
            NTAccount everybody = everybodyId.Translate(typeof(NTAccount)) as NTAccount;
            RegistryAccessRule rule = new RegistryAccessRule(
                everybody.ToString(),
                RegistryRights.FullControl,
                InheritanceFlags.ContainerInherit | InheritanceFlags.ObjectInherit,
                PropagationFlags.None,
                AccessControlType.Allow);
            RegistrySecurity security = new RegistrySecurity();
            security.AddAccessRule(rule);
            key.SetAccessControl(security);
            ip.Text = (string)key.GetValue(null, "127.0.0.1");
            key.Close();
            programs.Items.Clear();
            readAvaiablePrograms();
            setInstallButtonCaption();
        }

        private void readAvaiablePrograms() {
            RegistryKey key;
            key = Registry.LocalMachine.CreateSubKey("Software\\Klatt\\Forced Proxy");
            foreach (string name in key.GetValueNames()) {
                if (name != "" && ! programs.Items.Contains(name))
                    programs.Items.Add(name, (int)key.GetValue(name) != 0);
            }
            key.Close();
        }

        private void setInstallButtonCaption()
        {
            if (isDllLocked()) {
                install.Text = "Install";
            } else  {
                install.Text = "Uninstall";
            }
        }

        private bool isDllLocked() {
            String path = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            Native.OBJECT_ATTRIBUTES fileattributes = new Native.OBJECT_ATTRIBUTES("\\DosDevices\\" + path + "\\ws2_32.dll",
                Native.OBJ_CASE_INSENSITIVE);
            IntPtr ws2_32;
            Native.IO_STATUS_BLOCK iosb = new Native.IO_STATUS_BLOCK();
            Native.NtStatus stat = Native.ZwOpenFile(out ws2_32, Native.FILE_GENERIC_READ | Native.FILE_GENERIC_WRITE | Native.FILE_GENERIC_EXECUTE,
                ref fileattributes, ref iosb, Native.FILE_SHARE_READ | Native.FILE_SHARE_WRITE, 0);
            if (stat == 0) {
                Native.ZwClose(ws2_32);
            }
            return stat == 0;
        }

        private void ip_TextChanged(object sender, EventArgs e)
        {
            IPAddress address;
            RegistryKey key;
            key = Registry.LocalMachine.CreateSubKey("Software\\Klatt\\Forced Proxy");
            save.Enabled = IPAddress.TryParse(ip.Text, out address)
                && address.AddressFamily == System.Net.Sockets.AddressFamily.InterNetwork
                && (string) key.GetValue(null, "127.0.0.1") != ip.Text;
        }

        private void save_Click(object sender, EventArgs e)
        {
            IPAddress address;
            IPAddress.TryParse(ip.Text, out address);
            ip.Text = address.ToString();
            RegistryKey key;
            key = Registry.LocalMachine.CreateSubKey("Software\\Klatt\\Forced Proxy");
            key.SetValue(null, ip.Text);
            key.Close();
        }

        private void programs_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            if (programs.SelectedIndex == -1) {
                return;
            }
            String name = programs.SelectedItem.ToString();
            RegistryKey key;
            key = Registry.LocalMachine.CreateSubKey("Software\\Klatt\\Forced Proxy");
            key.SetValue(name, e.NewValue, RegistryValueKind.DWord);
            key.Close();
        }

        private void install_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;
            if (persistent.Checked) {
                try { ManagedInstallerClass.InstallHelper(new string[] { Assembly.GetExecutingAssembly().Location }); }
                catch (InvalidOperationException) { }
                ServiceController service = new ServiceController("LibraryInjecter");
                try { service.Start(); }
                catch (InvalidOperationException) { }
                try { service.WaitForStatus(ServiceControllerStatus.Running, TimeSpan.FromSeconds(10)); }
                catch (System.ServiceProcess.TimeoutException) { }
                service.Stop();
                service.WaitForStatus(ServiceControllerStatus.Stopped, TimeSpan.FromSeconds(10));
                ManagedInstallerClass.InstallHelper(new string[] { "/u", Assembly.GetExecutingAssembly().Location });
            } else {
                String msg = LibraryInjecter.installHook();
                if (msg != null) {
                    MessageBox.Show(msg);
                }
            }
            Cursor.Current = Cursors.Default;            
            setInstallButtonCaption();
        }

        private void timer_Tick(object sender, EventArgs e)
        {
            readAvaiablePrograms();
        }
    }    
}
