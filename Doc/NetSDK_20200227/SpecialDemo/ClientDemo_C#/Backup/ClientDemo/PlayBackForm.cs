﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.IO;
using System.Threading;
using User.NetSDK;
using SDK_HANDLE = System.Int32;

namespace ClientDemo
{
    public class ComboxItem
    {
        public string Text = "";

        public object Value = new object() ;
        public ComboxItem(string _Text, object _Value)
        {
            Text = _Text;
            Value = _Value;
        }

        public override string ToString()
        {
            return Text;
        }
    }


    public partial class PlayBackForm : Form
    {
        enum PlayBackFileType
        {
            SDK_RECORD_ALL = 0,
            SDK_RECORD_ALARM = 1, //外部报警录像
            SDK_RECORD_DETECT,	  //视频侦测录像
            SDK_RECORD_REGULAR,	  //普通录像
            SDK_RECORD_MANUAL,	  //手动录像
            SDK_PIC_ALL = 10,
            SDK_PIC_ALARM,		  //外部报警录像
            SDK_PIC_DETECT,		  //视频侦测录像
            SDK_PIC_REGULAR,      //普通录像
            SDK_PIC_MANUAL,       //手动录像
            SDK_TYPE_NUM
        }

        enum PlayBackAction
        {
            SDK_PLAY_BACK_PAUSE,		/*<! 暂停回放 */
            SDK_PLAY_BACK_CONTINUE,		/*<! 继续回放 */
            SDK_PLAY_BACK_SEEK,			/*<! 回放定位，时间s为单位 */
            SDK_PLAY_BACK_FAST,	        /*<! 加速回放 */
            SDK_PLAY_BACK_SLOW,	        /*<! 减速回放 */
            SDK_PLAY_BACK_SEEK_PERCENT, /*<! 回放定位百分比 */
        };

        int m_lLoginID = 0;
        static int PLAYBACK_MAX_FILE_NUM = 64;
        System.Timers.Timer timerLocalPlayBack = new System.Timers.Timer(200);
        System.Timers.Timer timerNetPlayBack = new System.Timers.Timer(200);
        System.Timers.Timer timerDownload = new System.Timers.Timer(200);

        bool m_bPauseLocalPlay = false;  // 暂停标志
        bool m_bPauseNetPlay = false;
        int m_nFastTypeLocal = 0; //本地快播速度    
        int m_nFastTypeNet = 0; //网络快播速度  
        int m_nSlowTypeLocal = 0;  // 网络慢波速度
        int m_nSlowTypeNet = 0;  // 网络慢波速度
       
	    bool m_bDownloadByTime = false;
	    bool m_bDownloadByName = true;
	    uint m_nPlaybackDecHandle = 0;
        int m_nLocalplayHandle = 0;   
	    int m_nNetPlayHandle = 0;
	    int m_DownLoadFileHandle = 0;
	    int m_nCurRecNum=0;	
	    int m_ListFindNum=0;
        int m_SliderPos = 0;
	    long m_PreTime = 0;
        
        H264_DVR_FINDINFO[] m_nSearchInfo = new H264_DVR_FINDINFO[100];
        
        List<H264_DVR_FILE_DATA> m_listFile = new List<H264_DVR_FILE_DATA>();
        List<H264_DVR_FINDINFO> m_listFindInfo = new List<H264_DVR_FINDINFO>();
             

        int   m_nGetPage = 0;
        int   m_nCurPage = 0;
        int   m_nTotalPage = 0;
      
        bool m_bOpenAudio = false;



        public delegate int NetDataCallBack_V2(SDK_HANDLE lRealHandle, ref PACKET_INFO_EX pFrame, int dwUser);
        public delegate void SDKDownLoadPosCallback(SDK_HANDLE lPlayHandle, int lTotalSize, int lDownLoadSize, int dwUser);

        private int NetCallBack_V2(SDK_HANDLE lRealHandle, ref PACKET_INFO_EX pFrame, int dwUser)
        {	

            //bool bResult = ;
            //try
            //{

            //        if (bResult)
            //        {
            //            return bResult;
            //        }	
            //}

            return 0;
        	
        }

        private void DownLoadPosCallback(SDK_HANDLE lPlayHandle, 
														   int lTotalSize, 
														   int lDownLoadSize, 
														   int dwUser)
        {	         	
	        if ( 0xfffffff == lDownLoadSize )
	        {
                this.pictureBoxNetVideoWnd.Refresh();	
		        progressBarDownloadPos.Value = 0;
                timerDownload.Stop();
	        }
        }

        private int RealDataCallBack(SDK_HANDLE lRealHandle, int dwDataType, string strBuf, int lbufsize, int dwUser)
        {
            m_nNetPlayHandle = 0;
            return 0;
        }

        public PlayBackForm()
        {
            InitializeComponent();
            trackBarLocalPlayPos.SetRange(0, 1000);
            trackBarNetPlayPos.SetRange(0, 1000);
            beginTime.Value = System.DateTime.Now.Date;
            
        }


        void FileEndCallBack(SDK_HANDLE lPlayHand, uint nUser)
        {
            if (NetSDK.H264_DVR_StopLocalPlay(m_nLocalplayHandle))
            {
                m_nLocalplayHandle = 0;
            }
            timerLocalPlayBack.Enabled = false;
            trackBarLocalPlayPos.Value = 0;
        }

