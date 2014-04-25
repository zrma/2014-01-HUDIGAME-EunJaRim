﻿using System;
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
        //test field
        YamangDll.MESHOBJECT info;
        IntPtr infoPtr;
        bool isRunning = false;

        ~Main()
        {
            isRunning = false;
            YamangDll.D3DCleanUp();
        }

        public Main()
        {
            InitializeComponent();
            YamangDll.InitD3D(this.Window.Handle);
            Render();
        }

        async private void Render()
        {
            int i = 0;
            while (isRunning)
            {
                YamangDll.RenderingTool(ref infoPtr);

                button1.Text = "" + i;
                await Task.Delay(10);
                ++i;
            }
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
            isRunning = false;
            YamangDll.MeshObjectCleanUp(ref infoPtr);
            YamangDll.D3DCleanUp();
            Application.Exit();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (!isRunning)
            {
                isRunning = true;
                info = new YamangDll.MESHOBJECT();
                infoPtr = Marshal.AllocHGlobal(Marshal.SizeOf(info));

                Marshal.StructureToPtr(info, infoPtr, false);
                info = (YamangDll.MESHOBJECT)Marshal.PtrToStructure(infoPtr, typeof(YamangDll.MESHOBJECT));

                string filename = "UnitArrow.x";
                YamangDll.InitGeometry(this.Window.Handle, filename, ref infoPtr);
                YamangDll.MoveCamera(0.0f, 1.0f, -10.0f);
                Render();
            }
        }

        private float GetNumber(object target)
        {
            float result;

            try
            {
                result = Convert.ToSingle(((TextBox)target).Text);
            }
            catch
            {
                ((TextBox)target).Text = "0";
                result = 0;
            }
            return result;
        }

        private void MoveCamera_Click(object sender, EventArgs e)
        {
            YamangDll.MoveCamera(Convert.ToInt32(GetNumber(CameraXVal)), Convert.ToInt32(GetNumber(CameraYVal)), Convert.ToInt32(GetNumber(CameraZVal)));
        }

        private void SetCamera_Click(object sender, EventArgs e)
        {
            
        }
    }
}
