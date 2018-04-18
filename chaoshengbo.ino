#include <Syn6288.h>

#include <Adafruit_BMP085.h>

#include <OneWire.h>

#include <Wire.h>

const int RedledPin = 13;      // 连接红色LED的引脚
const int GreenledPin = 12;      // 连接LED的引脚

const int echopin=5; // echo接5端口
const int trigpin=4; // trig接4端口  左侧超声波测距器


const int echopin2=3;
const int trigpin2=2;    // 右侧超声波测距器


const int echopin3=6;
const int trigpin3=7;   // 前方超声波测距器


const int dq = 10;// DS18B20温度传感器
OneWire ds(dq);

const int PIN_SPEAKER = 8;  


boolean initflag;

uint8_t text3[]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};   //0,1,2,3,4,5,6,7,8,9

uint8_t tempsound[];

void setup(void)
{
  Serial.begin(9600);
  pinMode(RedledPin,OUTPUT);
  pinMode(GreenledPin,OUTPUT);
  pinMode(echopin,INPUT); //设定echo为输入模式
  pinMode(trigpin,OUTPUT);//设定trig为输出模式
  pinMode(echopin2,INPUT);
  pinMode(trigpin2,OUTPUT);
  pinMode(echopin3,INPUT);
  pinMode(trigpin3,OUTPUT);
  pinMode(PIN_SPEAKER,OUTPUT);
  initflag = true;

  
}


float distanceL_last =0;
float distanceL_curr =0;
 
float distanceR_last =0;
float distanceR_curr =0;
 
float distanceF_last =0;
float distanceF_curr =0;


void loop(void)
{
  if(initflag){   

    Serial.print("欢迎使用汽车倒车雷达系统 ");
    Serial.print("系统正在启动，请稍后");
    delay(1000);

    // *************************************测量温度********************************************/
    float temperature = getTemp();
    int shi = temperature / 10;
    int ge = temperature % 10;
    int _ge = (temperature *10) % 100;
    delay(100);
    Serial.print("temperature : ");
    Serial.print(temperature);
    Serial.println("C");
    delay(100);
    // *********************************** 左方测距*************************************/
    digitalWrite(trigpin,LOW);
    delayMicroseconds(2);
    digitalWrite(trigpin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigpin,LOW); //发一个10ms的高脉冲去触发TrigPin
    float distanceL = pulseIn(echopin,HIGH);//接收高电平时间
    distanceL = distanceL/58.0;//计算距离
    distanceL_curr = distanceL;
    Serial.print("distanceL_curr : ");  //输出距离
    Serial.print(distanceL_curr);
    Serial.println("cm");  //输出单位
    delay(100);   //间隔100uS


    //*************************************右方测距******************************************/
    
    digitalWrite(trigpin2,LOW);
    delayMicroseconds(2);
    digitalWrite(trigpin2,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigpin2,LOW); //发一个10ms的高脉冲去触发TrigPin
    float distanceR = pulseIn(echopin2,HIGH);//接收高电平时间
    distanceR = distanceR/58.0;//计算距离
    distanceR_curr = distanceR;
    Serial.print("distanceR_curr : ");
    Serial.print(distanceR_curr);
    Serial.println("cm");
    
    // *************************************前方测距********************************************/
    digitalWrite(trigpin3,LOW);
    delayMicroseconds(2);
    digitalWrite(trigpin3,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigpin3,LOW); //发一个10ms的高脉冲去触发TrigPin 
    float distanceF = pulseIn(echopin3,HIGH);//接收高电平时间
    distanceF = distanceF/58.0;//计算距离
    distanceF_curr = distanceF;
    Serial.print("distanceF_curr : ");
    Serial.print(distanceF_curr);
    Serial.println("cm");
    delay(100);   //循环间隔100uS

    initflag = false;

    }
    else {

    // *************************************测量温度********************************************/
    float temperature = getTemp();
    delay(100);
    Serial.print("temperature : ");
    Serial.print(temperature);
    Serial.println("C");
    delay(100);

    
    // *********************************** 左方测距*************************************/
    distanceL_last = distanceL_curr;
    distanceL_curr = 0;
    digitalWrite(trigpin,LOW);
    delayMicroseconds(2);
    digitalWrite(trigpin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigpin,LOW); //发一个10ms的高脉冲去触发TrigPin
    float distanceL = pulseIn(echopin,HIGH);//接收高电平时间
    distanceL = distanceL/58.0;//计算距离
    distanceL_curr = distanceL;
    Serial.print("distanceL_curr : ");  //输出距离
    Serial.print(distanceL_curr);
    Serial.println("cm");  //输出单位
    if (distanceL_last - distanceL_curr > 20){
      
      Serial.print("车速过快，已启动紧急制动！");
      
      }
//    delay(100);   //间隔100uS


    //*************************************右方测距******************************************/
    distanceR_last = distanceR_curr;
    distanceR_curr = 0;
    digitalWrite(trigpin2,LOW);
    delayMicroseconds(2);
    digitalWrite(trigpin2,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigpin2,LOW); //发一个10ms的高脉冲去触发TrigPin
    float distanceR = pulseIn(echopin2,HIGH);//接收高电平时间
    distanceR = distanceR/58.0;//计算距离
    distanceR_curr = distanceR;
    Serial.print("distanceR_curr : ");
    Serial.print(distanceR_curr);
    Serial.println("cm");
    if ((distanceR_last - distanceR_curr) >= 20){
      
      Serial.print("车速过快，已启动紧急制动！");
      
      }
    // *************************************前方测距********************************************/
    distanceF_last = distanceF_curr;
    distanceF_curr = 0;
    digitalWrite(trigpin3,LOW);
    delayMicroseconds(2);
    digitalWrite(trigpin3,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigpin3,LOW); //发一个10ms的高脉冲去触发TrigPin 
    float distanceF = pulseIn(echopin3,HIGH);//接收高电平时间
    distanceF = distanceF/58.0;//计算距离
    distanceF_curr = distanceF;
    Serial.print("distanceF_curr : ");
    Serial.print(distanceF_curr);
    Serial.println("cm");
//    delay(100);   //循环间隔100uS
    if ((distanceF_last - distanceF_curr) >= 20){
      
      Serial.print("车速过快，已启动紧急制动！");
      
      }

    initflag = false;
  
      }
}
float getTemp(){
  
  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad
  
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  } 
  ds.reset_search();
  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  return TemperatureSum;
  
  }

