using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WEEK04_03
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

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            button1.BackColor = Color.Red;
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            button1.BackColor = Color.Green;
        }

        private void radioButton3_CheckedChanged(object sender, EventArgs e)
        {
            button1.BackColor = Color.Blue;
        }

        private void radioButton4_CheckedChanged(object sender, EventArgs e)
        {
            button1.TextAlign = ContentAlignment.MiddleLeft;
        }

        private void radioButton5_CheckedChanged(object sender, EventArgs e)
        {
            button1.TextAlign = ContentAlignment.MiddleCenter;
        }

        private void radioButton6_CheckedChanged(object sender, EventArgs e)
        {
            button1.TextAlign = ContentAlignment.MiddleRight;
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (((CheckBox)sender).Checked)
            {
                button1.Enabled = false;
            }
            else
            {
                button1.Enabled = true;
            }
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            if (((CheckBox)sender).Checked)
            {
                button1.Visible = false;
            }
            else
            {
                button1.Visible = true;
            }
        }

        private void ColorChanged(object sender, EventArgs e)
        {
            RadioButton R = sender as RadioButton;
            if (R == radioButton1) button1.BackColor = Color.Red;
            if (R == radioButton2) button1.BackColor = Color.Green;
            if (R == radioButton3) button1.BackColor = Color.Blue;
        }

        private void radioButton6_CheckedChanged_1(object sender, EventArgs e)
        {

        }

        private void radioButton5_CheckedChanged_1(object sender, EventArgs e)
        {

        }

        private void groupBox2_Enter(object sender, EventArgs e)
        {

        }
    }
}
