using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Threading;
using System.Diagnostics;
using User.NetSDK;
using SDK_HANDLE = System.Int32;

namespace ClientDemo
{
    public struct MyNetAlarInfo
    {
        public long lLoginID;
        public SDK_ALARM_INFO pAlarm;
        public uint dwBufLen;
    };

    public partial class NetAlarmForm : Form
    {
        DEV_INFO m_pDevInfo;
        private static NetAlarmForm s_instance = null;
        NetSDK.fMessCallBack msgcallback = null;
        private SynchronizationContext m_UIThreadSyncContext;
        public static NetAlarmForm Instance
        {
            get
            {
                if (null == s_instance)
                {
                    s_instance = new NetAlarmForm();
                }
                return s_instance;
            }
        }

        public static void Exit()
        {
            s_instance = null;
        }

        private NetAlarmForm()
        {
            InitializeComponent();
            m_UIThreadSyncContext = SynchronizationContext.Current;
            msgcallback = new NetSDK.fMessCallBack(AlarmMessCallBack);
            NetSDK.H264_DVR_SetDVRMessCallBack(msgcallback, this.Handle);
        }

        bool AlarmMessCallBack(SDK_HANDLE lLoginID, IntPtr pAlarm, uint dwBufLen, IntPtr dwUser, Int32 nType)
        {
            if (dwBufLen < 0 || pAlarm == null)
            {
                return false;
            }
            SDK_ALARM_INFO alarmInfo = (SDK_ALARM_INFO)Marshal.PtrToStructure(pAlarm, typeof(SDK_ALARM_INFO));
            MyNetAlarInfo myInfo = new MyNetAlarInfo();
            myInfo.lLoginID = lLoginID;
            myInfo.pAlarm = new SDK_ALARM_INFO();
            myInfo.pAlarm.iEvent = alarmInfo.iEvent;
            myInfo.pAlarm.iStatus = alarmInfo.iStatus;
            myInfo.pAlarm.nChannel = alarmInfo.nChannel;
            myInfo.pAlarm.SysTime = new SDK_SYSTEM_TIME();
            myInfo.pAlarm.SysTime.year = alarmInfo.SysTime.year;
            myInfo.pAlarm.SysTime.month = alarmInfo.SysTime.month;
            myInfo.pAlarm.SysTime.day = alarmInfo.SysTime.day;
            myInfo.pAlarm.SysTime.hour = alarmInfo.SysTime.hour;
            myInfo.pAlarm.SysTime.minute = alarmInfo.SysTime.minute;
            myInfo.pAlarm.SysTime.second = alarmInfo.SysTime.second;
            myInfo.dwBufLen = dwBufLen;
            m_UIThreadSyncContext.Post(new SendOrPostCallback(DealwithAlarm), myInfo);
            return true;
        }

        public void DealwithAlarm(object obj)
        {
            MyNetAlarInfo info = (MyNetAlarInfo)obj;
            ListViewItem item = new ListViewItem();
            SDK_ALARM_INFO alarmInfo = info.pAlarm;
            item.Text = "" + (m_alarmList.Items.Count + 1);
            item.SubItems.Add(this.GetDevNameById(info.lLoginID));
            item.SubItems.Add("" + alarmInfo.nChannel);
            item.SubItems.Add(this.GetAlarmNameByType(alarmInfo.iEvent));
            item.SubItems.Add("" + alarmInfo.iStatus);
            string sTime = String.Format("{0:D4}-{1:D2}-{2:D2} {3:D2}:{4:D2}:{5:D2}",
                alarmInfo.SysTime.year,
                alarmInfo.SysTime.month,
                alarmInfo.SysTime.day,
                alarmInfo.SysTime.hour,
                alarmInfo.SysTime.minute,
                alarmInfo.SysTime.second);
            item.SubItems.Add(sTime);
            m_alarmList.Items.Insert(0, item);
        }

