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

namespace YamangTools
{
    public partial class Main : Form
    {
        bool isRunning = false;
        IntPtr infoPtr;
        YamangDll.ARGMATRIX info;

        public Main()
        {
            InitializeComponent();
            YamangDll.InitD3D(this.MainWindow.Handle, 816, 660);
            Render();
            
        }

        ~Main()
        {
            isRunning = false;
            YamangDll.HeightMapCleanup();
            YamangDll.D3DCleanUp();
        }

        async private void Render()
        {
            //int i = 0;
            while (isRunning)
            {
                YamangDll.PreRendering();
                YamangDll.HeightMapRender();

              


                info = new YamangDll.ARGMATRIX();
                info._11 = 1;
                info._12 = 0;
                info._13 = 0;
                info._14 = 0;
                info._21 = 0;
                info._22 = 1;
                info._23 = 0;
                info._24 = 0;
                info._31 = 0;
                info._32 = 0;
                info._33 = 1;
                info._34 = 0;
                info._41 = 0;
                info._42 = 0;
                info._43 = 0;
                info._44 = 1;

                infoPtr = Marshal.AllocHGlobal(Marshal.SizeOf(info));

                Marshal.StructureToPtr(info, infoPtr, false);
                info = (YamangDll.ARGMATRIX)Marshal.PtrToStructure(infoPtr, typeof(YamangDll.ARGMATRIX));

                bool cameraSet = false;
                YamangDll.SetMatrix(ref infoPtr, cameraSet);

                //DLL 정상 연동 확인 코드
                YamangDll.RenderText("HI", 200, 200);
                
                YamangDll.PostRendering();
                
                //button1.Text = "" + i;
                await Task.Delay(10);
                //++i;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (!isRunning)
            {
                isRunning = true;

                string heightMap = ".\\Resource\\heightmap_1024_1024_korea.bmp";
                string mapTexture = ".\\Resource\\heightmap_texture_1024_1024_korea.bmp";

                YamangDll.HeightMapTextureImport(this.MainWindow.Handle, heightMap, mapTexture);

                Render();
            }
        }
    }
}
