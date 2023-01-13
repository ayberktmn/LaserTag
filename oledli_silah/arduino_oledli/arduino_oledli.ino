#define blue 9
#define red 3
#define green 6
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_GFX.h" //Includes core graphics library
#include "Adafruit_SSD1351.h"
bool durum=false;
#define SCREEN_WIDTH 128 //pixel width
#define SCREEN_HEIGHT 128 //pixel height

#define SCLK_PIN 13 //defines s clock pin
#define MOSI_PIN 11 //defines master-out slave-in SDA pin
#define RST_PIN   2 //defines reset pin
#define DC_PIN    5 //defines master-in slave-out pin
#define CS_PIN    4 //defines chip select pinh 
// Colour definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF 
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF
#define GREY            0x8410
#define ORANGE          0xE880

extern const uint16_t smiley51[];
extern const uint16_t heart[];
extern const uint16_t timer[];
extern const uint16_t battery1[];
extern const uint16_t kirmizi[];
extern const uint16_t yesil[];
extern const uint16_t kill[];
extern const uint16_t death[];

Adafruit_SSD1351 display = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, CS_PIN, DC_PIN, RST_PIN);

String gelen="";
int mermi = 65;
int can = 100;
int dakika=0;
int saniye=60;
int pil=100;
bool state=0;
float kat_sayi = 0.6;
int gibih = 45;
int tamdoldur=65;
int timer1=0;

void setup() {
  Serial.begin(9600);
  display.begin();
  display.fillScreen(BLACK);
  //  Timer1 Ayarlanması
  TCCR1A=(1<<WGM01);    //Set the CTC mode   
  OCR1A=0xF9; //Value for ORC0A for 1ms 
  
  TIMSK1|=(1<<OCIE1A);   //Set the interrupt request
  sei(); //Enable interrupt
  
  TCCR1B|=(1<<CS01);    //Set the prescale 1/64 clock
  TCCR1B|=(1<<CS00);
  tv();
  Wire.begin(1);
  Wire.onReceive(veriGeldiginde);
  pinMode(blue,OUTPUT);
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
}
 
void rgb(int r,int g,int b){
  analogWrite(blue,b);
  analogWrite(red,r);
  analogWrite(green,g);
}




void veriGeldiginde(int veri)
{
  /* I2C hattında bu cihaz için yeni veri olduğunda bu fonksiyon çalışır */
  char gelenKarakter;
  /* Hattaki veri okunarak gelenKarakter değişkenine kaydedilir */
  while(Wire.available()){
    gelenKarakter = Wire.read();
    gelen+=gelenKarakter;
  }
  Serial.println(gelen);
  //display.fillScreen(BLACK);
  display.setCursor(0,0);
  display.setTextSize(2);
  delay(300);
  if(gelen.length()==3){
    //mermi
    String mermo = gelen.substring(0,1);
    if(gelen=="BAS"){
      
  display.fillScreen(BLACK);
      Serial.println("oyun basladi");
      durum=true;
      mermi=65;
      can=100;
      gibih = 45;
      tamdoldur=65;
      tv();
    }
    if(mermo=="M"){
        mermi = gelen.substring(1,gelen.length()).toInt();
        mermi_goster();
    }
  }
  else if(gelen.length()==4){
    //can
    String cano = gelen.substring(0,1);
    if(cano=="C"){
      can = gelen.substring(1,gelen.length()).toInt();
      kalp();
    }
  }
  else if(gelen.length()==5){
    String timero = gelen.substring(0,1);
    if(timero=="T"){
      dakika = gelen.substring(1,3).toInt();
      saniye  = gelen.substring(3,5).toInt();
      tv();
    }
  }

  
  else if(gelen=="Y"){
    rgb(0,255,0);
  }
  else if(gelen=="K"){
    rgb(255,0,0);
  }
  else if(gelen=="M"){
    rgb(0,0,255);
  }
  else if(gelen=="B"){
    display.fillScreen(BLACK);
  display.fillScreen(BLACK);
    rgb(255,255,255);
    durum=false;
    display.setTextColor(GREY);
    display.print("Bitti");
  }
  
  gelen="";
  
}





void loop() {
  if(timer1 >=550 && durum){
    //550
    state=!state;
    timer1=0;
    saniye--;
    if(saniye==0&&dakika==0){
      //oyun durumunu false a çeker 
      durum = false; 
    }
    if(saniye==0 && durum){
      dakika--;
      saniye=60;
    }
      timer_zaman(); 
  }
  
  
}







