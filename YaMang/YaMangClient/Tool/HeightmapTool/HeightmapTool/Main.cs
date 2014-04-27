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

                YamangDll.CalcPickingRay(Convert.ToInt32(GetNumber(textBox1)), Convert.ToInt32(GetNumber(textBox2)));
                YamangDll.GetPickedTriangle();

                YamangDll.HeightMapRender();

                YamangDll.PostRendering();

                await Task.Delay(10);
            }
        }

        private void Main_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing)
            {
                YamangDll.HeightMapCleanup();
                YamangDll.D3DCleanUp();
                if (MessageBox.Show(this, "Really?", "Closing...", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.Cancel)
                {
                    e.Cancel = true;
                }
            }
        }
    }
}
