#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

SoftwareSerial bt(2,3);

#define NEO_PIN 6
Adafruit_NeoPixel neo=Adafruit_NeoPixel(30,NEO_PIN,NEO_GRB+NEO_KHZ800);

unsigned long previoustime=0;
unsigned long currenttime;

#define RED1 4 //적외선센서
#define RED2 7
#define RED3 8
#define RED4 11

int read_RED1; //적외선세로 읽은 약물의 유무를 저장하는 변수
int read_RED2;
int read_RED3;
int read_RED4;

double avr_RED1;
double avr_RED2;
double avr_RED3;
double avr_RED4;

void setup() {
  Serial.begin(9600);
  bt.begin(9600);
  neo.begin();
  neo.show();
  
//  previoustime=millis();
  currenttime=millis();
}

void setNeopixel(){     //네오픽셀 동작 클래스
  rainbow(20);
  //rainbowCycle(20);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<neo.numPixels(); i++) {
     neo.setPixelColor(i, Wheel((i+j) & 255));
    }
    neo.show();
    delay(wait);
  }
}
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { 
    for(i=0; i< neo.numPixels(); i++) {
      neo.setPixelColor(i, Wheel(((i * 256 / neo.numPixels()) + j) & 255));
    }
    neo.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return neo.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return neo.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return neo.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void colorWipe(uint32_t c, uint8_t wait){    
  for(uint16_t i=0; i<neo.numPixels(); i++){
    neo.setPixelColor(i,c);
    neo.show();
    delay(wait);
  }
}

void time_pill(){
  currenttime=millis();

    if(currenttime-previoustime<10000){  //10초
     
      avr_RED1=(avr_RED1+read_RED1)/2;
      avr_RED2=(avr_RED2+read_RED2)/2;
      avr_RED3=(avr_RED3+read_RED3)/2;
      avr_RED4=(avr_RED4+read_RED4)/2;
     
   }  
    
   avr_RED1=1;
    
    if(currenttime-previoustime=='10000'){ //안드로이드에 데이터 전송 
      
      if(avr_RED1<'1') {Serial.println("1번 복용"); bt.write("1번 복용");}
      else {Serial.println("1번 미복용");bt.write("1번 미복용");setNeopixel();} //미복용 상태 전송
      
      if(avr_RED2<'1') {Serial.println("2번 복용");bt.write("2번 복용");}
      else {Serial.println("2번 미복용");bt.write("2번 미복용");setNeopixel();} 
      
      if(avr_RED3<'1') {Serial.println("3번 복용");bt.write("3번 복용");}
      else {Serial.println("3번 미복용");bt.write("3번 미복용");setNeopixel();}
      
      if(avr_RED4<'1') {Serial.println("4번 복용");bt.write("4번 복용");}
      else {Serial.println("4번 미복용");bt.write("4번 미복용");setNeopixel();}
      
   
   } 


  if(currenttime-previoustime=='20000'){ 
     if(avr_RED1=='0') {
      Serial.println("약통을 채우세요!");
      bt.write("약통을 채우세요!");
     }        
  } 
  
  previoustime=currenttime;

}

void check_pill(){ //약물 유무 확인 클래스
    
    read_RED1=digitalRead(RED1);
    read_RED2=digitalRead(RED2);
    read_RED3=digitalRead(RED3);
    read_RED4=digitalRead(RED4);

    Serial.print("적외선 센서 1 : ");
    Serial.println(read_RED1);
    Serial.print("적외선 센서 2 : ");
    Serial.println(read_RED2);
    Serial.print("적외선 센서 3 : ");
    Serial.println(read_RED3);
    Serial.print("적외선 센서 4 : ");
    Serial.println(read_RED4);
    Serial.println("");

    time_pill();
    
    delay(1000);
}

void loop() { 
  
    check_pill(); //적외선센서 동작//나중에 블투 실행문 안으로 수정
  

    if(bt.available())
    {      
      char num=(char)bt.read();
      Serial.print("Received data : ");
      Serial.write(num);
      Serial.println("");
      
      
      
      if(num=='1'){ //알람발생 
        
        Serial.println("start neopixel");
          int i=0;
          
          while(i<5){//일정시간 경과 후 알람 종료 
                  char num2=(char)bt.read(); //사용자가 알람 종료   
                  if(num2=='2') {
                    Serial.print("Received data : ");
                    Serial.println(num2);
                    Serial.println("stop neopixel");
                    break; 
                  }
                  
                  setNeopixel(); //네오픽셀 on
                  i++;
                     
               }    
          colorWipe(neo.Color(0,0,0),0);  //네오픽셀 off         
      }
      
      else{
        colorWipe(neo.Color(0,0,0),0);
        Serial.println("stop neopixel");
      }
    }      
}