        public void recordTime(object source,System.Timers.ElapsedEventArgs e)
        {
            if (!m_bPauseLocalPlay)
            { 
                float pos = NetSDK.H264_DVR_GetPlayPos(m_nLocalplayHandle);
                trackBarLocalPlayPos.Value = Convert.ToInt32(pos*1000);
            }
 
            if (!m_bPauseNetPlay)
            {
                float pos = NetSDK.H264_DVR_GetPlayPos(m_nNetPlayHandle);
                trackBarNetPlayPos.Value = Convert.ToInt32(pos * 1000);
            }

            if (m_DownLoadFileHandle != 0)
            {
                int nPos = NetSDK.H264_DVR_GetDownloadPos(m_DownLoadFileHandle);
                if (nPos < 0)		//fail
                {
                    NetSDK.H264_DVR_StopGetFile(m_DownLoadFileHandle);
                    progressBarDownloadPos.Value = 0;
                    m_DownLoadFileHandle = 0;
                    btnDownload.Text = "Download";
                    MessageBox.Show("Get download process fail !");
                    timerDownload.Stop();
                    
                }
                if (nPos == 100)		//download end
                {
                    NetSDK.H264_DVR_StopGetFile(m_DownLoadFileHandle);
                    m_DownLoadFileHandle = 0;

                    timerDownload.Stop();
                    progressBarDownloadPos.Value = 0;
                    btnDownload.Text = "Download";
                    MessageBox.Show("Download Finished");
                    return;

                }
                if (nPos > 100)		//abnormal
                {
                    NetSDK.H264_DVR_StopGetFile(m_DownLoadFileHandle);
                    m_DownLoadFileHandle = 0;
                    progressBarDownloadPos.Value = 0;
                    btnDownload.Text = "Download";
                    MessageBox.Show("Download Error");
                }
                else
                {
                    progressBarDownloadPos.Value = nPos;
                }
            }
            
         }


        private void btnOpenFile_Click(object sender, EventArgs e)
        {
            OpenFileDialog fileDialog = new OpenFileDialog();
            // fileDialog.InitialDirectory = "d:\\";
            fileDialog.Title = "选择文件";
            fileDialog.Filter = "h264 files (*.h264)|*.h264";
            fileDialog.FilterIndex = 1;
            fileDialog.RestoreDirectory = true;

            if (fileDialog.ShowDialog() == DialogResult.OK)
            {
                String fileName = fileDialog.FileName;
                // 使用文件名
                m_nLocalplayHandle =  Convert.ToInt32(NetSDK.H264_DVR_StartLocalPlay( fileName, pictureBoxVideoWnd.Handle,null,Convert.ToUInt32(0) ));
                if ( m_nLocalplayHandle > 0)
                {
                 //   MessageBox.Show("success");
                    timerLocalPlayBack.Elapsed += new System.Timers.ElapsedEventHandler(recordTime);
                    timerLocalPlayBack.AutoReset = true;
                    timerLocalPlayBack.Enabled = true;
                    NetSDK.fLocalPlayFileCallBack fileEndCallBack = new NetSDK.fLocalPlayFileCallBack(FileEndCallBack);
                    NetSDK.H264_DVR_SetFileEndCallBack(m_nLocalplayHandle,fileEndCallBack, this.Handle );
                }
                else
                {
                    MessageBox.Show("failed");
                }
            }
            else
            {
                // 没有选择文件时的操作
            } 

        }

        private void btnLocalStop_Click(object sender, EventArgs e)
        {
            if (NetSDK.H264_DVR_StopLocalPlay(m_nLocalplayHandle))
            {
                m_nLocalplayHandle = 0;
            }
            timerLocalPlayBack.Enabled = false;
            trackBarLocalPlayPos.Value = 0;
        }

        private void btnLocalPause_Click(object sender, EventArgs e)
        {
            if (NetSDK.H264_DVR_LocalPlayCtrl(m_nLocalplayHandle, SDK_LoalPlayAction.SDK_Local_PLAY_PAUSE, 0))
            {
                m_bPauseLocalPlay = true;
            }
        }

        private void btnLocalFast_Click(object sender, EventArgs e)
        {
            if (++m_nFastTypeLocal > 4)
            {
                m_nFastTypeLocal = 1;
            }
            m_nSlowTypeLocal = 0;
            NetSDK.H264_DVR_LocalPlayCtrl(m_nLocalplayHandle, SDK_LoalPlayAction.SDK_Local_PLAY_FAST, Convert.ToUInt32(m_nFastTypeLocal));	
        }

        private void btnLocalSlow_Click(object sender, EventArgs e)
        {
            if (++m_nSlowTypeLocal > 4)
            {
                m_nSlowTypeLocal = 1;
            }
            m_nFastTypeLocal = 0;
            NetSDK.H264_DVR_LocalPlayCtrl(m_nLocalplayHandle, SDK_LoalPlayAction.SDK_Local_PLAY_SLOW, Convert.ToUInt32(m_nSlowTypeLocal));	
        }

        private void btnLocalPlay_Click(object sender, EventArgs e)
        {
            if (m_bPauseLocalPlay)
            {
                NetSDK.H264_DVR_LocalPlayCtrl(m_nLocalplayHandle, SDK_LoalPlayAction.SDK_Local_PLAY_CONTINUE, 0);
                m_bPauseLocalPlay = false;
            }

            if (m_nSlowTypeLocal != 0)
            {
                NetSDK.H264_DVR_LocalPlayCtrl(m_nLocalplayHandle, SDK_LoalPlayAction.SDK_Local_PLAY_SLOW, 0);
            }
            if (m_nFastTypeLocal != 0)
            {
                NetSDK.H264_DVR_LocalPlayCtrl(m_nLocalplayHandle, SDK_LoalPlayAction.SDK_Local_PLAY_FAST, 0);
            }
            m_nSlowTypeLocal = 0;
            m_nFastTypeLocal = 0;
	
        }

        private void trackBarPlayPos_Scroll(object sender, EventArgs e)
        {
            int value = trackBarLocalPlayPos.Value;
            NetSDK.H264_DVR_SetPlayPos(m_nLocalplayHandle, (float)(value / 1000.0));
        }


