using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WEEK09_06
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            for( int i = progressBar1.Minimum; i < progressBar1.Maximum; i++)
            {
                progressBar1.Value = i;
               
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
    }
}
