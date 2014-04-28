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
        bool renderStop = false;
        bool meshSizeChanged = true;
        bool mouseEventFlag = false;
        int actionFlag = 0;

        int preWidth = 0;
        int preHeight = 0;
        int preMapSpace = 0;

        int mouseXPosition = 0;
        int mouseYPosition = 0;

        ~Main()
        {
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

        private void SetAction()
        {
            if(Higher.Checked)
            {
                actionFlag = 1;
            }
            else if(Lower.Checked)
            {
                actionFlag = 2;
            }
            else 
            {
                actionFlag = 0;
            }
        }

        async private void Render()
        {
            while (!renderStop)
            {
                YamangDll.PreRendering();
                int curWidth = Convert.ToInt32(GetNumber(MapHeightVal));
                int curHeight = Convert.ToInt32(GetNumber(MapWidthVal));
                int curMapSpace = Convert.ToInt32(GetNumber(MapVertexSpacingVal));
                
                if((preWidth != curWidth) || (preHeight != curHeight) || (preMapSpace != curMapSpace))
                {
                    meshSizeChanged = true;
                }

                if(meshSizeChanged)
                {
                    meshSizeChanged = false;

                    YamangDll.InitGroundMesh( curWidth, curHeight );
                    YamangDll.CreateRawGround(curWidth, curHeight, curMapSpace);
                }

                
                YamangDll.PreSettingForTool();

                if(mouseEventFlag)
                {
                    SetAction();

                    YamangDll.CalcPickingRay(mouseXPosition, mouseYPosition);
                    YamangDll.TransPickedTriangle(actionFlag);
                    mouseEventFlag = false;
                }
                

                YamangDll.HeightMapRender();

                YamangDll.PostRendering();

                preWidth = curWidth;
                preHeight = curHeight;
                preMapSpace = curMapSpace;

                await Task.Delay(10);
            }
        }

        private void Main_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing)
            {
                renderStop = true;
                YamangDll.HeightMapCleanup();
                YamangDll.D3DCleanUp();
                MessageBox.Show(this, "맵툴을 종료합니다", "Closing...", MessageBoxButtons.OK, MessageBoxIcon.Question);
            }
        }

        private void RenderFrameClick(object sender, MouseEventArgs e)
        {
            mouseEventFlag = true;

            mouseXPosition = (e.X + ((Control)sender).Location.X) - 295;
            mouseYPosition = (e.Y + ((Control)sender).Location.Y) - 51;

            Console.WriteLine(mouseXPosition);
            Console.WriteLine(mouseYPosition);
        }
    }
}