        // 点击网网络的选项卡
        private void tabControlPlayBack_Click(object sender, EventArgs e)
        {
            if (tabControlPlayBack.SelectedTab.Name == "tabPageRemovePlayBack")
            {
                comboDev.Items.Clear();
                ClientDemo clientForm = ClientDemo.Instance;
                foreach (Form form in Application.OpenForms)
                {
                    if (form.Name == "ClientDemo")
                    {
                        clientForm = (ClientDemo)form;
                        break;
                    }
                }

                foreach (TreeNode node in clientForm.devForm.DevTree.Nodes)
                {
                    ComboxItem item = new ComboxItem(node.Text,node.Tag);
                    comboDev.Items.Add(item);
                }

                comboRecordType.Items.Clear();

                comboRecordType.Items.Add("All video");
                comboRecordType.Items.Add("External alarm video");
                comboRecordType.Items.Add("Video detection video");
                comboRecordType.Items.Add("Ordinary video");
                comboRecordType.Items.Add("Manual recording");
                comboRecordType.Items.Add("All pictures");
                comboRecordType.Items.Add("Alarm to capture");
                comboRecordType.Items.Add("Video capture detection");
                comboRecordType.Items.Add("Capture timing");
                comboRecordType.Items.Add("Manual capture");
                comboRecordType.SelectedIndex = 0;
                
                checkFile.Checked = true;
            }
            else
            {

            }
           
        }

        private void checkFile_CheckedChanged(object sender, EventArgs e)
        {
            if (checkFile.Checked == true)
            {
                checkTime.Checked = false;
                m_bDownloadByTime = false;
                m_bDownloadByName = true;
            }
        }

        private void checkTime_CheckedChanged(object sender, EventArgs e)
        {
            if (checkTime.Checked == true)
            {
                m_bDownloadByTime = true;
                m_bDownloadByName = false;
                checkFile.Checked = false;
            } 

        }

        private void btnSearch_Click(object sender, EventArgs e)
        {

            m_nGetPage = 0;
            m_nCurPage = 0;
            m_nTotalPage = 0;

            // 条件检测
           if(comboDev.SelectedIndex < 0)
           {
               MessageBox.Show("Select a Device!");
               return;
           }
           if (!checkFile.Checked && !checkTime.Checked)
           {
               MessageBox.Show("Select a Search mode!");
               return;
           }


           ClearResult();

           H264_DVR_TIME StartTime;
           H264_DVR_TIME StopTime;

           int nChannel = comboChannel.SelectedIndex;	//channel No.
           int nFileType = comboRecordType.SelectedIndex;		//file type
           if (nFileType >= 5)
           {
               nFileType += 5;
           }
             
           StartTime.dwYear = beginDate.Value.Year;
           StartTime.dwMonth = beginDate.Value.Month;
           StartTime.dwDay = beginDate.Value.Day;
           StartTime.dwHour = beginTime.Value.Hour;
           StartTime.dwMinute = beginTime.Value.Minute;
           StartTime.dwSecond = beginTime.Value.Second;

           StopTime.dwYear = endDate.Value.Year;
           StopTime.dwMonth = endDate.Value.Month;
           StopTime.dwDay = endDate.Value.Day;
           StopTime.dwHour =endDate.Value.Hour;
           StopTime.dwMinute = endDate.Value.Minute;
           StopTime.dwSecond = endDate.Value.Second;
           H264_DVR_FILE_DATA[] szSend = new H264_DVR_FILE_DATA[64];

           ComboxItem item = (ComboxItem)comboDev.SelectedItem;
         
           if (item.Value != null)
           {
               DEV_INFO devInfo = (DEV_INFO)item.Value;


               SDK_HANDLE lLoginID = devInfo.lLoginID;
               int nMaxLen = 100;
               int waitTime = 4000;
               int nNum = 0;
               H264_DVR_FINDINFO searchInfo = new H264_DVR_FINDINFO();
               searchInfo.startTime = StartTime;
               searchInfo.endTime = StopTime;
               searchInfo.nChannelN0 = nChannel;
               searchInfo.nFileType = nFileType;


               IntPtr ptr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(H264_DVR_FILE_DATA)) * 100);



               int nRet = NetSDK.H264_DVR_FindFile(lLoginID, ref searchInfo, ptr, nMaxLen, out nNum, waitTime);
               m_nCurRecNum = nNum;

