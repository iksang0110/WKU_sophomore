namespace MP3
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
            this.components = new System.ComponentModel.Container();
            this.Ib_MusicTimer = new System.Windows.Forms.Label();
            this.MP3TrackBar = new System.Windows.Forms.TrackBar();
            this.bt_Open = new System.Windows.Forms.Button();
            this.bt_Close = new System.Windows.Forms.Button();
            this.MP3Timer = new System.Windows.Forms.Timer(this.components);
            this.bt_Play = new System.Windows.Forms.Button();
            this.bt_Stop = new System.Windows.Forms.Button();
            this.bt_Pause = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.MP3TrackBar)).BeginInit();
            this.SuspendLayout();
            // 
            // Ib_MusicTimer
            // 
            this.Ib_MusicTimer.AutoSize = true;
            this.Ib_MusicTimer.Location = new System.Drawing.Point(262, 9);
            this.Ib_MusicTimer.Name = "Ib_MusicTimer";
            this.Ib_MusicTimer.Size = new System.Drawing.Size(55, 12);
            this.Ib_MusicTimer.TabIndex = 0;
            this.Ib_MusicTimer.Text = "00:00:000";
            this.Ib_MusicTimer.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // MP3TrackBar
            // 
            this.MP3TrackBar.Location = new System.Drawing.Point(14, 28);
            this.MP3TrackBar.Maximum = 999999;
            this.MP3TrackBar.Name = "MP3TrackBar";
            this.MP3TrackBar.Size = new System.Drawing.Size(318, 90);
            this.MP3TrackBar.TabIndex = 1;
            this.MP3TrackBar.MouseDown += new System.Windows.Forms.MouseEventHandler(this.MP3TrackBar_MouseDown);
            this.MP3TrackBar.MouseMove += new System.Windows.Forms.MouseEventHandler(this.MP3TrackBar_MouseMove);
            this.MP3TrackBar.MouseUp += new System.Windows.Forms.MouseEventHandler(this.MP3TrackBar_MouseUp);
            // 
            // bt_Open
            // 
            this.bt_Open.Location = new System.Drawing.Point(257, 79);
            this.bt_Open.Name = "bt_Open";
            this.bt_Open.Size = new System.Drawing.Size(95, 35);
            this.bt_Open.TabIndex = 2;
            this.bt_Open.Text = "파일 열기";
            this.bt_Open.UseVisualStyleBackColor = true;
            this.bt_Open.Click += new System.EventHandler(this.bt_Open_Click);
            // 
            // bt_Close
            // 
            this.bt_Close.Location = new System.Drawing.Point(257, 120);
            this.bt_Close.Name = "bt_Close";
            this.bt_Close.Size = new System.Drawing.Size(95, 33);
            this.bt_Close.TabIndex = 3;
            this.bt_Close.Text = "종료";
            this.bt_Close.UseVisualStyleBackColor = true;
            this.bt_Close.Click += new System.EventHandler(this.bt_Close_Click);
            // 
            // MP3Timer
            // 
            this.MP3Timer.Interval = 20;
            this.MP3Timer.Tick += new System.EventHandler(this.MP3Timer_Tick);
            // 
            // bt_Play
            // 
            this.bt_Play.Location = new System.Drawing.Point(14, 79);
            this.bt_Play.Name = "bt_Play";
            this.bt_Play.Size = new System.Drawing.Size(75, 52);
            this.bt_Play.TabIndex = 4;
            this.bt_Play.Text = "재생";
            this.bt_Play.UseVisualStyleBackColor = true;
            this.bt_Play.Click += new System.EventHandler(this.bt_Play_Click);
            // 
            // bt_Stop
            // 
            this.bt_Stop.Location = new System.Drawing.Point(95, 79);
            this.bt_Stop.Name = "bt_Stop";
            this.bt_Stop.Size = new System.Drawing.Size(75, 52);
            this.bt_Stop.TabIndex = 5;
            this.bt_Stop.Text = "정지";
            this.bt_Stop.UseVisualStyleBackColor = true;
            this.bt_Stop.Click += new System.EventHandler(this.bt_Stop_Click);
            // 
            // bt_Pause
            // 
            this.bt_Pause.Location = new System.Drawing.Point(176, 79);
            this.bt_Pause.Name = "bt_Pause";
            this.bt_Pause.Size = new System.Drawing.Size(75, 52);
            this.bt_Pause.TabIndex = 6;
            this.bt_Pause.Text = "일시정지";
            this.bt_Pause.UseVisualStyleBackColor = true;
            this.bt_Pause.Click += new System.EventHandler(this.bt_Pause_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(558, 239);
            this.Controls.Add(this.bt_Pause);
            this.Controls.Add(this.bt_Stop);
            this.Controls.Add(this.bt_Play);
            this.Controls.Add(this.bt_Close);
            this.Controls.Add(this.bt_Open);
            this.Controls.Add(this.MP3TrackBar);
            this.Controls.Add(this.Ib_MusicTimer);
            this.Name = "Form1";
            this.Text = "MP3 Player";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.MP3TrackBar)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label Ib_MusicTimer;
        private System.Windows.Forms.TrackBar MP3TrackBar;
        private System.Windows.Forms.Button bt_Open;
        private System.Windows.Forms.Button bt_Close;
        private System.Windows.Forms.Timer MP3Timer;
        private System.Windows.Forms.Button bt_Play;
        private System.Windows.Forms.Button bt_Stop;
        private System.Windows.Forms.Button bt_Pause;
    }
}

