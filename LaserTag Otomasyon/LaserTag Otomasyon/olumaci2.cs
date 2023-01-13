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
using System.IO.Ports;

namespace silerim_calis
{
    public partial class olumaci2 : Form
    {
        private string data;
        public int sure;
        public string portname;
        int tmp_can;
        public olumaci2()
        {
            InitializeComponent();
        }
        SqlConnection baglanti = new SqlConnection("Data Source=DESKTOP-1IJTFAI;Initial Catalog=laser-tag;Integrated Security=True");
        void yenileme()
        {
            //hem sıralamayı büyükten küçüğe göre yapıyor
            baglanti.Open();
            SqlCommand solo = new SqlCommand("select id,isim,durum,can,puan FROM tbl_grupOlum ORDER BY puan DESC", baglanti);
            SqlDataAdapter da = new SqlDataAdapter(solo);
            DataTable dt = new DataTable();
            DataTable dt2 = new DataTable();
            da.Fill(dt);
            dataGridView1.DataSource = dt;
            baglanti.Close();
        }

        private void olumaci2_Load(object sender, EventArgs e)
        {
            timer1.Start();
            yenileme();
            serialPort1.PortName = portname;
            serialPort1.Open();
            serialPort1.DataReceived += new SerialDataReceivedEventHandler(SerialPort1_DataReceived); //DataReceived eventini oluşturma
        }
        private void SerialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            data = serialPort1.ReadLine();                      //Veriyi al
            this.Invoke(new EventHandler(displayData_event));
        }

        string deger;
        public string VuranV(int vuran_id)
        {
            baglanti.Open();
            SqlCommand komut = new SqlCommand("select isim from tbl_grupOlum where id=@p1", baglanti);
            komut.Parameters.AddWithValue("@p1", vuran_id);
            SqlDataReader dr = komut.ExecuteReader();
            while (dr.Read())
            {
                deger = dr[0].ToString();
            }
            baglanti.Close();
            return deger;

        }
        public string VurulanV(int vuran_id)
        {
            baglanti.Open();
            SqlCommand komut = new SqlCommand("select isim from tbl_grupOlum where id=@p1 ", baglanti);
            komut.Parameters.AddWithValue("@p1", vuran_id);
            SqlDataReader dr = komut.ExecuteReader();
            while (dr.Read())
            {
                deger = dr[0].ToString();
            }
            baglanti.Close();
            return deger;

        }
        private void displayData_event(object sender, EventArgs e)
        {
            string[] ayirlma = data.Split(' ');
            int vuran = Convert.ToInt32(ayirlma[0]);
            int vurulan = Convert.ToInt32(ayirlma[1]);
            tmp_can = Convert.ToInt32(ayirlma[2]);
            string vuran_isim = VuranV(vuran);
            string vurulan_isim = VurulanV(vurulan);
            //MessageBox.Show("Vuran:"+vuran+"\nVurulan:"+vurulan);
            listBox1.Items.Add(vuran_isim + " ----> " + vurulan_isim);
            guncelle(vuran, vurulan);
        }
        int vuran_puan;
        int vurulan_can;
        void guncelle(int vuran, int vurulan)
        {

            baglanti.Open();
            //vuranın önceki puanını alıyor
            SqlCommand vuran_puanb = new SqlCommand("select puan from tbl_grupOlum where id=@p1", baglanti);
            vuran_puanb.Parameters.AddWithValue("@p1", vuran);
            SqlDataReader dr = vuran_puanb.ExecuteReader();
            while (dr.Read())
            {
                vuran_puan = Convert.ToInt32(dr["puan"]) + 10;
            }
            baglanti.Close();

            //puan guncelliyor
            baglanti.Open();
            SqlCommand puan = new SqlCommand("update tbl_grupOlum set puan=@p1 where id=@p2", baglanti);
            puan.Parameters.AddWithValue("@p1", vuran_puan);
            puan.Parameters.AddWithValue("@p2", vuran);
            puan.ExecuteNonQuery();
            baglanti.Close();

            //vurulanın id sini çekme
            //tmp_can
            //can guncelleme
            baglanti.Open();
            SqlCommand can = new SqlCommand("update tbl_grupOlum set durum=@p3,can=@p1 where id=@p2", baglanti);
            if (tmp_can == 0)
            {
                can.Parameters.AddWithValue("@p3", false);
            }
            else
            {
                can.Parameters.AddWithValue("@p3", true);
            }
            can.Parameters.AddWithValue("@p1", tmp_can);
            can.Parameters.AddWithValue("@p2", vurulan);
            can.ExecuteNonQuery();
            baglanti.Close();

            yenileme();

        }
        private void timer1_Tick(object sender, EventArgs e)
        {
            sure--;
            int dakika;
            int saniye;
            dakika = sure / 60;
            saniye = sure % 60;
            if (dakika < 10)
            {
                if (saniye < 10)
                {
                    lbl_sure.Text = "0" + dakika + ":0" + saniye;
                }
                else
                {
                    lbl_sure.Text = "0" + dakika + ":" + saniye;
                }
            }

            else
            {
                lbl_sure.Text = dakika + ":" + saniye;
            }
            if (sure <= 0)
            {
                if (!serialPort1.IsOpen)
                {
                    serialPort1.Open();
                }
                if (serialPort1.IsOpen)
                {
                    serialPort1.Close();
                }
                timer1.Stop();
                serialPort1.PortName = portname;
                serialPort1.Open();
                serialPort1.Write("i");//oyunu bitir
                serialPort1.Close();
                timer1.Stop();
                MessageBox.Show("Gule gule canisi", "Oyun Bitti", MessageBoxButtons.OK, MessageBoxIcon.Question);
                dogruanamenu fr = new dogruanamenu();
                fr.portname = portname;
                fr.Show();
                this.Hide();

            }

            //------usb kontrol-----------------------------------------------
            int b = 1;
            string[] ports = SerialPort.GetPortNames();
            foreach (string port in ports)
            {
                if (port == portname || portname != String.Empty || portname != "")
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

        private void button1_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("Bitirmek istedigine emin misin ?", "Bitir", MessageBoxButtons.OKCancel, MessageBoxIcon.Information) == DialogResult.OK)
            {
                timer1.Stop();
                if (!serialPort1.IsOpen)
                {
                    serialPort1.Open();
                }
                MessageBox.Show("bitti");
                serialPort1.Write("i");//oyunu bitir
                serialPort1.Close();
                timer1.Stop();
                dogruanamenu fr = new dogruanamenu();
                fr.portname = portname;
                fr.Show();
                this.Hide();
            }
        }
    }
}
