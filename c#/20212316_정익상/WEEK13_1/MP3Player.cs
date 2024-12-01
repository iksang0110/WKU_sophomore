using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MP3
{
    class MP3Player
    {
        public bool loop { get; set; }
        public bool isOpened = false;

        private StringBuilder returnData;

        [Dlllmport("winmm.dll")]
        private static extern long mcSendString
            (string _command, StringBuilder _returnData, int _returnLength, IntPtr _hwncallBack);

        public MP3Player()
        {
            returnData = new StringBuilder(128);
        }
        //파일 열기
        public void Open(string _fileName)
        {
            if(isOpened)
            {
                Close();
            }

            string command = "open \"" + _fileName + "\" type mpegVideo alias MediaFile";
            mcSendString(command, null, 0, IntPtr.Zero);
            isOpened = true;
        }
        
        //파일 닫기
        public void Close()
        {
            string command = "close MediaFile";
            mcSendString(command, null, 0, IntPtr.Zero);
            loop = false;
            isOpened = false;
        }
        //음악 재생
        public void Play()
        {
            if (isOpened)
            {
                string command = "play MediaFile";
                if (loop)
                    command += " REPEAT";
                mcSendString(command, null, 0, IntPtr.Zero);
            }
           
        }
        //음악 일시정지
        public void Pause()
        {
            if (isOpened)
            {
                string command = "pause MediaFile";
                mcSendString(command, null, 0, IntPtr.Zero);
            }
        }
        //음악 정지
        public void Stop()
        {
            if (isOpened) Seek(0);

        }
        //재생지점을 절정하는 메소드
        public void Seek(int time)
        {
            string command = "seek MediaFile to " + time.ToString();
            mcSendString(command, null, 0, IntPtr.Zero);
        }
        //현재 상태를 얻는 메소드  (playing, pause, stopped)
        public string GetStatus()
        {
            returnData.Clear();
            string command = "status MediaFile mode";
            mcSendString(command, returnData, returnData.Capacity, IntPtr.Zero);
            return returnData.ToString();
        }
        //음악 길이를 얻는 메소드
        public int GetLength()
        {
            returnData.Clear();
            if (isOpened)
            {
                string command = "status MediaFile length";
                mcSendString(command, returnData, returnData.Capacity, IntPtr.Zero);

                int lenght = int.Parse(returnData.ToString());
                return lenght;
            }
            else
                return 0;
        }
        //현재 재생지점(경과 시간)을 얻는 메소드
        public int GetPosition()
        {
            
            if (isOpened)
            {
                string command = "status MediaFile position";
                mcSendString(command, returnData, returnData.Capacity, IntPtr.Zero);
                int position = int.Parse(returnData.ToString());
                return position;
            }
            else
                return 0;
        }
    }
}
