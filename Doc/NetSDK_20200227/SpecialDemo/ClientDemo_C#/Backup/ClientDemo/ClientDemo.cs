using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Threading;
using User.NetSDK;
using SDK_HANDLE = System.Int32;

namespace ClientDemo
{
    public struct DEV_INFO
    {
        public int nListNum;		   //position in the list
        public SDK_HANDLE lLoginID;			//login handle
        public int lID;				//device ID
        public string szDevName;		//device name
        public string szIpaddress;		//device IP
        public string szUserName;		//user name
        public string szPsw;			//pass word
        public int nPort;				//port number
        public int nTotalChannel;		//total channel
        public SDK_CONFIG_NET_COMMON_V2 NetComm;                  // net config
        public H264_DVR_DEVICEINFO NetDeviceInfo;
        public byte bSerialID;//be SerialNumber login
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
        public string szSerIP;//server ip
        public int nSerPort;           //server port
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string szSerialInfo;  //SerialNumber
        public bool bStartAlarm;
    }

    public struct CHANNEL_INFO
    {
        string szChnnelName;			            // 通道名称.
        public int nChnnID;							// 用于地图节点管理
        public int nChannelNo;					    // 通道号.
        public int bUserRight;					    // 用户权限(使能).
        public int PreViewChannel;				    // 预览通道
        public int nStreamType;						// 码流类型
        public DEV_INFO DeviceInfo;					// 设备信息.
        public int nCombinType;						// 组合编码模式
        public int dwTreeItem;						//记录设备树中的节点句柄，可以节省查找事件
        public int nFlag;							//1为选择为录像 0 为没有被选择 2 为正在录像
        public int nWndIndex;
    };

    public partial class ClientDemo : Form
    {
        public PTZForm m_formPTZ;
        public DevConfigForm m_formCfg;
        public NetAlarmForm m_netAlaramForm;
        public int m_nCurIndex = -1;
        public int m_nTotalWnd = 4;
        public DEV_INFO m_devInfo = new DEV_INFO();
        public VideoForm[] m_videoform = new VideoForm[32];
        public bool m_bArray;
        public static Dictionary<long, DEV_INFO> dictDevInfo = new Dictionary<long, DEV_INFO>();
        public static Dictionary<long, DEV_INFO> dictDiscontDev = new Dictionary<long, DEV_INFO>();
        private System.Timers.Timer timerDisconnect = new System.Timers.Timer(30000);
        private NetSDK.fDisConnect disCallback;
        private System.Timers.ElapsedEventHandler reconnect;

        void DisConnectBackCallFunc(SDK_HANDLE lLoginID, string pchDVRIP, int nDVRPort, IntPtr dwUser)
        {
            for (int i = 0; i < 16; i++)
            {
                if (lLoginID == m_videoform[i].GetLoginHandle())
                {
                    m_videoform[i].OnDisconnct();
                }
            }
           
          
            foreach (DEV_INFO devinfo in dictDevInfo.Values)
            {
                if (devinfo.lLoginID == lLoginID)
                {
                    NetSDK.H264_DVR_Logout(lLoginID);
                    dictDevInfo.Remove(devinfo.lLoginID);
			        dictDiscontDev.Add(devinfo.lLoginID,devinfo);
                    break;
                }
            }

            if ( dictDiscontDev.Count > 0 )
            {

                timerDisconnect.Enabled = true;
                timerDisconnect.Start();
            }
        }

        private static ClientDemo s_instance = null;
        public static ClientDemo Instance
        {
            get
            {
                if (null == s_instance)
                {
                    s_instance = new ClientDemo();
                }
                return s_instance;
            }
        }

        private ClientDemo()
        {
            s_instance = this;
            for (int i = 0; i < 32; i++)
            {
                m_videoform[i] = new VideoForm();
                this.Controls.Add(this.m_videoform[i]);
                m_videoform[i].SetWndIndex(i);
            }
            InitializeComponent();
            
            devForm = new DevForm();
            this.Controls.Add(devForm);
            devForm.Location = new System.Drawing.Point(880, 10);
            devForm.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));  
            this.comboBox1.SelectedIndex = 1;
            InitSDK();
            m_netAlaramForm = NetAlarmForm.Instance;
            m_netAlaramForm.StartPosition = FormStartPosition.CenterScreen;
            m_netAlaramForm.Owner = this;
            reconnect = new System.Timers.ElapsedEventHandler(ReConnect);
            GC.KeepAlive(reconnect);
            timerDisconnect.Elapsed += new System.Timers.ElapsedEventHandler(reconnect); 
       
