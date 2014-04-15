using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace CharacterTool
{
    public partial class Main : Form
    {
        IntPtr infoPtr;
        bool isRunning = false;

        public Main()
        {
            InitializeComponent();
            YamangDll.InitD3D(this.MainWindow.Handle, 816, 660);
        }

        ~Main()
        {
            isRunning = false;
            YamangDll.D3DCleanUp();
        }
    }
}
