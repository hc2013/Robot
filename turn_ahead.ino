#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <IRremote.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMID  375
#define SERVOSUB  165

#define MID1 345 
#define MID2 345  
#define MID3 345

#define MID5 342.5
#define MID6 357.5
#define MID7 365

#define MID9 350
#define MID10 356
#define MID11 345

#define MID13 355
#define MID14 335
#define MID15 340

#define INTERVAL 15

#define LEFT -2
#define RIGHT 2
#define FRONT 1
#define BACK -1
#define STOP 0

int num1[]={1,2,3,9,10,11};
int num2[]={5,6,7,13,14,15};

int mid1[]={MID1,MID2,MID3,MID9,MID10,MID11};
int mid2[]={MID5,MID6,MID7,MID13,MID14,MID15};

int RECV_PIN=2;
IRrecv irrecv(RECV_PIN);
decode_results result;

int state=0;

void setup() {
  Serial.begin(9600);
  
  pwm.begin();
  pwm.setPWMFreq(60);  
  
  irrecv.enableIRIn();

  attachInterrupt(0,dealInt,CHANGE);
}

void loop() {
   switch(state){
      case FRONT:
        goAhead(FRONT);
        Serial.println("front");
        break;
      case BACK:
        goAhead(BACK);
        Serial.println("back");
        break;
      case LEFT:
        turn(LEFT,15);
        Serial.println("left");
        break;
      case RIGHT:
        turn(RIGHT,15);
        Serial.println("right");
        break;
      case STOP:
        stop();
        Serial.println("stop");
        break;
   }
}

