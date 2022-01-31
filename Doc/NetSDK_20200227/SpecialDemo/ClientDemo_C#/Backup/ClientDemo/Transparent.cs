using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using User.NetSDK;
using SDK_HANDLE = System.Int32;

namespace ClientDemo
{
    public partial class Form_Transpanrent : Form
    {
        public void MyCallBack(SDK_HANDLE lLoginID, int lTransComType, StringBuilder pBuffer, uint dwBufSize, uint dwUser)
        {
            MessageBox.Show("");
        }
        public void null_Fuc(Int32 lLoginID, string pchDVRIP,int nDVRPort, IntPtr dwUser)
        {

        }

        public Form_Transpanrent()
        {
            InitializeComponent();
            for (int i = 1; i <= 5; i++)
            {
                this.comboBox_rate.Items.Add(i * 9600);
            }
            this.comboBox_rate.SelectedIndex = 0;
            this.comboBox_check.Items.Add(@"None");
            this.comboBox_check.Items.Add(@"Odd");
            this.comboBox_check.Items.Add(@"Even");
            this.comboBox_check.Items.Add(@"Mark");
            this.comboBox_check.Items.Add(@"Space");
            this.comboBox_check.SelectedIndex = 0;

            for (int i = 1; i <= 2; i++)
            {
                this.comboBox_stop.Items.Add(i);
            }
            this.comboBox_stop.SelectedIndex = 0;

            for (int i = 5; i <= 8; i++)
            {
                this.comboBox_data.Items.Add(i);
            }
            this.comboBox_data.SelectedIndex = 3;

        }
        SDK_HANDLE lLoginID = 0;
        private void button_login_Click(object sender, EventArgs e)
        {
            H264_DVR_DEVICEINFO OutDev = new H264_DVR_DEVICEINFO();
            short nError = 0;
            StringBuilder strIP = new StringBuilder(this.textBox_ip.Text.ToString().Trim());
            ushort usPort = Convert.ToUInt16(this.textBox_port.Text.ToString().Trim());
            StringBuilder strUserName = new StringBuilder(this.textBox_username.Text.ToString());
            StringBuilder strPassword = new StringBuilder(this.textBox_password.Text.ToString());

            lLoginID = NetSDK.H264_DVR_Login(strIP, usPort, strUserName, strPassword, out OutDev, out nError, SocketStyle.TCPSOCKET);
            if (lLoginID <= 0)
            {
                MessageBox.Show(@"login wrong");
            }
            else
            {
                MessageBox.Show(@"login ok");
            }
        }

        private void button_open_Click(object sender, EventArgs e)
        {
            
            TransComChannel param = new TransComChannel();
            string tempStr;
            tempStr = this.comboBox_rate.Text.ToString();
            param.baudrate = Convert.ToUInt16(tempStr);
            tempStr = this.comboBox_data.Text.ToString();
            param.databits = Convert.ToUInt16(tempStr);
            tempStr = this.comboBox_stop.Text.ToString();
            param.stopbits = Convert.ToUInt16(tempStr);

            param.parity = Convert.ToUInt16(this.comboBox_check.SelectedIndex);
            param.TransComType =SERIAL_TYPE.RS485;
            NetSDK.fTransComCallBack fcallback = new NetSDK.fTransComCallBack(MyCallBack);
            if (NetSDK.H264_DVR_OpenTransComChannel(lLoginID, ref param, fcallback, this.Handle))
            {
                MessageBox.Show(@"open ok");
            }
            else
            {
                MessageBox.Show(@"wrong");
            }
            
        }

        private void button_close_Click(object sender, EventArgs e)
        {
            if (NetSDK.H264_DVR_CloseTransComChannel(lLoginID, SERIAL_TYPE.RS485))
            {
                MessageBox.Show(@"close ok");
            }
            else
            {
                MessageBox.Show(@"close wrong");
            }
        }

        private void button_write_Click(object sender, EventArgs e)
        {
            string strWrite = this.textBox_write.Text.ToString();
            if (NetSDK.H264_DVR_SerialWrite(lLoginID, SERIAL_TYPE.RS485, strWrite, strWrite.Length))
            {
                MessageBox.Show(@"write ok");
            }
            else
            {
                MessageBox.Show(@"write wrong");
            }
          
        }
    }
}