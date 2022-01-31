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
    public partial class DevConfigForm : Form
    {
        int m_nLoginID = 0;
        uint m_nConfig = 0;
        int m_nChannel = 0;
        public DevConfigForm()
        {
            InitializeComponent();
        }

        private void DevConfigForm_Shown(object sender, EventArgs e)
        {
            comboBoxDevice.ResetText();
            ClientDemo clientForm = (ClientDemo)this.Owner;
            TreeNodeCollection nodeCollection = clientForm.devForm.DevTree.Nodes;
            foreach (TreeNode node in nodeCollection)
            {
                if ( node.Name == "Device" )
                {
                    comboBoxDevice.Items.Add(node);
                }
            }
            if ( comboBoxDevice.Items.Count > 0 )
            {
                comboBoxDevice.SelectedIndex = 0;
            }
        }

        private void comboBoxDevice_SelectedIndexChanged(object sender, EventArgs e)
        {
            TreeNode node = (TreeNode)comboBoxDevice.SelectedItem;
            if ( node.Tag !=null )
            {
                DEV_INFO devInfo = (DEV_INFO)node.Tag;
                SDK_HANDLE nLoginID = devInfo.lLoginID;
                m_nLoginID = nLoginID;

                ///获取系统能力集

                int nChannel = -1;
                TabPage tab = tabControlConfigs.SelectedTab;
                uint nConfig = 0;
                if (tab.Name == "tabPageGeneral")
                {
                    nConfig = (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_SYSNORMAL;
                }
                if (nConfig != 0)
                {
                    UpdateConfig(nLoginID, nChannel, nConfig);
                }
            }   
        }

        SDK_NetDHCPConfigAll curDHCPcfgAll = new SDK_NetDHCPConfigAll();
        USER_MANAGE_INFO m_userCfg = new USER_MANAGE_INFO();
        private void UpdateConfig(SDK_HANDLE nLoginID, int nChannel, uint nConfig)
        {
            if ( nLoginID > 0 )
            {
                IntPtr bufPtr;
                uint nBufSize = 0;
                uint nOutSize = 0;
                switch(nConfig)
                {
                    case (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_SYSNORMAL:
                        {
                            
                            nBufSize = (uint)Marshal.SizeOf(typeof(SDK_CONFIG_NORMAL));
                            bufPtr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(SDK_CONFIG_NORMAL)));
                            int nRet = NetSDK.H264_DVR_GetDevConfig(nLoginID, nConfig, nChannel, bufPtr, nBufSize, out nOutSize, 1000);
                            if ( nRet < 0 )
                            {
                                MessageBox.Show("Get config error!");
                                return;
                            }

                            SDK_CONFIG_NORMAL cfgNormal = new SDK_CONFIG_NORMAL();
                            cfgNormal = (SDK_CONFIG_NORMAL)Marshal.PtrToStructure(bufPtr, typeof(SDK_CONFIG_NORMAL));

                            nBufSize = (uint)Marshal.SizeOf(typeof(H264_DVR_DEVICEINFO));
                            bufPtr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(H264_DVR_DEVICEINFO)));
                            nRet = NetSDK.H264_DVR_GetDevConfig(nLoginID, (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_SYSINFO, nChannel, bufPtr, nBufSize, out nOutSize, 1000);
                            H264_DVR_DEVICEINFO cfgSysInfo = new H264_DVR_DEVICEINFO();
                            cfgSysInfo = (H264_DVR_DEVICEINFO)Marshal.PtrToStructure(bufPtr, typeof(H264_DVR_DEVICEINFO));

                            textBoxSerial.Text = cfgSysInfo.sSerialNumber.ToString();
                            textBoxDeviceNo.Text = cfgNormal.iLocalNo.ToString();
                            textBoxDeviceName.Text = cfgNormal.sMachineName.ToString();
                            textBoxVideoOut.Text = cfgNormal.iVideoStartOutPut.ToString();
                            textBoxAlarmIn.Text = cfgSysInfo.byAlarmInPortNum.ToString();
                            textBoxAlarmOut.Text = cfgSysInfo.byAlarmOutPortNum.ToString();
                            comboBoxDiskFullTo.SelectedIndex = cfgNormal.iOverWrite;
                            comboBoxVideoStandard.SelectedIndex = cfgNormal.iVideoFormat;
                            comboBoxDateFormat.SelectedIndex = cfgNormal.iDateFormat;
                            comboBoxDateSeparator.SelectedIndex = cfgNormal.iDateSeparator;
                            textBoxStandbyTime.Text = cfgNormal.iWorkDay.ToString();
                            comboBoxTimeFormat.SelectedIndex = cfgNormal.iTimeFormat;

                            textBoxRecordChannel.Text = (cfgSysInfo.byChanNum + cfgSysInfo.iDigChannel).ToString();
                            textBoxAudioIn.Text = cfgSysInfo.iAudioInChannel.ToString();
                            textBoxTalkIn.Text = cfgSysInfo.iTalkInChannel.ToString();
                            textBoxTalkOut.Text = cfgSysInfo.iTalkOutChannel.ToString();
                            textBoxVideoOut.Text = cfgSysInfo.iTalkOutChannel.ToString();
                            textBoxExChannel.Text = cfgSysInfo.iExtraChannel.ToString();
                            textBoxDigitalChannel.Text = cfgSysInfo.iDigChannel.ToString();
                            textBox1SystemVersion.Text = cfgSysInfo.sSoftWareVersion;

                            StringBuilder strTime;
                            strTime = new StringBuilder();
                            strTime.AppendFormat("{0:d}-{1:d}-{2:d} {3:d}:{4:d}:{5:d}", cfgSysInfo.tmBuildTime.year,
                                cfgSysInfo.tmBuildTime.month, cfgSysInfo.tmBuildTime.day, cfgSysInfo.tmBuildTime.hour,
                                cfgSysInfo.tmBuildTime.minute, cfgSysInfo.tmBuildTime.second);

                            textBoxReleaseDate.Text = strTime.ToString();
                            break;
                        }
                    case (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_NET_WIFI:
                        {
                            nBufSize = (uint)Marshal.SizeOf(typeof(SDK_NetWifiConfig));
                            bufPtr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(SDK_NetWifiConfig)));
                            int nRet = NetSDK.H264_DVR_GetDevConfig(nLoginID, nConfig, nChannel, bufPtr, nBufSize, out nOutSize, 1000);
                            if (nRet < 0)
                            {
                                MessageBox.Show("Get config error!");
                                return;
                            }

                            curcfgWifi = (SDK_NetWifiConfig)Marshal.PtrToStructure(bufPtr, typeof(SDK_NetWifiConfig));

                            checkEnableWifi.Checked = Convert.ToBoolean(curcfgWifi.bEnable);

                            bool bWifiEnable = checkEnableWifi.Checked;
                            comboBoxWifiEntryType.Enabled = bWifiEnable;
                            if (!checkBoxDHCP.Checked)
                            {
                                textBoxWifiIP.Enabled = bWifiEnable;
                                textBoxWifiSubmask.Enabled = bWifiEnable;
                                textBoxWifiGateway.Enabled = bWifiEnable;
                            } 
                            

                            StringBuilder strIP, strSubMask, strGateway;
                            strIP = new StringBuilder();
                            strIP.AppendFormat("{0:d}.{1:d}.{2:d}.{3:d}", (int)curcfgWifi.HostIP.c[0], (int)curcfgWifi.HostIP.c[1], (int)curcfgWifi.HostIP.c[2], (int)curcfgWifi.HostIP.c[3]);
                            strSubMask = new StringBuilder();
                            strSubMask.AppendFormat("{0:d}.{1:d}.{2:d}.{3:d}", (int)curcfgWifi.Submask.c[0], (int)curcfgWifi.Submask.c[1], (int)curcfgWifi.Submask.c[2], (int)curcfgWifi.Submask.c[3]);
                            strGateway = new StringBuilder();
                            strGateway.AppendFormat("{0:d}.{1:d}.{2:d}.{3:d}", (int)curcfgWifi.Gateway.c[0], (int)curcfgWifi.Gateway.c[1], (int)curcfgWifi.Gateway.c[2], (int)curcfgWifi.Gateway.c[3]);

                            textBoxWifiIP.Text = strIP.ToString();
                            textBoxWifiSubmask.Text = strSubMask.ToString();
                            textBoxWifiGateway.Text = strGateway.ToString();
                            textBoxWifiPassword.Text = curcfgWifi.sKeys;
                            textBoxSSID.Text = curcfgWifi.sSSID;
                            comboBoxWifiEntryType.Items.Clear();
                            comboBoxWifiEntryType.Items.Add(curcfgWifi.sEncrypType);
                            comboBoxWifiEntryType.SelectedIndex = 0;
                            comboBoxWifiKeyType.SelectedIndex = 1;

                            if (curcfgWifi.sEncrypType == "WEP")
                            {
                                comboBoxWifiKeyType.SelectedIndex = curcfgWifi.nKeyType;
                                comboBoxWifiKeyType.Show();
                                labelKeyType.Show();
                            }
                            else
                            {
                                comboBoxWifiKeyType.Hide();
                                labelKeyType.Hide();
                            }

                            checkBoxDHCP.Checked = false;
                            textBoxWifiIP.Enabled = true;
                            textBoxWifiSubmask.Enabled = true;
                            textBoxWifiGateway.Enabled = true;
                            break;
                        }
                    case (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_NET_DHCP:
                        {
                            nBufSize = (uint)Marshal.SizeOf(typeof(SDK_NetDHCPConfigAll));
                            bufPtr = Marshal.AllocHGlobal((int)nBufSize);
                            int nRet = NetSDK.H264_DVR_GetDevConfig(nLoginID, (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_NET_DHCP, -1, bufPtr, nBufSize, out nOutSize, 1000);
                            if (nRet > 0)
                            {
                                SDK_NetDHCPConfigAll dhcpAll = new SDK_NetDHCPConfigAll();
                                dhcpAll = (SDK_NetDHCPConfigAll)Marshal.PtrToStructure(bufPtr, typeof(SDK_NetDHCPConfigAll));
                                curDHCPcfgAll = dhcpAll;

                                if (Convert.ToBoolean(dhcpAll.vNetDHCPConfig[2].bEnable))
                                {
                                    checkBoxDHCP.Checked = true;
                                    textBoxWifiIP.Enabled = false;
                                    textBoxWifiSubmask.Enabled = false;
                                    textBoxWifiGateway.Enabled = false;
                                }

                            }
                            break;
                        }
                    case (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_NET_WIFI_AP_LIST:
                        {
                           
                            nBufSize = (uint)Marshal.SizeOf(typeof(SDK_NetWifiDeviceAll));
                            bufPtr = Marshal.AllocHGlobal((int)nBufSize);
                            int nRet = NetSDK.H264_DVR_GetDevConfig(nLoginID, (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_NET_WIFI_AP_LIST, -1, bufPtr, nBufSize, out nOutSize, 5000);


                            if (nRet >= 0)
                            {
                                listViewWifi.Items.Clear();

                                SDK_NetWifiDeviceAll wifiDeviAll = new SDK_NetWifiDeviceAll();
                                wifiDeviAll = (SDK_NetWifiDeviceAll)Marshal.PtrToStructure(bufPtr, typeof(SDK_NetWifiDeviceAll));
                                for (int i = 0; i < wifiDeviAll.nDevNumber; i++)
                                {

                                    string strRSSI;
                                    switch (wifiDeviAll.vNetWifiDeviceAll[i].nRSSI)
                                    {
                                        case (int)SDK_RSSI_SINGNAL.SDK_RSSI_NO_SIGNAL:
                                            strRSSI = "ConfigNet.NoSignal";
                                            break;
                                        case (int)SDK_RSSI_SINGNAL.SDK_RSSI_VERY_LOW:
                                            strRSSI = "VeryLow";
                                            break;
                                        case (int)SDK_RSSI_SINGNAL.SDK_RSSI_LOW:
                                            strRSSI = "Low";
                                            break;
                                        case (int)SDK_RSSI_SINGNAL.SDK_RSSI_GOOD:
                                            strRSSI = "Good";
                                            break;
                                        case (int)SDK_RSSI_SINGNAL.SDK_RSSI_VERY_GOOD:
                                            strRSSI = "VeryGood";
                                            break;
                                        case (int)SDK_RSSI_SINGNAL.SDK_RSSI_EXCELLENT:
                                            strRSSI = "Excellent";
                                            break;
                                        default:
                                            strRSSI = "NoSignal";
                                            break;
                                    }

                                    SDK_NetWifiDevice wifi = new SDK_NetWifiDevice();
                                    wifi = wifiDeviAll.vNetWifiDeviceAll[i];
                                    ListViewItem item = new ListViewItem(new string[] { wifi.sSSID, wifi.sAuth, strRSSI });

                                    item.Tag = wifi;
                                    listViewWifi.Items.Add(item);
                                }
                            }
                            break;
                        }
                    case (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_ABILITY_SYSFUNC:
                        {
                            nBufSize = (uint)Marshal.SizeOf(typeof(SDK_SystemFunction));
                            bufPtr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(SDK_SystemFunction)));
                            int nRet = NetSDK.H264_DVR_GetDevConfig(nLoginID, (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_ABILITY_SYSFUNC, -1, bufPtr, nBufSize, out nOutSize, 2000);

                            if (nRet > 0)
                            {
                                SDK_SystemFunction sysFunction = new SDK_SystemFunction();
                                sysFunction = (SDK_SystemFunction)Marshal.PtrToStructure(bufPtr, typeof(SDK_SystemFunction));
                                if (!Convert.ToBoolean(sysFunction.vNetServerFunction[(int)SDK_NetServerTypes.SDK_NET_SERVER_TYPES_WIFI]))
                                {
                                    tabPageWifi.Hide();
                                }
                                else
                                {
                                    tabPageWifi.Show();
                                }

                            }
                        }
                        break;
                    case (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_NET_NTP:
                        {
                            nBufSize = (uint)Marshal.SizeOf(typeof(SDK_NetNTPConfig));
                            bufPtr = Marshal.AllocHGlobal((int)nBufSize);
                            int nRet = NetSDK.H264_DVR_GetDevConfig(nLoginID, nConfig, -1, bufPtr, nBufSize, out nOutSize, 5000);
                            if (nRet > 0)
                            {
                                SDK_NetNTPConfig cfg = (SDK_NetNTPConfig)Marshal.PtrToStructure(bufPtr, typeof(SDK_NetNTPConfig));
                                m_ntpEnable.Checked = cfg.Enable == 1;
                                m_ntpAuto.Checked = cfg.TimeZone == 13;
                                m_ntpUserDef.Checked = cfg.TimeZone == 14;
                                m_ntpServerIP.Text = cfg.Server.ServerName;
                                m_ntpServerPort.Text = "" + cfg.Server.Port;
                                m_ntpUpdateTime.Text = "" + cfg.UpdatePeriod;
                            }
                        }
                        break;
                    case (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_USER:
                        {
                            this.btnUserRefresh_Click(null, null);
                        }
                        break;
                    case (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_SYS_TIME:
                        {
                            nBufSize = (uint)Marshal.SizeOf(typeof(SDK_SYSTEM_TIME));
                            bufPtr = Marshal.AllocHGlobal((int)nBufSize);
                            int nRet = NetSDK.H264_DVR_GetDevConfig(nLoginID, nConfig, -1, bufPtr, nBufSize, out nOutSize, 5000);
                            if (nRet > 0)
                            {
                                SDK_SYSTEM_TIME cfg = (SDK_SYSTEM_TIME)Marshal.PtrToStructure(bufPtr, typeof(SDK_SYSTEM_TIME));
                                m_tbDevTime.Text = "" + cfg.year + "-" + cfg.month + "-" + cfg.day + " " + cfg.hour + ":" + cfg.minute + ":" + cfg.second;
                            }

                            nBufSize = (uint)Marshal.SizeOf(typeof(SDK_TimeZone));
                            bufPtr = Marshal.AllocHGlobal((int)nBufSize);
                            uint nZoneCmdId = (uint)SDK_CONFIG_TYPE.E_SDK_CFG_TIME_ZONE;
                            nRet = NetSDK.H264_DVR_GetDevConfig(nLoginID, nZoneCmdId, -1, bufPtr, nBufSize, out nOutSize, 5000);
                            if (nRet > 0)
                            {
                                SDK_TimeZone cfg = (SDK_TimeZone)Marshal.PtrToStructure(bufPtr, typeof(SDK_TimeZone));
                                SetComZone(cfg.minuteswest);
                            }
                        }
                        break;
                    case (uint)SDK_CONFIG_TYPE.E_SDK_CFG_OEM_GETINFO:
                        {
                            nBufSize = (uint)Marshal.SizeOf(typeof(SDK_OemGetInfo));
                            bufPtr = Marshal.AllocHGlobal((int)nBufSize);
                            int nRet = NetSDK.H264_DVR_GetDevConfig(nLoginID, nConfig, -1, bufPtr, nBufSize, out nOutSize, 4000);
                            if (nRet > 0)
                            {
                                SDK_OemGetInfo cfg = (SDK_OemGetInfo)Marshal.PtrToStructure(bufPtr, typeof(SDK_OemGetInfo));
                                string sStates = 
                                    "Connect Count: " + cfg.iConnectNum + "\r\n" +
                                    "Disk Count: " + cfg.iDisk + "\r\n";
                                for (int i = 0; i < cfg.iDisk; i++)
                                {
                                    sStates += "\tDisk[" + (i + 1) + "] Size: " + cfg.cHardDisk[i].uiRemainSpace + "M/" + cfg.cHardDisk[i].uiTotalSpace + "M\r\n";
                                }
                                sStates += "Record states:\r\n";

                                H264_DVR_DEVICEINFO info = ClientDemo.dictDevInfo[nLoginID].NetDeviceInfo;
                                int nChanCout = info.byChanNum + info.iDigChannel;
                                for (int i = 0; i < nChanCout; i++)
                                {
                                    sStates += "\tChannel[" + (i + 1) + "] Record State: " + cfg.cRecordState[i] + "\r\n";
                                }
                                sStates += "Version: " + cfg.version + "\r\n";
                                m_tbStates.Text = sStates;
                            }
                            else
                            {
                                MessageBox.Show("Failed to get status(Device may not supported).");
                            }
                        }
                        break;
                }
          
            }
        }
        void SetComZone(int nZoneValue)
        {
            int nIndex = nZoneValue / 60;
            for (int i = 0; i < m_cbTimeZone.Items.Count; i++)
            {
                string sValue = m_cbTimeZone.GetItemText(m_cbTimeZone.Items[i]);
                if (Int32.Parse(sValue) == nIndex)
                {
                    m_cbTimeZone.SelectedIndex = i;
                    break;
                }
            }
        }
        int GetTimeZoneValue()
        {
            if (m_cbTimeZone.SelectedIndex < 0)
            {
                return 0;
            }
            string sValue = m_cbTimeZone.GetItemText(m_cbTimeZone.Items[m_cbTimeZone.SelectedIndex]);
            return Int32.Parse(sValue) * 60;
        }

        SDK_NetWifiConfig curcfgWifi = new SDK_NetWifiConfig();
        private void SaveConfig(int nLoginID, int nChannel, uint nConfig)
        {
            if( nLoginID <= 0 )
            {
                return;
            }
            uint nBufSize = 0;
            string sResult = "";
            int nSaveRet = 0;
            switch (nConfig)
            {
                case (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_SYSNORMAL:
                    {
                         nBufSize = (uint)Marshal.SizeOf(typeof(SDK_CONFIG_NORMAL));
                        SDK_CONFIG_NORMAL configNormal = new SDK_CONFIG_NORMAL();
                        configNormal.iDateSeparator = comboBoxDateSeparator.SelectedIndex;
                        configNormal.iDSTRule = 0;
                       // configNormal.iLanguage = 0;
                        configNormal.iLocalNo = System.Int32.Parse(textBoxDeviceNo.Text);
                        configNormal.iOverWrite = comboBoxDiskFullTo.SelectedIndex;
                        configNormal.iVideoFormat = comboBoxVideoStandard.SelectedIndex;
                        configNormal.iVideoStartOutPut = 0;
                        configNormal.sMachineName = textBoxDeviceName.Text;
                        configNormal.iWorkDay = System.Int32.Parse(textBoxStandbyTime.Text);
                        configNormal.iTimeFormat = comboBoxTimeFormat.SelectedIndex;
                        configNormal.iDateFormat = comboBoxDateFormat.SelectedIndex;
                       
                        IntPtr ptrBuf =  Marshal.AllocHGlobal(Marshal.SizeOf(typeof(SDK_CONFIG_NORMAL)));


                        Marshal.StructureToPtr(configNormal, ptrBuf, false);
                        nSaveRet = NetSDK.H264_DVR_SetDevConfig(nLoginID, nConfig, nChannel, ptrBuf, nBufSize, 5000);
                        if (nSaveRet < 0)
                        {
                            if (nSaveRet == (int)SDK_RET_CODE.H264_DVR_OPT_REBOOT)
                            {
                               sResult = "Need reboot device!";
                            }
                            else
                            {
                                sResult = "Save config error!";
                            }
                        }
                        break;
                    }

                case (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_NET_WIFI:
                    {
                        nBufSize = (uint)Marshal.SizeOf(typeof(SDK_NetWifiConfig));
                        curcfgWifi.bEnable = Convert.ToByte(checkEnableWifi.Checked);
                        curcfgWifi.sSSID = textBoxSSID.Text;
                        curcfgWifi.nKeyType = 1;  //默认是ASCII
                        curcfgWifi.sKeys = textBoxWifiPassword.Text;
                        curcfgWifi.nChannel = 0;

                        //对WEP加密方式，限制条件
                        if (Convert.ToBoolean(curcfgWifi.bEnable))
                        {
                            if (comboBoxWifiEntryType.SelectedItem != null)
                            {
                                curcfgWifi.sEncrypType = comboBoxWifiEntryType.SelectedItem.ToString();
                                if (curcfgWifi.sEncrypType == "WEP")
                                {
                                    curcfgWifi.nKeyType = comboBoxWifiKeyType.SelectedIndex;
                                }
                            }
                        }

                        string strIP, strMask, strGateway;
                        strIP = textBoxWifiIP.Text;
                        char[] ch = { '.' };
                        string[] arrIP = strIP.Split(ch, StringSplitOptions.RemoveEmptyEntries);
                        if (arrIP.GetLength(0) == 4)
                        {
                           
                            byte[] byIP = { byte.Parse(arrIP[0]), byte.Parse(arrIP[1]), byte.Parse(arrIP[2]),byte.Parse(arrIP[3])};
                            curcfgWifi.HostIP.c = byIP;
                        }
                        strMask = textBoxWifiSubmask.Text;
                        string[] arrMask = strMask.Split(ch, StringSplitOptions.RemoveEmptyEntries);
                        if (arrMask.GetLength(0) == 4)
                        {

                            byte[] byMask = { byte.Parse(arrMask[0]), byte.Parse(arrMask[1]), byte.Parse(arrMask[2]), byte.Parse(arrMask[3]) };
                            curcfgWifi.Submask.c = byMask;
                        }

                        strGateway = textBoxWifiGateway.Text;
                        string[] arrGateway = strGateway.Split(ch, StringSplitOptions.RemoveEmptyEntries);
                        if (arrGateway.GetLength(0) == 4)
                        {

                            byte[] byGateway = { byte.Parse(arrGateway[0]), byte.Parse(arrGateway[1]), byte.Parse(arrGateway[2]), byte.Parse(arrGateway[3]) };
                            curcfgWifi.Gateway.c = byGateway;
                        }


                        if (curWifiDevice.sAuth != null && curWifiDevice.sNetType != null) //有修改
                        {
                            curcfgWifi.sAuth = curWifiDevice.sAuth;
                            curcfgWifi.sNetType = curWifiDevice.sNetType;
                        }

                        IntPtr ptrBufWifi = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(SDK_NetWifiConfig)));
                        Marshal.StructureToPtr(curcfgWifi, ptrBufWifi, false);
                        nSaveRet = NetSDK.H264_DVR_SetDevConfig(nLoginID, nConfig, nChannel, ptrBufWifi, nBufSize, 5000);
                        break;
                    }
                case (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_NET_DHCP:
                    {
                        nBufSize = (uint)Marshal.SizeOf(typeof(SDK_NetDHCPConfigAll));
                        curDHCPcfgAll.vNetDHCPConfig[2].bEnable = Convert.ToByte(checkBoxDHCP.Checked);

                        IntPtr ptrBufDHCP = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(SDK_NetDHCPConfigAll)));
                        Marshal.StructureToPtr(curDHCPcfgAll, ptrBufDHCP, false);
                        nSaveRet = NetSDK.H264_DVR_SetDevConfig(nLoginID, nConfig, nChannel, ptrBufDHCP, nBufSize, 1000);                      
                        break;
                    }
                case (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_NET_NTP:
                    {
                        int nCfgSize = Marshal.SizeOf(typeof(SDK_NetNTPConfig));
                        SDK_NetNTPConfig cfg = new SDK_NetNTPConfig();
                        cfg.Enable = (byte)(m_ntpEnable.Checked ? 1 : 0);
                        m_ntpEnable.Checked = cfg.Enable == 1;
                        cfg.TimeZone = m_ntpAuto.Checked ? 13 : 14;
                        cfg.Server.ServerName = m_ntpServerIP.Text;
                        cfg.Server.Port = NetSDK.ToInt(m_ntpServerPort.Text, 80);
                        cfg.UpdatePeriod = NetSDK.ToInt(m_ntpUpdateTime.Text, 300);
                        IntPtr pCfg = Marshal.AllocHGlobal(nCfgSize);
                        Marshal.StructureToPtr(cfg, pCfg, false);
                        nSaveRet = NetSDK.H264_DVR_SetDevConfig(nLoginID, nConfig, nChannel, pCfg, (uint)nCfgSize, 5000);
                    }
                    break;
            }

            if (sResult.Length > 0)
            {
                MessageBox.Show(sResult);
            } 
            else if (nSaveRet < 0)
            {
                if (nSaveRet == (int)SDK_RET_CODE.H264_DVR_OPT_REBOOT)
                {
                    MessageBox.Show("Need reboot device!");
                }
                else
                {
                    MessageBox.Show("Save config failed[error=" + nSaveRet + "]!");
                }
            }
            else
            {
                MessageBox.Show("Save config success!");
            } 
        }

        private void buttongGeneralOk_Click(object sender, EventArgs e)
        {
            TreeNode node = (TreeNode)comboBoxDevice.SelectedItem;
            if ( node.Tag != null )
            {
                DEV_INFO devInfo = (DEV_INFO)node.Tag;
                SDK_HANDLE nLoginID = devInfo.lLoginID;
                int nChannel = -1;
                TabPage tab = tabControlConfigs.SelectedTab;
                uint nConfig = 0;
                if (tab.Name == "tabPageGeneral")
                {
                    nConfig = (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_SYSNORMAL;
                }
                if (nConfig != 0)
                {
                    SaveConfig(nLoginID, nChannel, nConfig);
                }
            }
        }

        private void tabControlConfigs_Selected(object sender, TabControlEventArgs e)
        {
            uint nConfig = 0;
            TreeNode node = (TreeNode)comboBoxDevice.SelectedItem;
            if (node == null || node.Tag == null)
            {
                return;
            }

            DEV_INFO devInfo = (DEV_INFO)node.Tag;
            int nLoginID = devInfo.lLoginID;
            int nChannel = -1;

            TabPage tab = tabControlConfigs.SelectedTab;
            if (tab.Name == "tabPageGeneral")
            {
                nConfig = (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_SYSNORMAL;
            }
            else if (tab.Name == "tabPageWifi")
            {
                nConfig = (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_NET_WIFI;
                UpdateConfig(nLoginID, nChannel, (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_NET_DHCP);
            }
            else if (tab.Name == "pagUserManager")
            {
                nConfig = (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_USER;
            }
            else if (tab.Name == "pagNTP")
            {
                nConfig = (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_NET_NTP;
            }
            else if (tab.Name == "SynDevTime")
            {
                nConfig = (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_SYS_TIME;
            }
            else if (tab.Name == "tabDevState")
            {
                nConfig = (uint)SDK_CONFIG_TYPE.E_SDK_CFG_OEM_GETINFO;
            }

            m_nLoginID = nLoginID;
            m_nConfig = nConfig;
            m_nChannel = nChannel;
            if (nConfig != 0)
            {
                UpdateConfig(nLoginID, nChannel, nConfig);
            }
        }

        private void buttonSearchWifi_Click(object sender, EventArgs e)
        {
            TreeNode node = (TreeNode)comboBoxDevice.SelectedItem;
            if (node.Tag != null)
            {
                DEV_INFO devInfo = (DEV_INFO)node.Tag;
                SDK_HANDLE nLoginID = devInfo.lLoginID;
                int nChannel = -1;

                UpdateConfig(nLoginID, nChannel, (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_NET_WIFI_AP_LIST);
            }
        }

        private void checkBoxDHCP_CheckedChanged(object sender, EventArgs e)
        {
            bool bDHCPChecked= checkBoxDHCP.Checked;
            textBoxWifiIP.Enabled = !bDHCPChecked;
            textBoxWifiSubmask.Enabled = !bDHCPChecked;
            textBoxWifiGateway.Enabled = !bDHCPChecked;
        }

        private void checkEnableWifi_CheckedChanged(object sender, EventArgs e)
        {
            bool bWifiEnable = checkEnableWifi.Checked;
            comboBoxWifiEntryType.Enabled = bWifiEnable;
            if (!checkBoxDHCP.Checked)
            {
                textBoxWifiIP.Enabled = bWifiEnable;
                textBoxWifiSubmask.Enabled = bWifiEnable;
                textBoxWifiGateway.Enabled = bWifiEnable;
            } 
        }

        private void listViewWifi_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            ListView.SelectedListViewItemCollection itemcollection = listViewWifi.SelectedItems;
            foreach (ListViewItem itemseleted in itemcollection)
            {
               
                if (itemseleted.Tag != null)
                {
                    SDK_NetWifiDevice wifiDevice = (SDK_NetWifiDevice)itemseleted.Tag;
                    curWifiDevice = wifiDevice;

                    textBoxSSID.Text = wifiDevice.sSSID;
                    comboBoxWifiEntryType.Items.Clear();
                    comboBoxWifiEntryType.Items.Add(wifiDevice.sEncrypType);
                    comboBoxWifiEntryType.SelectedIndex = 0 ;
                    if (wifiDevice.sEncrypType == "WEP")
                    {
                        comboBoxWifiKeyType.SelectedIndex = 0;
                        comboBoxWifiKeyType.Show();
                        labelKeyType.Show();
                    }
                    else
                    {
                        labelKeyType.Hide();
                        comboBoxWifiKeyType.Hide();
                    }
                    break;
                }
            }
        }
        SDK_NetWifiDevice curWifiDevice;

        private void buttonWifiOK_Click(object sender, EventArgs e)
        {
             TreeNode node = (TreeNode)comboBoxDevice.SelectedItem;
            if ( node.Tag != null )
            {
                DEV_INFO devInfo = (DEV_INFO)node.Tag;
                SDK_HANDLE nLoginID = devInfo.lLoginID;
                int nChannel = -1;
                TabPage tab = tabControlConfigs.SelectedTab;
                uint nConfig = 0;
                if (tab.Name == "tabPageWifi")
                {
                    nConfig = (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_NET_WIFI;
                    SaveConfig(nLoginID, nChannel, (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_NET_DHCP);
                }
                if (nConfig != 0)
                {
                    SaveConfig(nLoginID, nChannel, nConfig);
                }
            }
        }

        

        private void OnSaveClick(object sender, EventArgs e)
        {
            this.SaveConfig(m_nLoginID, m_nChannel, m_nConfig);
        }

        private void btnUserRefresh_Click(object sender, EventArgs e)
        {
            IntPtr bufPtr;
            uint nBufSize = 0;
            uint nOutSize = 0;
            nBufSize = (uint)Marshal.SizeOf(typeof(USER_MANAGE_INFO));
            bufPtr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(USER_MANAGE_INFO)));
            int nRet = NetSDK.H264_DVR_GetDevConfig(m_nLoginID, (int)SDK_CONFIG_TYPE.E_SDK_CONFIG_USER, -1, bufPtr, nBufSize, out nOutSize, 5000);
            if (nRet > 0)
            {
                m_userList.Items.Clear();
                USER_MANAGE_INFO cfg = (USER_MANAGE_INFO)Marshal.PtrToStructure(bufPtr, typeof(USER_MANAGE_INFO));
                m_userCfg = cfg;
                for (int i = 0; i < cfg.userNum; i++)
                {
                    ListViewItem item = new ListViewItem();
                    item.Text = cfg.userList[i].name;
                    item.SubItems.Add(cfg.userList[i].Groupname);
                    item.Tag = item;
                    m_userList.Items.Insert(i, item);
                }
            }
        }

        string GetSeletectUser()
        {
            if (m_userList.SelectedIndices != null && m_userList.SelectedIndices.Count > 0)
            {
                ListView.SelectedIndexCollection c = m_userList.SelectedIndices;
                return m_userList.Items[c[0]].Text;
            }
            return "";
        }

        private void btnAddUser_Click(object sender, EventArgs e)
        {
            UserOperation user = new UserOperation(m_nLoginID, "", m_userCfg);
            user.ShowDialog(this);
            this.btnUserRefresh_Click(null, null);
        }

        private void btnModifyUser_Click(object sender, EventArgs e)
        {
            string sUserName = GetSeletectUser();
            if (sUserName.Length <= 0)
            {
                return;
            }
            UserOperation user = new UserOperation(m_nLoginID, sUserName, m_userCfg);
            user.ShowDialog(this);
            this.btnUserRefresh_Click(null, null);
        }

        private void btnDeleteUser_Click(object sender, EventArgs e)
        {
            string sUserName = GetSeletectUser();
            if (sUserName.Length <= 0)
            {
                return;
            }
            if (sUserName == "admin")
            {
                MessageBox.Show("Error!");
                return;
            }

            bool bFindUser = false;
            USER_INFO userInfo = new USER_INFO();
            for (int i = 0; i < m_userCfg.userNum; i++)
            {
                if (m_userCfg.userList[i].name == sUserName)
                {
                    userInfo = m_userCfg.userList[i];
                    bFindUser = true;
                    break;
                }
            }

            if (bFindUser)
            {
                uint nCmdType = (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_DELETE_USER;
                int nCfgSize = Marshal.SizeOf(typeof(USER_INFO));
                IntPtr pCfg = Marshal.AllocHGlobal(nCfgSize);
                Marshal.StructureToPtr(userInfo, pCfg, false);
                int nSaveRet = NetSDK.H264_DVR_SetDevConfig(m_nLoginID, nCmdType, 0, pCfg, (uint)nCfgSize, 5000);

                this.btnUserRefresh_Click(null, null);
            }
        }

        private void BtnSynTime_Click(object sender, EventArgs e)
        {
            System.DateTime currentTime = System.DateTime.Now;
            SDK_SYSTEM_TIME sysTime = new SDK_SYSTEM_TIME();
            sysTime.year = currentTime.Year;
            sysTime.month = currentTime.Month;
            sysTime.day = currentTime.Day;
            sysTime.hour = currentTime.Hour;
            sysTime.minute = currentTime.Minute;
            sysTime.second = currentTime.Second;
            sysTime.wday = 0;
            sysTime.isdst = 0;

            int nCfgSize = Marshal.SizeOf(typeof(SDK_SYSTEM_TIME));
            IntPtr pCfg = Marshal.AllocHGlobal(nCfgSize);
            Marshal.StructureToPtr(sysTime, pCfg, false);
            int nSetRet = NetSDK.H264_DVR_SetDevConfig(m_nLoginID, (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_SYS_TIME, 0, pCfg, (uint)nCfgSize, 5000);
            if (nSetRet != 1)
            {
                MessageBox.Show("Error:" + nSetRet);
            }
        }

        private void btnSetDevTimeZone_Click(object sender, EventArgs e)
        {
            if (m_cbTimeZone.SelectedIndex < 0)
            {
                return;
            }

            int nCfgSize = Marshal.SizeOf(typeof(SDK_TimeZone));
            IntPtr pCfg = Marshal.AllocHGlobal(nCfgSize);
            SDK_TimeZone cfg = new SDK_TimeZone();
            cfg.minuteswest = GetTimeZoneValue();
            cfg.FistFlag = 0;
            Marshal.StructureToPtr(cfg, pCfg, false);
            int nSetRet = NetSDK.H264_DVR_SetDevConfig(m_nLoginID, (uint)SDK_CONFIG_TYPE.E_SDK_CFG_TIME_ZONE, 0, pCfg, (uint)nCfgSize, 5000);
            if (nSetRet != 1)
            {
                MessageBox.Show("Error:" + nSetRet);
            }
        }
    }
}