void stop(){
  
}
void turn(int dir,int angle){
   int number1[6]={0};
   int number2[6]={0};
   int middle1[6]={0};
   int middle2[6]={0};
   if(dir==LEFT){
      memcpy(number1,num1,sizeof(num1));
      memcpy(number2,num2,sizeof(num2));
      memcpy(middle1,mid1,sizeof(mid1));
      memcpy(middle2,mid2,sizeof(mid2));
   }
   else{
     memcpy(number1,num2,sizeof(num2));
     memcpy(number2,num1,sizeof(num1));
     memcpy(middle1,mid2,sizeof(mid2));
     memcpy(middle2,mid1,sizeof(mid1));
   }
   for(int k=0;k<=SERVOMAX-SERVOMID-SERVOSUB;k++){
    if(dir==RIGHT){
      k*=-1;
    }
    pwm.setPWM(number1[4], 0, middle1[4]+k);
    pwm.setPWM(number1[3], 0, middle1[3]+k);
    pwm.setPWM(number1[1], 0, middle1[1]-k);
    pwm.setPWM(number1[0], 0, middle1[0]-k);
    if(dir==RIGHT){
      k*=-1;
    }
    delay(INTERVAL);     
  }
  for(int k=0;k<=angle;k++){
    if(dir==RIGHT){
      k*=-1;
    }
    pwm.setPWM(number1[2], 0, middle1[2]+k);
    pwm.setPWM(number1[5], 0, middle1[5]-k);
    if(dir==RIGHT){
      k*=-1;
    }
    delay(INTERVAL); 
  }
  //斜前迈
  
  for(int k=0;k<=angle;k++){
    if(dir==RIGHT){
      k*=-1;
    }
    pwm.setPWM(number2[4], 0, middle2[4]+k);
    pwm.setPWM(number2[3], 0, middle2[3]+k);
    pwm.setPWM(number2[1], 0, middle2[1]-k);
    pwm.setPWM(number2[0], 0, middle2[0]-k);
    pwm.setPWM(number2[5], 0, middle2[5]+k);
    pwm.setPWM(number2[2], 0, middle2[2]-k);
    if(dir==RIGHT){
      k*=-1;
    }
    delay(INTERVAL);
  }
  for(int k=angle;k<=SERVOMAX-SERVOMID-SERVOSUB;k++){
    if(dir==RIGHT){
      k*=-1;
    }
    pwm.setPWM(number2[4], 0, middle2[4]+k);
    pwm.setPWM(number2[3], 0, middle2[3]+k);
    pwm.setPWM(number2[1], 0, middle2[1]-k);
    pwm.setPWM(number2[0], 0, middle2[0]-k);
    if(dir==RIGHT){
      k*=-1;
    }
    delay(INTERVAL); 
  }
  //斜后蹬
  
  
  for(int k=SERVOMAX-SERVOMID-SERVOSUB;k>=angle;k--){
    if(dir==RIGHT){
      k*=-1;
    }
    pwm.setPWM(number1[4], 0, middle1[4]+k);
    pwm.setPWM(number1[3], 0, middle1[3]+k);
    pwm.setPWM(number1[1], 0, middle1[1]-k);
    pwm.setPWM(number1[0], 0, middle1[0]-k);
    if(dir==RIGHT){
      k*=-1;
    }
    delay(INTERVAL);
  }
  for(int k=angle;k>=0;k--){
    if(dir==RIGHT){
      k*=-1;
    }
   pwm.setPWM(number1[4], 0, middle1[4]+k);
    pwm.setPWM(number1[3], 0, middle1[3]+k);
    pwm.setPWM(number1[1], 0, middle1[1]-k);
    pwm.setPWM(number1[0], 0, middle1[0]-k);
    pwm.setPWM(number1[2], 0, middle1[2]+k);
    pwm.setPWM(number1[5], 0, middle1[5]-k);
    if(dir==RIGHT){
      k*=-1;
    }
    delay(INTERVAL);
  }
  //斜前迈恢复
  
  for(int k=angle;k>=0;k--){
    if(dir==RIGHT){
      k*=-1;
    }
    pwm.setPWM(number2[5], 0, middle2[5]+k);
    pwm.setPWM(number2[2], 0, middle2[2]-k);
    if(dir==RIGHT){
      k*=-1;
    }
    delay(INTERVAL);
  }
  
  int sub=SERVOMAX-SERVOMID-SERVOSUB;
  for(int k=0;k<=sub;k++){
    if(dir==RIGHT){
      k*=-1;
      sub*=-1;
    }
     pwm.setPWM(number2[4], 0, middle2[4]+sub-k);
     pwm.setPWM(number2[3], 0, middle2[3]+sub+k);
     pwm.setPWM(number2[1], 0, middle2[1]-sub+k);
     pwm.setPWM(number2[0], 0, middle2[0]-sub-k);
     delay(INTERVAL);
     if(dir==RIGHT){
      k*=-1;
      sub*=-1;
    }
  }
  for(int k=2*sub;k>=0;k--){
     if(dir==RIGHT){
      k*=-1;
     }
     pwm.setPWM(number2[3], 0, middle2[3]+k);
     pwm.setPWM(number2[0], 0, middle2[0]-k);
     if(dir==RIGHT){
      k*=-1;
    }
     delay(INTERVAL);
  }
  
   //斜后蹬恢复
  delay(200);
  //////////////////////////////////////////////半个周期完成
}
void goAhead(int dir){
   setAllVertical();
   delay(1000);
  
   int number1[6]={0};
   int number2[6]={0};
   int middle1[6]={0};
   int middle2[6]={0};
   if(dir==LEFT){
      memcpy(number1,num1,sizeof(num1));
      memcpy(number2,num2,sizeof(num2));
      memcpy(middle1,mid1,sizeof(mid1));
      memcpy(middle2,mid2,sizeof(mid2));
   }
   else{
     memcpy(number1,num2,sizeof(num2));
     memcpy(number2,num1,sizeof(num1));
     memcpy(middle1,mid2,sizeof(mid2));
     memcpy(middle2,mid1,sizeof(mid1));
   }
  
  int k=0;
  for(;k<=SERVOMAX-SERVOMID-SERVOSUB;k++){
    pwm.setPWM(number1[1], 0, middle1[1]+k);
    pwm.setPWM(number1[0], 0, middle1[0]+k);
    pwm.setPWM(number1[4], 0, middle1[4]-k);
    pwm.setPWM(number1[3], 0, middle1[3]-k);
    delay(15);     
  }//前迈
  
  k=0;
  for(;k<=SERVOMAX-SERVOMID-SERVOSUB;k++){
    pwm.setPWM(number2[1], 0, middle2[1]+k);
    pwm.setPWM(number2[0], 0, middle2[0]+k);
    pwm.setPWM(number2[4], 0, middle2[4]+k);
    pwm.setPWM(number2[3], 0, middle2[3]+k);
    delay(15);
  }//后蹬

  k=SERVOMAX-SERVOMID-SERVOSUB;
  for(;k>=0;k--){
    pwm.setPWM(number1[1], 0, middle1[1]+k);
    pwm.setPWM(number1[0], 0, middle1[0]+k);
    pwm.setPWM(number1[4], 0, middle1[4]-k);
    pwm.setPWM(number1[3], 0, middle1[3]-k);
    delay(15);
  }//前迈恢复
  
  k=SERVOMAX-SERVOMID-SERVOSUB;
  
  for(int i=0;i<=20;i++){
    pwm.setPWM(number2[0], 0, middle2[0]+k+i);
    pwm.setPWM(number2[3], 0, middle2[3]-k-i);
    delay(10);
  }
  
  for(;k>=0;k--){
     pwm.setPWM(number2[1], 0, middle2[1]+k);
    pwm.setPWM(number2[4], 0, middle2[4]-k);
    delay(15);
  }
  
  k=SERVOMAX-SERVOMID-SERVOSUB;
  for(;k>=-20;k--){
    pwm.setPWM(number2[0], 0, middle2[0]+k+20);
    pwm.setPWM(number2[3], 0, middle2[3]-k-20);
    delay(15);
  }
  //后蹬恢复
  delay(200);
  
  //////////////////////////////////////////////半个周期完成
  k=0;
  for(;k<=SERVOMAX-SERVOMID-SERVOSUB;k++){
    pwm.setPWM(number2[1], 0, middle2[1]-k);
    pwm.setPWM(number2[0], 0, middle2[0]-k);
    pwm.setPWM(number2[4], 0, middle2[4]+k);
    pwm.setPWM(number2[3], 0, middle2[3]+k);
    delay(15);     
  }

  k=0;
  for(;k<=SERVOMAX-SERVOMID-SERVOSUB;k++){
    pwm.setPWM(number1[1], 0, middle1[1]-k);
    pwm.setPWM(number1[0], 0, middle1[0]-k);
    pwm.setPWM(number1[4], 0, middle1[4]+k);
    pwm.setPWM(number1[3], 0, middle1[3]+k);
    delay(15);
  }

  k=SERVOMAX-SERVOMID-SERVOSUB;
  for(;k>=0;k--){
     pwm.setPWM(number2[1], 0, middle2[1]-k);
    pwm.setPWM(number2[0], 0, middle2[0]-k);
    pwm.setPWM(number2[4], 0, middle2[4]+k);
    pwm.setPWM(number2[3], 0, middle2[3]+k);
    delay(15);
  }
  
  k=SERVOMAX-SERVOMID-SERVOSUB;

  for(int i=0;i<=20;i++){
    pwm.setPWM(number1[0], 0, middle1[0]-k-i);
    pwm.setPWM(number1[3], 0, middle1[3]+k+i);
    delay(10);
  }
  
  for(;k>=0;k--){
    pwm.setPWM(number1[1], 0, middle1[1]+k);
    pwm.setPWM(number1[4], 0, middle1[4]-k);
    delay(15);
  }
  
  k=SERVOMAX-SERVOMID-SERVOSUB;
  for(;k>=-20;k--){
    pwm.setPWM(number1[0], 0, middle1[0]-k-20);
    pwm.setPWM(number1[3], 0, middle1[3]+k+20);
    delay(15);
  }
   delay(200);  
}

