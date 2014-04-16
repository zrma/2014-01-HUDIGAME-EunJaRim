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
        bool isRunning = false;

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

                string heightMap = "C:\\Users\\trizdreaming\\Documents\\GitHub\\2014-01-HUDIGAME-EunJaRim\\YaMang\\YaMangClient\\Tool\\HeightmapTool\\HeightmapTool\\heightmap_128_128.bmp";
                string mapTexture = "C:\\Users\\trizdreaming\\Documents\\GitHub\\2014-01-HUDIGAME-EunJaRim\\YaMang\\YaMangClient\\Tool\\HeightmapTool\\HeightmapTool\\heightmap_texture.tga";

                YamangDll.HeightMapTextureImport(this.MainWindow.Handle, heightMap, mapTexture);

                Render();
            }
        }

        private void MakeMatrix(float[,] matrix)
        {

        }
    }
}
