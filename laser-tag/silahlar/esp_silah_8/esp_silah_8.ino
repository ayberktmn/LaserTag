int id = 8;//silah in kendi id si
uint8_t takim[] = {0xD0,0x6C};//0.indeks kirmizi , 1.indeks mavi
int takim_indeks;//bu senin hanki takımda olduguna gore ates ettiginde hangisinin secilecegini soyler
#include <ESP8266WiFi.h>//esp kartta mevcut yüklemeye gerek yok
#include <espnow.h>//esp kartta mevcut yüklemeye gerek yok
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>
#define irvericipin 4//D2
#define iralicipin 5//D1
#define degistirme 0//D3
#define titresim 14//D5
#define red 15//D8
#define blue 13//D7
#define tetik 16//d0
IRsend irsend(irvericipin); // IR pin
IRrecv irrecv(iralicipin);
int can = 15;
int sarsor = 2;
int mermi = 5;
decode_results results;
int alinanistekmesaji=0;
int istekiletildi=0;
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
  tmp_takim= gelenveriler.renk;
  tmp_oyun_durumu= gelenveriler.durum;
  tmp_can= gelenveriler.can;
  islem_kodu = gelenveriler.islem;
  
  
  gelencevap = gelenveriler.istekgonder;//diğer karttan istek gelmiş mi bakıyoruz
  //takim = gelenveriler.renk;//diğer karttan istek gelmiş mi bakıyoruz
}

  
void setup() {
  Serial.begin(9600);
  pinMode(degistirme,INPUT);
  pinMode(tetik,INPUT);
  pinMode(red,OUTPUT);
  pinMode(blue,OUTPUT);
  irrecv.enableIRIn(); // IR haberleşme başlatıldı.
  // Cihazı station olarak tanımlayıp bağlıysa ağlardan çıkıyoruz
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
          digitalWrite(red,1);
          digitalWrite(blue,0);
          takim_indeks = 0;//kirmizi takima ayarlandı verici
        }
        if(tmp_takim=="M"){
          digitalWrite(red,0);
          digitalWrite(blue,1);
          takim_indeks = 1;//mavi takima ayarlandı verici
        }
      
    }
    else if(islem_kodu==2){
      //oyun durumu
      if(tmp_oyun_durumu){
        //oyuna başla silah dostum
        digitalWrite(titresim,1);
        delay(500);
        digitalWrite(titresim,0);
      }
      else{
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
      if(results.value>=101&&results.value<=109&&tmp_takim!="M"){
        //mavi takimdan biri oyuncuyu vurdugunda buraya girer
        int vuran_id = vuran % 100;
        vuruldu(vuran_id,id);//once vuran sonra vurulan gonderilecek metota
        Serial.println("mavi gardas");
      }
      else if(results.value>=201&&results.value<=209&&tmp_takim!="K"){
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
            Serial.print("mermi:");
            Serial.println(mermi);
            Serial.print("sarsor:");
            Serial.println(sarsor);
            irsend.begin();
            irsend.sendNEC(takim[takim_indeks], 32);
            irrecv.enableIRIn(); 
            
          }
        }
      if(digitalRead(degistirme)){
        //sarsor degisiyor
        if(mermi == 0&&sarsor>0){
          
        Serial.println("sarsor degistiriyor");
        
        mermi=5;
        sarsor--;
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
  istekyolla.islem = 4;//vuruldu bilgisi gonderilecegi zaman bu islem kodu calismali
  istekyolla.vuran = mvuran;
  istekyolla.vurulan = mvurulan;
  esp_now_send(alici_macadresi, (uint8_t *) &istekyolla, sizeof(istekyolla));
  delay(250);
}
