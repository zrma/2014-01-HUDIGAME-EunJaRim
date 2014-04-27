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

        ~Main()
        {

            YamangDll.HeightMapCleanup();
            YamangDll.D3DCleanUp();
        }

        public Main()
        {
            InitializeComponent();
            YamangDll.InitD3D(this.RenderTarget.Handle);
            Render();
        }
        
        

        private float GetNumber(object target)
        {
            float result;

            try
            {
                result = Convert.ToSingle(((TextBox)target).Text);
                if(result <= 0 )
                {
                    ((TextBox)target).Text = "1";
                    result = 1;
                }
            }
            catch
            {
                ((TextBox)target).Text = "1";
                result = 1;
            }
            return result;
        }



        async private void Render()
        {
            while (true)
            {
                YamangDll.PreRendering();
                YamangDll.CreateRawGround(Convert.ToInt32(GetNumber(MapHeightVal)), Convert.ToInt32(GetNumber(MapWidthVal)), GetNumber(MapVertexSpacingVal));
                YamangDll.PreSettingForTool();

                YamangDll.CalcPickingRay( 100, 100 );

                YamangDll.HeightMapRender();

                YamangDll.PostRendering();

                await Task.Delay(10);
            }
        }

    }
}
