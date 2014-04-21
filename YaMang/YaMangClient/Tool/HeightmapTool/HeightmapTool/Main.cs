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

        ~Main()
        {
            isRunning = false;
            
            YamangDll.HeightMapCleanup();
            YamangDll.D3DCleanUp();
        }

        public Main()
        {
            InitializeComponent();
            YamangDll.InitD3D(this.RenderTarget.Handle);
            Render();
        }
        
        async private void Render()
        {
            int i = 0;
            while (isRunning)
            {
                YamangDll.PreRendering();
                
                YamangDll.RenderText("hi", 50, 50);
                YamangDll.HeightMapRender();

                YamangDll.PostRendering();

                button1.Text = "" + i;
                await Task.Delay(10);
                ++i;
            }
        }


        private void button1_Click(object sender, EventArgs e)
        {
            if (!isRunning)
            {
                isRunning = true;
                
                string heightMap = ".\\Resource\\heightmap_1024_1024_korea.bmp";
                string mapTexture = ".\\Resource\\heightmap_texture_1024_1024_korea.bmp";

                YamangDll.HeightMapTextureImport(this.RenderTarget.Handle, heightMap, mapTexture);


                Render();
            }
        }

    }
}
