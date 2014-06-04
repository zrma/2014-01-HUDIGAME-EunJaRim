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

        private String serverRootPath = "http://scope.hosting.bizfree.kr/next/YaMang/loaderTemplate.php";

        public Form1()
        {
            InitializeComponent();
        }

        private static Boolean IsDoubleOpen = false;

        private void Form1_Load(object sender, EventArgs e)
        {
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.WindowState = FormWindowState.Maximized;
            //string curDir = Directory.GetCurrentDirectory();

            this.webBrowser1.WebBrowserShortcutsEnabled = false;
            this.webBrowser1.Url = new Uri(serverRootPath);
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
                //Application.Exit();
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

            timer1.Start();
           // Application.Exit();
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

            this.webBrowser1.Navigate(_LoginUrl.AbsoluteUri);
        }

        private String m_Name;
        private String m_ID;
        private String m_Token;
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


            m_Name = name;
            m_ID = id;
            m_Token = accessToken;

           // MessageBox.Show("name:" + name + " id:" + id);
            Console.WriteLine("name:" + name + " id:" + id);
            //string curDir = Directory.GetCurrentDirectory();
            //this.webBrowser1.Url = new Uri(serverRootPath);

            // 총 3개의 POST 데이터 만들기
            string strPostData = string.Format("name={0}&id={1}&token={2}&status={3}", name, id, accessToken, "facebookLogin");
            byte[] postData = Encoding.Default.GetBytes(strPostData);
            this.webBrowser1.Navigate(serverRootPath, null, postData, "Content-Type: application/x-www-form-urlencoded"); 
        }
        #endregion FaceBook


        #region FaceBookUpload

        bool gameStarted = false;
        private static bool doubleOpenCheck = false;

        const int WM_COPYDATA = 74;
        const int WM_CLOSE = 0x10;
        const int WM_NCACTIVATE = 0x0086;


        const int WM_USER1 = 1024;

        protected override void WndProc(ref Message m)
        {
            try
            {
                switch (m.Msg)
                {

                        // test! 이게 되면 삽질 안할텐데...
                    case WM_COPYDATA:
                        //COPYDATASTRUCT cds = (COPYDATASTRUCT)m.GetLParam(typeof(COPYDATASTRUCT));
                        MessageBox.Show("DATA GET1!");
                        break;
                    case WM_USER1:
                        MessageBox.Show("DATA GET2!");
                        break;
                    
                       
                    default:

                        if (IsDoubleOpen && gameStarted && (m.Msg == WM_NCACTIVATE) )
                        {
                            if (doubleOpenCheck == false)
                            {
                                
                                gameStarted = false;
                                timer1.Stop();
                                doubleOpenCheck = true;
                                sendGameResult();
                            }
                        }

                        base.WndProc(ref m);
                        break;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

       
        private void timer1_Tick(object sender, EventArgs e)
        {
            gameStarted = true;
        }



        private void sendGameResult()
        {
            FileStream fs = new FileStream("temp.txt", FileMode.OpenOrCreate);
            StreamReader sr = new StreamReader(fs);

            String line = sr.ReadLine();

            sr.Close();
            fs.Close();

            if(line == "")
            {
                timer1.Start();
                doubleOpenCheck = false;
                return;
            }

            // "1001,1003,win"
            String[] datas = line.Split(new char[] { ',', ' ' }, StringSplitOptions.RemoveEmptyEntries);

            String myGameID = datas[0];
            String otherGameID = datas[1];
            String win = datas[2];

            string strPostData = string.Format("myGameID={0}&otherGameID={1}&id={2}&token={3}&status={4}&win={5}", myGameID, otherGameID, m_ID, m_Token, "gameResult", win);
            byte[] postData = Encoding.Default.GetBytes(strPostData);
            this.webBrowser1.Navigate(serverRootPath, null, postData, "Content-Type: application/x-www-form-urlencoded");

        }

        #endregion FaceBookUpload
    }
}
