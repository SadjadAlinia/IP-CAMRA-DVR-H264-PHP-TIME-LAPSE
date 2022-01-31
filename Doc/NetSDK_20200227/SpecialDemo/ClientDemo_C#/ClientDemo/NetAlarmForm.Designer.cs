namespace ClientDemo
{
    partial class NetAlarmForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.comboBoxDevice = new System.Windows.Forms.ComboBox();
            this.m_btnStart = new System.Windows.Forms.Button();
            this.m_btnStop = new System.Windows.Forms.Button();
            this.m_alarmList = new System.Windows.Forms.ListView();
            this.NO = new System.Windows.Forms.ColumnHeader();
            this.Device = new System.Windows.Forms.ColumnHeader();
            this.Channel = new System.Windows.Forms.ColumnHeader();
            this.AlarmType = new System.Windows.Forms.ColumnHeader();
            this.State = new System.Windows.Forms.ColumnHeader();
            this.Time = new System.Windows.Forms.ColumnHeader();
            this.SuspendLayout();
            // 
            // comboBoxDevice
            // 
            this.comboBoxDevice.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxDevice.FormattingEnabled = true;
            this.comboBoxDevice.Location = new System.Drawing.Point(12, 12);
            this.comboBoxDevice.Name = "comboBoxDevice";
            this.comboBoxDevice.Size = new System.Drawing.Size(236, 20);
            this.comboBoxDevice.TabIndex = 0;
            this.comboBoxDevice.SelectedIndexChanged += new System.EventHandler(this.comboBoxDevice_SelectedIndexChanged);
            // 
            // m_btnStart
            // 
            this.m_btnStart.Location = new System.Drawing.Point(274, 12);
            this.m_btnStart.Name = "m_btnStart";
            this.m_btnStart.Size = new System.Drawing.Size(100, 23);
            this.m_btnStart.TabIndex = 1;
            this.m_btnStart.Text = "Start";
            this.m_btnStart.UseVisualStyleBackColor = true;
            this.m_btnStart.Click += new System.EventHandler(this.StartNetAlarm_Click);
            // 
            // m_btnStop
            // 
            this.m_btnStop.Location = new System.Drawing.Point(392, 12);
            this.m_btnStop.Name = "m_btnStop";
            this.m_btnStop.Size = new System.Drawing.Size(100, 23);
            this.m_btnStop.TabIndex = 1;
            this.m_btnStop.Text = "Stop";
            this.m_btnStop.UseVisualStyleBackColor = true;
            this.m_btnStop.Click += new System.EventHandler(this.StopNetAlarm_Click);
            // 
            // m_alarmList
            // 
            this.m_alarmList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.NO,
            this.Device,
            this.Channel,
            this.AlarmType,
            this.State,
            this.Time});
            this.m_alarmList.Location = new System.Drawing.Point(12, 53);
            this.m_alarmList.Name = "m_alarmList";
            this.m_alarmList.Size = new System.Drawing.Size(557, 376);
            this.m_alarmList.TabIndex = 2;
            this.m_alarmList.UseCompatibleStateImageBehavior = false;
            this.m_alarmList.View = System.Windows.Forms.View.Details;
            // 
            // NO
            // 
            this.NO.Text = "No.";
            // 
            // Device
            // 
            this.Device.Text = "Device";
            this.Device.Width = 100;
            // 
            // Channel
            // 
            this.Channel.Text = "Channel";
            // 
            // AlarmType
            // 
            this.AlarmType.Text = "Type";
            this.AlarmType.Width = 100;
            // 
            // State
            // 
            this.State.Text = "State";
            // 
            // Time
            // 
            this.Time.Text = "Time";
            this.Time.Width = 140;
            // 
            // NetAlarmForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(581, 441);
            this.Controls.Add(this.m_alarmList);
            this.Controls.Add(this.m_btnStop);
            this.Controls.Add(this.m_btnStart);
            this.Controls.Add(this.comboBoxDevice);
            this.MaximizeBox = false;
            this.Name = "NetAlarmForm";
            this.Text = "NetAlarmForm";
            this.VisibleChanged += new System.EventHandler(this.NetAlarmForm_VisibleChanged);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.NetAlarmForm_FormClosing);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ComboBox comboBoxDevice;
        private System.Windows.Forms.Button m_btnStart;
        private System.Windows.Forms.Button m_btnStop;
        private System.Windows.Forms.ListView m_alarmList;
        private System.Windows.Forms.ColumnHeader NO;
        private System.Windows.Forms.ColumnHeader Device;
        private System.Windows.Forms.ColumnHeader Channel;
        private System.Windows.Forms.ColumnHeader AlarmType;
        private System.Windows.Forms.ColumnHeader State;
        private System.Windows.Forms.ColumnHeader Time;
    }
}