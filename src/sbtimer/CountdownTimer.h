
#ifndef CountdownTimer_h
#define CountdownTimer_h

#include <Arduino.h>

class CountdownTimer {
    private:
        unsigned long durationLastSetMs = 0ul;  // wenn der Timer zu Ende gelaufe ist, wird die Duration wieder auf diesen zuletzt gesetzten Wert gesetzt
        unsigned long durationMs;       // enthaelt die aktuelle Zeitdauer des Timers (am Startzeitpunkt oder am Pause-Resume Zeitpunkt) 
        unsigned long startMs = 0ul;    // wenn 0, steht der Timer entweder auf Pause oder ist fertig
        unsigned long pausedRemainingMs = 0ul;  // wenn der Timer pausiert wird, ist die Restzeit enthalten. Wenn der Timer nicht pausiert ist, is 0 enthalten
        
        const unsigned long maxMs = 4208567296ul; // max unsigned long 2 hoch 32 Millisekunden minus ein Tag
        char timerChars[32];
    
    public: 

        void ringring() {
            toggleMag();
        }

        /*
         * setzen der Zeitdauer
         */
        void setDurationMs(unsigned long dur) {
                if (dur > maxMs) {
                    this->durationMs = maxMs;
                } else {
                    this->durationMs = dur;
                }
                this->durationLastSetMs = this->durationMs;
        }

        /*
         * Timer starten 
         */
        void startCountdown() {
            this->startMs = millis();
        }

        /*
         * verbleibende Restdauer in ms. 
         * Das kann auch die verbleibende Restdauer des pausierten Timers sein.
         */
        unsigned long getRemainingMs() {
            // wenn pausiert, dann die stehende Restzeit liefert
            if (pausedRemainingMs > 0ul) {
                return pausedRemainingMs;
            }

            // wenn steht, dann 0 liefern
            if (startMs == 0ul) {
                return 0ul;
            }
            
            unsigned long rem = startMs + durationMs - millis();

            // wenn Timer Ueberlauf passiert ist, wurde die 0 unterschritten => fertig
            if (rem > maxMs) {
                reset();
                ringring();
                return 0ul;     // Null wurde unterschritten => Ueberlauf auf 2 hoch 32
            }
            return rem;
        }

        /*
         * Timer zueruecksetzen auf Stop und neue Zeitdauer fuer naechsten Start setzen (nur setzen, nicht starten)
         */
        void reset(unsigned long newDurationMs) {
            reset();
            setDurationMs(newDurationMs);
        }

        /*
         * Timer zueruecksetzen auf Stop 
         */
        void reset() {
            this->startMs = 0ul;
            this->pausedRemainingMs = 0ul;
            setDurationMs(durationLastSetMs);                        
        }

        /*
         * Pausierung beenden und weiterlaufen
         */
        void resume() {
                        
            // wenn pausiert, dann mit Restzeit fortsetzen
            if (pausedRemainingMs > 0ul) {
                setDurationMs(pausedRemainingMs);
                pausedRemainingMs = 0ul;
                startCountdown();
                return;
            }

           // wenn steht, dann starten
            if (startMs == 0ul) {
                startCountdown();
                return;
            }
            
            //Serial.println("Timer laeuft schon. Resume ist wirkungslos");
        }

        /*
         * Timer pausieren
         */
        void pause() {
            if (startMs == 0) {
                //Serial.println("Timer laeuft nicht. pause ist wirkungslos");
                return;
            }
            if (pausedRemainingMs > 0ul) {
                //Serial.println("Timer ist schon pausiert. pause ist wirkungslos");
                return;                
            }
            pausedRemainingMs = getRemainingMs();
            startMs = 0ul;
            
            //Serial.println("pause aktiviert");
        }

        /*
         * true, wenn der Timer aktuell laeuft
         * false, wenn er pausiert ist oder abgelaufen ist
         */
        boolean isRunning() {
            if (startMs > 0ul) {
                return true; 
            }
            return false;
        }

        /*
         * true, wenn der Timer pausiert ist
         * false, wenn er laeuft oder abgelaufen ist
         */
        boolean isPaused() {
            if (isRunning()) {
                return false;
            }
            if (pausedRemainingMs == 0ul) {
                return false;
            }
            return true;
        }

        /*
         * String im Format 00:00:00 mit der 
         * - aktuellen Ablaufzeit bei laufendem Timer
         * - Restzeit bei pausiertem Timer
         * - bei stehendem Timer die Zielzeit, der er beim Start laufen wuerde
         */
        char* getTimerChars() {
            unsigned long remainingMs;

           if (pausedRemainingMs > 0ul) {
                // wenn pausiert, gespeicherte Restzeit anzeigen
                remainingMs = pausedRemainingMs;
           } else {
                // wenn laeuft, diese aktuelle Restzeit berechnen            
                remainingMs = this->getRemainingMs();
                if (remainingMs == 0ul) {
                    // wenn nicht laeuft, dann Vorgabezeit anzeigen            
                    remainingMs = durationLastSetMs;
                }
           }

            if (remainingMs == 0ul) {
                sprintf(this->timerChars, "%02u:%02u:%02u", 0ul, 0ul, 0ul);
                return timerChars;
            }

            unsigned long seconds = remainingMs / 1000;
            unsigned long hours = seconds / 3600;
            seconds = seconds - (hours * 3600);
            unsigned long minutes = seconds / 60;
            seconds = seconds - (minutes * 60);

            sprintf(this->timerChars, "%02u:%02u:%02u", hours, minutes, seconds);
            return timerChars;
        }       
};

#endif