        private void StartNetAlarm_Click(object sender, EventArgs e)
        {
            if (m_pDevInfo.lLoginID != 0)
            {
                NetSDK.H264_DVR_SetupAlarmChan(m_pDevInfo.lLoginID);
                m_pDevInfo.bStartAlarm = true;
                m_btnStart.Enabled = false;
                m_btnStop.Enabled = true;
            }
        }

        private void StopNetAlarm_Click(object sender, EventArgs e)
        {
            if (m_pDevInfo.lLoginID != 0)
            {
                NetSDK.H264_DVR_CloseAlarmChan(m_pDevInfo.lLoginID);
                m_pDevInfo.bStartAlarm = false;
                m_btnStart.Enabled = true;
                m_btnStop.Enabled = false;
            }
        }

        private void NetAlarmForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (s_instance != null)
            {
                e.Cancel = false;
                this.Hide();
            }
        }

        private void NetAlarmForm_VisibleChanged(object sender, EventArgs e)
        {
            if (!this.Visible)
            {
                return;
            }

            comboBoxDevice.ResetText();
            ClientDemo clientForm = (ClientDemo)this.Owner;
            TreeNodeCollection nodeCollection = clientForm.devForm.DevTree.Nodes;
            foreach (TreeNode node in nodeCollection)
            {
                if (node.Name == "Device")
                {
                    comboBoxDevice.Items.Add(node);
                }
            }
            if (comboBoxDevice.Items.Count > 0)
            {
                comboBoxDevice.SelectedIndex = 0;
            }
        }

        private void comboBoxDevice_SelectedIndexChanged(object sender, EventArgs e)
        {
            TreeNode node = (TreeNode)comboBoxDevice.SelectedItem;
            if (node.Tag != null)
            {
                m_pDevInfo = (DEV_INFO)node.Tag;
                m_btnStart.Enabled = m_pDevInfo.bStartAlarm == false;
                m_btnStop.Enabled = m_pDevInfo.bStartAlarm;
            }
        }

        public string GetDevNameById(long nLoginId)
        {
            TreeNodeCollection nodeCollection = ClientDemo.Instance.devForm.DevTree.Nodes;
            foreach (TreeNode node in nodeCollection)
            {
                if (node.Name == "Device")
                {
                    DEV_INFO devInfo = (DEV_INFO)node.Tag;
                    if (devInfo.lLoginID == nLoginId)
                    {
                        return devInfo.szDevName;
                    }
                }
            }
            return "";
        }

        public string GetAlarmNameByType(int nAlarm)
        {
            switch ((SDK_EventCodeTypes)nAlarm)
            {
                case SDK_EventCodeTypes.SDK_EVENT_CODE_VIDEO_MOTION:
                    return "VIDEO_MOTION";
                case SDK_EventCodeTypes.SDK_EVENT_CODE_VIDEO_LOSS:
                    return "VIDEO_LOSS";
                case SDK_EventCodeTypes.SDK_EVENT_CODE_VIDEO_BLIND:
                    return "VIDEO_BLIND";
                case SDK_EventCodeTypes.SDK_EVENT_CODE_STORAGE_FAILURE:
                    return "STORAGE_FAILURE";
                case SDK_EventCodeTypes.SDK_EVENT_CODE_STORAGE_NOT_EXIST:
                    return "NOT_EXIST";
                case SDK_EventCodeTypes.SDK_EVENT_CODE_STORAGE_READ_ERROR:
                    return "STORAGE_READ_ERROR";
                case SDK_EventCodeTypes.SDK_EVENT_CODE_STORAGE_WRITE_ERROR:
                    return "STORAGE_WRITE_ERROR";
                case SDK_EventCodeTypes.SDK_EVENT_CODE_NET_IPCONFLICT:
                    return "NET_IPCONFLICT";
                case SDK_EventCodeTypes.SDK_EVENT_CODE_LOCAL_ALARM:
                    return "LOCAL_ALARM";
                case SDK_EventCodeTypes.SDK_EVENT_CODE_VideoAnalyze:
                    return "VideoAnalyze";
                    default:
                    return "Event:" + nAlarm;
            }
        }
    }
}
