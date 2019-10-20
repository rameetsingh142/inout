#include <ESP8266WiFi.h>
#define Switch D4
#define MUX_A D5
#define MUX_B D6
#define MUX_C D7
#define Switch D4
#define ANALOG_INPUT A0
#include <FirebaseArduino.h>
#include <Firebase.h>
#include "Wire.h"
// Set these to run example.
#define FIREBASE_HOST "flex2-54ea3.firebaseio.com"
#define FIREBASE_AUTH "CnpK4YvqCaDUtO5z1jVBOpCoZBfZTw4Z7lqL9I4m"
#define WIFI_SSID "hackinout legacy"
#define WIFI_PASSWORD "devfolio"
#include "Wire.h"
 
const uint8_t MPU_addr=0x68; // I2C address of the MPU-6050
  float AcZ,AcY,AcX,GyX,GyY,GyZ;
const float MPU_GYRO_250_SCALE = 131.0;
const float MPU_GYRO_500_SCALE = 65.5;
const float MPU_GYRO_1000_SCALE = 32.8;
const float MPU_GYRO_2000_SCALE = 16.4;
const float MPU_ACCL_2_SCALE = 16384.0;
const float MPU_ACCL_4_SCALE = 8192.0;
const float MPU_ACCL_8_SCALE = 4096.0;
const float MPU_ACCL_16_SCALE = 2048.0;
 
  
 
struct rawdata {
int16_t AcX;
int16_t AcY;
int16_t AcZ;
int16_t Tmp;
int16_t GyX;
int16_t GyY;
int16_t GyZ;
};
 
struct scaleddata{
float AcX;
float AcY;
float AcZ;
float Tmp;
float GyX;
float GyY;
float GyZ;
};
 
bool checkI2c(byte addr);
void mpu6050Begin(byte addr);
rawdata mpu6050Read(byte addr, bool Debug);
void setMPU6050scales(byte addr,uint8_t Gyro,uint8_t Accl);
void getMPU6050scales(byte addr,uint8_t &Gyro,uint8_t &Accl);
scaleddata convertRawToScaled(byte addr, rawdata data_in,bool Debug);
int data = 0;
int data1 = 0;
int data2 = 0;
int data3 = 0;
int data4 = 0;
String ch;
int a;
void setup() {
  //Deifne output pins for Mux
  Serial.begin(9600);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
Wire.write(0x6B); // PWR_MGMT_1 register
Wire.write(0); // set to zero (wakes up the MPU-6050)
Wire.endTransmission(true);
  pinMode(MUX_A, OUTPUT);
  pinMode(MUX_B, OUTPUT);
  pinMode(MUX_C, OUTPUT);
  pinMode(Switch,INPUT);
  Wire.begin();
Serial.begin(9600);
 
mpu6050Begin(MPU_addr);
}

void changeMux(int c, int b, int a) {
  digitalWrite(MUX_A, a);
  digitalWrite(MUX_B, b);
  digitalWrite(MUX_C, c);
}

