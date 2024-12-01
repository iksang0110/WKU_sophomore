using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WEEK12_n
{
    public partial class Form1 : Form
    {
        private int PenWidth = 3;
        private Color InColor = Color.Blue;
        public Form1()
        {
            InitializeComponent();
        }

       
        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            Pen pen = new Pen(Color.Black, (float)this.PenWidth);
            SolidBrush solidBrush = new SolidBrush(this.InColor);

            Graphics g = e.Graphics;
            Point[] points3 = new Point[10]
                    {
                        new Point(110, 40),
                        new Point(125, 91),
                        new Point(180, 91),
                        new Point(135, 123),
                        new Point(152, 172),
                        new Point(110, 141),
                        new Point(66, 172),
                        new Point(82, 122),
                        new Point(40, 91),
                        new Point(95, 91)
                    };
            e.Graphics.FillClosedCurve((Brush)solidBrush, points3);
            e.Graphics.DrawClosedCurve(pen, points3);
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
    }
}
