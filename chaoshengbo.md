#include "Syn6288.h"
Syn6288 syn;

#include <Adafruit_BMP085.h>

#include <OneWire.h>

#include <Wire.h>
#include<LiquidCrystal.h>
LiquidCrystal lcd(48,46,42,12,44,13);
const int RedledPin = 32;      // 连接红色LED的引脚
const int GreenledPin = 34;      // 连接LED的引脚

const int echopin=5; // echo接5端口
const int trigpin=22; // trig接4端口  左侧超声波测距器


const int echopin2=3;
const int trigpin2=24;    // 右侧超声波测距器


const int echopin3=6;
const int trigpin3=26;   // 前方超声波测距器


const int dq = 10;// DS18B20温度传感器
OneWire ds(dq);

const int PIN_SPEAKER = 28;  
boolean initflag,chesuflag;
uint8_t text1[]={0xC4,0xE3,0xBA,0xC3,0xA3,0xAC,0xBB,0xB6,0xD3,0xAD,0xCA,0xB9,0xD3,0xC3,0xC6,0xFB,0xB3,0xB5,0xB5,0xB9,0xB3,0xB5,0xC0,0xD7,0xB4,0xEF,0xCF,0xB5,0xCD,0xB3};//你好，欢迎使用汽车倒车雷达系统
uint8_t text2[]={0xCF,0xB5,0xCD,0xB3,0xD5,0xFD,0xD4,0xDA,0xC6,0xF4,0xB6,0xAF,0xA3,0xAC,0xC7,0xEB,0xC9,0xD4,0xBA,0xF3};//系统正在启动，请稍后
uint8_t text3[]={0xBF,0xAA,0xCA,0xBC,0xB5,0xB9,0xB3,0xB5};   //开始倒车
uint8_t text4[]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};   //0,1,2,3,4,5,6,7,8,9
uint8_t text5[]={0xB3,0xB5,0xCB,0xD9,0xB9,0xFD,0xBF,0xEC,0xA3,0xAC,0xD2,0xD1,0xC6,0xF4,0xB6,0xAF,0xBD,0xF4,0xBC,0xB1,0xD6,0xC6,0xB6,0xAF}; //车速过快，已启动紧急制动
uint8_t text6[]={0xC9,0xE3,0xCA,0xCF,0xB6,0xC8};  //摄氏度
uint8_t text7[]={0xCF,0xD6,0xD4,0xDA,0xCE,0xC2,0xB6,0xC8,0,0,0xB5,0xE3,0,0xC9,0xE3,0xCA,0xCF,0xB6,0xC8};  //现在温度
uint8_t text8[]={0xB5,0xE3}; //点
uint8_t text9[]={0xC0,0xE5,0xC3,0xD7}; //厘米
uint8_t text10[]={0xBA,0xF3,0xB7,0xBD,0,0,0,0xB5,0xE3,0,0x2C,0,0xC0,0xE5,0xC3,0xD7}; //后方大于100厘米
uint8_t text11[]={0xBA,0xF3,0xB7,0xBD,0,0,0xB5,0xE3,0,0x2C,0,0xC0,0xE5,0xC3,0xD7}; //后方小于100厘米
uint8_t text12[]={0xD3,0xD2,0xB7,0xBD,0,0,0,0xB5,0xE3,0,0x2C,0,0xC0,0xE5,0xC3,0xD7}; //右方大于100厘米
uint8_t text13[]={0xD3,0xD2,0xB7,0xBD,0,0,0xB5,0xE3,0,0x2C,0,0xC0,0xE5,0xC3,0xD7}; //右方小于100厘米
uint8_t text14[]={0,0,0,0xB5,0xE3,0,0x2C,0,0xC0,0xE5,0xC3,0xD7}; //下方大于100厘米
uint8_t text15[]={0,0,0xB5,0xE3,0,0x2C,0,0xC0,0xE5,0xC3,0xD7}; //下方小于100厘米
uint8_t text16[]={0xC2,0xB7,0xC3,0xE6,0xB2,0xBB,0xC6,0xBD,0xA3,0xAC,0xC7,0xEB,0xD7,0xA2,0xD2,0xE2};
uint8_t text17[]={0xC2,0xB7,0xC3,0xE6,0xD2,0xEC,0xB3,0xA3,0x2C,0xC2,0xB7,0xC3,0xE6,0xD2,0xEC,0xB3,0xA3};
void setup(void)
{
  Serial.begin(9600);
  pinMode(RedledPin,OUTPUT);//设定红色led为输出模式
  pinMode(GreenledPin,OUTPUT);//设定绿色led为输出模式
  pinMode(echopin,INPUT); //设定echo为输入模式
  pinMode(trigpin,OUTPUT);//设定trig为输出模式
  pinMode(echopin2,INPUT);//设定echo2为输入模式
  pinMode(trigpin2,OUTPUT);//设定trig2为输出模式
  pinMode(echopin3,INPUT);//设定trig2为输出模式
  pinMode(trigpin3,OUTPUT);//设定trig3为输出模式
  pinMode(PIN_SPEAKER,OUTPUT);//设定蜂鸣器为输出模式
  lcd.begin(16,2);             //初始化LCD1602
//  lcd.print("Welcome to use!");             //液晶显示Welcome to use！
//  delay(1000);                         //延时1000ms
  lcd.clear(); 
  initflag = true;
  chesuflag = true;

  
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

    Serial.println("欢迎使用汽车倒车雷达系统 ");
    syn.play(text1,sizeof(text1),0);
    lcd.print("Welcome to use!");             //液晶显示Welcome to use！
    delay(1000);                         //延时1000ms
    lcd.clear(); 
    Serial.println("系统正在启动，请稍后");
    syn.play(text2,sizeof(text2),0);
    lcd.setCursor(0,0); 
    lcd.print("Loading......");  
    delay(100);
    lcd.setCursor(0,1); 
    lcd.print("Start!");  
    delay(1000);
    lcd.clear();
    digitalWrite(RedledPin,LOW);
    digitalWrite(GreenledPin,HIGH);
    
    // *************************************测量温度********************************************/
    float temperature = getTemp();
    int shi = temperature / 10;
    text7[8] = text4[shi];
    int ge = (int)temperature % 10;
    text7[9] = text4[ge];
    int baige = ((int)(temperature *10))%10;
    text7[12] = text4[baige];
    lcd.setCursor(0,0);                      //设置液晶开始显示的指针位置
    lcd.print("Temperature =");   //液晶显示“LM35 temp =”
    lcd.setCursor(0,1);                      //设置液晶开始显示的指针位置
    lcd.print((int)temperature); //液晶显示温度整数值
    lcd.print(".");                                      //液晶显示小数点
    lcd.print(baige);      //液晶显示温度小数值
    lcd.print((char)223);                    //液晶显示“°”
    lcd.print("C");                                    //液晶显示“C”
    delay(1000); //延时1000ms
    lcd.clear(); //清屏
    syn.play(text7,sizeof(text7),0);
    Serial.print("temperature : ");
    Serial.print(temperature);
    Serial.println("C");
    delay(100);
    syn.play(text3,sizeof(text3),0);


    
    // *********************************** 下方测距*************************************/
    digitalWrite(trigpin,LOW);
    delayMicroseconds(2);
    digitalWrite(trigpin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigpin,LOW); //发一个10ms的高脉冲去触发TrigPin
    float distanceL = pulseIn(echopin,HIGH);//接收高电平时间
    distanceL = distanceL/58.0;//计算距离
    distanceL_curr = distanceL;
    int distanceL_curr_bai,distanceL_curr_shi,distanceL_curr_ge,distanceL_curr_baige,distanceL_curr_baishi;
    distanceL_curr_bai = distanceL_curr / 100;
    if(distanceL_curr_bai>0){
      text14[0] = text4[distanceL_curr_bai];
      distanceL_curr = distanceL;
      distanceL_curr_shi = (int)distanceL_curr % 100 /10; 
      text14[1] = text4[distanceL_curr_shi];
      distanceL_curr = distanceL;
      distanceL_curr_ge = ((int)distanceL_curr % 100) % 10; 
      text14[2] = text4[distanceL_curr_ge];
      distanceL_curr = distanceL;
      distanceL_curr_baige = (int)(distanceL_curr * 10) % 10; 
      text14[5] = text4[distanceL_curr_baige];
      distanceL_curr = distanceL;
      distanceL_curr_baishi = (int)(distanceL_curr * 100)% 10; 
      text14[7] = text4[distanceL_curr_baishi];      
      syn.play(text14,sizeof(text14),0);
    }
    else{
      text15[0] = text4[distanceL_curr_bai];
      distanceL_curr = distanceL;
      int distanceL_curr_shi = (int)distanceL_curr % 100 /10; 
      text15[0] = text4[distanceL_curr_shi];
      distanceL_curr = distanceL;
      int distanceL_curr_ge = ((int)distanceL_curr % 100) % 10; 
      text15[1] = text4[distanceL_curr_ge];
      distanceL_curr = distanceL;
      int distanceL_curr_baige = (int)(distanceL_curr * 10) % 10; 
      text15[4] = text4[distanceL_curr_baige];
      distanceL_curr = distanceL;
      int distanceL_curr_baishi = (int)(distanceL_curr * 100)% 10; 
      text15[6] = text4[distanceL_curr_baishi];      
      syn.play(text15,sizeof(text15),0);
      }
    lcd.setCursor(0,0);                      //设置液晶开始显示的指针位置
    lcd.print("distanceL  =");   
    lcd.setCursor(0,1);                      
    lcd.print((int)distanceL_curr); //液晶显示x下方整数值
    lcd.print(".");                 //液晶显示小数点
    lcd.print(distanceL_curr_baige);      //液晶显示下方小数值
    lcd.print(distanceL_curr_baishi);   //液晶显示下方小数值
    lcd.print(" cm");  //液晶显示单位
    delay(1000); //延时1000ms
    lcd.clear(); //清屏
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
    int distanceR_curr_bai,distanceR_curr_shi,distanceR_curr_ge,distanceR_curr_baige,distanceR_curr_baishi;
    distanceR_curr_bai = distanceR_curr / 100;
    if(distanceR_curr_bai>0)
    {
      text12[4] = text4[distanceR_curr_bai];
      distanceR_curr = distanceR;
      distanceR_curr_shi = (int)distanceR_curr % 100 /10; 
      text12[5] = text4[distanceR_curr_shi];
      distanceR_curr = distanceR;
      distanceR_curr_ge = ((int)distanceR_curr % 100) % 10; 
      text12[6] = text4[distanceR_curr_ge];
      distanceR_curr = distanceR;
      distanceR_curr_baige = ((int)(distanceR_curr * 10) % 10); 
      text12[9] = text4[distanceR_curr_baige];
      distanceR_curr = distanceR;
      distanceR_curr_baishi = ((int)(distanceR_curr * 100)% 10); 
      text12[11] = text4[distanceR_curr_baishi];      
      syn.play(text12,sizeof(text12),0);
    }
    else{
      text12[4] = text4[distanceR_curr_bai];
      distanceR_curr = distanceR;
      distanceR_curr_shi = (int)distanceR_curr % 100 /10; 
      text13[4] = text4[distanceR_curr_shi];
      distanceR_curr = distanceR;
      distanceR_curr_ge = ((int)distanceR_curr % 100) % 10; 
      text13[5] = text4[distanceR_curr_ge];
      distanceR_curr = distanceR;
      distanceR_curr_baige = ((int)(distanceR_curr * 10) % 10); 
      text13[8] = text4[distanceR_curr_baige];
      distanceR_curr = distanceR;
      distanceR_curr_baishi = ((int)(distanceR_curr * 100)% 10); 
      text13[10] = text4[distanceR_curr_baishi];      
      syn.play(text13,sizeof(text13),0);

      }
    lcd.setCursor(0,0);                      //设置液晶开始显示的指针位置
    lcd.print("distanceR  =");   
    lcd.setCursor(0,1);                      
    lcd.print((int)distanceR_curr); //液晶显示x下方整数值
    lcd.print(".");                 //液晶显示小数点
    lcd.print(distanceR_curr_baige);      //液晶显示下方小数值
    lcd.print(distanceR_curr_baishi);   //液晶显示下方小数值
    lcd.print(" cm");   //液晶显示单位
    delay(1000); //延时1000ms
    lcd.clear(); //清屏
    Serial.print("distanceR_curr : ");
    Serial.print(distanceR_curr);
    Serial.println("cm");
    
    // *************************************后方测距********************************************/
    digitalWrite(trigpin3,LOW);
    delayMicroseconds(2);
    digitalWrite(trigpin3,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigpin3,LOW); //发一个10ms的高脉冲去触发TrigPin 
    float distanceF = pulseIn(echopin3,HIGH);//接收高电平时间
    distanceF = distanceF/58.0;//计算距离
    distanceF_curr = distanceF;
    if(distanceF_curr<=20){
      
            
      
      }
    int distanceF_curr_bai,distanceF_curr_shi,distanceF_curr_ge,distanceF_curr_baige,distanceF_curr_baishi;
    distanceF_curr_bai = distanceF_curr / 100;
    if(distanceF_curr_bai>0){
      text10[4] = text4[distanceF_curr_bai];
      distanceF_curr = distanceF;
      distanceF_curr_shi = (int)distanceF_curr % 100 /10; 
      text10[5] = text4[distanceF_curr_shi];
      distanceF_curr = distanceF;
      distanceF_curr_ge = ((int)distanceF_curr % 100) % 10; 
      text10[6] = text4[distanceF_curr_ge];
      distanceF_curr = distanceF;
      distanceF_curr_baige = ((int)(distanceF_curr * 10) % 10); 
      text10[9] = text4[distanceF_curr_baige];
      distanceF_curr = distanceF;
      distanceF_curr_baishi = ((int)(distanceF_curr * 100)% 10); 
      text10[11] = text4[distanceF_curr_baishi];   
      distanceF_curr = distanceF;   
      syn.play(text10,sizeof(text10),0);
    }
    else{
      
      text10[4] = text4[distanceF_curr_bai];
      distanceF_curr = distanceF;
      distanceF_curr_shi = (int)distanceF_curr % 100 /10; 
      text11[4] = text4[distanceF_curr_shi];
      distanceF_curr = distanceF;
      distanceF_curr_ge = ((int)distanceF_curr % 100) % 10; 
      text11[5] = text4[distanceF_curr_ge];
      distanceF_curr = distanceF;
      distanceF_curr_baige = ((int)(distanceF_curr * 10) % 10); 
      text11[8] = text4[distanceF_curr_baige];
      distanceF_curr = distanceF;
      distanceF_curr_baishi = ((int)(distanceF_curr * 100)% 10); 
      text11[10] = text4[distanceF_curr_baishi];   
      distanceF_curr = distanceF;   
      syn.play(text11,sizeof(text11),0);
      }
      lcd.setCursor(0,0);                      //设置液晶开始显示的指针位置
      lcd.print("distanceF  =");   
      lcd.setCursor(0,1);                      
      lcd.print((int)distanceF_curr); //液晶显示x下方整数值
      lcd.print(".");                 //液晶显示小数点
      lcd.print(distanceF_curr_baige);      //液晶显示下方小数值
      lcd.print(distanceF_curr_baishi);   //液晶显示下方小数值
      lcd.print(" cm");  //液晶显示单位
      delay(1000); //延时1000ms
      lcd.clear(); //清屏
    Serial.print("distanceF_curr : ");
    Serial.print(distanceF_curr);
    Serial.println("cm");
    delay(100);   //循环间隔100uS

    initflag = false;
   }


    else if (chesuflag){
      
        digitalWrite(RedledPin,LOW);
        digitalWrite(GreenledPin,HIGH);
        // *************************************测量温度********************************************/
        float temperature = getTemp();
        int shi = temperature / 10;
        text7[8] = text4[shi];
        int ge = (int)temperature % 10;
        text7[9] = text4[ge];
        int baige = ((int)(temperature *10))%10;
        text7[12] = text4[baige];
        lcd.setCursor(0,0);                      //设置液晶开始显示的指针位置
        lcd.print("Temperature =");   //液晶显示“LM35 temp =”
        lcd.setCursor(0,1);                      //设置液晶开始显示的指针位置
        lcd.print((int)temperature); //液晶显示温度整数值
        lcd.print(".");                                      //液晶显示小数点
        lcd.print(baige);      //液晶显示温度小数值
        lcd.print((char)223);                    //液晶显示“°”
        lcd.print("C");                                    //液晶显示“C”
        delay(1000); 
        lcd.clear(); //清屏
    //    syn.play(text7,sizeof(text7),0);
        Serial.print("temperature : ");
        Serial.print(temperature);
        Serial.println("C");
        delay(100);
    
        
        // *********************************** 下方测距*************************************/
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
        int distanceL_curr_bai,distanceL_curr_shi,distanceL_curr_ge,distanceL_curr_baige,distanceL_curr_baishi;
        distanceL_curr_bai = distanceL_curr / 100;
        if(distanceL_curr_bai>0){
          text14[0] = text4[distanceL_curr_bai];
          distanceL_curr = distanceL;
          distanceL_curr_shi = (int)distanceL_curr % 100 /10; 
          text14[1] = text4[distanceL_curr_shi];
          distanceL_curr = distanceL;
          distanceL_curr_ge = ((int)distanceL_curr % 100) % 10; 
          text14[2] = text4[distanceL_curr_ge];
          distanceL_curr = distanceL;
          distanceL_curr_baige = (int)(distanceL_curr * 10) % 10; 
          text14[5] = text4[distanceL_curr_baige];
          distanceL_curr = distanceL;
          distanceL_curr_baishi = (int)(distanceL_curr * 100)% 10; 
          text14[7] = text4[distanceL_curr_baishi];      
//          syn.play(text10,sizeof(text10),0);
        }
        else{
          text15[0] = text4[distanceL_curr_bai];
          distanceL_curr = distanceL;
          distanceL_curr_shi = (int)distanceL_curr % 100 /10; 
          text15[0] = text4[distanceL_curr_shi];
          distanceL_curr = distanceL;
          distanceL_curr_ge = ((int)distanceL_curr % 100) % 10; 
          text15[1] = text4[distanceL_curr_ge];
          distanceL_curr = distanceL;
          distanceL_curr_baige = (int)(distanceL_curr * 10) % 10; 
          text15[4] = text4[distanceL_curr_baige];
          distanceL_curr = distanceL;
          distanceL_curr_baishi = (int)(distanceL_curr * 100)% 10; 
          text15[6] = text4[distanceL_curr_baishi];      
//          syn.play(text11,sizeof(text11),0);
          }
    
          if (distanceL_last - distanceL_curr >=30){

            syn.play(text17,sizeof(text17),0);
            lcd.setCursor(0,0); 
            lcd.print("unnormal");
            delay(1000);
            lcd.clear(); //清屏
            error();
            }
            else if(distanceL_last - distanceL_curr >=10){
              
            syn.play(text16,sizeof(text16),0);
      //      stop();
              }
             else{
              
                    lcd.setCursor(0,0);                      //设置液晶开始显示的指针位置
                    lcd.print("distanceL  =");   
                    lcd.setCursor(0,1);                      
                    lcd.print((int)distanceL_curr); //液晶显示x下方整数值
                    lcd.print(".");                 //液晶显示小数点
                    lcd.print(distanceL_curr_baige);      //液晶显示下方小数值
                    lcd.print(distanceL_curr_baishi);   //液晶显示下方小数值
                    lcd.print(" cm");   //液晶显示单位
                    delay(1000); //延时1000ms
                    lcd.clear(); //清屏
                    Serial.print("distanceL_curr : ");  //输出距离
                    Serial.print(distanceL_curr);
                    Serial.println("cm");  //输出单位
              
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
        if(distanceR_curr<=20){
              Beep();
              digitalWrite(RedledPin,HIGH);
              digitalWrite(GreenledPin,LOW);
          
          }
            int distanceR_curr_bai,distanceR_curr_shi,distanceR_curr_ge,distanceR_curr_baige,distanceR_curr_baishi;
            distanceR_curr_bai = distanceR_curr / 100;
            if(distanceR_curr_bai>0)
            {
              text12[4] = text4[distanceR_curr_bai];
              distanceR_curr = distanceR;
              distanceR_curr_shi = (int)distanceR_curr % 100 /10; 
              text12[5] = text4[distanceR_curr_shi];
              distanceR_curr = distanceR;
              distanceR_curr_ge = ((int)distanceR_curr % 100) % 10; 
              text12[6] = text4[distanceR_curr_ge];
              distanceR_curr = distanceR;
              distanceR_curr_baige = ((int)(distanceR_curr * 10) % 10); 
              text12[9] = text4[distanceR_curr_baige];
              distanceR_curr = distanceR;
              distanceR_curr_baishi = ((int)(distanceR_curr * 100)% 10); 
              text12[11] = text4[distanceR_curr_baishi];      
              syn.play(text12,sizeof(text12),0);
            }
            else{
              text12[4] = text4[distanceR_curr_bai];
              distanceR_curr = distanceR;
              distanceR_curr_shi = (int)distanceR_curr % 100 /10; 
              text13[4] = text4[distanceR_curr_shi];
              distanceR_curr = distanceR;
              distanceR_curr_ge = ((int)distanceR_curr % 100) % 10; 
              text13[5] = text4[distanceR_curr_ge];
              distanceR_curr = distanceR;
              distanceR_curr_baige = ((int)(distanceR_curr * 10) % 10); 
              text13[8] = text4[distanceR_curr_baige];
              distanceR_curr = distanceR;
              distanceR_curr_baishi = ((int)(distanceR_curr * 100)% 10); 
              text13[10] = text4[distanceR_curr_baishi];      
              syn.play(text13,sizeof(text13),0);
        
              }
            lcd.setCursor(0,0);                      //设置液晶开始显示的指针位置
            lcd.print("distanceR  =");   
            lcd.setCursor(0,1);                      
            lcd.print((int)distanceR_curr); //液晶显示x下方整数值
            lcd.print(".");                 //液晶显示小数点
            lcd.print(distanceR_curr_baige);      //液晶显示下方小数值
            lcd.print(distanceR_curr_baishi);   //液晶显示下方小数值
            lcd.print(" cm");   //液晶显示单位
            delay(1000); //延时1000ms
            lcd.clear(); //清屏
            Serial.print("distanceR_curr : ");
            Serial.print(distanceR_curr);
            Serial.println("cm");
            Serial.print("distanceR_curr : ");
            Serial.print(distanceR_curr);
            Serial.println("cm");
         
    
        // *************************************后方测距********************************************/
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
        if(distanceF_curr<=20){
              Beep();
              digitalWrite(RedledPin,HIGH);
              digitalWrite(GreenledPin,LOW);
          
          }
        if ((distanceF_last - distanceF_curr) >= 20){
            syn.play(text5,sizeof(text5),0);
            stop();
        }
            int distanceF_curr_bai,distanceF_curr_shi,distanceF_curr_ge,distanceF_curr_baige,distanceF_curr_baishi;
            distanceF_curr_bai = distanceF_curr / 100;
            if(distanceF_curr_bai>0){
              text10[4] = text4[distanceF_curr_bai];
              distanceF_curr = distanceF;
              distanceF_curr_shi = (int)distanceF_curr % 100 /10; 
              text10[5] = text4[distanceF_curr_shi];
              distanceF_curr = distanceF;
              distanceF_curr_ge = ((int)distanceF_curr % 100) % 10; 
              text10[6] = text4[distanceF_curr_ge];
              distanceF_curr = distanceF;
              distanceF_curr_baige = ((int)(distanceF_curr * 10) % 10); 
              text10[9] = text4[distanceF_curr_baige];
              distanceF_curr = distanceF;
              distanceF_curr_baishi = ((int)(distanceF_curr * 100)% 10); 
              text10[11] = text4[distanceF_curr_baishi];   
              distanceF_curr = distanceF;   
              syn.play(text10,sizeof(text10),0);
            }
            else{
              text10[4] = text4[distanceF_curr_bai];
              distanceF_curr = distanceF;
              distanceF_curr_shi = (int)distanceF_curr % 100 /10; 
              text11[4] = text4[distanceF_curr_shi];
              distanceF_curr = distanceF;
              distanceF_curr_ge = ((int)distanceF_curr % 100) % 10; 
              text11[5] = text4[distanceF_curr_ge];
              distanceF_curr = distanceF;
              distanceF_curr_baige = ((int)(distanceF_curr * 10) % 10); 
              text11[8] = text4[distanceF_curr_baige];
              distanceF_curr = distanceF;
              distanceF_curr_baishi = ((int)(distanceF_curr * 100)% 10); 
              text11[10] = text4[distanceF_curr_baishi];   
              distanceF_curr = distanceF;   
              syn.play(text11,sizeof(text11),0);
              }
              lcd.setCursor(0,0);                      //设置液晶开始显示的指针位置
              lcd.print("distanceF  =");   
              lcd.setCursor(0,1);                      
              lcd.print((int)distanceF_curr); //液晶显示x下方整数值
              lcd.print(".");                 //液晶显示小数点
              lcd.print(distanceF_curr_baige);      //液晶显示下方小数值
              lcd.print(distanceF_curr_baishi);   //液晶显示下方小数值
              lcd.print(" cm");   //液晶显示单位
              delay(1000); //延时1000ms
              lcd.clear(); //清屏
              Serial.print("distanceF_curr : ");
              Serial.print(distanceF_curr);
              Serial.println("cm");
      //    delay(100);   //循环间隔100uS
        initflag = false;
  
      }
      else{
          Beep();
        // *************************************测量温度********************************************/
          float temperature = getTemp();
          int shi = temperature / 10;
          text7[8] = text4[shi];
          int ge = (int)temperature % 10;
          text7[9] = text4[ge];
          int baige = ((int)(temperature *10))%10;
          text7[12] = text4[baige];
          lcd.setCursor(0,0);                      //设置液晶开始显示的指针位置
          lcd.print("Temperature =");   //液晶显示“LM35 temp =”
          lcd.setCursor(0,1);                      //设置液晶开始显示的指针位置
          lcd.print((int)temperature); //液晶显示温度整数值
          lcd.print(".");                                      //液晶显示小数点
          lcd.print(baige);      //液晶显示温度小数值
          lcd.print((char)223);                    //液晶显示“°”
          lcd.print("C");                                    //液晶显示“C”
          delay(1000); 
          lcd.clear(); //清屏
          syn.play(text7,sizeof(text7),0);
          Serial.print("temperature : ");
          Serial.print(temperature);
          Serial.println("C");  
          lcd.print("Danger !!!");                                    //液晶显示“C”
          delay(2000); 
          lcd.clear(); //清屏
        
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
    digitalWrite(RedledPin,HIGH);  //红色LED亮
    digitalWrite(GreenledPin,LOW);  //绿色LED灭
    digitalWrite(PIN_SPEAKER, HIGH);   //蜂鸣器通电 
    delay(1000);   //延时1000ms
    digitalWrite(PIN_SPEAKER, LOW);  //蜂鸣器断电 
  }
void stop(){
  lcd.clear(); //清屏
  chesuflag = false;
  lcd.setCursor(0,0); 
  lcd.print("So Fast !!!");   //液晶显示单位
  delay(1000); //延时1000ms
  lcd.clear(); //清屏
  Beep();
  }
void error(){
  lcd.clear(); //清屏
  chesuflag = false;
  lcd.setCursor(0,0); 
  lcd.print("Error!!!");   //液晶显示单位
  delay(1000); //延时1000ms
  lcd.clear(); //清屏
  Beep();
  
  }





