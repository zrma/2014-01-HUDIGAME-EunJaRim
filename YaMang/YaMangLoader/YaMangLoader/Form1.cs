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
using Facebook;
using System.Dynamic;

namespace YaMangLoader
{
    public partial class Form1 : Form
    {

        private const String AppId = "836820156346904";
        private Uri _LoginUrl;
        private const String _ExtendedPermissions = "user_about_me,publish_stream,offline_access,read_friendlists,publish_actions,user_photos";
        FacebookClient fbClient = new FacebookClient();

        private String accessToken;



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
            if (IsDoubleOpen)
            {
                Application.Exit();
                return;
            }

            if (e.Url.AbsolutePath != (sender as WebBrowser).Url.AbsolutePath)
            {
                //return;
            }

            //String checkURL = e.Url.OriginalString;
            String checkURL = (sender as WebBrowser).Url.OriginalString;

            if (checkURL.Contains("status=loginProcess"))
            {
                faceBookLogin();
                return;
            }

            if (checkURL.Contains("access_token="))
            {
                faceBookParsing(checkURL);
                return;
            }
            
            
            if (checkURL.Contains("?r="))
            {
                gameParse(checkURL);
                return;
            }
            
        }

        private void gameParse( String parseData )
        {
            IsDoubleOpen = true;

            int index = parseData.IndexOf("?r=");
            String room = parseData.Substring(index);
            room = room.Replace("?r=", "");
            room = room.Replace("&i=", " ");
            room = room.Replace("#", "");


            String curDir = Directory.GetCurrentDirectory();

            String ClientPath = curDir + "\\" + "YamangClient.exe";

            ProcessStartInfo startInfo = new ProcessStartInfo();
            startInfo.FileName = ClientPath;
            startInfo.Arguments = room;
            Process.Start(startInfo);


            Application.Exit();
        }

        #region FaceBook
        private void faceBookLogin()
        {
            dynamic parameters = new ExpandoObject();
            parameters.client_id = AppId;
            parameters.redirect_uri = "https://www.facebook.com/connect/login_success.html";

            parameters.response_type = "token";
            parameters.display = "popup";

            if (!String.IsNullOrWhiteSpace(_ExtendedPermissions))
            {
                parameters.scope = _ExtendedPermissions;
            }

            var fb = new FacebookClient();
            _LoginUrl = fb.GetLoginUrl(parameters);

            webBrowser1.Navigate(_LoginUrl.AbsoluteUri);
        }

        private void faceBookParsing( String parseData )
        {
            //https://www.facebook.com/connect/login_success.html#access_token=CAAL5FWAo8hgBAOU7VTTuS1g25xvG1Mf47UVVpZC6qRMbYtZBCZAo2Hooax4nhHh2kuBdE8C527f6lQKJphp0KpSgSt5uABlHhG9T1cTIjinSJWZBHIbOVKi5eeCXtbDIZAlZASow9Hw5Bml1lBKB0TMjTbDPn4wLwcTqwiu39tUYyKzR9c62fVmxF2FVXA0egZD&expires_in=6471"

            parseData = parseData.Replace("https://www.facebook.com/connect/login_success.html#access_token=", "");
            int endIndex = parseData.IndexOf('&');

            accessToken = parseData.Substring(0, endIndex);
            Console.WriteLine("accessToken:" + accessToken);

            var fb = new FacebookClient(accessToken);

            dynamic result = fb.Get("/me");
            var name = result.name;
            var id = result.id;

            MessageBox.Show("name:" + name + " id:" + id);
            Console.WriteLine("name:" + name + " id:" + id);
            string curDir = Directory.GetCurrentDirectory();
            this.webBrowser1.Url = new Uri(String.Format("file:///{0}/loaderTemplate.html", curDir));
        }
        #endregion FaceBook

    }
}
