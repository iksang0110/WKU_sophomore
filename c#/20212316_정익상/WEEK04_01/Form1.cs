using System;
using System.Drawing;
using System.Windows.Forms;

namespace WEEK04_01
{
    public partial class Form1 : Form
    {
        private Button randomButton;
        private Random random;

        public Form1()
        {
            InitializeComponent();
            random = new Random();

            // 버튼 생성 및 설정
            randomButton = new Button
            {
                Text = "Random Button",
                Size = new Size(75, 23)
            };

            this.Controls.Add(randomButton);

            // 폼 로드 이벤트 핸들러 추가
            this.Load += Form1_Load;

            // 버튼 클릭 이벤트 핸들러 추가
            randomButton.Click += RandomButton_Click;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            SetRandomButtonPosition();
        }

        private void RandomButton_Click(object sender, EventArgs e)
        {
            SetRandomButtonColor();
            SetRandomButtonPosition();
        }

        private void SetRandomButtonPosition()
        {
            int maxX = this.ClientSize.Width - randomButton.Width;
            int maxY = this.ClientSize.Height - randomButton.Height;

            int newX = random.Next(0, maxX + 1);
            int newY = random.Next(0, maxY + 1);

            randomButton.Location = new Point(newX, newY);
        }

        private void SetRandomButtonColor()
        {
            randomButton.BackColor = Color.FromArgb(random.Next(256), random.Next(256), random.Next(256));
        }
    }
}