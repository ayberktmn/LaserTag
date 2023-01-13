int id = 5;//silah in kendi id si
String oyun_modu;
uint8_t takim[] = {0xCD,0x69,0x05};//0.indeks kirmizi , 1.indeks mavi
int takim_indeks;//bu senin hanki takımda olduguna gore ates ettiginde hangisinin secilecegini soyler
#include <ESP8266WiFi.h>//esp kartta mevcut yüklemeye gerek yok
#include <espnow.h>//esp kartta mevcut yüklemeye gerek yok
#define titresim 13//D7
#define sarsor_o 0
#define tetik 16//d0
#include <SPI.h>
#include <Wire.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>
#define irvericipin 14//D5
#define iralicipin 12//D6
IRsend irsend(irvericipin); // IR 
IRrecv irrecv(iralicipin);
decode_results results;
int alinanistekmesaji=0;
int istekiletildi=0;
int can = 100;
int sarsor = 2;
int mermi = 65;
int dakika=5;
int saniye=60;
String tmp_takim;
bool tmp_oyun_durumu;
int tmp_can;
int islem_kodu;
String tmp_vuran;
String tmp_vurulan;
uint8_t alici_macadresi[] = {0xE8, 0xDB, 0x84, 0xE5, 0xFE, 0xAD};
typedef struct struct_message {
    bool istekgonder;
    String renk;
    int can;
    bool durum;
    String mod;
    int islem;
    int vuran;
    int vurulan;
    bool cevap;
} struct_message;

bool gelencevap =0;
// İstek göndermek için yarattığımız veri yapısı
struct_message istekyolla;

// gelen verileri almak için yarattığımız veri yapısı
struct_message gelenveriler;

// Veriler gönderildiğinde çalıştıracağımız fonksiyon
void VerilerGonderildiginde(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Son Verinin GÖnderilme Durumu: ");
  if (sendStatus == 0){
    Serial.println("İstek mesajı iletildi");
    istekiletildi=1;
  }
  else{
    Serial.println("İstek İletilemedi !!!");
    istekiletildi=0;//hata mesajını oled ekranda göstermek için kullandım
  }
}
// Veriler alındığında çalışacak fonksiyon
void VerilerAlindiginda(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&gelenveriler, incomingData, sizeof(gelenveriler));
  
  Serial.print("Alınan veri boyutu: ");
  Serial.println(len);
  Serial.print("Islem kodu:");
  Serial.println(gelenveriler.islem);
  Serial.print("Renk kodu:");
  Serial.println(gelenveriler.renk);
  Serial.print("Oyun durumu:");
  Serial.println(gelenveriler.durum);
  Serial.print("istek durumu");
  Serial.println(gelenveriler.istekgonder);
  tmp_takim= gelenveriler.renk;
  tmp_oyun_durumu= gelenveriler.durum;
  tmp_can= gelenveriler.can;
  islem_kodu = gelenveriler.islem;
  oyun_modu = gelenveriler.mod;
  
  gelencevap = gelenveriler.istekgonder;//diğer karttan istek gelmiş mi bakıyoruz
  //takim = gelenveriler.renk;//diğer karttan istek gelmiş mi bakıyoruz
}

