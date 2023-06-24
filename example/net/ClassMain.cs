using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LogicAPIUI
{
    public class ClassMain
    {
        [DllImport("UnityUtils.dll", CharSet = CharSet.Ansi, SetLastError = true)]
        public static extern int ExecuteScript(string lpText);
        private static bool Inited;
        public static int Main(string parameter)
        {
            if (Inited)
                return 0;

            new System.Threading.Thread(Visual).Start();
            Inited = true;
            return 0;
        }
        [STAThread]
        public static void Visual()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}
