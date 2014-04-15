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

namespace HeightmapTool
{
    public partial class Main : Form
    {
        IntPtr infoPtr;
        bool isRunning = false;

        public Main()
        {
            InitializeComponent();
            YamangDll.InitD3D(this.Window.Handle);
        }

        ~Main()
        {
            isRunning = false;
            YamangDll.D3DCleanUp();
        }
    }
}
