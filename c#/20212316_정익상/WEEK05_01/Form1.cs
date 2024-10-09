using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WEEK05_01
{
    public partial class Form1 : Form
    {
        private Boolean m_blLoginCheck = false;
        public Boolean LoginCheck
        {
            get { return m_blLoginCheck; }
            set { m_blLoginCheck = value; }
        }
        public Form1()
        {
            InitializeComponent();
        }

        private void maskedTextBox1_MaskInputRejected(object sender, MaskInputRejectedEventArgs e)
        {
            if (maskedTextBox1.MaskCompleted)
            {
                label1.Text = "휴대전화 입력 완료";
            }
            else
            {
                label1.Text = "번호 형식이 맞지 않습니다.";
            }
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            Form2 _Form2 = new Form2(this);
            _Form2.ShowDialog();
            if (!m_blLoginCheck) this.Close();
        }
    }
}
