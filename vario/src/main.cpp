#include <main.h>

// Last measure time
double lastTime;

// Variometer buzzer
float climb_threshold = 0.5;
float sink_threshold = -1.8;
float start_freq = 556.0;
float final_freq = 1605.0;
float start_beeps_per_second = 0.1;
float final_beeps_per_second = 3.9;
float max_climb = 10.0;
float time_started_tone = 0.0;
float curr_freq = 556.0;
float curr_waiting_time = 1000.0;

double last_smoother_ascent_rate = 1.0;
const float SMOOTHING_WEIGHT = 0.15;

const enum {
    UP,
    LEFT,
    DOWN,
    PRESS,
    RIGHT,
    NONE
};

void setup() {
    Serial.begin(115200);
    Serial.print("VARIOMETER v0.1 booting..");

    pinMode(JOYSTICK_PIN, INPUT_PULLUP);
    
    setupBarometer();
    lastTime = millis();
}

void loop() {
    /*
    Serial.println("");
    Serial.println("###################");

    Serial.print("Temperature: ");
    Serial.println(getTemperature());

    Serial.print("Pressure: ");
    Serial.println(getAbsolutePressure());

    Serial.print("Altitude: ");
    Serial.println(getAltitude());

    Serial.print("Altitude delta: ");
    Serial.println(getAltitudeDelta());
    */
    int b = readButtons(JOYSTICK_PIN);

    /*
    float ascent_rate = getAscentRate(lastTime);
    Serial.print("Ascent rate: ");
    Serial.print(ascent_rate);
    Serial.println(" m/s");
    float smoothed_ascent_rate = SMOOTHING_WEIGHT * ascent_rate + (1-SMOOTHING_WEIGHT) * last_smoother_ascent_rate;
    last_smoother_ascent_rate = smoothed_ascent_rate;
    Serial.print("Smoothed ascent rate: ");
    Serial.print(smoothed_ascent_rate);
    Serial.println(" m/s");
    */

    //beep(smoothed_ascent_rate);
    
    lastTime = millis();
    delay(10);
}

int readButtons(int pin) {
    int c = analogRead(JOYSTICK_PIN);
    if(c > 870) return UP;
    else if(c < 840 && c > 800) return(LEFT);
    else if(c < 660 && c > 600) return(DOWN);
    else if(c < 580 && c > 520) return(PRESS);
    else if(c < 450 && c > 400) return(RIGHT);
    return(NONE);
}

void beep(float smoothed_ascent_rate) {
    // TEST: to move to function
    // todo: wait for next bps time before calling function again
    if (millis() - time_started_tone > curr_waiting_time) {
        if (smoothed_ascent_rate > climb_threshold) {
            // Calculate bps
            float bps = smoothed_ascent_rate * final_beeps_per_second / max_climb;

            // Calculate waiting time
            curr_waiting_time = 1.0/bps*1000.0;

            // Calculate freq
            curr_freq = smoothed_ascent_rate * final_freq / max_climb;

            // 1/bps to get the duration, * 1000 to switch to ms
            tone(BUZZER_PIN, curr_freq, curr_waiting_time);
        } 
        else if (smoothed_ascent_rate < sink_threshold) {
            //tone(BUZZER_PIN, 1000, 200);
        }
        else{
            curr_waiting_time = 1000.0;
            tone(BUZZER_PIN, 400, curr_waiting_time);
        }
        time_started_tone = millis();
    }
}
