using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Threading;
using System.IO.Ports;

namespace silerim_calis
{
    public partial class dogruanamenu : Form
    {
        public dogruanamenu()
        {
            InitializeComponent();
        }
        public int boundrate;

        public string portname;

        SqlConnection baglanti = new SqlConnection("Data Source=DESKTOP-1IJTFAI;Initial Catalog=laser-tag;Integrated Security=True");


        void degistirme(string id)
        {
           
        }
        private void dogruanamenu_Load(object sender, EventArgs e)
        {

            serialPort1.PortName = portname;
            timer1.Start();
            toolTip1.ToolTipTitle = "Buton Giris";
            toolTip1.IsBalloon = true;
            toolTip1.UseAnimation = true;
            toolTip1.ToolTipIcon = ToolTipIcon.Info;
            toolTip1.SetToolTip(button1, "Oyunu baslatir");


            id1.SelectedIndex = 0;
            id2.SelectedIndex = 1;
            id3.SelectedIndex = 2;
            id4.SelectedIndex = 3;
            id5.SelectedIndex = 4;
            id6.SelectedIndex = 5;
            id7.SelectedIndex = 6;
            id8.SelectedIndex = 7;
            id9.SelectedIndex = 8;
            id10.SelectedIndex = 9;


            comboBox12.SelectedIndex = 0;

            comboBox1.SelectedItem = "KIRMIZI";
            comboBox2.SelectedItem = "KIRMIZI";
            comboBox3.SelectedItem = "KIRMIZI";
            comboBox4.SelectedItem = "KIRMIZI";
            comboBox5.SelectedItem = "KIRMIZI";

            comboBox6.SelectedItem = "MAVI";
            comboBox7.SelectedItem = "MAVI";
            comboBox8.SelectedItem = "MAVI";
            comboBox9.SelectedItem = "MAVI";
            comboBox10.SelectedItem = "MAVI";

            comboBox11.SelectedItem = "5";

            label24.Text = Convert.ToString(boundrate);
            label23.Text = portname;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            //------usb kontrol-----------------------------------------------
            int b = 1;
            string[] ports = SerialPort.GetPortNames();
            foreach (string port in ports)
            {


                if (port == portname)
                {
                    b = 0;
                }
            }
            if (b == 1)
            {
                //çıkış
                timer1.Stop();
                MessageBox.Show("Bağlantı kaybedildi");
                Form1 fr = new Form1();
                fr.Show();
                this.Hide();
            }
        }

