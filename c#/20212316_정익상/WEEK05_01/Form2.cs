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
    public partial class Form2 : Form
    {
        private Form1 _Form1;

        public Form2(Form1 form1)
        {
            InitializeComponent();
            _Form1 = form1;
        }

        private void Form2_Load(object sender, EventArgs e)
        {
            //시작시 버튼을 포커스로 설정
            this.ActiveControl = button1;
        }
        private void bt_OK()
        {
            if (textBox1.Text == "윈도우프로그래밍실습" && textBox2.Text == "1234")
            {
                _Form1.LoginCheck = true;
                this.Close();
            }
            else
            {
                MessageBox.Show("아이디 또는 비밀번호가 틀립니다.");

                textBox1.Text = "아이디를 입력해 주세요";
                textBox2.Text = "비밀번호를 입력해 주세요";
                textBox2.PasswordChar = '\0';
                button1.Focus();
            }
        }
        private void textBox1_Click(object sender, EventArgs e)
        {
            if (textBox1.Text == "아이디를 입력해 주세요")
            {
                textBox1.Text = "";
            }
        }

        private void textBox1_Leave(object sender, EventArgs e)
        {
            if (textBox1.Text == "")
            {
                textBox1.Text = "아이디를 입력해 주세요";
            }
        }

        private void textBox2_Leave(object sender, EventArgs e)
        {
            if (textBox2.Text == "")
            {
                textBox2.Text = "비밀번호를 입력해 주세요";
                textBox2.PasswordChar = '\0';
            }
        }

        private void textBox2_Click(object sender, EventArgs e)
        {
            if (textBox2.Text == "비밀번호를 입력해 주세요")
            {
                textBox2.Text = "";
                textBox2.PasswordChar = '*';
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            bt_OK();
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start(linkLabel1.Text); // 링크 라벨 클릭시 링크로 이동
        }
    }
}
