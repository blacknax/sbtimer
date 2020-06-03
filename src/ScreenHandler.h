
#ifndef ScreenHandler_h
#define ScreenHandler_h

#include <Arduino.h>

#include "Img128x96Test4.h"
#include "icons.h"


void drawMagIcon() {
  if (magState == LOW) {
    display.drawXbm(128 - magOff24x24_width, 20, magOff24x24_width, magOff24x24_height, magOff24x24_bits);
  } else {
    display.drawXbm(128 - magOn24x24_width, 20, magOn24x24_width, magOn24x24_height, magOn24x24_bits);
  }
}

void drawUpDownArrows(unsigned int x) {
    display.drawXbm(x, 14, up15x08_width, up15x08_height, up15x08_bits);
    display.drawXbm(x, 46, down15x08_width, down15x08_height, down15x08_bits);
}

void drawPauseIcon() {
    const int xPos = 128 - pause24x14_width;
    const int yPos = 50;
    display.drawXbm(xPos, yPos,  pause24x14_width,  pause24x14_height, pause24x14_bits);    
    if (cdtimer.isPaused()) {
        display.drawXbm(xPos, yPos,  frame24x14_width,  frame24x14_height, frame24x14_bits);            
    }
}
void drawPlayIcon() {
    const int xPos = 128 - play24x14_width;
    const int yPos = 0;
    display.drawXbm(xPos, yPos,  play24x14_width,  play24x14_height, play24x14_bits);    
    if (cdtimer.isRunning()) {
        display.drawXbm(xPos, yPos,  frame24x14_width,  frame24x14_height, frame24x14_bits);            
    }
}



void drawModeTitle() {
  String title;
  switch (currentMode) {
    case SETSECONDS:
      title = "Set seconds";
      break;
    case SETMINUTES:
      title = "Set minutes";
      break;
    case SETHOURS:
      title = "Set hours";
      break;
    case COUNTDOWN:
    default:
      title = "Countdown";
      break;
  }
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, title);
}


void drawTitle(String t) {
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, t);
}



void drawCountdownScreen() {
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.clear();
  drawMagIcon();
  drawTitle("Countdown");
  drawPlayIcon();
  drawPauseIcon();

  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 20, cdtimer.getTimerChars());

  display.display();

}


void drawSetDurationScreen(String title, unsigned int x) {
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.clear();
  drawMagIcon();
  drawTitle(title);

  drawUpDownArrows(x);

  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 20, durationTarget.getDurationChars());

  display.display();

}

void drawSetHoursScreen() {
    drawSetDurationScreen("Set hours", 8);
}

void drawSetMinutesScreen() {
    drawSetDurationScreen("Set minutes", 40);
}

void drawSetSecondsScreen() {
    drawSetDurationScreen("Set Seconds", 72);
}



void drawStartScreen(String f1_text) {
  Serial.print("drawStartScreen: ");
  Serial.println(f1_text);

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  drawMagIcon();
  drawModeTitle();

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 20, cdtimer.getTimerChars());
  //display.drawString(0, 34, "Start Screen 1.3");
  //display.setFont(ArialMT_Plain_10);
  //display.drawString(0, 20, f1_text);
  display.display();

}


void drawSetTimeScreen(String f2_text) {
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "set time");
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 20, f2_text);

}

void drawImageDemo() {
  Serial.println("drawImage");
  display.clear();
  display.drawXbm(0, 0, Img128x96Test4_width, Img128x96Test4_height, Img128x96Test4_bits);
  display.display();
}











#endif
