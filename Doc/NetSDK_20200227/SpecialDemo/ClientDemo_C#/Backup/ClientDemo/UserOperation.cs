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
    public partial class UserOperation : Form
    {
        USER_INFO m_userInfo = new USER_INFO();
        USER_MANAGE_INFO m_userCfg = new USER_MANAGE_INFO();
        string m_sUserName;
        int m_lDevLoinId;
        public UserOperation(int lDevLoinId, string sUserName, USER_MANAGE_INFO userCfg)
        {
            InitializeComponent();
            m_sUserName = sUserName;
            m_userCfg = userCfg;
            m_lDevLoinId = lDevLoinId;
            for (int i = 0; i < m_userCfg.userNum; i++)
            {
                if (m_userCfg.userList[i].name == m_sUserName)
                {
                    m_userInfo = m_userCfg.userList[i];
                    break;
                }
            }

            if (m_sUserName.Length > 0)
            {
                m_tbMemo.Text = m_userInfo.memo;
                m_tbUser.Text = m_userInfo.name;
                m_tbPwd.Text = m_userInfo.passWord;
                m_cbResue.Checked = m_userInfo.shareable == 1;
                m_tbUser.Enabled = false;
            }
            else
            {
                IntPtr bufPtr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(USER_INFO)));
                m_userInfo = (USER_INFO)Marshal.PtrToStructure(bufPtr, typeof(USER_INFO));
                m_userInfo.reserved = 0;
                m_cbResue.Checked = true;
            }

            int nSelected = -1;
            for (int i = 0; i < userCfg.groupNum; i++)
            {
                m_comGroup.Items.Add(userCfg.groupList[i].name);
                if (nSelected == -1 && userCfg.groupList[i].name == m_userInfo.Groupname)
                {
                    nSelected = i;
                }
            }

            if (nSelected == -1 && m_comGroup.Items.Count > 0)
            {
                nSelected = 0;
            }

            if (nSelected >= 0)
            {
                m_comGroup.SelectedIndex = nSelected;
            }
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (m_comGroup.SelectedIndex < 0 
                || m_comGroup.SelectedIndex >= m_userCfg.groupNum 
                || m_tbUser.Text.Length <= 0)
            {
                return;
            }
            USER_GROUP_INFO group = m_userCfg.groupList[m_comGroup.SelectedIndex];

            m_userInfo.name = m_tbUser.Text;
            m_userInfo.passWord = m_tbPwd.Text;
            m_userInfo.memo = m_tbMemo.Text;
            m_userInfo.Groupname = group.name;
            m_userInfo.shareable = (byte)(m_cbResue.Checked ? 1 : 0);

            int nRightCount = 0;
            for (int i = 0; i < m_userRightList.Items.Count; i++)
            {
                if (m_userRightList.Items[i].Checked)
                {
                    m_userInfo.rights[nRightCount++].name = m_userRightList.Items[i].Text;
                }
            }
            m_userInfo.rigthNum = nRightCount;

            int nSaveRet = 0;
            if (m_sUserName.Length <= 0)
            {
                m_userInfo.reserved = 0;
                uint nCmdType = (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_ADD_USER;
                int nCfgSize = Marshal.SizeOf(typeof(USER_INFO));
                IntPtr pCfg = Marshal.AllocHGlobal(nCfgSize);
                Marshal.StructureToPtr(m_userInfo, pCfg, false);
                nSaveRet = NetSDK.H264_DVR_SetDevConfig(m_lDevLoinId, nCmdType, 0, pCfg, (uint)nCfgSize, 5000);
            }
            else
            {
                uint nCmdType = (uint)SDK_CONFIG_TYPE.E_SDK_CONFIG_MODIFY_USER;
                int nCfgSize = Marshal.SizeOf(typeof(CONF_MODIFYUSER));
                IntPtr pCfg = Marshal.AllocHGlobal(nCfgSize);
                CONF_MODIFYUSER modify = new CONF_MODIFYUSER();
                modify.User = m_userInfo;
                modify.UserName = m_userInfo.name;
                Marshal.StructureToPtr(modify, pCfg, false);
                nSaveRet = NetSDK.H264_DVR_SetDevConfig(m_lDevLoinId, nCmdType, 0, pCfg, (uint)nCfgSize, 5000);
            }

            if (nSaveRet == 1)
            {
                this.Close();
            }
            else
            {
                MessageBox.Show("Error:" + nSaveRet);
            }
        }

        private void m_comGroup_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (m_comGroup.SelectedIndex < 0 || m_comGroup.SelectedIndex >= m_userCfg.groupNum)
            {
                return;
            }

            USER_GROUP_INFO group = m_userCfg.groupList[m_comGroup.SelectedIndex];
            m_userRightList.Items.Clear();
            for (int i = 0; i < group.rigthNum; i++)
            {
                ListViewItem item = new ListViewItem();
                item.Text = group.rights[i].name;
                item.Checked = false;
                m_userRightList.Items.Add(item);
            }

            if (m_sUserName.Length > 0 && m_userInfo.Groupname == group.name)
            {
                for (int i = 0; i < m_userRightList.Items.Count; i++)
                {
                    for (int j = 0; j < m_userInfo.rigthNum; j++)
                    {
                        if (m_userInfo.rights[j].name == m_userRightList.Items[i].Text)
                        {
                            m_userRightList.Items[i].Checked = true;
                            break;
                        }
                    }
                }
            }
            else
            {
                for (int i = 0; i < m_userRightList.Items.Count; i++)
                {
                    m_userRightList.Items[i].Checked = true;
                }
            }
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