void setup() {
  
        Wire.begin();
  pinMode(tetik,INPUT);
  pinMode(sarsor_o,INPUT);
  pinMode(titresim,OUTPUT);
  Serial.begin(9600); 
  irrecv.enableIRIn(); // IR haberleşme başlatıldı.
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // ESP-NOW'u başlatıyoruz
  if (esp_now_init() != 0) {
    Serial.println("ESP NOW Başaltılamadı!!!");
    return;
  }

  // Bu kartın rolünü tanımlıyoruz. Çift yönlü iletişimde kartlar COMBO olmalı
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  // Yukarıda tanımladığımız ve veri gönderildiğinde çalışacak fonksiyonu atıyoruz
  esp_now_register_send_cb(VerilerGonderildiginde);//veriler gonderildikten sonra kontrol yapmak icin kullaniyoruz

  // Bağlanılacak diğer karta bağlantıyı tanımlıyoruz
  esp_now_add_peer(alici_macadresi, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  
  // Yukarıda tanımladığımız ve veri alındığında çalışacak fonksiyonu atıyoruz
  esp_now_register_recv_cb(VerilerAlindiginda);
}


void loop() {

  
  
  if(gelencevap){
      if(islem_kodu==1){
        //renk ayarlanmasi
        if(tmp_takim=="K"){
          Wire.beginTransmission(1);
          Wire.write("K");
          Serial.println("K");
          Wire.endTransmission();
          takim_indeks = 0;//kirmizi takima ayarlandı verici
        }
        if(tmp_takim=="M"){
          Wire.beginTransmission(1);
          Wire.write("M");
          Serial.println("M");
          Wire.endTransmission();
          takim_indeks = 1;//mavi takima ayarlandı verici
        }
      
    }
    else if(islem_kodu==2){
      //oyun durumu
      if(tmp_oyun_durumu){
        Serial.println("basladi");
          if(oyun_modu=="solo"){
            Wire.beginTransmission(1);
            Wire.write("Y");
            Serial.println("Y");
            Wire.endTransmission();
            Serial.println("herkes tek");
            takim_indeks = 2;//herkes tek
          }

        String zaman_birles="";
        if(dakika<10){
          zaman_birles = "T0"+String(dakika)+String(saniye);
          Wire.beginTransmission(1);
          Wire.write(zaman_birles.c_str());
          Serial.println(zaman_birles);
          Wire.endTransmission();
        }else{
          zaman_birles = "T0"+String(dakika)+String(saniye);
          Wire.beginTransmission(1);
          Wire.write(zaman_birles.c_str());
          Serial.println(zaman_birles);
          Wire.endTransmission();
        }

        delay(1000);
        
        Wire.beginTransmission(1);
        Wire.write("C100");
        Serial.println("can 100");
        Wire.endTransmission();
        
        Wire.beginTransmission(1);
        Wire.write("M65");
        Serial.println("mermi 65");
        Wire.endTransmission();
        
        Wire.beginTransmission(1);
        Wire.write("BAS");
        Serial.println("BAS");
        Wire.endTransmission();
        
        //oyuna başla silah dostum
        can = 100;
        mermi = 65;
        digitalWrite(titresim,1);
        delay(500);
        digitalWrite(titresim,0);
      }
      else{
        Serial.println("bitti");
        Wire.beginTransmission(1);
        Wire.write("B");
        Serial.println("B");
        Wire.endTransmission();
        //oyuna bitir silah dostum
      }
    }
    else if(islem_kodu==3){
      //can durumu
    }
    else if(islem_kodu==4){
      //vurulan vuran
    }
    gelencevap = false;
  }
  


if(can!=0 && tmp_oyun_durumu){

    if (irrecv.decode(&results)) {
      // serialPrintUint64(results.value);
      int vuran = results.value; 
      if(vuran>=1 && vuran<=10){
        //solo da birbirini vuranlar var
        vuruldu(vuran,id);
        Serial.println("hit by solo");
      }
      if(results.value>=101&&results.value<=109&&takim_indeks==0){
        //mavi takimdan biri oyuncuyu vurdugunda buraya girer
        int vuran_id = vuran % 100;
        vuruldu(vuran_id,id);//once vuran sonra vurulan gonderilecek metota
        Serial.println("mavi gardas");
      }
      else if(results.value>=201&&results.value<=209&&takim_indeks==1){
        //kirmizi takimdan biri oyuncuyu vurdugunda buraya girer
        int vuran_id = vuran % 200;
        vuruldu(vuran_id,id);//once vuran sonra vurulan gonderilecek metota
        Serial.println("kirmizi gardas");
    }
      irrecv.resume();  // Receive the next value
    }
    delay(100);
    
    if (digitalRead(tetik)){

          if(mermi ==0 && sarsor == 0){
            //oyun bitti
            Serial.println("oyun bitti");
            tmp_oyun_durumu = false;
          }
          else if(mermi ==0 ){
            //mermi kalmadi 
            Serial.println("mermi kalmadi");
          }
          else{
            //sikicak
            mermi--;
            String mermi_sayisi="";
            if(mermi<10){
              mermi_sayisi="M0"+String(mermi); 
            }else{
              mermi_sayisi="M"+String(mermi); 
            }
            
            Wire.beginTransmission(1);
            Wire.write(mermi_sayisi.c_str());
            Serial.println(mermi_sayisi);
            Wire.endTransmission();
            Serial.print("mermi:");
            Serial.println(mermi);
            Serial.print("sarsor:");
            Serial.println(sarsor);
            irsend.begin();
            irsend.sendNEC(takim[takim_indeks], 32);
            irrecv.enableIRIn(); 
            delay(500);
          }
        }
      if(digitalRead(sarsor_o)){
        //sarsor degisiyor
        if(mermi == 0&&sarsor>0){
          
        Serial.println("sarsor degistiriyor");
        mermi=65;
        sarsor--;

        String mermi_sayisi="";
            if(mermi<10){
              mermi_sayisi="M0"+String(mermi); 
            }else{
              mermi_sayisi="M"+String(mermi); 
            }
            
            Wire.beginTransmission(1);
            Wire.write(mermi_sayisi.c_str());
            Serial.println(mermi_sayisi);
            Wire.endTransmission();
        
        Serial.print("Kalan sarsor:");
        Serial.println(sarsor);
        delay(200);
          }
          else if(sarsor == 0){
        //sarsor bos
        Serial.println("Sarsor bos");
      }
        
      }
      
      
     }
}


void vuruldu(int mvuran,int mvurulan){
  can--;
  istekyolla.islem = 4;//vuruldu bilgisi gonderilecegi zaman bu islem kodu calismali
  istekyolla.vuran = mvuran;
  istekyolla.vurulan = mvurulan;
  istekyolla.can = can;
  String can_sayisi="";
    if(can<10){
      can_sayisi="C00"+String(can); 
    }else if(can>=10&&can<100){
      can_sayisi="C0"+String(can); 
    }else{
      can_sayisi="C"+String(can); 
    } 
  Wire.beginTransmission(1);
  Wire.write(can_sayisi.c_str());
  Serial.println(can_sayisi);
  Wire.endTransmission();
  
  Serial.print("Vuran:");
  Serial.println(mvuran);
  Serial.print("Vurulan:");
  Serial.println(mvurulan);
  esp_now_send(alici_macadresi, (uint8_t *) &istekyolla, sizeof(istekyolla));
  digitalWrite(titresim,1);
  delay(250);
  digitalWrite(titresim,0);

}