        int oyuncu_top;
        private void button1_Click(object sender, EventArgs e)
        {
           // string oyuncu_top_str="";
           // oyuncu_top = 0;
           //int oy =  Convert.ToInt32(oyuncusayisi.Text);
          
           //     for(int i = 1; i <= oy; i++)
           //     {
           //     if (i < 10)
           //     {
           //         oyuncu_top += 3;
           //     }
           //     else if (i >= 10)
           //     {
           //         oyuncu_top += 4;
           //     }
           //     }
           // if (oyuncu_top<10)
           // {
           //     oyuncu_top_str = "00" + oyuncu_top.ToString();
           // }
           // else if (oyuncu_top>=10&&oyuncu_top<99)
           // {
           //     oyuncu_top_str = "0" + oyuncu_top.ToString();
           // }
           // MessageBox.Show(oyuncu_top_str.ToString());


            int[] labels = { Convert.ToInt32(label1.Text), Convert.ToInt32(label2.Text), Convert.ToInt32(label3.Text), Convert.ToInt32(label4.Text), Convert.ToInt32(label5.Text), Convert.ToInt32(label6.Text), Convert.ToInt32(label7.Text), Convert.ToInt32(label8.Text), Convert.ToInt32(label9.Text), Convert.ToInt32(label10.Text) };
            string[] textboxs = { textBox1.Text.Trim(), textBox2.Text.Trim(), textBox3.Text.Trim(), textBox4.Text.Trim(), textBox5.Text.Trim(), textBox6.Text.Trim(), textBox7.Text.Trim(), textBox8.Text.Trim(), textBox9.Text.Trim(), textBox10.Text.Trim() };
            string[] comboboxs = { comboBox1.Text, comboBox2.Text, comboBox3.Text, comboBox4.Text, comboBox5.Text, comboBox6.Text, comboBox7.Text, comboBox8.Text, comboBox9.Text, comboBox10.Text };
            ComboBox[] noncomboboxs = { comboBox1, comboBox2, comboBox3, comboBox4, comboBox5, comboBox6, comboBox7, comboBox8, comboBox9, comboBox10 };

            serialPort1.PortName = portname;
            serialPort1.BaudRate = 9600;




            if (comboBox12.SelectedIndex == 0)
            {
                //takim
                baglanti.Open();
                SqlCommand silme = new SqlCommand("delete from tbl_grupOlum",baglanti);
                silme.ExecuteNonQuery();
                baglanti.Close();
                bool degisken = false;
                int index = 0;

                //serialPort1.Open();
                //serialPort1.Write(oyuncu_top_str);
                //serialPort1.Close();

                //for (int batu = 0;batu < Convert.ToUInt32(oyuncusayisi.Text);batu++)
                //{
                //    string takim = comboboxs[batu].ToString();
                //    string id = labels[batu].ToString();
                //    serialPort1.Open();
                //    serialPort1.Write(id + " " + takim.Substring(0, 1));//oyunu baslatmas
                //    serialPort1.Close();

                foreach (ComboBox eleman in noncomboboxs)
                {
                    string takim = eleman.Text;
                    string id = labels[index].ToString();
                    index++;
                    serialPort1.Open();
                    serialPort1.Write(id + " " + takim.Substring(0, 1));//oyunu baslatmas
                    serialPort1.Close();
                }


                serialPort1.Open();
                MessageBox.Show("basladi");
                serialPort1.Write("b");//oyun baslatiliyor
                serialPort1.Close();

                foreach (string elaman in textboxs)
                {

                    if (elaman.Trim() == "" || elaman.Trim() == String.Empty)
                    {
                        degisken = true;
                    }
                }
                for (int i = 0; i < textboxs.Length; i++)
                {
                    baglanti.Open();
                    SqlCommand komut = new SqlCommand("insert into tbl_grupOlum(id,isim,puan,durum,can,takim) values(@p1,@p2,@p3,@p4,@p5,@p6)", baglanti);
                    komut.Parameters.AddWithValue("@p1", labels[i]);
                    komut.Parameters.AddWithValue("@p2", textboxs[i]);
                    komut.Parameters.AddWithValue("@p3", 0);
                    komut.Parameters.AddWithValue("@p4", true);
                    komut.Parameters.AddWithValue("@p5", 15);
                    komut.Parameters.AddWithValue("@p6", comboboxs[i]);
                    komut.ExecuteNonQuery();
                    baglanti.Close();
                }

                if (degisken) { MessageBox.Show("yallah"); }
                else
                {
                    timer1.Stop();
                    grupmac fr = new grupmac();
                    fr.sure = Convert.ToInt32(comboBox11.Text) * 60;
                    fr.portname = portname;
                    fr.Show();
                    this.Hide();
                }
            }
            else if (comboBox12.SelectedIndex == 1)
            {
                //solo
                baglanti.Open();
                SqlCommand silme = new SqlCommand("delete from tbl_grupOlum", baglanti);
                silme.ExecuteNonQuery();
                baglanti.Close();
                bool degisken = false;
                serialPort1.Open();
                MessageBox.Show("solo");
                serialPort1.Write("s");//solo oldugunu belirtir 
                serialPort1.Close();

                serialPort1.Open();
                MessageBox.Show("basladi");
                serialPort1.Write("b");//oyunu baslat
                serialPort1.Close();
                foreach (string elaman in textboxs)
                {

                    if (elaman == "" || elaman == String.Empty)
                    {
                        degisken = true;
                    }
                }

                for (int i = 0; i < textboxs.Length; i++)
                {
                    baglanti.Open();
                    SqlCommand komut = new SqlCommand("insert into tbl_grupOlum(id,isim,puan,durum,can,takim) values(@p1,@p2,@p3,@p4,@p5,@p6)", baglanti);
                    komut.Parameters.AddWithValue("@p1", labels[i]);
                    komut.Parameters.AddWithValue("@p2", textboxs[i]);
                    komut.Parameters.AddWithValue("@p3", 0);
                    komut.Parameters.AddWithValue("@p4", true);
                    komut.Parameters.AddWithValue("@p5", 15);
                    komut.Parameters.AddWithValue("@p6", "yok");
                    komut.ExecuteNonQuery();
                    baglanti.Close();
                }

                if (degisken) { MessageBox.Show("yallah"); }
                else
                {
                    timer1.Stop();
                    olumaci2 fr = new olumaci2();
                    fr.sure = Convert.ToInt32(comboBox11.Text) * 60;
                    fr.portname = portname;
                    fr.Show();
                    this.Hide();
                }
            }
        }

        private void id10_SelectedIndexChanged(object sender, EventArgs e)
        {
            degistirme(id10.Text);
        }
    }
}
