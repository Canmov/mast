#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;

#define MY_PERIOD 250  // период в мс
#define time_held_long 2500
#define but1 2     // номер входа, подключенный к кнопке 1
#define but2 3     // номер входа, подключенный к кнопке 2
//#define but3 4     // номер входа, подключенный к кнопке 3
#define pin_motor1_1 4
#define pin_motor1_2 8
//#define pin_motor2_1 3;
//#define pin_motor2_2 4;
//#define pin_motor3_3 5;
//#define pin_motor3_4 6;
//#define pin_solinoid 7;

uint32_t tmr1;
int sec = 0;

//ПАРАМЕТРЫ
int mode_telescopic = 0;
int spead_m1 = 0; //скважность м1 от 0 до 1023
int length_wave = 1; // 
int period_t1 = 255; //
//int spead_m2 = 0; //скважность м2
//int spead_m3 = 0; //скважность м3

// КНОПКИ
//Кнопка 1
int s_b1 = 0;      // статус (прошлый) 
int sr_b1 = 0;     // статус текущий (read)
int th_b1 = 0;     //время нажатия 
int t_b1 = 0;
int rs_b1 =0;      //разница между текущим и временем нажатия
//Кнопка 2
int s_b2 = 0;      // статус (прошлый) 
int sr_b2 = 0;     // статус текущий (read)
int th_b2 = 0;     //время нажатия 
int t_b2 = 0;      //разница между текущим и временем нажатия
int rs_b2 =0; 

//Кнопка 3
//int s_b3 = 0;
//int sr_b3 = 0;

// МЕНЮ
void page_1(){
  oled.clear();
  oled.setCursor(1,0);
  oled.print("t:");
  oled.print(sec);
  oled.print("до");
  oled.print(period_t1);
  //oled.setCursor(1,1);
  //oled.print("Телескопия");
  oled.setCursor(1,2);
  oled.print("V1: ");
  //oled.print(map(spead_m1,0,255,0,100));
  oled.print(spead_m1);
  oled.setCursor(1,3);
  oled.print("Д_в: ");
  oled.print(length_wave);
  oled.setCursor(1,4);
  if(sr_b1 == LOW){
    oled.print("кнопка 1 нажата");
  }
  else{
    oled.print("кнопка 1 не нажата");
  }
  if(sr_b2 == LOW){
    oled.print("кнопка 2 нажата");
  }
  else{
    oled.print("кнопка 2 не нажата");
  }
  oled.setCursor(1,5);
  if(rs_b1 == 1){
    oled.print("кнопка 1 нажата");
  }
  else if(rs_b1==2){
    oled.print("кнопка 1 нажата долго");
  }
  else{
    oled.print("кнопка 1 не нажата");
  }
  if(rs_b2 == 1){
    oled.print("кнопка 2 нажата");
  }
  else  if(rs_b2==2){
    oled.print("кнопка 2 нажата долго");
  }
  else{oled.print("кнопка 2 не нажата");}
}
 
// ТИКИ

void tick(){
  
  if (millis() - tmr1 >= MY_PERIOD) {  // ищем разницу
    tmr1 = millis();
    if (sec < period_t1){
      sec=sec+1;
      }
    else{
      sec = 0;
    }
    oled.update();
  }
}


// ДЕЙСТВИЕ
int action = 0;     // новое дествие
int old_action = 0; // прошлое действие 
// s1 - 0 (короткое)
// l1 - 1 (длинное)
// s2 - 2
// l2 - 3
// s3 - 4
// l3 - 5

void setup() {
  oled.init(6,7);        // инициализация
  oled.clear();       // очистка
  oled.setScale(1); 
  pinMode(but1, INPUT_PULLUP);
  pinMode(pin_motor1_1, OUTPUT);
  pinMode(pin_motor1_2, OUTPUT);
  //pinMode(but2, INPUT_PULLUP); 
  //pinMode(but3, INPUT_PULLUP);  

}

