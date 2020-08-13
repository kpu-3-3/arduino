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

String myString="";




 
void setNeopixel(){     //네오픽셀 동작 클래스
    
        Serial.println("start neopixel");
          int i=0;
          
          while(i<5){//일정시간 경과 후 알람 종료 
                  char num=(char)bt.read(); //사용자가 알람 종료   
                  if(num=='2') {
                    Serial.print("Received data : ");
                    Serial.println(num);
                    Serial.println("stop neopixel");
                    break; 
                  }
                  rainbow(15); //네오픽셀 on
                  i++;
                     
               }    
          colorWipe(neo.Color(0,0,0),0);  //네오픽셀 off              
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

uint32_t Wheel(byte WheelPos) { //led별 색상 제어
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


void check_pill(int check_num){//적외선 센서로 약물 여부 확인
   
   if(check_num==1){
    while(1){
      read_RED1=digitalRead(RED1);
      Serial.print("적외선 센서 1 : ");
      Serial.println(read_RED1); //0일 때 약물 미복용 //1일 때 약물 복용
      
      if(read_RED1==1){
        Serial.println("1번 복용");
        bt.println("10");//복용 상태 전송

        break;
      }
      else{
        Serial.println("1번 미복용");
        bt.println("18");//미복용 상태 전송
        
        setNeopixel();//미복용 알람
      }
      delay(10000);//테스트용이라 10초     
    }
   }
   
   else if(check_num==2){
    while(1){
      read_RED2=digitalRead(RED2);
      Serial.print("적외선 센서 2 : ");
      Serial.println(read_RED2); //0일 때 약물 미복용 //1일 때 약물 복용
      
      if(read_RED2==1){
        Serial.println("2번 복용");
        bt.println("20");//복용 상태 전송

        break;
      }
      else{
        Serial.println("2번 미복용");
        bt.println("28");//미복용 상태 전송
        
        setNeopixel();//미복용 알람
      }
      delay(10000);//테스트용이라 10초     
    }
   }

    
   else if(check_num==3){
    while(1){
      read_RED3=digitalRead(RED3);
      Serial.print("적외선 센서 3 : ");
      Serial.println(read_RED3); //0일 때 약물 미복용 //1일 때 약물 복용
      
      if(read_RED3==1){
        Serial.println("3번 복용");
        bt.println("30");//복용 상태 전송

        break;
      }
      else{
        Serial.println("3번 미복용");
        bt.println("38");//미복용 상태 전송
        
        setNeopixel();//미복용 알람
      }
      delay(10000);//테스트용이라 10초     
    }
   }
    
   else if(check_num==4){
    while(1){
      read_RED4=digitalRead(RED4);
      Serial.print("적외선 센서 4: ");
      Serial.println(read_RED4); //0일 때 약물 미복용 //1일 때 약물 복용
      
      if(read_RED4==1){
        Serial.println("4번 복용");
        bt.println("40");//복용 상태 전송
        //약통 채우기 알람 함수 호출
        break;
      }
      else{
        Serial.println("4번 미복용");
        bt.println("48");//미복용 상태 전송
        
        setNeopixel();//미복용 알람
      }
      delay(10000);//테스트용이라 10초     
    }
   }
      
    else{
      Serial.println("타입 오류2");
    }
  }
  
void fill_pill_box(){//약통 채웠는지 확인 
  while(1){
  for(int i=0;i<20;i++){
    avr_RED1=(avr_RED1+read_RED1)/2;
    avr_RED2=(avr_RED2+read_RED2)/2;
    avr_RED3=(avr_RED3+read_RED3)/2;
    avr_RED4=(avr_RED4+read_RED4)/2;
  
  if(avr_RED1>=0.9&&avr_RED2>=0.9&&avr_RED3>=0.9&&avr_RED4>=0.9){//약통이 빈 경우
   setNeopixel();
   Serial.println("약통을 채우세요");
   bt.println("약통을 채우세요");
   }
  }
  if((avr_RED1<0.9)&&(avr_RED2<0.9)&&(avr_RED3<0.9)&&(avr_RED4<0.9)) { //약통을 채운경우
    Serial.println("약통을 채웠습니다!");
    bt.println("약통을 채웠습니다");
    break;
  }
  delay(1000);
 }
}  
void time_pill(int type_num){
     
  switch(type_num){
    case 1:
      check_pill(1);
      break;
      
    case 2:
      check_pill(2);
      break;
    
    case 3:
      check_pill(3);
      break;
    
    case 4:
      check_pill(4);
      delay(30000);    
      fill_pill_box();//약통채우기 알람 발생
      break;
      
    default : 
      Serial.println("타입 오류1");
      break;
    }
 }


void receive_message(){
    while(bt.available()){
      char data=(char)bt.read();//메세지 저장
      myString+=data;
      delay(5);
    }
      
    if(!myString.equals(""))
    {
      Serial.println(" send message : " + myString);
      if(myString.equals("type1")){
      setNeopixel();
      delay(10000); //테스트용이라 10초 //나중에 1800*1000=1800000=30분으로 수정하기 
      time_pill(1); 
    }       
    else if(myString.equals("type2")){
      setNeopixel();
      delay(10000); //테스트용이라 10초 //나중에 1800*1000=1800000=30분으로 수정하기 
      time_pill(2);

    }
    else if(myString.equals("type3")){
      setNeopixel();
      delay(10000); //테스트용이라 10초 //나중에 1800*1000=1800000=30분으로 수정하기 
      time_pill(3);
    }
    else if(myString.equals("type4")){
      setNeopixel();
      delay(10000); //테스트용이라 10초 //나중에 1800*1000=1800000=30분으로 수정하기 
      time_pill(4);
    }

      myString="";
    }        

    

}

void loop() { 

  receive_message();

}
