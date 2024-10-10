using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Printing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WEEK06_01
{
    public partial class Form1 : Form
    {
        private Color originalBackColor;

        public Form1()
        {
            InitializeComponent();
            originalBackColor = textBox1.BackColor;
        }

        private String m_strInfo = "";

        public String strInfo
        {
            get { return m_strInfo; }
            set
            {
                m_strInfo = value;
                label1.Text = m_strInfo + "종료";
            }
        }

        private void bt_Modal_Click(object sender, EventArgs e)
        {
            Form2 _Form2 = new Form2(this);
            _Form2.strText = "모달";
            _Form2.ShowDialog();
        }

        private void bt_Modeless_Click(object sender, EventArgs e)
        {
            Form2 _Form2 = new Form2(this);
            _Form2.strText = "모달리스";
            _Form2.Show();
        }

        private void bt_FileOpen_Click(object sender, EventArgs e)
        {
            openFileDialog1.InitialDirectory = "C:\\";
            openFileDialog1.Filter = "텍스트 파일(*.txt)|*.txt|모든 파일(*.*)|*.*";
            openFileDialog1.FilterIndex = 1;
            openFileDialog1.RestoreDirectory = true;
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    textBox1.Text = System.IO.File.ReadAllText(openFileDialog1.FileName);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("파일을 읽을 수 없습니다. 오류: " + ex.Message);
                }
                finally{ }
            }
        }

        private void bt_FlieSave_Click(object sender, EventArgs e)
        {
            saveFileDialog1.InitialDirectory = "C:\\";
            saveFileDialog1.Filter = "텍스트 파일(*.txt)|*.txt|모든 파일(*.*)|*.*";
            saveFileDialog1.FilterIndex = 1;
            saveFileDialog1.RestoreDirectory = true;
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    System.IO.File.WriteAllText(saveFileDialog1.FileName, textBox1.Text);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("파일을 저장할 수 없습니다. 오류: " + ex.Message);
                }
                finally { }
            }
        }

        private void bt_Fornt_Click(object sender, EventArgs e)
        {
            fontDialog1.ShowDialog();
            textBox1.Font = fontDialog1.Font;
            textBox1.ForeColor = fontDialog1.Color;
            
        }

        private void bt_Color_Click(object sender, EventArgs e)
        {
            
            if (colorDialog1.ShowDialog() == DialogResult.OK)
            {
                textBox1.BackColor = colorDialog1.Color;
                
            }
        }

        private void pd_PrintPage(object sender, System.Drawing.Printing.PrintPageEventArgs e)
        {
            e.Graphics.DrawString(textBox1.Text, textBox1.Font, new SolidBrush(textBox1.ForeColor), 10, 10);
        }

        private void bt_Print_Click(object sender, EventArgs e)
        {
            PrinterSettings printer = new PrinterSettings();
            PrintDocument pd = new PrintDocument();
            printDialog1.Document = pd;
            pd.PrintPage += new PrintPageEventHandler(this.pd_PrintPage);
            DialogResult result = printDialog1.ShowDialog();
            if (result == DialogResult.OK)
            {
                pd.Print();
            }

        }

        private void fontDialog1_Apply(object sender, EventArgs e)
        {

        }
    }
}
