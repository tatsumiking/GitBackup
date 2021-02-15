namespace SSD
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
            this.textSpan = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.btnSpan = new System.Windows.Forms.Button();
            this.ｂｔｎEnvSet = new System.Windows.Forms.Button();
            this.ｂｔｎEnd = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // textSpan
            // 
            this.textSpan.Location = new System.Drawing.Point(78, 12);
            this.textSpan.Name = "textSpan";
            this.textSpan.Size = new System.Drawing.Size(67, 19);
            this.textSpan.TabIndex = 0;
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
            // ｂｔｎEnvSet
            // 
            this.ｂｔｎEnvSet.Location = new System.Drawing.Point(70, 44);
            this.ｂｔｎEnvSet.Name = "ｂｔｎEnvSet";
            this.ｂｔｎEnvSet.Size = new System.Drawing.Size(75, 23);
            this.ｂｔｎEnvSet.TabIndex = 3;
            this.ｂｔｎEnvSet.Text = "環境設定";
            this.ｂｔｎEnvSet.UseVisualStyleBackColor = true;
            // 
            // ｂｔｎEnd
            // 
            this.ｂｔｎEnd.Location = new System.Drawing.Point(156, 44);
            this.ｂｔｎEnd.Name = "ｂｔｎEnd";
            this.ｂｔｎEnd.Size = new System.Drawing.Size(75, 23);
            this.ｂｔｎEnd.TabIndex = 4;
            this.ｂｔｎEnd.Text = "終了";
            this.ｂｔｎEnd.UseVisualStyleBackColor = true;
            // 
            // SsdForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(243, 80);
            this.Controls.Add(this.ｂｔｎEnd);
            this.Controls.Add(this.ｂｔｎEnvSet);
            this.Controls.Add(this.btnSpan);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textSpan);
            this.Name = "SsdForm";
            this.Text = "名札転送";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textSpan;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnSpan;
        private System.Windows.Forms.Button ｂｔｎEnvSet;
        private System.Windows.Forms.Button ｂｔｎEnd;
    }
}

