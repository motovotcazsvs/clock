 /*
чассы с дзвонком в заданое времья(лед матрица,чассы реального времени и динамик) 
*/

#include "LedControl.h"
#include <iarduino_RTC.h>                     // Подключаем библиотеку 
#define zvuk 8  //к цыфровому пину 3 подключен динамик
//RST    – К цифровому пину Arduino ( в примере – пин 45, Arduino MEGA 2560)
//CLK    – К цифровому пину Arduino ( в примере – пин 46, Arduino MEGA 2560)
//DAT    – К цифровому пину Arduino ( в примере – пин 47, Arduino MEGA 2560)
#define RST 2
#define CLK 3
#define DAT 4
iarduino_RTC time(RTC_DS1302, RST, CLK, DAT);
//Синтаксис создания класса LedControl(dataPin,clockPin,csPin,numDevices)
//Где LedControl - объект класса
//dataPin  - пин на плате Arduino к которому будет подключен пин DIN
//clockPin - пин на плате Arduino к которому будет подключен пин CLK
//csPin    - пин на плате Arduino к которому будет подключен пин CS
//numDevices - количество устройств на шин
//Создать объект класса matrix в нашем случае с одним подключенным устройством
LedControl matrix = LedControl(5, 6, 7, 4); //LedControl(dataPin = 5, clkPin = 6, csPin = 7, numDevices=4);



void setup() {
  //Устройству с адресом 0 по SPI интерфейсу выйти из спящего режима по умолчанию
  matrix.shutdown(0, false);
  //Установить яркость Led матрицы на 8 из 15      
  matrix.setIntensity(0, 8); 
  //Очистить дисплей
  matrix.clearDisplay(0);
  matrix.shutdown(1, false);
  matrix.shutdown(2, false);
  matrix.shutdown(3, false);
  matrix.setIntensity(1, 8);
  matrix.setIntensity(2, 8);
  matrix.setIntensity(3, 8); 
  matrix.clearDisplay(1);
  matrix.clearDisplay(2);
  matrix.clearDisplay(3);
  pinMode(zvuk, OUTPUT);
  time.begin();
}

boolean bell = false;
//Объявляем массив из 10-ти символов
//Каждый символ включает в себя массив из 8-ти байт
//закодированных числом в шестнадцатиричном коде  
byte CountDigits[10][8] = 
{
  {0x1C, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x1C},  //0
  {0x4, 0xC, 0x1C, 0x2C, 0xC, 0xC, 0xC, 0xC},        //1
  {0xC, 0x1E, 0x36, 0x6, 0x1C, 0x30, 0x3E, 0x3E},    //2
  {0x1C, 0x36, 0x6, 0x1C, 0x1C, 0x6, 0x36, 0x1C},    //3
  {0x6, 0xE, 0x1A, 0x32, 0x3E, 0x6, 0x6, 0x6},       //4
  {0x3E, 0x30, 0x30, 0x3C, 0x6, 0x6, 0x26, 0x1C},    //5
  {0x1C, 0x36, 0x30, 0x3C, 0x36, 0x36, 0x36, 0x1C},  //6
  {0x3E, 0x6, 0xE, 0x1C, 0x18, 0x18, 0x18, 0x18},    //7
  {0x1C, 0x36, 0x36, 0x1C, 0x3E, 0x36, 0x36, 0x1C},  //8
  {0x1C, 0x36, 0x36, 0x1E, 0x6, 0x6, 0x36, 0x1C}     //9
};

void print_number(int ho, int mi)
{
  int hours1, hours2, minutes1, minutes2; // две цифры на чассы и две на минуты
  if (ho < 10)
  {
    hours1 = 0;
    hours2 = ho;
  }
  else if (ho > 9)
  {
    hours1 = ho / 10;
    hours2 = ho % 10;
  }
  if (mi < 10)
  {
    minutes1 = 0;
    minutes2 = mi;
  }
  else if (mi > 9)
  {
    minutes1 = mi / 10;
    minutes2 = mi % 10;
  }
  for(int i = 0; i < 8; i ++)
  {
    //0 - адрес, либо номер устройства на шине SPI
    //j - индекс массива байт с символом
    //i - текущий ряд на матрице
    //CountDigits[i] - значение(byte) которым заполнится ряд
    matrix.setRow(3, i, CountDigits[hours1][i]);
  }
  for(int i = 0; i < 8; i ++)
  {
    //0 - адрес, либо номер устройства на шине SPI
    //j - индекс массива байт с символом
    //i - текущий ряд на матрице
    //CountDigits[i] - значение(byte) которым заполнится ряд
    matrix.setRow(2, i, CountDigits[hours2][i]);
  }
  for(int i = 0; i < 8; i ++)
  {
    //0 - адрес, либо номер устройства на шине SPI
    //j - индекс массива байт с символом
    //i - текущий ряд на матрице
    //CountDigits[i] - значение(byte) которым заполнится ряд
    matrix.setRow(1, i, CountDigits[minutes1][i]);
  }
  for(int i = 0; i < 8; i ++)
  {
    //0 - адрес, либо номер устройства на шине SPI
    //j - индекс массива байт с символом
    //i - текущий ряд на матрице
    //CountDigits[i] - значение(byte) которым заполнится ряд
    matrix.setRow(0, i, CountDigits[minutes2][i]);
  }
}

const int nachalo = 25; //включается будильник
const int konec = 26; //выключается будильник
bool perevedeno = false; 
bool perevod = true;//автоматический перевод времени(можно изменить на false, для отключения) 

void loop() {
 time.gettime();                 // читаем время, обновляя значения всех переменных.
 print_number(time.Hours, time.minutes);
 if (time.Hours == 6 && time.minutes == nachalo && time.weekday != 0 && time.weekday != 6) //будильник на 6:15
  bell = true;
 if (bell) // если включен звонок, то подаем звуки на динамик
 {
  digitalWrite(zvuk,HIGH);
  delay(100);
  digitalWrite(zvuk,LOW);
 
 if(time.minutes == konec)
 {
  digitalWrite(zvuk,LOW);
  bell = false;
 } 
}
if (time.minutes != nachalo || time.Hours != 18)
 { 
  matrix.setColumn(1,0,B00000000);
  delay(500);
  matrix.setColumn(1,0,B00100100);
  delay(500);
 }

 if(perevod && (time.month == 3 || time.month == 10) && time.day > 24 && time.weekday == 0 && time.Hours >= 3 && time.Hours <= 5)
  automatic_time_translation(); 
}


void automatic_time_translation()
{
  if(!perevedeno && time.month == 3 && time.Hours == 3){
    //time.settime(-1, -1, 4);// перевод на 1час вперед
    time.settime(15, 59, 3);//перевод на час и подстройка на 45сек. назад
    perevedeno = true;
  }
  else if(!perevedeno && time.month == 10 && time.Hours == 4){
    //time.settime(-1, -1, 3);//перевод на 1 час назад
    time.settime(15, 59, 2);//перевод на час и подстройка на 45сек. назад
    perevedeno = true;
  }
  else if(perevedeno && time.Hours >= 5) perevedeno = false;
}
