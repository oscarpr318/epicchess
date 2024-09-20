using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace NEA___Chess_Engine
{
    public partial class FormOptions : Form
    {
        int colour = -1;
        int time = 0;

        public FormOptions()
        {
            InitializeComponent();
        }


        private void whiteBtn_Click(object sender, EventArgs e)
        {
            colour = 1;

            whiteBtn.BackColor = Color.FromArgb(0, 150, 255);
            blackBtn.BackColor = Color.FromArgb(224, 224, 224);
        }

        private void blackBtn_Click(object sender, EventArgs e)
        {
            colour = 0;

            blackBtn.BackColor = Color.FromArgb(0, 150, 255);
            whiteBtn.BackColor = Color.FromArgb(224, 224, 224);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            time = 1;

            button2.BackColor = Color.FromArgb(0, 150, 255);
            button1.BackColor = Color.FromArgb(224, 224, 224);
            button3.BackColor = Color.FromArgb(224, 224, 224);
            button4.BackColor = Color.FromArgb(224, 224, 224);
            button5.BackColor = Color.FromArgb(224, 224, 224);
            button6.BackColor = Color.FromArgb(224, 224, 224);
            button7.BackColor = Color.FromArgb(224, 224, 224);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            time = 3;

            button1.BackColor = Color.FromArgb(0, 150, 255);
            button2.BackColor = Color.FromArgb(224, 224, 224);
            button3.BackColor = Color.FromArgb(224, 224, 224);
            button4.BackColor = Color.FromArgb(224, 224, 224);
            button5.BackColor = Color.FromArgb(224, 224, 224);
            button6.BackColor = Color.FromArgb(224, 224, 224);
            button7.BackColor = Color.FromArgb(224, 224, 224);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            time = 5;

            button3.BackColor = Color.FromArgb(0, 150, 255);
            button1.BackColor = Color.FromArgb(224, 224, 224);
            button2.BackColor = Color.FromArgb(224, 224, 224);
            button4.BackColor = Color.FromArgb(224, 224, 224);
            button5.BackColor = Color.FromArgb(224, 224, 224);
            button6.BackColor = Color.FromArgb(224, 224, 224);
            button7.BackColor = Color.FromArgb(224, 224, 224);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            time = 10;

            button4.BackColor = Color.FromArgb(0, 150, 255);
            button1.BackColor = Color.FromArgb(224, 224, 224);
            button3.BackColor = Color.FromArgb(224, 224, 224);
            button2.BackColor = Color.FromArgb(224, 224, 224);
            button5.BackColor = Color.FromArgb(224, 224, 224);
            button6.BackColor = Color.FromArgb(224, 224, 224);
            button7.BackColor = Color.FromArgb(224, 224, 224);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            time = 15;

            button5.BackColor = Color.FromArgb(0, 150, 255);
            button1.BackColor = Color.FromArgb(224, 224, 224);
            button3.BackColor = Color.FromArgb(224, 224, 224);
            button4.BackColor = Color.FromArgb(224, 224, 224);
            button2.BackColor = Color.FromArgb(224, 224, 224);
            button6.BackColor = Color.FromArgb(224, 224, 224);
            button7.BackColor = Color.FromArgb(224, 224, 224);
        }

        private void button6_Click(object sender, EventArgs e)
        {
            time = 20;

            button6.BackColor = Color.FromArgb(0, 150, 255);
            button1.BackColor = Color.FromArgb(224, 224, 224);
            button3.BackColor = Color.FromArgb(224, 224, 224);
            button4.BackColor = Color.FromArgb(224, 224, 224);
            button5.BackColor = Color.FromArgb(224, 224, 224);
            button2.BackColor = Color.FromArgb(224, 224, 224);
            button7.BackColor = Color.FromArgb(224, 224, 224);
        }

        private void button7_Click(object sender, EventArgs e)
        {
            if (colour != -1 & time != 0)
            {
                new FormGame(colour, time).Show();
                Dispose();
                Close();
            }
        }
    }
}
