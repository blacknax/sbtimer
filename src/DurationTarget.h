#ifndef DurationTarget_h
#define DurationTarget_h

#include <Arduino.h>

class DurationTarget {
    private:
        byte hours = 0;
        byte minutes = 15;
        byte seconds = 0;

        char durationChars[32];

    
    public: 

        void hoursUp() {
            hours++;
            if (hours > 99) {
                hours = 0;
            }
        }

        void hoursDown() {
            hours--;
            if (hours > 99) {
                hours = 99;
            }
        }

        void minutesUp() {
            minutes++;
            if (minutes > 59) {
                minutes = 0;
            }
        }

        void minutesDown() {
            minutes--;
            if (minutes > 99) {
                minutes = 59;
            }
        }

        void secondsUp() {
            seconds++;
            if (seconds > 59) {
                seconds = 0;
            }
        }

        void secondsDown() {
            seconds--;
            if (seconds > 99) {
                seconds = 59;
            }
        }

        unsigned long getDurationMs() {
            unsigned long ms = hours * 3600ul;
            ms += minutes * 60ul;
            ms += seconds;
            ms = ms * 1000ul;
            return ms;
        }

       char* getDurationChars() {
            sprintf(this->durationChars, "%02u:%02u:%02u", hours, minutes, seconds);
            return durationChars;
        }
};

#endif
