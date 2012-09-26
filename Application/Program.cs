using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.ServiceProcess;

namespace TransparentProxyForcer
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            if (System.Environment.UserInteractive) {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new ForcedProxy());
            } else {
                ServiceBase[] ServicesToRun;
                ServicesToRun = new ServiceBase[]
                {
                    new LibraryInjecter()
                };
                ServiceBase.Run(ServicesToRun);
            }
        }
    }
}
