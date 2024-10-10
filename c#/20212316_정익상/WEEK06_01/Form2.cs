using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WEEK06_01
{
    public partial class Form2 : Form
    {
        private String _strText = "";
        private Form1 _Form1;
        public Form2(Form1 form1)
        {
            _Form1 = form1;
            InitializeComponent();
        }

        public String strText
        {
            get { return _strText; }
            set { _strText = value + "대화상자"; }
        }
        private void Form2_Load(object sender, EventArgs e)
        {
            this.Text = _strText;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            _Form1.strInfo = _strText;
            this.Close();
        }
    }
}
