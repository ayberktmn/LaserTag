using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;


namespace silerim_calis
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

        private void timer1_Tick(object sender, EventArgs e)
        {
            //------usb kontrol-----------------------------------------------
            comboBox1.Items.Clear();

            string[] ports = SerialPort.GetPortNames();
            foreach (string port in ports)
            {

                comboBox1.Items.Add(port);

                if (ports[0] != null)
                {
                    uyarıLabel1.Visible = false;
                    pictureBox1.Visible = false;
                    comboBox1.SelectedItem = ports[0];
                }
                else if (port == null)
                {
                    comboBox1.Items.Clear();
                }
            }
            if (comboBox1.Text == string.Empty)
            {
                uyarıLabel1.Visible = true;
                pictureBox1.Visible = true;
                button1.Enabled = false;
                button1.Visible = false;
            }
            else if (comboBox1.Text != string.Empty)
            {
                uyarıLabel1.Visible = false;
                pictureBox1.Visible = false;
                button1.Enabled = true;
                button1.Visible = true;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                dogruanamenu fr = new dogruanamenu();
                fr.boundrate = 9600;
                fr.portname = comboBox1.SelectedItem.ToString();
                fr.Show();
                this.Hide();
                timer1.Stop();
            }
            catch (Exception bg)
            {
                MessageBox.Show(bg.Message, "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("BEDO'nun evine gelmesini ister misin ?", "Geliyor", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
            {
                MessageBox.Show("Nereye gidiyon amoon");
            }
        }
    }
}