            ArrayWindow(4);
            SetActiveWnd(0);   
        }

        private void btnTransparent_Click(object sender, EventArgs e)
        {
            Form_Transpanrent formTransparent = new Form_Transpanrent();
            formTransparent.Show();
        }

        public int InitSDK()
        {
            //initialize
            NetSDK.g_config.nSDKType = NetSDK.SDK_TYPE.SDK_TYPE_GENERAL;

            disCallback = new NetSDK.fDisConnect(DisConnectBackCallFunc);
            GC.KeepAlive(disCallback);
            int bResult = NetSDK.H264_DVR_Init(disCallback, this.Handle);
            NetSDK.H264_DVR_SetConnectTime(5000, 3);
            return bResult;
        }
        public bool ExitSDK()
        {
            return NetSDK.H264_DVR_Cleanup();
        }

        public void ArrayWindow(int iNumber)
        {
            m_nTotalWnd = iNumber;

            Rectangle rect = this.ClientRectangle;
            int  iWidth, iHeight;
            int nFullWidth = rect.Width;
            int nFullHeight = rect.Height;
            iWidth = (int)(nFullWidth * 0.75515625);
            iHeight = (int)(nFullHeight * 0.91);

            int i = 0;
            for (i = 0; i < 32; i++)   //MAXWNDNUM ==32
            {
                m_videoform[i].Hide();
            }
            
            int nNull = 3;

            switch (iNumber)
            {
                case 1:
                    m_videoform[0].SetBounds(3+0,0,iWidth,iHeight);
                    m_videoform[0].Show();
                    break;
                case 4:
                    for (i = 0; i < 2; i++)
                    {
                        m_videoform[i].SetBounds(3 + i * (iWidth / 2) + i * nNull, 0, (iWidth / 2), iHeight / 2);
                        m_videoform[i].Show();
                    }
                    for (i = 2; i < 4; i++)
                    {
                        m_videoform[i].SetBounds(3 + (i - 2) * (iWidth / 2) + (i - 2) * nNull, iHeight / 2 + nNull, (iWidth / 2), iHeight / 2);
                        m_videoform[i].Show();
                    }
                    break;
                case 9:
                    for (i = 0; i < 3; i++)
                    {
                        m_videoform[i].SetBounds(3 + i * (iWidth / 3) + i * nNull, 0, (iWidth / 3), iHeight / 3);
                        m_videoform[i].Show();
                    }
                    for (i = 3; i < 6; i++)
                    {
                        m_videoform[i].SetBounds(3 + (i - 3) * (iWidth / 3) + (i - 3) * nNull, iHeight / 3 + nNull, (iWidth / 3), iHeight / 3);
                        m_videoform[i].Show();
                    }
                    for (i = 6; i < 9; i++)
                    {
                        m_videoform[i].SetBounds(3 + (i - 6) * (iWidth / 3) + (i - 6) * nNull, 2 * iHeight / 3 + 2 * nNull, (iWidth / 3), iHeight / 3);
                        m_videoform[i].Show();
                    }
                    break;
                case 16:
                    for (i = 0; i < 4; i++)
                    {
                        m_videoform[i].SetBounds(3 + i * (iWidth / 4) + (i) * nNull, 0, (iWidth / 4), iHeight / 4);
                        m_videoform[i].Show();
                    }
                    for (i = 4; i < 8; i++)
                    {
                        m_videoform[i].SetBounds(3 + (i - 4) * (iWidth / 4) + (i - 4) * nNull, iHeight / 4 + nNull, (iWidth / 4), iHeight / 4);
                        m_videoform[i].Show();
                    }
                    for (i = 8; i < 12; i++)
                    {
                        m_videoform[i].SetBounds(3 + (i - 8) * (iWidth / 4) + (i - 8) * nNull, iHeight / 2 + 2 * nNull, (iWidth / 4), iHeight / 4);
                        m_videoform[i].Show();
                    }
                    for (i = 12; i < 16; i++)
                    {
                        m_videoform[i].SetBounds(3 + (i - 12) * (iWidth / 4) + (i - 12) * nNull, 3 * iHeight / 4 + 3 * nNull, (iWidth / 4), iHeight / 4);
                        m_videoform[i].Show();
                    }
                    break;
                default:
                    break;
            }
        }

        public void DrawActivePage(bool bActive)
        {
            Rectangle rt = new Rectangle(m_videoform[m_nCurIndex].Left,m_videoform[m_nCurIndex].Top, m_videoform[m_nCurIndex].Width, m_videoform[m_nCurIndex].Height);
            if (!bActive)
            {
                Rectangle rtInvalidate = new Rectangle(rt.X - 1, rt.Y - 1, rt.Width + 2, rt.Height + 2);
                Invalidate(rtInvalidate, true);
            }
            else
            {
                Graphics graphic = Graphics.FromHwnd(this.Handle);
                Pen pen = new Pen(Color.Red, 2);
                graphic.DrawRectangle(pen, rt);
            }
        }

        private void SetColor(int nIndex)
        {
            int nBright = 0;
            int nHue = 0;
            int nSaturation = 0;
            int nContrast = 0;

            IntPtr lPlayHandle = m_videoform[m_nCurIndex].Handle;
            if (lPlayHandle.ToInt32() <=0 )
            {
                return;
            }
            m_videoform[nIndex].GetColor(out nBright, out nContrast, out nSaturation, out nHue);
        }

        public bool SetDevChnColor(uint nBright, uint nContrast, uint nSaturation, uint nHue)
        {
            SDK_CONFIG_VIDEOCOLOR videocolor = new SDK_CONFIG_VIDEOCOLOR();
            for (int i = 0; i < 2; i++)
            {
                videocolor.dstVideoColor[i].tsTimeSection.enable = 1;
                videocolor.dstVideoColor[i].tsTimeSection.startHour = 0;
                videocolor.dstVideoColor[i].tsTimeSection.startMinute = 0;
                videocolor.dstVideoColor[i].tsTimeSection.startSecond = 0;
                videocolor.dstVideoColor[i].tsTimeSection.endHour = 24;
                videocolor.dstVideoColor[i].tsTimeSection.endMinute = 0;
                videocolor.dstVideoColor[i].tsTimeSection.endSecond = 0;
                videocolor.dstVideoColor[i].iEnable = 1;
                videocolor.dstVideoColor[i].dstColor.nBrightness = (int)nBright * 100 / 128;
                videocolor.dstVideoColor[i].dstColor.nHue = (int)nHue * 100 / 128;
                videocolor.dstVideoColor[i].dstColor.nSaturation = (int)nSaturation * 100 / 128;
                videocolor.dstVideoColor[i].dstColor.nContrast = (int)nContrast * 100 / 128;
                videocolor.dstVideoColor[i].dstColor.mGain = 0;
                videocolor.dstVideoColor[i].dstColor.mWhitebalance = 0;
            }
            m_videoform[m_nCurIndex].SetDevChnColor(ref videocolor);
            return true;
        }

        public void SetActiveWnd(int nIndex)
        {
            if (-1 != m_nCurIndex && m_nCurIndex != nIndex)
            {
                DrawActivePage(false);
            }
            if (m_nCurIndex!=nIndex)
            {
                   m_nCurIndex = nIndex;
            }
            

            DrawActivePage(true);

            SetColor(m_nCurIndex);

        }
        public long Connect(ref DEV_INFO pDev, int nChannel, int nWndIndex)
        {
            int nRet = 0;

            //if device did not login,login first
            if (pDev.lLoginID <= 0)
            {
                H264_DVR_DEVICEINFO OutDev = new H264_DVR_DEVICEINFO();
                OutDev.Init();
                int nError = 0;
                //设置尝试连接设备次数和等待时间
                int lLogin = NetSDK.H264_DVR_Login(pDev.szIpaddress, (ushort)pDev.nPort, pDev.szUserName, pDev.szPsw, ref OutDev, out nError, SocketStyle.TCPSOCKET);
              //  int lLogin = NetSDK.H264_DVR_Login(pDev.szIpaddress, (ushort)pDev.nPort, pDev.szUserName, pDev.szPsw, ref OutDev, out nError, SocketStyle.NATOBJECT);
                if (lLogin <= 0)
                {

                    int nErr = NetSDK.H264_DVR_GetLastError();
                    if (nErr == (int)SDK_RET_CODE.H264_DVR_PASSWORD_NOT_VALID)
                    {
                        MessageBox.Show(("Error.PwdErr"));
                    }
                    else
                    {
                        MessageBox.Show(("Error.NotFound"));
                    }

                    return nRet;
                }

                pDev.lLoginID = lLogin;
                NetSDK.H264_DVR_SetupAlarmChan(lLogin);
            }

            int nWnd = m_nCurIndex;
            if (nWndIndex >= 0)
            {
                nWnd = nWndIndex;
            }

            if (nWnd >= m_nTotalWnd)
            {
                return nRet;
            }

            return m_videoform[nWnd].ConnectRealPlay(ref pDev, nChannel);	
        }
        public void SetColor(uint nBright, uint nContrast, uint nSaturation, uint nHue)
        {
            IntPtr lPlayHandle = m_videoform[m_nCurIndex].Handle;
            unsafe
            {
                if (lPlayHandle.ToPointer() == null)
                {
                    return;
                }
            }
         
            m_videoform[m_nCurIndex].SetColor((int)nBright, (int)nContrast, (int)nSaturation, (int)nHue);
            SetDevChnColor(nBright, nContrast, nSaturation, nHue);
        }
        public void PtzControl(uint dwBtn, bool dwStop)
        {
            long lPlayHandle = m_videoform[m_nCurIndex].GetHandle();
            if (lPlayHandle <= 0)
            {
                return;
            }
        }
        public void KeyBoardMsg(uint dwValue, uint dwState)
        {
            IntPtr lPlayHandle = m_videoform[m_nCurIndex].Handle;
            unsafe
            {
                if (lPlayHandle.ToPointer() == null)
                {
                    return;
                }
            }
          
            SDK_NetKeyBoardData vKeyBoardData;
            vKeyBoardData.iValue = (int)dwValue;
            vKeyBoardData.iState = (int)dwState;
            m_nCurIndex = m_nCurIndex < 0 ? 0 : m_nCurIndex;
            if (!NetSDK.H264_DVR_ClickKey(m_videoform[m_nCurIndex].m_lLogin, ref vKeyBoardData))
               MessageBox.Show("AccountMSG.Failed");
        }

        public void NetAlarmMsg(uint dwValue, uint dwState)
        {
            if (m_devInfo.lLoginID > 0)
            {
                SDK_NetAlarmInfo vAlarmInfo;
                vAlarmInfo.iEvent = 0;
                vAlarmInfo.iState = (int)(dwState << (int)dwValue);
                m_nCurIndex = m_nCurIndex < 0 ? 0 : m_nCurIndex;
                if (!NetSDK.H264_DVR_SendNetAlarmMsg(m_devInfo.lLoginID, ref vAlarmInfo))
                    MessageBox.Show("AccountMSG.Failed");
            }
        }

        public void SetDevInfo(ref DEV_INFO pDev)
        {
            m_devInfo = pDev;
        }

        public void ReConnect(object source, System.Timers.ElapsedEventArgs e)
        {
        	foreach( DEV_INFO devinfo in dictDiscontDev.Values )
            {

                H264_DVR_DEVICEINFO OutDev = new H264_DVR_DEVICEINFO();
                OutDev.Init();
                int nError = 0;

                int lLogin = NetSDK.H264_DVR_Login(devinfo.szIpaddress, (ushort)devinfo.nPort, devinfo.szUserName, devinfo.szPsw, ref OutDev, out nError, SocketStyle.TCPSOCKET);
  //              int lLogin = NetSDK.H264_DVR_Login(devinfo.szIpaddress, (ushort)devinfo.nPort, devinfo.szUserName, devinfo.szPsw, ref OutDev, out nError, SocketStyle.NATOBJECT);
                if (lLogin <= 0)
                {

                    int nErr = NetSDK.H264_DVR_GetLastError();
                    if (nErr == (int)SDK_RET_CODE.H264_DVR_PASSWORD_NOT_VALID)
                    {
                        MessageBox.Show(("Password Error"));
                    }
                    else if (nErr == (int)SDK_RET_CODE.H264_DVR_LOGIN_USER_NOEXIST)
                    {
                        MessageBox.Show(("User Not Exist"));
                    }

                    return;
                }
                dictDiscontDev.Remove(devinfo.lLoginID);

                ClientDemo clientForm = ClientDemo.Instance;
                DEV_INFO devAdd = new DEV_INFO();
                devAdd = devinfo;
                devAdd.lLoginID = lLogin;
           

                foreach (TreeNode node in clientForm.devForm.DevTree.Nodes)
                {
                    if (node.Name == "Device")
                    {
                        DEV_INFO dev = (DEV_INFO)node.Tag;
                        if (dev.lLoginID == devinfo.lLoginID)
                        {
                            node.Text = devAdd.szDevName;
                            node.Tag = devAdd;
                            node.Name = "Device";

                            foreach (TreeNode channelnode in node.Nodes)
                            {
                                CHANNEL_INFO chInfo = (CHANNEL_INFO)channelnode.Tag;
                                if ( chInfo.nWndIndex > -1  )
                                {
                                    clientForm.m_videoform[chInfo.nWndIndex].ConnectRealPlay(ref devAdd, chInfo.nChannelNo);
                                    Thread.Sleep(10);
                                }
                             }
                            break;
                        }

                    }

                }

                dictDevInfo.Add(lLogin, devAdd);
                NetSDK.H264_DVR_SetupAlarmChan(lLogin);
            }
            if (0 == dictDiscontDev.Count)
            {
                timerDisconnect.Enabled = false;
                timerDisconnect.Stop();
            }
        }

        private void ClientDemo_Paint(object sender, PaintEventArgs e)
        {
            SetActiveWnd(m_nCurIndex);
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int nWndNum = 4;
            if (comboBox1.SelectedIndex == 0)
            {
                nWndNum = 1;
            }
            else if (comboBox1.SelectedIndex == 1)
            {
                nWndNum = 4;
            }
            else if (comboBox1.SelectedIndex == 2)
            {
                nWndNum = 9;
            }
            else if (comboBox1.SelectedIndex == 3)
            {
                nWndNum = 16;
            }
            ArrayWindow(nWndNum);
        }

        private void ClientDemo_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                 
    
            }
        }

        private void btnplayback_Click(object sender, EventArgs e)
        {
            PlayBackForm formPlayBack = new PlayBackForm();
            formPlayBack.Show();
        }

        private void ClientDemo_FormClosed(object sender, FormClosedEventArgs e)
        {
            foreach (DEV_INFO devinfo in dictDevInfo.Values)
            {
                NetSDK.H264_DVR_Logout(devinfo.lLoginID);   
            }
            timerDisconnect.Stop();
            ExitSDK();
        }

        private void btnPTZ_Click(object sender, EventArgs e)
        {
            m_formPTZ = new PTZForm();
            m_formPTZ.StartPosition = FormStartPosition.CenterScreen;
            m_formPTZ.Owner = this;
            m_formPTZ.Show(); 
        }

        private void btnDevConfig_Click(object sender, EventArgs e)
        {
            m_formCfg = new DevConfigForm();
            m_formCfg.StartPosition = FormStartPosition.CenterScreen;
            m_formCfg.Owner = this;
            m_formCfg.Show();
        }

        private void ClientDemo_Load(object sender, EventArgs e)
        {
        }

        private void btnNetAlarm_Click(object sender, EventArgs e)
        {
            if (m_netAlaramForm != null)
            {
                m_netAlaramForm.Show();
            }
        }

        private void btncolor_Click(object sender, EventArgs e)
        {
        }

        protected override void WndProc(ref Message m)
        {
            //Console.WriteLine(m.Msg);
            const int WM_SYSCOMMAND = 0x0112;
            const int SC_CLOSE = 0xF060;
            //捕捉关闭窗体消息   
            if (m.Msg == WM_SYSCOMMAND && (int)m.WParam == SC_CLOSE)
            {
                NetAlarmForm.Exit();
                m_netAlaramForm = null;
            }
            base.WndProc(ref m);
        }
    }
}