int zaman=0;

void mermi_goster(){
  Serial.println("mermiye geldi");
  display.fillRoundRect(100, 110, 50, 25, 0, BLACK);
  display.drawRGBBitmap(103,18,smiley51,25,25);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(3,113);
  tamdoldur--;
  gibih++;
  display.drawRGBBitmap(105,45,yesil,15,65);
  display.fillRoundRect(105, gibih, 15, tamdoldur, 0, GREEN);
  if(mermi == 65){
    display.setCursor(102,113);
      display.print("FL");
  }
  else if(mermi<10){
    display.setCursor(107,113);
    display.print(mermi);
  }
  else{
    display.setCursor(102,113);
      display.print(mermi);
  }
}

int id=5;
void id_goster(){
  display.setTextSize(1);
  display.setCursor(0,5);
  display.print("ID ");
  display.print(id);
  display.fillRoundRect(0, 15, 128,1, 0, WHITE);
}


void timer_zaman(){
  // play
  display.fillRoundRect(34, 34, 65, 15, 8, BLACK);
  //display.drawRGBBitmap(0,0,timer,15,15);
  if(durum==false&&saniye<=0){
    display.setTextColor(RED);
  }else{
    display.setTextColor(WHITE);
  }
  display.setTextSize(2);
  //display.setFont(&FreeMono9pt7b);
  display.setCursor(33,35);
  
  if(dakika<10){
    display.print("0");
    display.print(dakika);
  }
  else if(dakika>=10){
    display.print(dakika);
  }

  if(saniye<10){
    display.print(":0");// : kaldırdım
    display.print(saniye);
  }
  if(saniye>=10){
    display.print(":");
    display.print(saniye);
  }
  display.setFont();
}

void tv(){
  
  delay(200);
  display.fillScreen(BLACK); 
   
  id_goster();
  pil_goster();
  timer_zaman();
  mermi_goster();
  display.drawRGBBitmap(0,18,heart,25,25);
  //display.setFont(&FreeMono9pt7b);
  
  display.drawRGBBitmap(35,60,kill,24,24);
  display.setCursor(64,65);
  display.print("10");
  display.drawRGBBitmap(35,85,death,24,24);
  display.setCursor(64,92);
  display.print("0");
  
  kalp();
  
  /*
  if(can<50){
    display.setTextColor(ORANGE);
  }
  if(can>=50){
    
  display.setTextColor(GREEN);
  }
  */

  display.setFont();
}

ISR(TIMER1_COMPA_vect)
{
  timer1++;
}


void kalp(){
  display.setTextSize(2);
  display.setCursor(2,113);
  display.drawRGBBitmap(5,45,kirmizi,15,65);
  int taban = floor(kat_sayi * float(can));
  int maxo = 65-taban;
  Serial.println(taban);
  Serial.println(maxo);
  
  display.fillRoundRect(0, 110, 30, 30, 0, BLACK);
  
  if(can>=100){
    display.fillRoundRect(5, 45, 15, 65, 0, ORANGE);
    display.setTextColor(WHITE);
    display.setCursor(2,113);
    display.print("FL");
    
  }
  else{
    display.fillRoundRect(5, maxo+45, 15, taban, 0, ORANGE);
    display.setTextColor(WHITE);
      if(can<10){
      display.setCursor(6,113);
      display.print(can);
      }
      else if(can>=10){
      display.setCursor(2,113);
        display.print(can);
    }
}
}

void pil_goster(){
  display.drawRGBBitmap(103,0,battery1,30,15);
  if(pil>0&&pil<50){
    display.fillRoundRect(108, 4, 3, 7,0, ORANGE);
  }
  else if(pil>=20&&pil<80){
    display.fillRoundRect(108, 4, 3, 7,0, YELLOW);
    display.fillRoundRect(112, 4, 3, 7,0, YELLOW);  
  }
  else if(pil>=80&&pil<=100){
    display.fillRoundRect(108, 4, 3, 7,0, GREEN);
    display.fillRoundRect(112, 4, 3, 7,0, GREEN);
    display.fillRoundRect(116, 4, 3, 7,0, GREEN);
  }
  
  if(pil==100){
    display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(76,4);
  display.print(pil);
  display.print("%");  
  }
  else if(pil>=10&&pil<100){
    display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(82,4);
  display.print(pil);
  display.print("%");
  }
 else if(pil<10){
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(87,4);
  display.print(pil);
  display.print("%");
 }
  
}
