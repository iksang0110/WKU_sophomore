using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WEEK05_02
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            if(listBox1.SelectedIndex > -1)
            {
                comboBox1.Items.Add(listBox1.SelectedItem);
                listBox1.Items.RemoveAt(listBox1.SelectedIndex);
            }
            else MessageBox.Show("콤보박스로 보낼 아이템을 선택해 주세요.");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (comboBox1.SelectedIndex > -1)
            {
                listBox1.Items.Add(comboBox1.SelectedItem);
                comboBox1.Items.RemoveAt(comboBox1.SelectedIndex);
                comboBox1.Text = "";
            }
            else MessageBox.Show("리스트박스로 보낼 아이템을 선택해 주세요.");
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (textBox1.Text != "")
            {
                listBox1.Items.Add(textBox1.Text);
                textBox1.Text = "";
            }
            else MessageBox.Show("추가할 아이템을 입력해 주세요.");
        }

        private void button4_Click(object sender, EventArgs e)
        {
            if(listBox1.SelectedIndex > -1)
            {
                listBox1.Items.Remove(listBox1.SelectedItems);
            }
            else MessageBox.Show("삭제할 아이템을 선택해 주세요.");
        }
    }
}
