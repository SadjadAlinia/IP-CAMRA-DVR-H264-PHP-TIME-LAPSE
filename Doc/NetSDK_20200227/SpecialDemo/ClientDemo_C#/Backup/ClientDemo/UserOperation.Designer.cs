namespace ClientDemo
{
    partial class UserOperation
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
            this.label1 = new System.Windows.Forms.Label();
            this.m_tbUser = new System.Windows.Forms.TextBox();
            this.m_cbResue = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.m_tbMemo = new System.Windows.Forms.TextBox();
            this.m_comGroup = new System.Windows.Forms.ComboBox();
            this.labelGroup = new System.Windows.Forms.Label();
            this.btnOK = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.m_userRightList = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.label3 = new System.Windows.Forms.Label();
            this.m_tbPwd = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 19);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(59, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "User Name";
            // 
            // m_tbUser
            // 
            this.m_tbUser.Location = new System.Drawing.Point(85, 16);
            this.m_tbUser.Name = "m_tbUser";
            this.m_tbUser.Size = new System.Drawing.Size(118, 21);
            this.m_tbUser.TabIndex = 0;
            // 
            // m_cbResue
            // 
            this.m_cbResue.AutoSize = true;
            this.m_cbResue.Location = new System.Drawing.Point(227, 21);
            this.m_cbResue.Name = "m_cbResue";
            this.m_cbResue.Size = new System.Drawing.Size(78, 16);
            this.m_cbResue.TabIndex = 1;
            this.m_cbResue.Text = "Resueable";
            this.m_cbResue.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 85);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(29, 12);
            this.label2.TabIndex = 3;
            this.label2.Text = "Memo";
            // 
            // m_tbMemo
            // 
            this.m_tbMemo.Location = new System.Drawing.Point(85, 82);
            this.m_tbMemo.Name = "m_tbMemo";
            this.m_tbMemo.Size = new System.Drawing.Size(118, 21);
            this.m_tbMemo.TabIndex = 3;
            // 
            // m_comGroup
            // 
            this.m_comGroup.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_comGroup.FormattingEnabled = true;
            this.m_comGroup.Location = new System.Drawing.Point(82, 109);
            this.m_comGroup.Name = "m_comGroup";
            this.m_comGroup.Size = new System.Drawing.Size(121, 20);
            this.m_comGroup.TabIndex = 4;
            this.m_comGroup.SelectedIndexChanged += new System.EventHandler(this.m_comGroup_SelectedIndexChanged);
            // 
            // labelGroup
            // 
            this.labelGroup.AutoSize = true;
            this.labelGroup.Location = new System.Drawing.Point(12, 117);
            this.labelGroup.Name = "labelGroup";
            this.labelGroup.Size = new System.Drawing.Size(35, 12);
            this.labelGroup.TabIndex = 3;
            this.labelGroup.Text = "Group";
            // 
            // btnOK
            // 
            this.btnOK.Location = new System.Drawing.Point(14, 493);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(132, 23);
            this.btnOK.TabIndex = 6;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.Location = new System.Drawing.Point(169, 493);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(132, 23);
            this.btnCancel.TabIndex = 6;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // m_userRightList
            // 
            this.m_userRightList.CheckBoxes = true;
            this.m_userRightList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1});
            this.m_userRightList.Location = new System.Drawing.Point(14, 162);
            this.m_userRightList.Name = "m_userRightList";
            this.m_userRightList.Size = new System.Drawing.Size(294, 325);
            this.m_userRightList.TabIndex = 7;
            this.m_userRightList.UseCompatibleStateImageBehavior = false;
            this.m_userRightList.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "User right";
            this.columnHeader1.Width = 160;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 55);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(53, 12);
            this.label3.TabIndex = 8;
            this.label3.Text = "Password";
            // 
            // m_tbPwd
            // 
            this.m_tbPwd.Location = new System.Drawing.Point(85, 46);
            this.m_tbPwd.Name = "m_tbPwd";
            this.m_tbPwd.Size = new System.Drawing.Size(118, 21);
            this.m_tbPwd.TabIndex = 2;
            // 
            // UserOperation
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(320, 526);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.m_userRightList);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.m_comGroup);
            this.Controls.Add(this.labelGroup);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.m_cbResue);
            this.Controls.Add(this.m_tbMemo);
            this.Controls.Add(this.m_tbPwd);
            this.Controls.Add(this.m_tbUser);
            this.Controls.Add(this.label1);
            this.Name = "UserOperation";
            this.Text = "UserOperation";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox m_tbUser;
        private System.Windows.Forms.CheckBox m_cbResue;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox m_tbMemo;
        private System.Windows.Forms.ComboBox m_comGroup;
        private System.Windows.Forms.Label labelGroup;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.ListView m_userRightList;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox m_tbPwd;
    }
}