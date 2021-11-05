#include <basicMPU6050.h>
basicMPU6050 <> mpu;

bool centeredx = false;
bool centeredy = false;
bool upJoy = false;
bool downJoy = false;
bool rightJoy = false;
bool leftJoy  = false;

int checkShake = 0;

int xPinJoy = 0;
int yPinJoy = 1;
int position = 0;
String beep = ""; 

unsigned long prevTime  = 0;
unsigned long currentTime;

unsigned long shakeEnd = 0;

int pinBuzzer = 3;

void setup() {
  // put your setup code here, to run once:
  mpu.setup();
  mpu.setBias();
  Serial.begin(9600);
  
  pinMode(pinBuzzer, OUTPUT);
}

void loop() {
//   put your main code here, to run repeatedly:

  if(Serial.available() > 0){
    beep = Serial.readString();
  }
  
  currentTime = millis();
  
  if (beep == "B"){
    prevTime = currentTime;
  }
  
  
  if(currentTime  < prevTime + 2000){
    buzz();
    beep  = "boop";
  }else{
    digitalWrite(pinBuzzer, LOW);
  }
  
  double gx = mpu.gx();
  double gy = mpu.gy();
  double gz = mpu.gz();

  double ax = mpu.ax();
  double ay = mpu.ay();
  double az = mpu.az();

//  Serial.println(String(ax) + " " + String(ay) + " " + String(az)); 

  if(ax>0){
    if(checkShake==0){
      checkShake = 1;
      shakeEnd = currentTime + 3000;
    }
    if((checkShake > 0)&((checkShake % 2) == 0)){
      if(currentTime < shakeEnd){
        checkShake++;
      }
      else{
        checkShake = 0;
      }
    }
  }
  if(ax<0){
    if((checkShake > 0)&((checkShake % 2) == 1)){
      if(currentTime < shakeEnd){
        checkShake++;
      }
      else{
        checkShake = 0;
      }
    }
  }
  
  if((gx >= -.1) & (gx<=.1)){
    centeredx = true;
    position = 0;
  }
  if((gy >= -.1) & (gy<=.1)){
    centeredy = true;
    position = 0;
  }
 
  if(((gx  < -1.5)  & centeredx)|((position == 1)&(gx  < -.5))){
    centeredx = false;
    Serial.println("Down");
    position = 1;
  }

  if(((gx  > 1.5)  & centeredx)|((position == 2)&(gx  > .5))){
    centeredx = false;
    Serial.println("Up");
    position = 2;
  }

  if(((gy  < -1.5)  & centeredy)|((position == 3)&(gy  < -.5))){
    centeredy = false;
    Serial.println("Left");
    position  = 3;
  }

  if(((gy  > 1.5)  & centeredy)|((position == 4)&(gy  > .5))){
    centeredy = false;
    Serial.println("Right");
    position = 4;
  }  
  
  if( ((analogRead(xPinJoy)) > 950)){
    rightJoy = true;
    leftJoy = false;
    upJoy = false;
    downJoy = false;
    Serial.println("Right");
  }
  else if( (analogRead(xPinJoy)) < 50 ){
    leftJoy = true;
    rightJoy = false;
    upJoy = false;
    downJoy = false;
    Serial.println("Left");
  }
  else if( (analogRead(yPinJoy)) > 950 ){
    upJoy = true;
    downJoy = false;
    leftJoy = false;
    rightJoy = false;
    Serial.println("Up");
  }
  else if((analogRead(yPinJoy)) < 50){
    downJoy = true;
    upJoy = false;
    Serial.println("Down");
  }
  if(checkShake >= 4){
    Serial.println("Shake");
  }
  
  if(currentTime > shakeEnd){
      checkShake = 0;
  }
  delay(100);
}

void buzz(){
  for(int i = 0;  i<10; i++){
    if((i%2)==0){
      digitalWrite(pinBuzzer, HIGH);
//    Serial.println(currentTime  - prevTime);
      }else{
        delay(10);
        digitalWrite(pinBuzzer, LOW);
      }
  }
}
