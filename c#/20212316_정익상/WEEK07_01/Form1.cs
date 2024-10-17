using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WEEK07_01
{
    public partial class Form1 : Form
    {
        private Boolean m_blLoginCheck = false;

        private void MouseWheelEvent(object sender, MouseEventArgs e)
        {
            if (e.Delta > 0)
            {
                MessageBox.Show("위로 스크롤");
            }
            else
            {
                MessageBox.Show("아래로 스크롤");
            }
        }
        public Form1()
        {
            InitializeComponent();
            this.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.MouseWheelEvent);
        }
        public Boolean LoginCheck
        {
            get { return m_blLoginCheck; }
            set { m_blLoginCheck = value; }
        }



        private void Form1_Load(object sender, EventArgs e)
        {
            Form2 _Form = new Form2(this);
            _Form.ShowDialog();

            if (!m_blLoginCheck) this.Close();
        }
        private void Form1_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
                MessageBox.Show("X : " + e.X + " Y : " + e.Y);
        }

        private void 파일FToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show(파일FToolStripMenuItem.Text);
        }

        private void 닫기cToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show(닫기cToolStripMenuItem.Text);
        }

        private void 새파일NToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show(새파일NToolStripMenuItem.Text);
        }

        private void 열기OToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show(열기OToolStripMenuItem.Text);
        }

        private void 저장SToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show(저장SToolStripMenuItem.Text);
        }

        private void 다른이름으로저장AToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show(다른이름으로저장AToolStripMenuItem.Text);
        }

        private void 인쇄PToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show(인쇄PToolStripMenuItem.Text);
        }

        private void 미리보기VToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show(미리보기VToolStripMenuItem.Text);
        }

        private void 종료XToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show(종료XToolStripMenuItem.Text);
        }

        private void 잘라내기ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show(잘라내기ToolStripMenuItem.Text);
        }

        private void 복사ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show(복사ToolStripMenuItem.Text);
        }

        private void 붙여넣기ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show(붙여넣기ToolStripMenuItem.Text);
        }

        private void 프로그램정보AToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show(프로그램정보AToolStripMenuItem.Text);
        }

        private void 복사ToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            MessageBox.Show(복사ToolStripMenuItem1.Text);
        }

        private void 모두선택ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show(모두선택ToolStripMenuItem.Text);
        }

        private void 실행취소ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show(실행취소ToolStripMenuItem.Text);
        }

        private void 잘라내기ToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            MessageBox.Show(잘라내기ToolStripMenuItem1.Text);
        }

        private void 삭제ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show(삭제ToolStripMenuItem.Text);
        }
        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            bool baseResult = base.ProcessCmdKey(ref msg, keyData);

            if (keyData == (Keys.F1))
            {
                MessageBox.Show("F1키 눌렸습니다.");
            }
            else if (keyData == (Keys.Control | Keys.F1))
            {
                MessageBox.Show("Ctrl + F1 키가 눌렸습니다.");
            }
            return baseResult;
        }
    }
}

