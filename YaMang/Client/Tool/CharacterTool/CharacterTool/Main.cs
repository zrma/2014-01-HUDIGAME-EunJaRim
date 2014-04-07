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
        public Main()
        {

            InitializeComponent();
            YamangDll.InitD3D(this.Window.Handle);
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog1 = new OpenFileDialog();

            openFileDialog1.Filter = "D3D Mesh File (.x)|*.x|All Files (*.*)|*.*";
            openFileDialog1.FilterIndex = 1; 
            openFileDialog1.RestoreDirectory = true; 
            openFileDialog1.InitialDirectory = System.IO.Directory.GetCurrentDirectory();

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                //Open X File

            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            YamangDll.MESHOBJECT info = new YamangDll.MESHOBJECT();
            IntPtr infoPtr = Marshal.AllocHGlobal(Marshal.SizeOf(info));
            Marshal.StructureToPtr(info, infoPtr, false);
            info = (YamangDll.MESHOBJECT)Marshal.PtrToStructure(infoPtr, typeof(YamangDll.MESHOBJECT));
            
            string filename = "tiger.x";
            YamangDll.InitGeometry(this.Window.Handle, filename, ref infoPtr );
            YamangDll.PreRendering(0, 0, 0);
            YamangDll.Rendering(ref infoPtr);
            YamangDll.PostRendering();
        }
    }
}