void set_parameter(){
  switch (mode_telescopic)
  {
  case 0:
    if (sr_b1 == 1 and spead_m1 <= 95){
      spead_m1 = spead_m1 + 5;
    }
    else{
      spead_m1 = 100;
    }
    if (sr_b2 == 1 and spead_m1 >= 5){
      spead_m1 = spead_m1 - 5;
    }
    else{
      spead_m1 = 0;
    }

    if (sr_b1 == 2 and mode_telescopic<=2){
      mode_telescopic=mode_telescopic+1;
    }
    else{
      mode_telescopic=3;
    }
    if (sr_b2 == 2 and mode_telescopic>=2){
      mode_telescopic=mode_telescopic-1;
    }
    else{
      mode_telescopic=1;
    }
  
  case 1:
    spead_m1 = abs(sin (sec/10)*100);
    if (sr_b1 == 2 and mode_telescopic<=2){
      mode_telescopic=mode_telescopic+1;
    }
    else{
      mode_telescopic=3;
    }
    if (sr_b2 == 2 and mode_telescopic>=2){
      mode_telescopic=mode_telescopic-1;
    }
    else{
      mode_telescopic=1;
    }

  case 2:
    if(fmod(sec,50)==0){
      spead_m1 = 0;
    }
    else{
      spead_m1=spead_m1+5;
    }
    if (sr_b1 == 2 and mode_telescopic<=2){
      mode_telescopic=mode_telescopic+1;
    }
    else{
      mode_telescopic=3;
    }
    if (sr_b2 == 2 and mode_telescopic>=2){
      mode_telescopic=mode_telescopic-1;
    }
    else{
      mode_telescopic=1;
    }
  }
}

void opros(){
  sr_b1 = digitalRead(but1);
  sr_b2 = digitalRead(but2);
  //sr_b3 = digitalRead(but3);
  //максимальное количесчто одновременно нажатых кнопок 2
  if (sr_b1 == LOW and s_b1 == 0) {   //дописать до 3 кнопок 
    th_b1 = millis();
    s_b1 = 1;
  }
  else if (sr_b1 == HIGH and s_b1 == 1){
    t_b1 = millis()-th_b1;
    s_b1 = 0;
    if (time_held_long <= t_b1){
      rs_b1 =2;
    }
    else if (time_held_long >= t_b1){
      rs_b1 =1;
    }
  }
  else {
    rs_b1=0;
  }

  if (sr_b2 == LOW and s_b2 == 0) {   //дописать до 3 кнопок 
    th_b2 = millis();
    s_b2 = 1;
  }
  else if (sr_b2 == HIGH and s_b2 == 1){
    t_b2 = millis()-th_b2;
    s_b2 = 0;
    if (time_held_long <= t_b2){
      rs_b2 =2;
    }
    else if (time_held_long >= t_b2){
      rs_b2 =1;
    }
  }
  else {
    rs_b2=0;
  }
}

void motor(){
  if (1){
    analogWrite(pin_motor1_1, map(spead_m1,0,100,0,1023));
    digitalWrite(pin_motor1_2, LOW);
  }
  //else{
  //  digitalWrite(pin_motor1_1, LOW);
  //  digitalWrite(pin_motor1_2, LOW);
  //}
}
//Math.Pow(baseNumber, exponent)
// РЕЖИМЫ ТЕЛЕСКОПИИ
// 1
// 0 до 100
// 2
// y=((x-255)**2-255**2)**0.5
// при x = 255 > x = 0
// y=((r*a**2-((x-r*a)**2)/r)*r/a**2)**0.5
// 3
// y =sin x
// 4
// пила
// y = kx

void loop() {

  tick(); // tmr1 секундомер, в уравнениях это x
  page_1(); // рисуем страцу и выводим в уголу секундомер
  opros();
  set_parameter();
  motor();
}