void setAllVertical(){
  pwm.setPWM(1, 0,SERVOMID-30 );
  pwm.setPWM(2, 0,SERVOMID-30 );
  pwm.setPWM(3, 0,SERVOMID-30);
  
  pwm.setPWM(5, 0,SERVOMID-32.5 );
  pwm.setPWM(6, 0,SERVOMID-17.5 );
  pwm.setPWM(7, 0,SERVOMID-10 );
  
  pwm.setPWM(9, 0,SERVOMID-25 );
  pwm.setPWM(10, 0,SERVOMID-18.5 );
  pwm.setPWM(11, 0,SERVOMID-30 );
  
  pwm.setPWM(13, 0,SERVOMID-20 );
  pwm.setPWM(14, 0,SERVOMID-40 );
  pwm.setPWM(15, 0,SERVOMID-35  );
}

void dealInt(){
  if(irrecv.decode(&result)){ 
      Serial.println("--------------------------");
      Serial.println(result.value,HEX);
      if(result.value!=0xFFFFFFFF){
          changeState(result.value);
      }
      Serial.println("--------------------------");
      irrecv.resume();
  }
}

void changeState(int val){
  switch(val){
    case 0xFF18E7:
    state=FRONT;
    break;
    case 0xFF4AB5:
    state=BACK;
    break;
    case 0xFF10EF:
    state=LEFT;
    break;
    case 0xFF5AA5:
    state=RIGHT;
    break;
    case 0xFF38C7:
    state=STOP;
    break;
  }
  Serial.println("state=");
  Serial.println(state,HEX);
}

