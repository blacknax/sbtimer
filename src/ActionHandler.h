#ifndef ActionHandler_h
#define ActionHandler_h

#include <Arduino.h>


enum MainMode {
  COUNTDOWN,
  SETHOURS,
  SETMINUTES,
  SETSECONDS
};

enum UserAction {
    ENTER,
    UP,
    DOWN,
    NEXTMODE
};

MainMode cycleMode(MainMode current) {
  switch (current) {
    case SETSECONDS:
      return SETMINUTES;

    case SETMINUTES:
      return SETHOURS;
      
    case SETHOURS:
      return COUNTDOWN;
      
    case COUNTDOWN:
        if (cdtimer.isRunning()) {
            return COUNTDOWN;
        }
      return SETSECONDS;
    
    default:
      return COUNTDOWN;
  }
}


MainMode handleSecondsAction(UserAction ui) {
    switch (ui) {
        case UP:
            durationTarget.secondsUp();
            cdtimer.reset(durationTarget.getDurationMs());
            break;
        case DOWN:
            durationTarget.secondsDown();
            cdtimer.reset(durationTarget.getDurationMs());
            break;
        case ENTER:
            toggleMag();
            break;
        case NEXTMODE:
            return cycleMode(SETSECONDS);
        default:
            Serial.print(" *** handleSecondsAction: UNKNOWN: ");
            Serial.println(ui);
            break;
    }
    return SETSECONDS;
}

MainMode handleMinutesAction(UserAction ui) {
    switch (ui) {
        case UP:
            durationTarget.minutesUp();
            cdtimer.reset(durationTarget.getDurationMs());
            break;
        case DOWN:
            durationTarget.minutesDown();
            cdtimer.reset(durationTarget.getDurationMs());
            break;
        case ENTER:
            toggleMag();
            break;
        case NEXTMODE:
            return cycleMode(SETMINUTES);
        default:
            Serial.print(" *** handleMinutesAction: UNKNOWN: ");
            Serial.println(ui);
            break;
    }
    return SETMINUTES;
}

MainMode handleHoursAction(UserAction ui) {
    switch (ui) {
        case UP:
            durationTarget.hoursUp();
            cdtimer.setDurationMs(durationTarget.getDurationMs());
            break;
        case DOWN:
            durationTarget.hoursDown();
            cdtimer.setDurationMs(durationTarget.getDurationMs());
            break;
        case ENTER:
            toggleMag();
            break;
        case NEXTMODE:
            return cycleMode(SETHOURS);
        default:
            Serial.print(" *** handleHoursAction: UNKNOWN: ");
            Serial.println(ui);
            break;
    }
    return SETHOURS;
}

MainMode handleCountdownAction(UserAction ui) {
    switch (ui) {
        case UP:
            cdtimer.resume();
            break;
        case DOWN:
            cdtimer.pause();
            break;
        case ENTER:
            toggleMag();
            break;
        case NEXTMODE:
            return cycleMode(COUNTDOWN);
        default:
            Serial.print(" ***UNKNOWN: ");
            Serial.println(ui);
            break;
    }
    return COUNTDOWN;
}

MainMode handleUserAction(MainMode currentMode, UserAction act) {
  switch (currentMode) {
    case SETSECONDS:
        return handleSecondsAction(act);
        break;
    case SETMINUTES:
        return handleMinutesAction(act);
        break;
    case SETHOURS:
        return handleHoursAction(act);
        break;
    case COUNTDOWN:
        return handleCountdownAction(act);
        break;
  }
}















#endif
