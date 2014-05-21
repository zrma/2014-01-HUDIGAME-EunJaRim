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

namespace YaMangLoader
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.WindowState = FormWindowState.Maximized;
            string curDir = Directory.GetCurrentDirectory();

            //sthis.webBrowser1.WebBrowserShortcutsEnabled = false;
            this.webBrowser1.Url = new Uri(String.Format("file:///{0}/loaderTemplate.html", curDir));
            this.webBrowser1.DocumentCompleted += webBrowser1_DocumentCompleted;
        }

        private void webBrowser1_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
        {
            if (e.Url.AbsolutePath != (sender as WebBrowser).Url.AbsolutePath)
            {
                MessageBox.Show("로드완료 !!");
                return;
            }
                
        }
       
    }
}
