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
        bool objectAttach = false;

        ~Main()
        {
            objectAttach = false;
            
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
            while (true)
            {
                YamangDll.PreRendering();
                
                if (objectAttach == true)
                {
                    YamangDll.HeightMapRender();
                }

                YamangDll.PostRendering();

                await Task.Delay(10);
            }
        }


        private void button1_Click(object sender, EventArgs e)
        {
            if (!objectAttach)
            {
                //string heightMap = ".\\Resource\\heightmap_1024_1024_korea.bmp";
                //string mapTexture = ".\\Resource\\heightmap_texture_1024_1024_korea.bmp";

                //YamangDll.HeightMapTextureImport(this.RenderTarget.Handle, heightMap, mapTexture);
                YamangDll.CreateRawGround(100, 100, 1.0f);
                YamangDll.PreSettingForTool();

                objectAttach = true;
            }
        }

    }
}
