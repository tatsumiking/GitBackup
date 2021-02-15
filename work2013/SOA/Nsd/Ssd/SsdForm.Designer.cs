namespace Ssd
{
    partial class SsdForm
    {
        /// <summary>
        /// 必要なデザイナ変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナで生成されたコード

        /// <summary>
        /// デザイナ サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディタで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SsdForm));
            this.textSpan = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.btnSpan = new System.Windows.Forms.Button();
            this.btnEnvSet = new System.Windows.Forms.Button();
            this.btnEnd = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.lstSend = new System.Windows.Forms.ListBox();
            this.label3 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // textSpan
            // 
            this.textSpan.Location = new System.Drawing.Point(78, 12);
            this.textSpan.Name = "textSpan";
            this.textSpan.Size = new System.Drawing.Size(46, 19);
            this.textSpan.TabIndex = 0;
            this.textSpan.TextChanged += new System.EventHandler(this.textSpan_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(60, 12);
            this.label1.TabIndex = 1;
            this.label1.Text = "チェック間隔";
            // 
            // btnSpan
            // 
            this.btnSpan.Location = new System.Drawing.Point(156, 10);
            this.btnSpan.Name = "btnSpan";
            this.btnSpan.Size = new System.Drawing.Size(75, 23);
            this.btnSpan.TabIndex = 2;
            this.btnSpan.Text = "設定変更";
            this.btnSpan.UseVisualStyleBackColor = true;
            this.btnSpan.Click += new System.EventHandler(this.btnSpan_Click);
            // 
            // btnEnvSet
            // 
            this.btnEnvSet.Location = new System.Drawing.Point(70, 44);
            this.btnEnvSet.Name = "btnEnvSet";
            this.btnEnvSet.Size = new System.Drawing.Size(75, 23);
            this.btnEnvSet.TabIndex = 3;
            this.btnEnvSet.Text = "環境設定";
            this.btnEnvSet.UseVisualStyleBackColor = true;
            this.btnEnvSet.Click += new System.EventHandler(this.btnEnvSet_Click);
            // 
            // btnEnd
            // 
            this.btnEnd.Location = new System.Drawing.Point(156, 44);
            this.btnEnd.Name = "btnEnd";
            this.btnEnd.Size = new System.Drawing.Size(75, 23);
            this.btnEnd.TabIndex = 4;
            this.btnEnd.Text = "終了";
            this.btnEnd.UseVisualStyleBackColor = true;
            this.btnEnd.Click += new System.EventHandler(this.btnEnd_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(128, 19);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(17, 12);
            this.label2.TabIndex = 5;
            this.label2.Text = "秒";
            // 
            // timer1
            // 
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // lstSend
            // 
            this.lstSend.FormattingEnabled = true;
            this.lstSend.ItemHeight = 12;
            this.lstSend.Location = new System.Drawing.Point(13, 79);
            this.lstSend.Name = "lstSend";
            this.lstSend.Size = new System.Drawing.Size(218, 352);
            this.lstSend.TabIndex = 6;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(10, 50);
            this.label3.Name = "version";
            this.label3.Size = new System.Drawing.Size(40, 12);
            this.label3.TabIndex = 7;
            this.label3.Text = "Ver1.6.0.0";
            // 
            // SsdForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(243, 437);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.lstSend);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.btnEnd);
            this.Controls.Add(this.btnEnvSet);
            this.Controls.Add(this.btnSpan);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textSpan);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "SsdForm";
            this.Text = "名札転送";
            this.Load += new System.EventHandler(this.SsdForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textSpan;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnSpan;
        private System.Windows.Forms.Button btnEnvSet;
        private System.Windows.Forms.Button btnEnd;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.ListBox lstSend;
        private System.Windows.Forms.Label label3;
    }
}

