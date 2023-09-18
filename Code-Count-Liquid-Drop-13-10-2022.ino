#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define IR 6 //กำหนดขาไว้ INPUT ข้อมูลจาก IR SENSOR
#define BT1 4
#define BUZZER 8 //กำหนดขาไว้ BUZZER
#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int count = 0; //นับจำนวนหยด
float a[3]; //ตัวแปรเก็บค่าเวลาของหยดแล้วนำมาเทียบกัน
float hour; //ตัวแปรเก็บ ชั่วโมงแบบมีทศนิยม
float minute; //ตัวแปรเก็บ นาทีแบบมีทศนิยม
float distance ; //ตัวแปรเก็บ ระยะห่างระหว่างหยด
float myTime[2]; //ตัวแปร เก็บเวลาของหยด
float F[2]; //ตัวแปรเก็บ ความถี่ (Frequency)
float ML; //ตัวแปรเก็บ มิลลิตร / ชั่วโมง (Milliliter)
int fullhour; //ตัวแปรเก็บ ชั่วโมงแบบไม่มีทศนิยม
int fullminute; //ตัวแปรเก็บ นาทีแบบไม่มีทศนิยม
int k = 2;
int p = 1;
int m = 2;
int z = 20;
int FperMin;




void setup() {
 pinMode(IR,INPUT);
 pinMode(BT1,INPUT);
 pinMode(BUZZER,OUTPUT);
 digitalWrite(BUZZER, HIGH); //เอาไว้ปิดการทำงาน Buzzer ก่อนกดปุ่ม 
 Serial.begin(115200);

 // initialize OLED display with address 0x3C for 128x64
if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
  Serial.println(F("SSD1306 allocation failed"));
  while (true);
}

}
void loop() 
{
int value = digitalRead(IR); //อ่านค่าจาก IR Sensor
int Button1 = digitalRead(BT1); //อ่านค่าจาก Button1
    myTime[1] = millis();
    myTime[1] = myTime[1] * 0.001;
    if(value == HIGH){ //รับค่าจาก IR Sensor แสดงเวลาที่น้ำหยด และเก็บค่าเพื่อไปคำนวณ
      count++;
      Serial.println("");
      Serial.print("count = ");
      Serial.println(count);
      Serial.print("Time: ");
      myTime[0] = millis(); // รับค่ามิลลิวินาที
      myTime[0] = myTime[0] * 0.001; //แปลงจาก มิลลิวินาที เป็น วินาที
      Serial.println(myTime[0]);
        if(count == p){  // เก็บเวลาที่ได้มาไว้ใน Array a[0] p == 1
          a[0] = myTime[0];
          p = p + 2;
        }
        if(count == k){ // เก็บเวลาที่ได้มาไว้ใน Array a[1] k == 2
          a[1] = myTime[0];
          k = k + 2;
        }
        delay(200);
      }
      if(myTime[1] > z && count == 0){  //ถ้ากดปุ่มแล้วไม่มีหยดน้ำดรอปหลังจาก 20 Sec ขึ้นไป
        Serial.print("No water drop in : ");
        Serial.print(myTime[1]);
        Serial.println(" Sec");
        oled.clearDisplay(); // clear display
        oled.setTextSize(1);          // text size
        oled.setTextColor(WHITE);     // text color
        oled.setCursor(0, 10);        // position to display
        oled.print("No water drop in : "); // text to display
        oled.print(myTime[1]);
        oled.println(" Sec");
        oled.display();
        for(int x = 0; x < 5 ;x++ ){
        digitalWrite(BUZZER,LOW); //เอาไว้เปิดเสียง Buzzer
        delay(1000);
        digitalWrite(BUZZER,HIGH); //เอาไว้ปิดเสียง Buzzer
        delay(1000);
        }
        z = z + 20;
      }
      if(myTime[1] > myTime[0] + z && count > 0){ //ถ้าไม่มีน้ำหยดหลังจากเคยมีน้ำหยดแล้ว 20 Sec
        Serial.print("No water drop after count : ");
        Serial.println(count);
        oled.clearDisplay(); // clear display
        oled.setTextSize(1);          // text size
        oled.setTextColor(WHITE);     // text color
        oled.setCursor(0, 10);        // position to display
        oled.print("No water drop "); // text to display
        oled.display();
        for(int x = 0; x < 5 ;x++ ){
        digitalWrite(BUZZER,LOW); //เอาไว้เปิดเสียง Buzzer
        delay(1000);
        digitalWrite(BUZZER,HIGH); //เอาไว้ปิดเสียง Buzzer
        delay(1000);
        }
        z = z + 20;
      }
    //Calculating Water Drop
      CWD(); 
    //Display 
      oled.clearDisplay(); // clear display
      oled.setCursor(0,0);        // position to display
      oled.setTextSize(2);          // text size
      oled.setTextColor(WHITE);     // text color
      oled.print("Drop|M"); // text to display

      oled.setCursor(20,32);        // position to display
      oled.setTextSize(2);          // text size
      oled.setTextColor(WHITE);     // text color
      oled.print(FperMin);

      oled.setCursor(90,0);        // position to display
      oled.setTextSize(2);          // text size
      oled.setTextColor(WHITE);
      oled.print("HR");

      oled.setCursor(90,20);        // position to display
      oled.setTextSize(2);          // text size
      oled.print(fullhour);

      oled.setCursor(90,50);        // position to display
      oled.setTextSize(2);          // text size
      oled.setTextColor(WHITE);
      oled.print(fullminute);

      oled.setCursor(115,50);        // position to display
      oled.setTextSize(2);          // text size
      oled.setTextColor(WHITE);
      oled.print("M");

      oled.display();


    if(Button1 == HIGH){
      countdown();
    }
}

