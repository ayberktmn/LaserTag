#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>//esp kartta mevcut yüklemeye gerek yok
#include <espnow.h>//esp kartta mevcut yüklemeye gerek yok

uint8_t friendmac[11][16] = {
   {0x34, 0xAB, 0x95, 0x24, 0x88, 0x61}//silah1
  ,{0x34, 0xAB, 0x95, 0x24, 0x8B, 0x18}//silah2
  ,{0x9C, 0x9C, 0x1F, 0x47, 0x09, 0x5D}//silah3
  ,{0x34, 0x56, 0x78, 0x90, 0x12, 0x34}
  ,{0x34, 0x56, 0x78, 0x90, 0x12, 0x34}
  ,{0x34, 0x56, 0x78, 0x90, 0x12, 0x34}
  ,{0x34, 0x56, 0x78, 0x90, 0x12, 0x34}
  ,{0x34, 0x56, 0x78, 0x90, 0x12, 0x34}
  ,{0x34, 0x56, 0x78, 0x90, 0x12, 0x34}
  ,{0x34, 0x56, 0x78, 0x90, 0x12, 0x34}
  };
  
int istekiletildi=0;
int alinanistekmesaji=0;
bool gelencevap =0;
int islem_kodu;
int tmp_vuran;
int tmp_vurulan;
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


// İstek göndermek için yarattığımız veri yapısı renk icin
struct_message istekyolla;

// gelen verileri almak için yarattığımız veri yapısı
struct_message gelenveriler;

// Veriler gönderildiğinde çalıştıracağımız fonksiyon
void VerilerGonderildiginde(uint8_t *mac_addr, uint8_t sendStatus) {
  //Serial.print("Son Verinin Gönderilme Durumu: ");
  if (sendStatus == 0){
    //Serial.println("İstek mesajı iletildi");
    istekiletildi=1;
  }
  else{
    //Serial.println("İstek İletilemedi !!!");
    istekiletildi=0;//hata mesajını oled ekranda göstermek için kullandım
  }
}

// Veriler alındığında çalışacak fonksiyon
void VerilerAlindiginda(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&gelenveriler, incomingData, sizeof(gelenveriler));
  /*
  Serial.print("Alınan veri boyutu: ");
  Serial.println(len);*/

  
  tmp_vuran=gelenveriler.vuran;
  tmp_vurulan=gelenveriler.vurulan;
  islem_kodu = gelenveriler.islem;
  
  
  gelencevap = !gelenveriler.istekgonder;//diğer karttan istek gelmiş mi bakıyoruz
  /*
  Serial.println(gelencevap);
  Serial.println();
  
  Serial.print("Vuran:");
  Serial.println(tmp_vuran);
  Serial.print("Vurulan:");
  Serial.print(tmp_vurulan);
  Serial.println();*/
}


  
#include <Wire.h> 
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display






void setup() {
  
  lcd.begin(); 
  lcd.backlight();
  lcd.home();
  Serial.begin(9600);
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

  for(int i=0;i<10;i++){
    // Bağlanılacak diğer karta bağlantıyı tanımlıyoruz
  esp_now_add_peer(friendmac[i], ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  
  // Yukarıda tanımladığımız ve veri alındığında çalışacak fonksiyonu atıyoruz
  esp_now_register_recv_cb(VerilerAlindiginda);
  }
  
}




 String gelen = ""; 
void loop() {

if(gelencevap){
    
      
    if(islem_kodu==4){
      //vurulan vuran
      String tmp_vuran1 = String(tmp_vuran);
      String tmp_vurulan1 = String(tmp_vurulan);
      Serial.println(tmp_vuran1+" "+tmp_vurulan1);
    }
    gelencevap = false;
  }

  
 if(Serial.available()){
     gelen += (char)Serial.read();
     /*
     lcd.print(gelen);
     delay(1000);
     lcd.clear();
     */
     if(gelen == "b"){
      //oyun durumu guncelleme yani 2
        lcd.print("Basladi");
        for(int i=0;i<10;i++){
          istekyolla.islem = 2;
          istekyolla.durum = true;
          esp_now_send(friendmac[i], (uint8_t *) &istekyolla, sizeof(istekyolla));
          //Serial.println("İstek Gönderilme Denendi");
          delay(200);
        }
        lcd.clear();
        gelen="";
     }
     if(gelen == "i"){
      //oyun durumu bitti bilgisini gonderiyor yani 2(işlem kodu)
      for(int i=0;i<10;i++){
          istekyolla.islem = 2;
          istekyolla.durum = false;
          esp_now_send(friendmac[i], (uint8_t *) &istekyolla, sizeof(istekyolla));
          //Serial.println("İstek Gönderilme Denendi");
          delay(200);
        }
        lcd.print("bitti");
        lcd.clear();
        gelen="";
     }
     if(gelen.length()==31){
      for(int i=0;i<gelen.length();i++){
        int catu = i-1;
      if(gelen.charAt(catu)==' '){
         int min3 = i-3;//cift basamagi gormek icin
        int min1 = i-2;//tek basamak icin
        int max1 = i-1;
         if(min3 == 27){
          min1 = min3;
         }
        int silah_id = gelen.substring(min1,max1).toInt();
        int min2 = i;
        int max2 = i+1;
        int liste_id = silah_id-1;
        String takim = gelen.substring(min2,max2);
        istekyolla.istekgonder = 1;//alıcıya istekgonder değişkenini 1 yapıp gönderiyoruz
        istekyolla.islem = 1;
        istekyolla.renk = takim;
        esp_now_send(friendmac[liste_id], (uint8_t *) &istekyolla, sizeof(istekyolla));
        //Serial.println("İstek Gönderilme Denendi");
        delay(200);
        lcd.print(takim);
        lcd.setCursor(0,1);
        lcd.print(liste_id);
        lcd.clear();
     }
     
  }
  gelen="";
}
 }

  

 
}
