using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MP3
{
    public partial class Form1 : Form
    {
        MP3Player mp3Player;

        //Track Bar
        bool isScrolled = false;
        int trackbarBlankSize = 14;
        int trackbarLength = 0;
        int trackbarMouseX = 0;
        public Form1()
        {
            InitializeComponent();

            mp3Player = new MP3Player();
            trackbarLength = MP3TrackBar.Size.Width - (trackbarBlankSize * 2); //Trackbar 의 실제 길이
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            
        }

        private void bt_Play_Click(object sender, EventArgs e)
        {
            mp3Player.Play();
        }

        private void bt_Stop_Click(object sender, EventArgs e)
        {
            mp3Player.Stop();
        }

        private void bt_Pause_Click(object sender, EventArgs e)
        {
            mp3Player.Pause();
        }

        private void bt_Open_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog open = new OpenFileDialog())
            {
                open.Filter = "MP3 파일|*.mp3";
                open.InitialDirectory = Environment.CurrentDirectory;
                if (open.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                {
                    string fileName = open.FileName;

                    mp3Player.Open(fileName);
                    MP3TrackBar.Maximum = mp3Player.GetLength();
                    MP3Timer.Enabled = true;
                }
            }
        }

        private void bt_Close_Click(object sender, EventArgs e)
        {
            MP3Timer.Enabled = false;
            MP3TrackBar.Value = 0;
            mp3Player.Close();
        }

        private void MP3TrackBar_MouseDown(object sender, MouseEventArgs e)
        {
            isScrolled = true;
            trackBarMouseX = e.X - trackbarBlankSize;
            SetPositionByMouse(trackBarMouseX);
        }

        private void MP3TrackBar_MouseMove(object sender, MouseEventArgs e)
        {
            if (isScrolled)
            {
                trackBarMouseX = e.X - trackBarBlankSize;
                SetPositionByMouse(trackBarMouseX);
            }
        }

        private void MP3TrackBar_MouseUp(object sender, MouseEventArgs e)
        {
            if (mp3Player.isOpened)
            {
                string status = mp3Player.GetStatus();

                mp3Player.Seek(MP3TrackBar.Value);

                if (status == "playing")
                    mp3Player.Play();
                else
                    mp3Player.Pause();
            }
            isScrolled = false;
        }
        private void MP3Timer_Tick(object sender, EventArgs e)
        {
            if (!mp3Player.isOpened) return;

            if (isScrolled == false)
                MP3TrackBar.Value = mp3Player.GetPosition();

            if (!mp3Player.loop && mp3Player.GetPosition() == mp3Player.GetLength())
                mp3Player.Stop();
            SetMusicTimer();
        }
        private void SetPositionByMouse(int position)
        {
            if (position < 0 || position > trackbarLength)
                return;

            float rate = (float)position / (float)trackbarLength;
            MP3TrackBar.Value = (int)(rate * (float)(MP3TrackBar.Maximum - MP3TrackBar.Minimum));
        }
        private void SetMusicTimer()
        {
            if (mp3Player.isOpened)
            {
                TimeSpan t = TimeSpan.FromMilliseconds(mp3Player.GetPosition());
                Ib_MusicTimer.Text = string.Format("{0:D2}:{1:D2}:{2:D2}", t.Hours, t.Minutes, t.Seconds);
            }
        }
    }
}
