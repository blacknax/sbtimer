
/*
 * Project sbtimer
 * 
 * Code developed for this board:
 * Innovateking-EU ESP8266 Nodemcu OLED Arduino WiFi Modul 0,96 Zoll Anzeige ESP8266 18650 5-12V 500mA Kompatibel mit NodeMCU
 * Weitere Informationen: https://www.amazon.de/dp/B07JFD21SW/ref=cm_sw_em_r_mt_dp_U_Bi51EbZWNA4DX 
 * 
 * In Arduino Ide select one of this boards:
 * - Lolin (WeMos) D1 R2
 * - Lolin (WeMos) D1 mini pro
 * - Lolin (WeMos) D1 mini Lite
 * I used: LOLIN(WEMOS) D1 R2 & mini (on COM4)
 * 
 * this board is not working:
 * - Wemos D1 R1 (variant=d1)
 * 
 * The esp is controling an electro magnet
 * I used this:
 * Homyl DC 3.3V-5V Keyestudio Electromagnet Module For Arduino
 * see: https://www.wish.com/product/5b5cd464ff666512b9871a11?share=web
 * 
 * 
 */



#include <Arduino.h>



/*
 * **************************************************
   Board settings for the InovateKing Oled-Esp-18650 Board
 * **************************************************
*/

/*
   Aus der Wemos Boardinfo kommt SDA=4 und SCL=5. Das ist fuer das InnovateKing Board genau falsch rum.
   Richtig sind:
       SDA = 5 = D1
       SCL = 4 = D2
*/
#define OLED_SDA 5
#define OLED_SCL 4

/*
   there is a green led left of the esp. It is ON on LOW. It is connected to
       16 = D0
*/
#define LED_GREEN_PIN 16

/*
   Joystick LEFT gets LOW when selected and goes back to HIGH automattically. LEFT is connected to
       0 = D3
*/
#define JOY_LEFT_PIN 0

/*
   Joystick RIGHT is hardwired to RESET. It is not programmable at all :-(
*/

/*
   Joystick UP gets LOW when selected and stays low. It is connected to
       12 = D6
*/
#define JOYSTICK_UP_PIN 12

/*
   Joystick DOWN  gets LOW when selected and stays low. It is connected to
       13 = D7
*/
#define JOYSTICK_DOWN_PIN 13


/*
   Joystick OK (center press)  gets LOW when pressed and stays low. It is connected to
       14 = D5
*/
#define JOYSTICK_OK_PIN 14


/*
   LED_BUILTIN, the blue led on the esp board is working fine with the standard pin definition
       it is connected to
       2 = LED_BUILTIN
*/




/*
   Initializing the buttons. They are managed with the help of the SimpleButton library
*/
#include <SimpleButton.h>
using namespace simplebutton;
Button* btnLeft = NULL;
Button* btnUp = NULL;
Button* btnDown = NULL;
Button* btnPress = NULL;


/*
   Magnet control
*/
int magState = LOW;
int ledMagnet = HIGH;
#define PIN_MAGNET D8

void turnOffMag() {
  if (magState == HIGH) {
    toggleMag();
  }
}
void turnOnMag() {
  if (magState != HIGH) {
    toggleMag();
  }
}
void toggleMag() {
  if (magState == LOW) {
    magState = HIGH;
  } else {
    magState = LOW;
  }
  ledMagnet = !magState;
  digitalWrite(LED_GREEN_PIN, ledMagnet);
  digitalWrite(PIN_MAGNET, magState);
}

/*
 * Magnet Power off 
 * Magnet LED off
 */
void initMag() {
  magState = LOW;
  ledMagnet = !magState;
  digitalWrite(LED_GREEN_PIN, ledMagnet);
}




#include "CountdownTimer.h"
CountdownTimer cdtimer;

#include "DurationTarget.h"
DurationTarget durationTarget;

#include "ActionHandler.h"

MainMode currentMode;


/*
   Initializing the oled display via I2C Wire
   Es werden direkt die API Funktionen von SSD1306Wire verwendet und nicht
   die darauf gesetzte Schicht von OLEDDisplayUi.
*/

#include <Wire.h>               //Bibliothek für die Arduino IDE <= 1.6.5
#include "SSD1306Wire.h"        //Display Bibliothek
SSD1306Wire  display(0x3c, OLED_SDA, OLED_SCL);


#include "ScreenHandler.h"



// PINS
// AD = ADC0, TOUT
// D3 = 0;
// D4 = 1; // TX1 // bottom 3rd pin right   !! stimmt nicht. D4=2 und damit = LED_BUILTIN
// D8 = 15;
// D9 = 3; // RX
// 10 = 1 // TX // top right


void setup() {

  Serial.begin(115200);

  delay(100);

  // LED_BUILTIN = 2 -> blue LED on Wemos Board HIGH=Aus LOW=An
  // on, when timer runs
  pinMode(LED_BUILTIN, OUTPUT);

  // green LED
  // on when magnet is on 
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(PIN_MAGNET, OUTPUT);
  initMag(); 

    cdtimer.setDurationMs(durationTarget.getDurationMs());

  // ButtonPullup will do "pinMode(12, INPUT_PULLUP)" instead of "pinMode(12, INPUT)".
  // That way you can connect it between the pin and GND and don't need the additional resistor.
  btnUp = new ButtonPullup(JOYSTICK_UP_PIN);
  btnDown = new ButtonPullup(JOYSTICK_DOWN_PIN);
  btnLeft = new ButtonPullup(JOY_LEFT_PIN);
  btnPress = new ButtonPullup(JOYSTICK_OK_PIN);

  currentMode = COUNTDOWN;

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);


}





void loop() {

  btnUp->update();
  if (btnUp->clicked()) {
    currentMode = handleUserAction(currentMode, UP);
  }

  btnDown->update();
  if (btnDown->clicked()) {
    currentMode = handleUserAction(currentMode, DOWN);
  }

  btnLeft->update();
  if (btnLeft->clicked()) {
    currentMode = handleUserAction(currentMode, NEXTMODE);
  }

  btnPress->update();
//  if (btnPress->doubleClicked()) {
//    cdtimer.setDurationMs(1234567);
//    cdtimer.startCountdown();
//  }
  if (btnPress->clicked()) {
    currentMode = handleUserAction(currentMode, ENTER);
  }


  switch (currentMode) {
    case SETSECONDS:
      drawSetSecondsScreen();
      break;
    case SETMINUTES:
      drawSetMinutesScreen();
      break;
    case SETHOURS:
      drawSetHoursScreen();
      break;
    case COUNTDOWN:
    default:
      drawCountdownScreen();
      break;
  }

    if (cdtimer.isRunning()) {
          digitalWrite(LED_BUILTIN, LOW);
    } else {
          digitalWrite(LED_BUILTIN, HIGH);
    }

    delay(100);
}