void Beep(){
    digitalWrite(PIN_SPEAKER, HIGH);  
    delay(1000);  
    digitalWrite(PIN_SPEAKER, LOW);  
    delay(1000);  
  }

//void measure(){
//  
//  // *********************************** 左方测距*************************************/
//    digitalWrite(trigpin,LOW);
//    delayMicroseconds(2);
//    digitalWrite(trigpin,HIGH);
//    delayMicroseconds(10);
//    digitalWrite(trigpin,LOW); //发一个10ms的高脉冲去触发TrigPin
//    float distanceL = pulseIn(echopin,HIGH);//接收高电平时间
//    distanceL = distanceL/58.0;//计算距离
//    Serial.print("distanceL : ");  //输出距离
//    Serial.print(distanceL);
//    Serial.println("cm");  //输出单位
//    delay(100);   //间隔100uS
//    
//    
//    //*************************************右方测距******************************************/
//    
//    digitalWrite(trigpin2,LOW);
//    delayMicroseconds(2);
//    digitalWrite(trigpin2,HIGH);
//    delayMicroseconds(10);
//    digitalWrite(trigpin2,LOW); //发一个10ms的高脉冲去触发TrigPin
//    float distanceR = pulseIn(echopin2,HIGH);//接收高电平时间
//    distanceR = distanceR/58.0;//计算距离
//    Serial.print("distanceR : ");
//    Serial.print(distanceR);
//    Serial.println("cm");
//
//
//    float distance;
//
//    if (distanceR>=distanceL){
//
//      distance = distanceL;
//
//      }
//      else{
//        
//         distance = distanceR;
//        
//        }
//     if ( distance <= 20){
//        digitalWrite(RedledPin,HIGH);
//        digitalWrite(GreenledPin,LOW);
//        Beep();
//        
//      }
//      else{
//        digitalWrite(GreenledPin,HIGH);
//        digitalWrite(RedledPin,LOW);
//        }
//    // *************************************前方测距********************************************/
//    digitalWrite(trigpin3,LOW);
//    delayMicroseconds(2);
//    digitalWrite(trigpin3,HIGH);
//    delayMicroseconds(10);
//    digitalWrite(trigpin3,LOW); //发一个10ms的高脉冲去触发TrigPin 
//    float distanceF = pulseIn(echopin3,HIGH);//接收高电平时间
//    distanceF = distanceF/58.0;//计算距离
//    Serial.print("distance3 : ");
//    Serial.print(distanceF);
//    Serial.println("cm");
//    delay(100);   //循环间隔100uS
//   // *************************************测量温度********************************************/
//    float temperature = getTemp();
//    delay(100);
//    Serial.print("temperature : ");
//    Serial.print(temperature);
//    Serial.println("C");
//    delay(100);
//}



