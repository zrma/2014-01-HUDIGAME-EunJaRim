using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;

namespace YaMangLoader
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private static Boolean IsDoubleOpen = false;

        private void Form1_Load(object sender, EventArgs e)
        {
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.WindowState = FormWindowState.Maximized;
            string curDir = Directory.GetCurrentDirectory();

            this.webBrowser1.WebBrowserShortcutsEnabled = false;
            this.webBrowser1.Url = new Uri(String.Format("file:///{0}/loaderTemplate.html", curDir));
            //this.webBrowser1.DocumentCompleted += webBrowser1_DocumentCompleted;
            this.webBrowser1.PreviewKeyDown += webBrowser1_PreviewKeyDown;
        }

        private void webBrowser1_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
            if (e.KeyCode == Keys.Escape)
            {
                Application.Exit();
                return;
            }
        }

        
        private void webBrowser1_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
        {
            if (e.Url.AbsolutePath != (sender as WebBrowser).Url.AbsolutePath)
            {
                return;
            }

            //String checkURL = e.Url.OriginalString;
            String checkURL = (sender as WebBrowser).Url.OriginalString;


            if (!checkURL.Contains("?r="))
            {
                return;
            }


            if (IsDoubleOpen)
            {
                Application.Exit();
                return;
            }

            IsDoubleOpen = true;

            int index = checkURL.IndexOf("?r=");
            String room = checkURL.Substring(index);
            room = room.Replace("?r=", "");
            room = room.Replace("#", "");


            String curDir = Directory.GetCurrentDirectory();

            String ClientPath = curDir + "\\" + "YamangClient.exe";

            ProcessStartInfo startInfo = new ProcessStartInfo();
            startInfo.FileName = ClientPath;
            startInfo.Arguments = room;
            Process.Start(startInfo);


            Application.Exit();
        }

       
    }
}
