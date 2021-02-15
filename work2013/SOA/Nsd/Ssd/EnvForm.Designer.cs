namespace Ssd
{
    partial class EnvForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EnvForm));
            this.btnSave = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.textDefAddUrl = new System.Windows.Forms.TextBox();
            this.textDefPath = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.listUser = new System.Windows.Forms.ListBox();
            this.textUser = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnAdd = new System.Windows.Forms.Button();
            this.btnDelete = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(216, 261);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(64, 23);
            this.btnSave.TabIndex = 5;
            this.btnSave.Text = "適用";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(51, 12);
            this.label1.TabIndex = 6;
            this.label1.Text = "保存URL";
            // 
            // textDefAddUrl
            // 
            this.textDefAddUrl.Location = new System.Drawing.Point(12, 21);
            this.textDefAddUrl.Name = "textDefAddUrl";
            this.textDefAddUrl.Size = new System.Drawing.Size(268, 19);
            this.textDefAddUrl.TabIndex = 7;
            // 
            // textDefPath
            // 
            this.textDefPath.Location = new System.Drawing.Point(12, 58);
            this.textDefPath.Name = "textDefPath";
            this.textDefPath.Size = new System.Drawing.Size(268, 19);
            this.textDefPath.TabIndex = 9;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 43);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(98, 12);
            this.label2.TabIndex = 8;
            this.label2.Text = "ファイル格納フォルダ";
            // 
            // listUser
            // 
            this.listUser.FormattingEnabled = true;
            this.listUser.ItemHeight = 12;
            this.listUser.Location = new System.Drawing.Point(12, 94);
            this.listUser.Name = "listUser";
            this.listUser.Size = new System.Drawing.Size(198, 172);
            this.listUser.TabIndex = 10;
            this.listUser.SelectedIndexChanged += new System.EventHandler(this.listUser_SelectedIndexChanged);
            // 
            // textUser
            // 
            this.textUser.Location = new System.Drawing.Point(12, 290);
            this.textUser.Name = "textUser";
            this.textUser.Size = new System.Drawing.Size(198, 19);
            this.textUser.TabIndex = 12;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(15, 275);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(57, 12);
            this.label3.TabIndex = 11;
            this.label3.Text = "ユーザー名";
            // 
            // btnCancel
            // 
            this.btnCancel.Location = new System.Drawing.Point(216, 290);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(64, 23);
            this.btnCancel.TabIndex = 13;
            this.btnCancel.Text = "キャンセル";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // btnAdd
            // 
            this.btnAdd.Location = new System.Drawing.Point(216, 94);
            this.btnAdd.Name = "btnAdd";
            this.btnAdd.Size = new System.Drawing.Size(64, 23);
            this.btnAdd.TabIndex = 14;
            this.btnAdd.Text = "追加";
            this.btnAdd.UseVisualStyleBackColor = true;
            this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
            // 
            // btnDelete
            // 
            this.btnDelete.Location = new System.Drawing.Point(216, 123);
            this.btnDelete.Name = "btnDelete";
            this.btnDelete.Size = new System.Drawing.Size(64, 23);
            this.btnDelete.TabIndex = 15;
            this.btnDelete.Text = "削除";
            this.btnDelete.UseVisualStyleBackColor = true;
            this.btnDelete.Click += new System.EventHandler(this.btnDelete_Click);
            // 
            // EnvForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 352);
            this.Controls.Add(this.btnDelete);
            this.Controls.Add(this.btnAdd);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.textUser);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.listUser);
            this.Controls.Add(this.textDefPath);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textDefAddUrl);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnSave);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "EnvForm";
            this.Text = "環境設定";
            this.Load += new System.EventHandler(this.EnvForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textDefAddUrl;
        private System.Windows.Forms.TextBox textDefPath;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ListBox listUser;
        private System.Windows.Forms.TextBox textUser;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnAdd;
        private System.Windows.Forms.Button btnDelete;
    }
}