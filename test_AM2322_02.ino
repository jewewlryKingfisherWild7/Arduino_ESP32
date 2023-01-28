/////////////////////////////////////////////////////////////////////////////////////////
//  AQM1602Y LCD display test
//     date: 2023.01.27
//     by:   jewelry.kingfisher
//
//     ref. https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/
//          https://wiki.seeedstudio.com/jp/Seeeduino-XIAO/
//          https://yokahiyori.com/xiao-esp32c3_aqm1602y_bme280/
//          https://github.com/RobTillaart/Arduino/blob/master/libraries/AM232X/examples/AM2322/AM2322.ino
/////////////////////////////////////////////////////////////////////////////////////////
//
//
//  ============================================
//  AM232X PIN layout
//  ============================================
//   bottom view
//          +---+
//   VDD ---|o  |
//   SDA ---|o  |
//   GND ---|o  |
//   SCL ---|o  |
//          +---+
//  ============================================
//
// do not forget pull up resistors between SDA, SCL and VDD.


#include <ST7032_asukiaaa.h>
#include <Wire.h>
#include "AM232X.h"

ST7032_asukiaaa lcd;
AM232X AM2322;


float tt;
float hh;
int   AM2322status;

/////  READ AM2322 DATA  /////////////////////////////////////////
void readAM2322( ) {
  AM2322status = AM2322.read();
  switch (AM2322status) {
  case AM232X_OK:
    tt = AM2322.getTemperature();
    hh = AM2322.getHumidity();
    break;
  }
}

///////////////////////////////////////////////////////////////////
void setup() {
  Wire.begin();        // I2C初期化
  AM2322.begin();
  AM2322.wakeUp();
  delay(3000);
  Serial.begin(115200);

  lcd.begin(16, 2);     //LCD表示初期設定(16桁, 2行)
  lcd.setContrast(45);  //コントラスト(0〜63)

  char startChar[] = "==setup started=";
  lcd.setCursor(0, 0);  //LCD display(col#1, line#1)
  lcd.print( startChar );
  // Serial.println("===== setup started =====");
  delay(1000);


  // "Sensor not found"
  AM2322status = AM2322.read();   // dummy read
  delay(3000);
  AM2322status = AM2322.read();
  if ( AM2322status != AM232X_OK ) {
    Serial.println("Sensor not found");
    lcd.setCursor(0, 0);  //LCD display(col#1, line#1)
    lcd.print( "Sensor not found" );
    while (1);
  }


  //AM2322.wakeUp();
  //delay(2000);
}


///////////////////////////////////////////////////////////////////
void loop() {
  char dispA[16];
  char titlechr[] = "AQM1602Y20230127";
  char dispChar[] =  "<<Hello!>>[XIAO ESP32C3]+[AM2322 Sensor]+[AQM1602Y]";
  int numDispChar = strlen(dispChar);    // dispChar[] の文字数
  int i;
  char buff0;
  
  lcd.setCursor(0, 0);  //LCD display(col#1, line#1)
  lcd.print( titlechr );
  delay(3000);


  // 2行目に流れる文字を表示する
  while(1) {
    buff0 = dispChar[0];
    for ( i= 0; i < numDispChar - 1 ; i++) {
      dispChar[i] = dispChar[i+1];  
      // Serial.print(".");  // for test
    }
    dispChar[ numDispChar - 1 ] = buff0;

    for ( i= 0; i < 16 ; i++) {
      dispA[i] = dispChar[i];
    }

    lcd.setCursor(0, 1);  //LCD display(col#1,line#2)
    lcd.print( dispA );
    delay(500);
    // Serial.println("\n----- loop process -----");

    readAM2322( );
    // Serial.print("Temperature = ");
    // Serial.print(tt, 1);
    // Serial.print(",\t");
    // Serial.print("Humidity = ");
    // Serial.println(hh, 1);

    lcd.setCursor(0, 0);
    lcd.print("T=     ,  ");
    lcd.setCursor(3, 0);
    lcd.print(String(tt,1));
    lcd.setCursor(9, 0);
    lcd.print("H=    %");
    lcd.setCursor(11, 0);
    lcd.print(String(hh,1));
    
    // lcd.setCursor(0, 0);  //LCD display(col#1, line#1)
    // lcd.print( titlechr );
  }
}