               for (int index = 0; index < nNum; index++)
               {
                   unsafe
                   {
                       int* pDev = (int*)ptr.ToPointer();
                       pDev += Marshal.SizeOf(typeof(H264_DVR_FILE_DATA)) * index / 4;

                       IntPtr ptrTemp = new IntPtr(pDev);
                       szSend[index] = (H264_DVR_FILE_DATA)Marshal.PtrToStructure(ptrTemp, typeof(H264_DVR_FILE_DATA));
                    //   m_listFile[index] = szSend[index];
                       
                   }

               }
               m_listFile.Clear();
               if ( nRet > 0 )
               {
                   if ( nNum > 0) 
                   {
                      
                       if (m_nCurRecNum > m_listFile.Capacity)
                       {
                           m_listFile.Capacity = m_nCurRecNum;
                       }
                       

                       for (int i = 0; i < m_nCurRecNum; i++)
                       {
                           m_listFile.Add(szSend[i]);
                       }
                       m_nSearchInfo[m_nCurPage] = searchInfo;
                       m_nCurPage = 1;
                       m_nSearchInfo[m_nCurPage].nChannelN0 = nChannel;
                       m_nSearchInfo[m_nCurPage].nFileType = nFileType;
                       m_nSearchInfo[m_nCurPage].startTime.dwYear = szSend[m_nCurRecNum - 1].stEndTime.year;
                       m_nSearchInfo[m_nCurPage].startTime.dwMonth = szSend[m_nCurRecNum - 1].stEndTime.month;
                       m_nSearchInfo[m_nCurPage].startTime.dwDay = szSend[m_nCurRecNum - 1].stEndTime.day;
                       m_nSearchInfo[m_nCurPage].startTime.dwHour = szSend[m_nCurRecNum - 1].stEndTime.hour;
                       m_nSearchInfo[m_nCurPage].startTime.dwMinute = szSend[m_nCurRecNum - 1].stEndTime.minute;
                       m_nSearchInfo[m_nCurPage].startTime.dwSecond = szSend[m_nCurRecNum - 1].stEndTime.second;
                       m_nSearchInfo[m_nCurPage].endTime = searchInfo.endTime;

                       m_nGetPage++;
                       
                       if (nNum < PLAYBACK_MAX_FILE_NUM)
                       {
                           m_nTotalPage = m_nGetPage;
                       }

                       AddFileListInfo(m_nCurRecNum);//add list item
                       SetPageBtnState(nNum);
                   }
                   else
                   {
                       MessageBox.Show("No File");
                   }
               }
               else
               {
                   MessageBox.Show("SearchFail");
               }
           }

        }
        private void AddFileListInfo(int iNum)
        {
	        if ( m_bDownloadByTime )
	        {
		        iNum = 1;
		        m_nCurPage=1;
	            m_nTotalPage=1;
	        }
            int count = listFile.Items.Count;

	        for ( int i = 0; i < iNum ; i++)
	        {
		     
        		string tempstring = string.Format("{0}", i + 1);


                ListViewItem.ListViewSubItem subitem1 = new ListViewItem.ListViewSubItem();
                subitem1.Text = tempstring;
                subitem1.Name = "No.";

		        string strBtme,strEtime;
		        if ( m_bDownloadByTime )
		        {
                    strBtme = string.Format("{0}-{1}-{2}-{3}:{4}:{5}", beginDate.Value.Year,
                        beginDate.Value.Month,
                        beginDate.Value.Day,
                        beginTime.Value.Hour,
                        beginTime.Value.Minute,
                        beginTime.Value.Second);

                    strEtime = string.Format("{0}-{1}-{2}-{3}:{4}:{5}", endDate.Value.Year, 
				        endDate.Value.Month,
                        endDate.Value.Day,
				        endTime.Value.Hour,
                        endTime.Value.Minute,
                        endTime.Value.Second);
        			
			        tempstring = string.Format("{0}-{1}", strBtme, strEtime);
		        }else
		        {
                    strBtme = string.Format("{0}-{1}-{2}-{3}:{4}:{5}", m_listFile[i].stBeginTime.year, 
				        m_listFile[i].stBeginTime.month,
                        m_listFile[i].stBeginTime.day,
                        m_listFile[i].stBeginTime.hour,
                        m_listFile[i].stBeginTime.minute,
                        m_listFile[i].stBeginTime.second);

                    strEtime = string.Format("{0}-{1}-{2}-{3}:{4}:{5}",
                        m_listFile[i].stEndTime.year,
                        m_listFile[i].stEndTime.month,
                        m_listFile[i].stEndTime.day,
                        m_listFile[i].stEndTime.hour,
                        m_listFile[i].stEndTime.minute,
                        m_listFile[i].stEndTime.second);

                    tempstring = string.Format("{0}-{1}({2}KB)", strBtme, strEtime, m_listFile[i].size);
		        }


        		ListViewItem item = new ListViewItem();
               
                ListViewItem.ListViewSubItem subitem2 = new ListViewItem.ListViewSubItem();
                subitem2.Text = tempstring;
                subitem2.Name = "FileTime";

                item.SubItems.Insert(0,subitem1);
                item.SubItems.Insert(1,subitem2);
                item.Tag = m_listFile[i];

                listFile.Items.Insert(count + i, item); 
 
	        }
        }

        private void SetPageBtnState(int num)
        {
            if (m_nTotalPage == m_nGetPage && m_nCurPage == m_nGetPage)
            {
                btnNextPage.Enabled = false;
            }
            else
            {
                btnNextPage.Enabled = true;
            }
	        
	        if ( m_nCurPage > 1 )
	        {
                btnPrePage.Enabled = true;
	        }
	        else
	        {
		        btnPrePage.Enabled = false;
	        }
            
        }

        private void btnDownload_Click(object sender, EventArgs e)
        {
            int nSelectedIndex = comboDev.SelectedIndex;
            if (nSelectedIndex < 0)
            {
                MessageBox.Show("SelectDevice!");
                return;
            }
           
            

             if (listFile.FocusedItem==null)
            {
                MessageBox.Show("Select a File !");
                return;
            }

            timerDownload.Elapsed += new System.Timers.ElapsedEventHandler(recordTime);
            timerDownload.AutoReset = true;
            timerDownload.Enabled = true;


            if (m_bDownloadByTime)
            {
                DownloadByTime();
            }
            else
            {
                DownloadByName();
            }
        }

        private void btnPrePage_Click(object sender, EventArgs e)
        {
            if (m_nGetPage <= 1 || m_nCurPage <= 1)
            {

                return;
            }

            m_nCurPage--;
            m_nGetPage--;

            ClearResult();

            m_ListFindNum = m_ListFindNum - 1;

           // if (GetNextPageInfo( m_listFindInfo[--m_ListFindNum]) > 0 )
            {
                H264_DVR_FILE_DATA[] szSend = new H264_DVR_FILE_DATA[100];

                int nNum = 0;

               // H264_DVR_FINDINFO findinfo = m_listFindInfo[m_ListFindNum];

                IntPtr ptr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(H264_DVR_FILE_DATA)) * 100);

                int nRet = NetSDK.H264_DVR_FindFile(m_lLoginID, ref m_nSearchInfo[m_nCurPage-1], ptr, 100, out nNum, 5000);
                
                for (int index = 0; index < nNum; index++)
                {
                    unsafe
                    {
                        int* pDev = (int*)ptr.ToPointer();
                        pDev += Marshal.SizeOf(typeof(H264_DVR_FILE_DATA)) * index / 4;

                        IntPtr ptrTemp = new IntPtr(pDev);
                        szSend[index] = (H264_DVR_FILE_DATA)Marshal.PtrToStructure(ptrTemp, typeof(H264_DVR_FILE_DATA));
                       // m_listFile[index] = szSend[index];
                    }

                }
                m_listFile.Clear();
               
                m_nCurRecNum = nNum;

                if (nRet > 0 && nNum > 0) //处理没有录象的情况
                {
                    if (m_nCurRecNum > m_listFile.Capacity)
                    {
                        m_listFile.Capacity = m_nCurRecNum;
                    }
                    

                    for (int i = 0; i < m_nCurRecNum; i++)
                    {
                        m_listFile.Add(szSend[i]);
                    }
                   
                    m_ListFindNum++;
                }
                else
                {
                   
                }
                AddFileListInfo(m_nCurRecNum);
                SetPageBtnState(nNum);
            }
        }
        private int GetNextPageInfo(H264_DVR_FINDINFO searchInfo)
        {
	        H264_DVR_FILE_DATA[] szSend = new H264_DVR_FILE_DATA[100];
	        int nNum=0;


            IntPtr ptr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(H264_DVR_FILE_DATA)) * 100);


            int ret = NetSDK.H264_DVR_FindFile(m_lLoginID, ref searchInfo, ptr ,100, out nNum, 5000);

            //for (int index = 0; index < 100; index++)
            //{
            //    unsafe
            //    {
            //        int* pDev = (int*)ptr.ToPointer();
            //        pDev += Marshal.SizeOf(typeof(H264_DVR_FILE_DATA)) * index / 4;

            //        IntPtr ptrTemp = new IntPtr(pDev);
            //        szSend[index] = (H264_DVR_FILE_DATA)Marshal.PtrToStructure(ptrTemp, typeof(H264_DVR_FILE_DATA));
            //    }

            //}
	       
        	
	        return ret;
        }

        private void btnNextPage_Click(object sender, EventArgs e)
        {
            if (m_nGetPage <= 0)
            {
                return;
            }

            ClearResult();

            //if (GetNextPageInfo(m_nSearchInfo) > 0 && (m_nCurPage == m_nGetPage))
            {
                H264_DVR_FILE_DATA[] szSend = new H264_DVR_FILE_DATA[100];

                int nNum = 0;

                IntPtr ptr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(H264_DVR_FILE_DATA)) * 100);

                int nRet = NetSDK.H264_DVR_FindFile(m_lLoginID, ref m_nSearchInfo[m_nCurPage], ptr, 100, out nNum, 5000);

                for (int index = 0; index < nNum; index++)
                {
                    unsafe
                    {
                        int* pDev = (int*)ptr.ToPointer();
                        pDev += Marshal.SizeOf(typeof(H264_DVR_FILE_DATA)) * index / 4;

                        IntPtr ptrTemp = new IntPtr(pDev);
                        szSend[index] = (H264_DVR_FILE_DATA)Marshal.PtrToStructure(ptrTemp, typeof(H264_DVR_FILE_DATA));
                        m_listFile[index] = szSend[index];
                    }

                }

                m_listFile.Clear();
                m_nCurRecNum = nNum;

                if (nRet > 0 && nNum > 0)  //处理没有录象的情况
                {
                   
                    if (m_nCurRecNum > m_listFile.Capacity)
                    {
                        m_listFile.Capacity = m_nCurRecNum;
                    }
                    

                    for (int i = 0; i < m_nCurRecNum; i++)
                    {
                        m_listFile.Add(szSend[i]);
                    }
                    m_nCurPage++;
                    m_nSearchInfo[m_nCurPage].nChannelN0 = m_nSearchInfo[0].nChannelN0;
                    m_nSearchInfo[m_nCurPage].nFileType = m_nSearchInfo[0].nFileType;
                    m_nSearchInfo[m_nCurPage].startTime.dwYear = szSend[m_nCurRecNum - 1].stEndTime.year;
                    m_nSearchInfo[m_nCurPage].startTime.dwMonth = szSend[m_nCurRecNum - 1].stEndTime.month;
                    m_nSearchInfo[m_nCurPage].startTime.dwDay = szSend[m_nCurRecNum - 1].stEndTime.day;
                    m_nSearchInfo[m_nCurPage].startTime.dwHour = szSend[m_nCurRecNum - 1].stEndTime.hour;
                    m_nSearchInfo[m_nCurPage].startTime.dwMinute = szSend[m_nCurRecNum - 1].stEndTime.minute;
                    m_nSearchInfo[m_nCurPage].startTime.dwSecond = szSend[m_nCurRecNum - 1].stEndTime.second;
                    m_nSearchInfo[m_nCurPage].endTime = m_nSearchInfo[0].endTime;

                    m_nGetPage++;
                    if (m_nCurRecNum < PLAYBACK_MAX_FILE_NUM)
                    {
                        m_nTotalPage = m_nGetPage;
                    }
                    
                   

                    AddFileListInfo(m_nCurRecNum);

                    SetPageBtnState(nNum);
                }
                else
                {
                   
                }
            }
        }
        
        private void DownloadByName() 
        {

            if (m_DownLoadFileHandle > 0)
	        {
                timerDownload.Stop();
                NetSDK.H264_DVR_StopGetFile(m_DownLoadFileHandle);
                m_DownLoadFileHandle = 0;
		        progressBarDownloadPos.Value = 0;
        		
		        btnDownload.Text = "Download";
        		
		        return;
	        }
        	

	        H264_DVR_FILE_DATA FileInfo = (H264_DVR_FILE_DATA)listFile.FocusedItem.Tag;
	      
        	
	        string strSaveName;
	        //strSaveName.Format("c:\\record\\record.h264");
	        int nSelectedIndex = comboDev.SelectedIndex;
	        if ( nSelectedIndex < 0)
	        {
		        MessageBox.Show("SelectDevice !");
		        return;
	        }
            DEV_INFO DevTemp = (DEV_INFO)((ComboxItem)comboDev.SelectedItem).Value;
        	
	        string strInitDir;

            strInitDir = Application.StartupPath;
	        if (strInitDir.EndsWith("\\")) 
	        {
		        strInitDir += "Download";
	        }
	        else
	        {
		        strInitDir += "\\Download";
	        }

            DirectoryInfo dir = new DirectoryInfo(strInitDir);

	        //if ( dir.Attributes ) 
	        {
		        dir.Create();
	        }
        	
	        string strFileTpye = "h264";
            string strFileName = FileInfo.sFileName;
	        if ( strFileName.EndsWith(".h264") )
	        {
		        strFileTpye = "h264";
	        }
	        else
	        {
		        strFileTpye = "jpg";
	        }
	        strFileName = string.Format("{0}_{1}_{2}-{3}_{4}{5}{6}{7}.{8}",
                DevTemp.szDevName, FileInfo.ch + 1, 
		        FileInfo.stBeginTime.year, FileInfo.stBeginTime.month,
		        FileInfo.stBeginTime.day, FileInfo.stBeginTime.hour,
		        FileInfo.stBeginTime.minute, FileInfo.stBeginTime.second, strFileTpye);

            SaveFileDialog dlg = new SaveFileDialog();
            dlg.AddExtension = true;
            dlg.CheckPathExists = true;
            dlg.DefaultExt = ".h264";
            dlg.Filter = "All File(*.h264;*.jpg)|*.*||";
            dlg.InitialDirectory = strInitDir;		//默认路径

            dlg.FileName = strFileName;
            int count = listFile.SelectedItems[0].Index;
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                
		        strSaveName = dlg.FileName;
		        //m_lSaveHandle = H264_DVR_GetFileByName(m_lLoginID, pstrFileInfo,strSaveName.GetBuffer(0),SDKDownLoadPosCallback,(DWORD)this);
                m_DownLoadFileHandle = NetSDK.H264_DVR_GetFileByName(m_lLoginID, ref FileInfo, strSaveName, null, 0, null);
                if (m_DownLoadFileHandle >= 0)
		        {
			        progressBarDownloadPos.Value = 0;
			        btnDownload.Text = "Stop";
			        //update the progress control
                    //SetTimer(1,0,NULL);
		        }
		        else
		        {
		            string sTemp =  string.Format("dowload fail ERROR = {0}",NetSDK.H264_DVR_GetLastError());
                    MessageBox.Show(sTemp);
		        }
            }
        }

        private void DownloadByTime() 
        {

            if (m_DownLoadFileHandle > 0)
	        {
                timerDownload.Stop();
                NetSDK.H264_DVR_StopGetFile(m_DownLoadFileHandle);
                m_DownLoadFileHandle = 0;
                progressBarDownloadPos.Value = 0;
        		
		        btnDownload.Text = "Download";
        		
		        return;
	        }
        	
	        string strSaveName = "";
	        //strSaveName.Format("c:\\record");
	        strSaveName = BrowseForFolder(this.Handle) ;
        	
	        if ( strSaveName == "")  //未选择路径时退出
	        {
		        return;
	        }

	        H264_DVR_FINDINFO info = new H264_DVR_FINDINFO();
        
	        info.nChannelN0 = comboChannel.SelectedIndex;	//channel No.
	        info.nFileType = comboRecordType.SelectedIndex;		//file type
	        info.startTime.dwYear = beginDate.Value.Year;
            info.startTime.dwMonth = beginDate.Value.Month;
            info.startTime.dwDay = beginDate.Value.Day;
	        info.startTime.dwHour = beginTime.Value.Hour;
            info.startTime.dwMinute = beginTime.Value.Minute;
            info.startTime.dwSecond = beginTime.Value.Second;
	        info.endTime.dwYear = endDate.Value.Year;
            info.endTime.dwMonth = endDate.Value.Month;
	        info.endTime.dwDay = endDate.Value.Day;
	        info.endTime.dwHour = endTime.Value.Hour;
            info.endTime.dwMinute = endTime.Value.Minute;
            info.endTime.dwSecond = endTime.Value.Second;
	        int nRecordFileType = comboRecordType.SelectedIndex;
            info.nFileType = (nRecordFileType <= (int)PlayBackFileType.SDK_RECORD_MANUAL) ? nRecordFileType : ((int)PlayBackFileType.SDK_PIC_ALL + nRecordFileType - (int)PlayBackFileType.SDK_RECORD_MANUAL - 1);

            m_DownLoadFileHandle = NetSDK.H264_DVR_GetFileByTime(m_lLoginID, ref info, strSaveName, false, null, 0, null);
            if (m_DownLoadFileHandle > 0)
	        {
              progressBarDownloadPos.Value = 0;
        		
		      btnDownload.Text = "Stop";
        		
		        //update the progress control
              timerDownload.Start();
	        }
	        else
	        {
		        string sTemp;
		        sTemp = string.Format("dowload fail ERROR = {0}",NetSDK.H264_DVR_GetLastError());
                MessageBox.Show(sTemp);
	        }
        }

        private string BrowseForFolder(IntPtr ptrWnd)
        {
            FolderBrowserDialog folderDlg = new FolderBrowserDialog();
            folderDlg.ShowNewFolderButton = true;
            folderDlg.ShowDialog();
            
            return folderDlg.SelectedPath;
        }
        private void comboChannel_SelectedIndexChanged(object sender, EventArgs e)
        {
            //DEV_INFO devInfo = new DEV_INFO();
            //ListViewItem selecteditem = (ListViewItem)comboDev.SelectedItem;

            //devInfo = (DEV_INFO)selecteditem.Tag;

            //for (int i = 0; i < devInfo.NetDeviceInfo.byChanNum + devInfo.NetDeviceInfo.iDigChannel; i++)
            //{
            //    comboChannel.Items.Add(i + 1);
            //}
            //comboChannel.SelectedIndex = 0;
        }

        private void btnNetPlayStop_Click(object sender, EventArgs e)
        {
            timerNetPlayBack.Enabled = false;
            if (m_nNetPlayHandle >= 0)
            {
                NetSDK.H264_DVR_StopPlayBack(m_nNetPlayHandle);
                m_nNetPlayHandle = 0;
            }
            trackBarNetPlayPos.Value = 0;
            m_bOpenAudio = false;
            m_bPauseNetPlay = false;    
        }

        private void btnNetPlay_Click(object sender, EventArgs e)
        {
            int nSelectedIndex = comboDev.SelectedIndex;
            if (nSelectedIndex < 0)
            {
                MessageBox.Show("SelectDevice !");
                return;
            }
            timerNetPlayBack.Elapsed += new System.Timers.ElapsedEventHandler(recordTime);
            timerNetPlayBack.AutoReset = true;
            timerNetPlayBack.Enabled = true;

            if (m_bDownloadByTime)
            {
                PlayByTime();
            }
            else
            {
                PlayByName();
            }
        }

        private void btnNetCatchImg_Click(object sender, EventArgs e)
        {
            if (m_nNetPlayHandle <= 0)
            {
                return;
            }

            String strPath;
            DateTime dt = System.DateTime.Now;
            int y = dt.Year;
            int m = dt.Month;
            int d = dt.Day;
            int h = dt.Hour;
            int min = dt.Minute;
            int s = dt.Second;
            strPath = String.Format(".\\Pictures\\{0:D4}{1:D2}{2:D2}{3:D2}{4:D2}{5:D2}.bmp", y, m, d, h, min, s);
            bool bCatch = NetSDK.H264_DVR_LocalCatchPic(m_nNetPlayHandle, strPath);
            if (bCatch)
            {
                System.Diagnostics.Process.Start(strPath);
            }
            else
            {
                MessageBox.Show("Catch Picture error !");
            }
        }

        
        private void  PlayByTime() 
        {
           
            if (m_nNetPlayHandle >= 0)
            {
                NetSDK.H264_DVR_StopPlayBack(m_nNetPlayHandle);
                m_nNetPlayHandle = 0;
            }
            trackBarNetPlayPos.Value = 0;
            m_bOpenAudio = false;
            m_bPauseNetPlay = false;  


            m_nFastTypeNet = 0;
            m_nSlowTypeNet = 0;

	        if(m_nNetPlayHandle == 0)
	        {
		        H264_DVR_FINDINFO info = new H264_DVR_FINDINFO();
        		
		        info.nChannelN0 = comboChannel.SelectedIndex;	//channel No.
		        info.nFileType = comboRecordType.SelectedIndex;		//file type
                info.startTime.dwYear = beginDate.Value.Year;
                info.startTime.dwMonth = beginDate.Value.Month;
                info.startTime.dwDay = beginDate.Value.Day;

                info.startTime.dwHour = beginTime.Value.Hour;
                info.startTime.dwMinute = beginTime.Value.Minute;
                info.startTime.dwSecond = beginTime.Value.Second;
		        info.endTime.dwYear = endDate.Value.Year;
                info.endTime.dwMonth = endDate.Value.Month;
                info.endTime.dwDay = endDate.Value.Day;
                info.endTime.dwHour = endTime.Value.Hour;
                info.endTime.dwMinute = endTime.Value.Minute;
                info.endTime.dwSecond = endTime.Value.Second;
                info.hWnd = (uint)pictureBoxNetVideoWnd.Handle;

                NetSDK.fDownLoadPosCallBack DownloadCallback = new NetSDK.fDownLoadPosCallBack(DownLoadPosCallback);
                NetSDK.fRealDataCallBack realDataCallBack = new NetSDK.fRealDataCallBack(RealDataCallBack);

                m_nNetPlayHandle = NetSDK.H264_DVR_PlayBackByTimeEx(m_lLoginID, ref info, null, this.Handle.ToInt32(),
                    null, this.Handle.ToInt32());

                if (m_nNetPlayHandle <= 0)
                {
                    MessageBox.Show("Playback fail");
                    m_nNetPlayHandle = 0;
                    return;
                }
                timerNetPlayBack.Start();
	        }
	        else
	        {

                if (m_bPauseNetPlay)
		        {
			        NetSDK.H264_DVR_PlayBackControl(m_nNetPlayHandle, (int)PlayBackAction.SDK_PLAY_BACK_CONTINUE, 0 );
                    m_bPauseNetPlay = !m_bPauseNetPlay;
		        }
                NetSDK.H264_DVR_PlayBackControl(m_nNetPlayHandle, (int)PlayBackAction.SDK_PLAY_BACK_FAST, 0);
                m_nFastTypeNet = 0;
                m_nSlowTypeNet = 0;
	        }
        }
        private void PlayByName() 
        {
            if (m_nNetPlayHandle >= 0)
            {
                timerNetPlayBack.Enabled = false;
                NetSDK.H264_DVR_StopPlayBack(m_nNetPlayHandle);
                m_nNetPlayHandle = 0;
                trackBarNetPlayPos.Value = 0;
                m_bOpenAudio = false;
                m_bPauseNetPlay = false;  
            }
          

            m_nFastTypeNet = 0;
            m_nSlowTypeNet = 0;
	        if(m_nNetPlayHandle == 0)
	        {
		        int nSelected = listFile.SelectedItems.Count;
		        if ( nSelected == 0 )
		        {
			        MessageBox.Show("SelectFile");
			        return;
		        }
        		
		        H264_DVR_FILE_DATA FileInfo = (H264_DVR_FILE_DATA)listFile.FocusedItem.Tag;
                FileInfo.hWnd = (IntPtr)pictureBoxNetVideoWnd.Handle;


                NetSDK.fDownLoadPosCallBack downloadCallBack = new NetSDK.fDownLoadPosCallBack(DownLoadPosCallback);
                NetSDK.fRealDataCallBack_V2 netdatacallbackv2 = new NetSDK.fRealDataCallBack_V2(NetCallBack_V2);

                m_nNetPlayHandle = NetSDK.H264_DVR_PlayBackByName_V2(m_lLoginID, ref  FileInfo, downloadCallBack, netdatacallbackv2, this.Handle.ToInt32());
		        if(m_nNetPlayHandle <= 0 )
		        {
			        MessageBox.Show("Playback fail");
			        return;
		        }
                timerNetPlayBack.Start();
	        }
	        else
	        {
                if (m_bPauseNetPlay)
		        {
			        NetSDK.H264_DVR_PlayBackControl(m_nNetPlayHandle, (int)PlayBackAction.SDK_PLAY_BACK_CONTINUE, 0 );//暂停恢复正常		
                    m_bPauseNetPlay = !m_bPauseNetPlay;
		        }
                NetSDK.H264_DVR_PlayBackControl(m_nNetPlayHandle, (int)PlayBackAction.SDK_PLAY_BACK_FAST, 0);	//快慢放恢复正常	
                m_nFastTypeNet = 0;
                m_nSlowTypeNet = 0;
	        }
        }

        private void btnNetFast_Click(object sender, EventArgs e)
        {
            if (++m_nFastTypeNet > 4)
            {
                m_nFastTypeNet = 1;
            }
            m_nSlowTypeNet = 0;
            NetSDK.H264_DVR_PlayBackControl(m_nNetPlayHandle, (int)PlayBackAction.SDK_PLAY_BACK_FAST, m_nFastTypeNet);
        }

        private void btnNetSlow_Click(object sender, EventArgs e)
        {
            if (++m_nSlowTypeNet > 4)
            {
                m_nSlowTypeNet = 1;
            }
            m_nFastTypeNet = 0;
            NetSDK.H264_DVR_PlayBackControl(m_nNetPlayHandle, (int)PlayBackAction.SDK_PLAY_BACK_SLOW, m_nSlowTypeNet);
        }

        private void btnNetPause_Click(object sender, EventArgs e)
        {
            if (!m_bPauseNetPlay)
                NetSDK.H264_DVR_PlayBackControl(m_nNetPlayHandle, (int)PlayBackAction.SDK_PLAY_BACK_PAUSE, 0);
            else
                NetSDK.H264_DVR_PlayBackControl(m_nNetPlayHandle, (int)PlayBackAction.SDK_PLAY_BACK_CONTINUE, 0);
            m_bPauseNetPlay = !m_bPauseNetPlay;
        }

        private void PlayBackForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (m_nNetPlayHandle > 0 )
            {
                NetSDK.H264_DVR_StopPlayBack(m_nNetPlayHandle);
                m_nNetPlayHandle = -1;
            }


            ClearResult();
            //ClearMapResult();
            timerLocalPlayBack.Stop();
            timerDownload.Stop();
            timerNetPlayBack.Stop();
        }

        private void ClearResult()
        {
            listFile.Items.Clear();
            //comboChannel.Items.Clear();
           // m_mapTemp.clear();
            m_ListFindNum = 0;
	        //m_pstrFileVector.clear();
        }

        private void btnOpenAudio_Click(object sender, EventArgs e)
        {
            if (m_nNetPlayHandle > 0)
            {
                if (!m_bOpenAudio)
                {
                    if (NetSDK.H264_DVR_OpenSound(m_nNetPlayHandle))
                    {

                        btnOpenAudio.Text = "CloseAudio";
                        m_bOpenAudio = true;
                    }
                }
                else
                {
                    if (NetSDK.H264_DVR_CloseSound(m_nNetPlayHandle))
                    {
                       btnOpenAudio.Text = "OpenAudio";
                        m_bOpenAudio = false;
                    }
                }


            }

        }

        private void trackBarPos_DragDrop(object sender, DragEventArgs e)
        {
            m_SliderPos = trackBarNetPlayPos.Value;
            m_PreTime = System.DateTime.Now.Ticks;
            NetSDK.H264_DVR_SetPlayPos(m_nNetPlayHandle, m_SliderPos / (float)1000.0);
        }
        
         private void btn_Drag_Scroll(object sender, EventArgs e)
         {
             if (!m_bDownloadByTime)
             {
                 m_SliderPos = trackBarNetPlayPos.Value;
                 m_PreTime = System.DateTime.Now.Ticks;

                 timerNetPlayBack.Stop();
                 NetSDK.H264_DVR_SetPlayPos(m_nNetPlayHandle, m_SliderPos / (float)1000.0);
                 Thread.Sleep(200);
                 timerNetPlayBack.Start();
             }
         }
        private void SetSliderPos(int nplaypos)
        {
	        if ( m_PreTime > 0 )
	        {
                long nTime = System.DateTime.Now.Ticks;
                if ((nTime - m_PreTime) >= 2000)
		        {
			        trackBarLocalPlayPos.Value= nplaypos * 1000;
        			
			        m_PreTime = 0;
		        }
	        }
	        else
	        {
		         trackBarLocalPlayPos.Value = nplaypos * 1000;
	        }
        }

        private void comboDev_SelectedIndexChanged(object sender, EventArgs e)
        {
            DEV_INFO devInfo = new DEV_INFO();
            ComboxItem selecteditem = (ComboxItem)comboDev.SelectedItem;

            devInfo = (DEV_INFO)selecteditem.Value;

            for (int i = 0; i < devInfo.NetDeviceInfo.byChanNum + devInfo.NetDeviceInfo.iDigChannel; i++)
            {
                comboChannel.Items.Add(i + 1);
            }
            comboChannel.SelectedIndex = 0;
            m_lLoginID = devInfo.lLoginID;

        }

        private void listFile_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void listFile_DoubleClick(object sender, EventArgs e)
        {
            btnNetPlay_Click(null,null);
        }

    }
}