/*
#include "SPI.h" //Includes library for SPI communication of display
#include "Adafruit_GFX.h" //Includes core graphics library
#include "Adafruit_SSD1351.h" //Includes hardware specific library
#define tetik 2
int timer1=0;
bool state=0;
int dakika=0;
int saniye=60;
int pil=50;
extern const uint16_t smiley51[];
extern const uint16_t heart[];
extern const uint16_t timer[];
extern const uint16_t battery1[];
//screen dimensions
#define SCREEN_WIDTH 128 //pixel width
#define SCREEN_HEIGHT 128 //pixel height

//pin definitions
#define SCLK_PIN 13 //defines s clock pin
#define MOSI_PIN 11 //defines master-out slave-in SDA pin
#define RST_PIN   3 //defines reset pin
#define DC_PIN    5 //defines master-in slave-out pin
#define CS_PIN    4 //defines chip select pin
int can = 30;
int zaman=0;
// Colour definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF
#define GREY            0x8410
#define ORANGE          0xE880
int mermi = 50;
String can_renk = "GREEN";
Adafruit_SSD1351 display = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, CS_PIN, DC_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  display.begin();
  display.fillScreen(BLACK);
  pinMode(tetik,INPUT);
  //  Timer1 Ayarlanması
  TCCR1A=(1<<WGM01);    //Set the CTC mode   
  OCR1A=0xF9; //Value for ORC0A for 1ms 
  
  TIMSK1|=(1<<OCIE1A);   //Set the interrupt request
  sei(); //Enable interrupt
  
  TCCR1B|=(1<<CS01);    //Set the prescale 1/64 clock
  TCCR1B|=(1<<CS00);
  tv();
}
bool durum = true;

void loop() {
  if(timer1 >=100 && durum){
    //550
    state=!state;
    timer1=0;
    saniye--;
    if(saniye==0&&dakika==0){
      display.setCursor(0,22);
      display.print("Bitti");
      durum = false; 
    }
    if(saniye==0){
      dakika--;
      saniye=60;
    }
      timer_zaman(); 
  }
  
  if(digitalRead(tetik)){
    mermi--;
    Serial.println(mermi);
    if(mermi==0){
      mermi=50;
    }
      
    mermi_goster();
    delay(200);
  }
}

void mermi_goster(){
  display.fillRoundRect(99, 100, 50, 25, 8, BLACK);
  display.drawRGBBitmap(73,99,smiley51,25,25);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(100,105);
  display.print(mermi);
}


void timer_zaman(){
  // play
  display.fillRoundRect(0, 0, 70, 15, 8, BLACK);
  display.drawRGBBitmap(0,0,timer,15,15);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(17,5);
  if(dakika<10){
    display.print("0");
    display.print(dakika);
  }
  else if(dakika>=10){
    display.print(dakika);
  }

  if(saniye<10){
    display.print(":0");
    display.print(saniye);
  }
  if(saniye>=10){
    display.print(":");
    display.print(saniye);
  }
}
void tv(){
  
  delay(200);
  display.fillScreen(BLACK); 
  
  display.setTextSize(2);
  display.setCursor(0,65);
  display.print("Kill:");
  display.setTextColor(GREEN);
  display.print("10");
  display.setCursor(0,45);
  display.print("ID:1");
  pil_goster();
  timer_zaman();
  mermi_goster();
  display.drawRGBBitmap(0,100,heart,25,25);
  display.setCursor(26,105);
  display.setTextColor(WHITE);
  if(can<50){
    display.setTextColor(ORANGE);
  }
  if(can>=50){
    
  display.setTextColor(GREEN);
  }
  display.print(can);
}

ISR(TIMER1_COMPA_vect)
{
  timer1++;
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
*/