void loop() {
rawdata next_sample;
setMPU6050scales(MPU_addr,0b00000000,0b00010000);
next_sample = mpu6050Read(MPU_addr, true);
convertRawToScaled(MPU_addr, next_sample,true);
 
delay(1000); // Wait 5 seconds and scan again
int value;
   int switchValue = digitalRead(Switch);
 
  changeMux(LOW, LOW, LOW);
  data = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 0 pin of Mux

  Serial.println(data);



  changeMux(LOW, LOW, HIGH);
  value = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 1 pin of Mux


  changeMux(LOW, HIGH, LOW);
  data1 = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 2 pin of Mux

  Serial.println(data1);



  changeMux(LOW, HIGH, HIGH);
  data2 = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 3 pin of Mux
  Serial.println(data2);

  changeMux(HIGH, LOW, LOW);
  value = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 4 pin of Mux

  changeMux(HIGH, LOW, HIGH);
  data3 = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 5 pin of Mux
  Serial.println(data3);



  changeMux(HIGH, HIGH, LOW);
  value = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 6 pin of Mux

  changeMux(HIGH, HIGH, HIGH);
  data4 = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 7 pin of Mux
  Serial.println(data4);
   if ( data < 30 && data1 >20 && data2 >20 && data3 <25 && data4 >50)
  { ch = " MESSAGE";
    String name = Firebase.pushString("logs", ch);
    
 } 
    if (  data >20 && data1 > 20 && data2 > 20 && data3 <30 && data4 >20)
  { ch = ".";
   String name = Firebase.pushString("logs", ch);
    
  } 
//   if (  data <115 && data1 < 115 && data2 < 115 && data3 <115 && data4 <115)
//  { ch = "Absolutely";
//    String name = Firebase.pushString("logs", ch);
//    
//  } 
 if ( data >20 && data1 >20 && data2 < 25 && data3 >20 && data4 <40) 
 { ch = "Thank you everyone";
    String name = Firebase.pushString("logs", ch);
    
 } 
    if ( data >15 && data1 <20 && data2 > 25 && data3 >20 && data4 <30)
  { ch = "Yes absolutely, it's obvious";
    String name = Firebase.pushString("logs", ch);
    
  } 
//   if (  data <115 && data1 < 115 && data2 < 115 && data3 >115 && data4 <115)
//  { ch = "you are talking";
//    String name = Firebase.pushString("logs", ch);
//    
//  } 
//  if (  AcY<0&&GyZ>130 &&data <115 && data1 < 115 && data2 < 115 && data3 >115 && data4 <115)
//  { ch = "like idot";
//    String name = Firebase.pushString("logs", ch);
//    
//  } 
    if (  data < 35 && data1 < 35 && data2 < 35 && data3 <25 && data4 >65)
  { ch = "A";
    String name = Firebase.pushString("logs", ch);
   delay(1000);
  } 
   if (  data >20 && data1 > 20 && data2 > 20 && data3 >20 && data4 <27)
  { ch = "B";
    String name = Firebase.pushString("logs", ch);
   delay(1000);
  } 
   if (  data < 30&&data>20 && data1 < 25 &&data1>15&& data2 < 40 && data2>20&&data3 <30 &&data3>20&& data4 <50 &&data4>30)
  { ch = "C";
    String name = Firebase.pushString("logs", ch);
   delay(1000);
  } 
   if (  data < 30 && data1 < 30 && data2 < 30 && data3 >30&& data4 <30)
  { ch = "D";
    String name = Firebase.pushString("logs", ch);
   delay(1000);
  } 
   if (  data < 30&& data1 < 30 && data2 < 29 && data3 <30 && data4 <30)
  { ch = " ";
    String name = Firebase.pushString("logs", ch);
   delay(1000);
  }
//   if (  data > 15 && data1 > 15 && data2 > 15 && data3 <28 && data4 <28)
//  { ch = "F";
//    String name = Firebase.pushString("logs", ch);
//   delay(1000);
//  }  
//   if (  AcY >-0.20 && AcY<-0.00&&GyX >0.1&&data < 30 && data1 < 30 && data2 > 30 && data3 >30 && data4 >30)
//  { ch = "G";
//    String name = Firebase.pushString("logs", ch);
//   delay(1000);
//  } 
   if (  data >25 && data1  <19 && data2 >20 && data3 >20 && data4 >50)
  { ch = "H";
    String name = Firebase.pushString("logs", ch);
   delay(1000);
  } 
    if (  data >28 && data1 < 30 && data2 < 30 && data3 <30 && data4 <30)
  { ch = "I";
    String name = Firebase.pushString("logs", ch);
   delay(1000);
  }
//      if (  AcY >-0.35 && AcY<-0.25&&data >25 && data1 < 20 && data2 < 25 && data3 <25 && data4 <25)
//  { ch = "J";
//    String name = Firebase.pushString("logs", ch);
//   delay(1000);
//  }
  if (  data < 30 && data1 <30 && data2 > 35 && data3 >35 && data4 <30)
  { ch = "K";
    String name = Firebase.pushString("logs", ch);
   delay(1000);
  } 
  if (  data <25 && data1 <25 && data2 < 25 && data3 >25&& data4 >28)
  { ch = "L";
    String name = Firebase.pushString("logs", ch);
   delay(1000);
  }
  if ( AcY>0.50&& data <30 && data1 <30 && data2 < 30 && data3 >30&& data4 >30)
  { ch = "P";
    String name = Firebase.pushString("logs", ch);
   delay(1000);
  }
  if ( data2 < 30 &&data2>37&& data3 <47&&data3>41)
  { ch = "R";
    String name = Firebase.pushString("logs", ch);
   delay(1000);
  } 
      if (  data >29 && data1 <25 && data2 < 25 && data3 <25&& data4 >50)
  { ch = "Emergency Call";
    String name = Firebase.pushString("logs", ch);
   delay(1000);
  }
        if (  data >30 && data1 <25 && data2 < 25 && data3 >30&& data4 >30)
  { ch = "T";
    String name = Firebase.pushString("logs", ch);
   delay(1000);
  }
      
  
  
   
  
}//void loop ends here
void mpu6050Begin(byte addr){
// This function initializes the MPU-6050 IMU Sensor
// It verifys the address is correct and wakes up the
// MPU.
if (checkI2c(addr)){
Wire.beginTransmission(MPU_addr);
Wire.write(0x6B); // PWR_MGMT_1 register
Wire.write(0); // set to zero (wakes up the MPU-6050)
Wire.endTransmission(true);
 
delay(30); // Ensure gyro has enough time to power up
}
}
 
bool checkI2c(byte addr){
// We are using the return value of
// the Write.endTransmisstion to see if
// a device did acknowledge to the address.
Serial.println(" ");
Wire.beginTransmission(addr);
 
if (Wire.endTransmission() == 0)
{
Serial.print(" Device Found at 0x");
Serial.println(addr,HEX);
return true;
}
else
{
Serial.print(" No Device Found at 0x");
Serial.println(addr,HEX);
return false;
}
}
 
  
 
