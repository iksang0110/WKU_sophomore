namespace WEEK06_01
{
    partial class Form1
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.bt_Modal = new System.Windows.Forms.Button();
            this.bt_Modeless = new System.Windows.Forms.Button();
            this.bt_FileOpen = new System.Windows.Forms.Button();
            this.bt_FlieSave = new System.Windows.Forms.Button();
            this.bt_Fornt = new System.Windows.Forms.Button();
            this.bt_Color = new System.Windows.Forms.Button();
            this.bt_Print = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.fontDialog1 = new System.Windows.Forms.FontDialog();
            this.colorDialog1 = new System.Windows.Forms.ColorDialog();
            this.printDialog1 = new System.Windows.Forms.PrintDialog();
            this.SuspendLayout();
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(15, 13);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(310, 135);
            this.textBox1.TabIndex = 0;
            // 
            // bt_Modal
            // 
            this.bt_Modal.Location = new System.Drawing.Point(15, 154);
            this.bt_Modal.Name = "bt_Modal";
            this.bt_Modal.Size = new System.Drawing.Size(143, 59);
            this.bt_Modal.TabIndex = 1;
            this.bt_Modal.Text = "모달(Modal)대화상자 열기";
            this.bt_Modal.UseVisualStyleBackColor = true;
            this.bt_Modal.Click += new System.EventHandler(this.bt_Modal_Click);
            // 
            // bt_Modeless
            // 
            this.bt_Modeless.Location = new System.Drawing.Point(182, 154);
            this.bt_Modeless.Name = "bt_Modeless";
            this.bt_Modeless.Size = new System.Drawing.Size(143, 59);
            this.bt_Modeless.TabIndex = 2;
            this.bt_Modeless.Text = "모달리스(Modeless)대화상자 열기";
            this.bt_Modeless.UseVisualStyleBackColor = true;
            this.bt_Modeless.Click += new System.EventHandler(this.bt_Modeless_Click);
            // 
            // bt_FileOpen
            // 
            this.bt_FileOpen.Location = new System.Drawing.Point(15, 220);
            this.bt_FileOpen.Name = "bt_FileOpen";
            this.bt_FileOpen.Size = new System.Drawing.Size(310, 42);
            this.bt_FileOpen.TabIndex = 3;
            this.bt_FileOpen.Text = "Txt 파일 열기";
            this.bt_FileOpen.UseVisualStyleBackColor = true;
            this.bt_FileOpen.Click += new System.EventHandler(this.bt_FileOpen_Click);
            // 
            // bt_FlieSave
            // 
            this.bt_FlieSave.Location = new System.Drawing.Point(15, 268);
            this.bt_FlieSave.Name = "bt_FlieSave";
            this.bt_FlieSave.Size = new System.Drawing.Size(310, 42);
            this.bt_FlieSave.TabIndex = 4;
            this.bt_FlieSave.Text = "파일 저장 하기";
            this.bt_FlieSave.UseVisualStyleBackColor = true;
            this.bt_FlieSave.Click += new System.EventHandler(this.bt_FlieSave_Click);
            // 
            // bt_Fornt
            // 
            this.bt_Fornt.Location = new System.Drawing.Point(15, 316);
            this.bt_Fornt.Name = "bt_Fornt";
            this.bt_Fornt.Size = new System.Drawing.Size(310, 42);
            this.bt_Fornt.TabIndex = 5;
            this.bt_Fornt.Text = "글꼴 변경하기";
            this.bt_Fornt.UseVisualStyleBackColor = true;
            this.bt_Fornt.Click += new System.EventHandler(this.bt_Fornt_Click);
            // 
            // bt_Color
            // 
            this.bt_Color.Location = new System.Drawing.Point(15, 364);
            this.bt_Color.Name = "bt_Color";
            this.bt_Color.Size = new System.Drawing.Size(310, 42);
            this.bt_Color.TabIndex = 6;
            this.bt_Color.Text = "텍스트 박스 색상 변경하기";
            this.bt_Color.UseVisualStyleBackColor = true;
            this.bt_Color.Click += new System.EventHandler(this.bt_Color_Click);
            // 
            // bt_Print
            // 
            this.bt_Print.Location = new System.Drawing.Point(15, 412);
            this.bt_Print.Name = "bt_Print";
            this.bt_Print.Size = new System.Drawing.Size(310, 42);
            this.bt_Print.TabIndex = 7;
            this.bt_Print.Text = "텍스트 박스 내용 출력하기";
            this.bt_Print.UseVisualStyleBackColor = true;
            this.bt_Print.Click += new System.EventHandler(this.bt_Print_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 468);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(38, 12);
            this.label1.TabIndex = 8;
            this.label1.Text = "label1";
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // fontDialog1
            // 
            this.fontDialog1.ShowColor = true;
            this.fontDialog1.Apply += new System.EventHandler(this.fontDialog1_Apply);
            // 
            // printDialog1
            // 
            this.printDialog1.UseEXDialog = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(327, 544);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.bt_Print);
            this.Controls.Add(this.bt_Color);
            this.Controls.Add(this.bt_Fornt);
            this.Controls.Add(this.bt_FlieSave);
            this.Controls.Add(this.bt_FileOpen);
            this.Controls.Add(this.bt_Modeless);
            this.Controls.Add(this.bt_Modal);
            this.Controls.Add(this.textBox1);
            this.Name = "Form1";
            this.Text = "WEEk06_01";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button bt_Modal;
        private System.Windows.Forms.Button bt_Modeless;
        private System.Windows.Forms.Button bt_FileOpen;
        private System.Windows.Forms.Button bt_FlieSave;
        private System.Windows.Forms.Button bt_Fornt;
        private System.Windows.Forms.Button bt_Color;
        private System.Windows.Forms.Button bt_Print;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.FontDialog fontDialog1;
        private System.Windows.Forms.ColorDialog colorDialog1;
        private System.Windows.Forms.PrintDialog printDialog1;
    }
}