void CWD() //คำนวณหยดน้ำ Calculating Water Drop
  {
    if(count > m || count == m){ // คำนวณหยด
      distance = a[1] - a[0]; // คำนวณระยะเวลาระหว่างหยด
      Serial.print("Drop distance  :  ");
      Serial.println(distance); //เอาเวลาที่ หยดที่ 2 กับ หยดที่ 3 มาลบกัน
      F[0] = 1 / distance; // คำนวณเวลาหยดใน 1 นาที /60 วิ
      Serial.print("Frequency  :  ");
      Serial.println(F[0]);
      FperMin = F[0] * 60; // จำนวนหยดใน 1 นาที 
      ML = F[0] * 3600 * 0.05; // คำนวณปริมาณ ML / ชั่วโมง
      Serial.print("Milliliter  :  ");
      Serial.println(ML);
      hour = 1000 / ML; // คำนวณเวลาที่ใช้ในการเติมน้ำเกลือ
      //Serial.print("hour :  ");
      //Serial.println(hour);

      //แปลงเวลาจากเป็นทศนิยม เป็น ชั่วโมง / นาที 
      fullhour = hour;
      minute = hour - fullhour; 
      minute = minute * 60; // เอาไปทำเป็นนาที
      fullminute = minute;
      /*Serial.print("Hour : ");
      Serial.print(fullhour);
      Serial.print(" Hr ");
      Serial.print("Minute : ");
      Serial.print(fullminute);
      Serial.println(" Min");*/
    
    m = m + 2;
  }
}

void countdown() //เอาไว้นับถอยหลัง
{ 
  int time[2];
  int cdmin = 0; //กำหนดนับถอยหลังนาที
  int cdhr = 0; //กำหนดนับถอยหลังชั่วโมง
  time[0] = fullhour; 
  time[1] = fullminute;
  while(cdhr < time[0] || cdmin < time[1]){
    if(cdmin == 60){
        cdhr++; //ถ้า cdmin นับถึง 60 ให้เพิ่ม cdhr +1
        cdmin = 0; //จะคืนค่า cdmin = 0
        Serial.print("HR : ");
        Serial.println(cdhr);
    }
    else{
        cdmin++; //ถ้าไม่ถึง 60 ให้ cdmin +1
        Serial.print("MIN : ");
        Serial.println(cdmin);
        oled.clearDisplay(); // clear display
        oled.setCursor(25,0);        // position to display
        oled.setTextSize(2);          // text size
        oled.setTextColor(WHITE);     // text color
        oled.print("Hr"); // text to display

        oled.setCursor(25,20);        // position to display
        oled.setTextSize(2);          // text size
        oled.setTextColor(WHITE);     // text color
        oled.print(fullhour); // text to display

        oled.setCursor(25,50);        // position to display
        oled.setTextSize(2);          // text size
        oled.setTextColor(WHITE);     // text color
        oled.print(cdhr);

        oled.setCursor(90,0);        // position to display
        oled.setTextSize(2);          // text size
        oled.setTextColor(WHITE);     // text color
        oled.print("Min"); // text to display
        
        oled.setCursor(96,20);        
        oled.setTextSize(2);          
        oled.setTextColor(WHITE);     
        oled.print(fullminute);

        oled.setCursor(96,50);        
        oled.setTextSize(2);          
        oled.setTextColor(WHITE);     
        oled.print(cdmin);

        oled.display();

        delay(60000); // Delay 60 วินาที ต้องแก้เป็น 60000
      }
  if(time[0] == cdhr && time[1] == cdmin){
        Serial.print("น้ำเกลือหมดแล้ว");
      oled.clearDisplay(); 
      oled.setTextSize(2);          
      oled.setTextColor(WHITE);     
      oled.setCursor(48, 0);
      oled.print("Out");
      oled.setCursor(48, 16);
      oled.print("Of");
      oled.setCursor(48, 32);
      oled.print("Time");
      oled.display();
      for(int x = 0; x < 5 ;x++ ){
        digitalWrite(BUZZER,LOW); //เอาไว้เปิดเสียง Buzzer
        delay(2000);
        digitalWrite(BUZZER,HIGH); //เอาไว้ปิดเสียง Buzzer
        delay(2000);
      }
      break;
    }
  }
}