rawdata mpu6050Read(byte addr, bool Debug){
// This function reads the raw 16-bit data values from
// the MPU-6050
 
rawdata values;
 
Wire.beginTransmission(addr);
Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
Wire.endTransmission(false);
Wire.requestFrom(addr,14,true); // request a total of 14 registers
values.AcX=Wire.read()<<8|Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
values.AcY=Wire.read()<<8|Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
values.AcZ=Wire.read()<<8|Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
values.Tmp=Wire.read()<<8|Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
values.GyX=Wire.read()<<8|Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
values.GyY=Wire.read()<<8|Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
values.GyZ=Wire.read()<<8|Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
 
 
if(Debug){
Serial.print(" GyX = "); Serial.print(values.GyX);
Serial.print(" | GyY = "); Serial.print(values.GyY);
Serial.print(" | GyZ = "); Serial.print(values.GyZ);  
Serial.print(" | AcX = "); Serial.print(values.AcX);
Serial.print(" | AcY = "); Serial.print(values.AcY);
Serial.print(" | AcZ = "); Serial.println(values.AcZ);
}
 
return values;
}
 
void setMPU6050scales(byte addr,uint8_t Gyro,uint8_t Accl){
Wire.beginTransmission(addr);
Wire.write(0x1B); // write to register starting at 0x1B
Wire.write(Gyro); // Self Tests Off and set Gyro FS to 250
Wire.write(Accl); // Self Tests Off and set Accl FS to 8g
Wire.endTransmission(true);
}
 
void getMPU6050scales(byte addr,uint8_t &Gyro,uint8_t &Accl){
Wire.beginTransmission(addr);
Wire.write(0x1B); // starting with register 0x3B (ACCEL_XOUT_H)
Wire.endTransmission(false);
Wire.requestFrom(addr,2,true); // request a total of 14 registers
Gyro = (Wire.read()&(bit(3)|bit(4)))>>3;
Accl = (Wire.read()&(bit(3)|bit(4)))>>3;
}
 
  
 
scaleddata convertRawToScaled(byte addr, rawdata data_in, bool Debug){
 
scaleddata values;
float scale_value = 0.0;
byte Gyro, Accl;
 
getMPU6050scales(MPU_addr, Gyro, Accl);
 
if(Debug){
Serial.print("Gyro Full-Scale = ");
}
 
switch (Gyro){
case 0:
scale_value = MPU_GYRO_250_SCALE;
if(Debug){
Serial.println("±250 °/s");
}
break;
case 1:
scale_value = MPU_GYRO_500_SCALE;
if(Debug){
Serial.println("±500 °/s");
}
break;
case 2:
scale_value = MPU_GYRO_1000_SCALE;
if(Debug){
Serial.println("±1000 °/s");
}
break;
case 3:
scale_value = MPU_GYRO_2000_SCALE;
if(Debug){
Serial.println("±2000 °/s");
}
break;
default:
break;
}
 
values.GyX = (float) data_in.GyX / scale_value;
values.GyY = (float) data_in.GyY / scale_value;
values.GyZ = (float) data_in.GyZ / scale_value;
 
scale_value = 0.0;
if(Debug){
Serial.print("Accl Full-Scale = ");
}
switch (Accl){
case 0:
scale_value = MPU_ACCL_2_SCALE;
if(Debug){
Serial.println("±2 g");
}
break;
case 1:
scale_value = MPU_ACCL_4_SCALE;
if(Debug){
Serial.println("±4 g");
}
break;
case 2:
scale_value = MPU_ACCL_8_SCALE;
if(Debug){
Serial.println("±8 g");
}
break;
case 3:
scale_value = MPU_ACCL_16_SCALE;
if(Debug){
Serial.println("±16 g");
}
break;
default:
break;
}
values.AcX = (float) data_in.AcX / scale_value;
values.AcY = (float) data_in.AcY / scale_value;
values.AcZ = (float) data_in.AcZ / scale_value;
 
  
 
values.Tmp = (float) data_in.Tmp / 340.0 + 36.53;
 
if(Debug){
Serial.print(" GyX = "); Serial.print(values.GyX);
Serial.print(" °/s| GyY = "); Serial.print(values.GyY);
Serial.print(" °/s| GyZ = "); Serial.print(values.GyZ);

Serial.print(" °C| AcX = "); Serial.print(values.AcX);
Serial.print(" g| AcY = "); Serial.print(values.AcY);
Serial.print(" g| AcZ = "); Serial.print(values.AcZ);Serial.println(" g");
AcX=values.AcX;
AcY=values.AcY;
AcZ=values.AcZ;
GyX=values.GyX;
GyY=values.GyY;
GyZ=values.GyZ;
}
 
return values;
}
 
