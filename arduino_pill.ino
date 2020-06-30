#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

SoftwareSerial bt(2,3);

#define NEO_PIN 6
Adafruit_NeoPixel neo=Adafruit_NeoPixel(30,NEO_PIN,NEO_GRB+NEO_KHZ800);   

#define RED1 4 //적외선센서 //1일때가 약물 없음(복용상태)
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
}


void setNeopixel(){     //네오픽셀 동작 클래스
    
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
                  rainbow(15); //네오픽셀 on
                  i++;
                     
               }    
          colorWipe(neo.Color(0,0,0),0);  //네오픽셀 off         
      }
      
      else{
        colorWipe(neo.Color(0,0,0),0);
        Serial.println("stop neopixel");
      }
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

void colorWipe(uint32_t c, uint8_t wait){    //led마다 개별 색상 설정
  for(uint16_t i=0; i<neo.numPixels(); i++){
    neo.setPixelColor(i,c);
    neo.show();
    delay(wait);
  }
}

int prev=0; //알람 시간 비교 변수
int cur=0;

int set_1=0;//메세지 전송여부 상태 변수, 전송 전 상태=0 전송 후=1, 재전송방지
int set_2=0;
int set_3=0;
int set_4=0;

void time_pill(){ //안드로이드에 데이터 전송

     cur++;

     if(cur-prev<10){  //test 용이라 10초
     
      avr_RED1=(avr_RED1+read_RED1)/2;
      avr_RED2=(avr_RED2+read_RED2)/2;
      avr_RED3=(avr_RED3+read_RED3)/2;
      avr_RED4=(avr_RED4+read_RED4)/2;    
      }  
    
    if(cur-prev==10){ //안드로이드에 데이터 전송 
      
      if(avr_RED1>=0.9) { //복용상태전송
        Serial.println("1번 복용"); 
        if(set_1==0){
           Serial.println("1)메세지 전송");
           set_1=1; //메세지 전송여부확인
           bt.print("10");//복용 상태 전송
           }
        } 
      else {Serial.println("1번 미복용");bt.print("18/n");setNeopixel();}//미복용상태전송
        
      
      if(avr_RED2>=0.9) {
        Serial.println("2번 복용");
        if(set_2==0){
          Serial.println("2)메세지 전송");
          set_2=1;
          bt.print("20");
          }
        }
      else {Serial.println("2번 미복용");bt.print("28/n");setNeopixel();}

      if(avr_RED3>=0.9){
        Serial.println("3번 복용");
        if(set_3==0){
          Serial.println("3)메세지 전송");
          set_3=1;
          bt.print("30");
          }
        }
      else  {Serial.println("3번 미복용");bt.print("38/n");setNeopixel();}
        
      if(avr_RED4>=0.9) {
        Serial.println("4번 복용");
        if(set_4==0){
          Serial.println("4)메세지 전송");
          set_4=1;
          bt.print("40");
          }
       }
      else{Serial.println("4번 미복용");bt.print("48/n");setNeopixel();}
      
   } 

  if(cur-prev==20){ //1000*60*60*18
     
     if((avr_RED1>=0.9)||(avr_RED2>=0.9)||(avr_RED3>=0.9)||(avr_RED4>=0.9)) 
     {
      prev=20;
      Serial.println("약통을 채우세요!\n");
      bt.write("약통을 채우세요!");
     }
    delay(5000);
    
    if((avr_RED1<0.9)&&(avr_RED2<0.9)&&(avr_RED3<0.9)&&(avr_RED4<0.9)) { //약통을 채운경우
      
      Serial.println("약통을 채웠습니다!\n");
      
      cur=0;
      prev=0;
       
      set_1=0;
      set_2=0;
      set_3=0;
      set_4=0;
       
    }    
  } 
  
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
    
    check_pill(); //적외선센서 동작
    
      
    if(bt.available())
    {  
      setNeopixel();
    }      
}
