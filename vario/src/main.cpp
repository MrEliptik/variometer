#include <main.h>

// Last measure time
double last_time;

// Loop timing
unsigned long loop_time;
unsigned long display_time;
unsigned long barometer_time;

bool first_time_state = false;

bool time_to_display = false;

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

double last_smoother_ascent_rate = 0.0;
double avg_ascent_rate = 0.0;
const float SMOOTHING_WEIGHT = 0.15;

JOYSTICK joystick_previous_state = NONE;

VARIO_STATE vario_state = VARIO;
// Different previous state to have a first update
VARIO_STATE previous_vario_state = HOME_SETTINGS;

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.print("VARIOMETER v0.1 booting..");
    setupDisplay(ORIENTATION_LANDSCAPE, Terminal12x16);
    displayStr(20, 50, "VARIOMETER v0.1");
    displayStr(50, 80, "booting..");

    pinMode(JOYSTICK_PIN, INPUT);

    // Setup buzzer
    ledcSetup(BUZZER_CHANNEL, 10000, 8);
    ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
    ledcWriteTone(BUZZER_CHANNEL, 400);
    
    setupBarometer();
    delay(500);
    ledcWriteTone(BUZZER_CHANNEL, 0);
    last_time = millis();
    display_time = micros();
}

void loop() {
    loop_time = micros();
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

    //beep(smoothed_ascent_rate);

    /* Read input */
    JOYSTICK b = readButtons(JOYSTICK_PIN);
    if (b != NONE) Serial.println(b);

    /* Changing state */
    switch(vario_state){
        case HOME_VARIO: 
            if(b == RIGHT) vario_state = HOME_GPS;
            else if(b == LEFT) vario_state = HOME_SETTINGS;
            else if(b == PRESS) vario_state = VARIO;
            break;
        case HOME_GPS: 
            if(b == RIGHT) vario_state = HOME_COMPASS;
            else if(b == LEFT) vario_state = HOME_VARIO;
            else if(b == PRESS) vario_state = GPS;
            break;
        case HOME_COMPASS: 
            if(b == RIGHT) vario_state = HOME_ALL;
            else if(b == LEFT) vario_state = HOME_GPS;
            else if(b == PRESS) vario_state = COMPASS;
            break;
        case HOME_ALL: 
            if(b == RIGHT) vario_state = HOME_SETTINGS;
            else if(b == LEFT) vario_state = HOME_COMPASS;
            else if(b == PRESS) vario_state = ALL;
            break;
        case HOME_SETTINGS: 
            if(b == RIGHT) vario_state = HOME_VARIO;
            else if(b == LEFT) vario_state = HOME_ALL;
            else if(b == PRESS) vario_state = SETTINGS;
            break;
        case VARIO: 

            break;
        case GPS: 

            break;
        case COMPASS: 

            break;
        case ALL: 

            break;
        case SETTINGS: 

            break;
    }

    // TODO: add time check maybe, to refresh data when not on menu
    if (vario_state != previous_vario_state) {
        first_time_state = true;
        previous_vario_state = vario_state;
        long time = micros();
        clearDisplay();
        Serial.print("Clearing takes: ");
        Serial.print((micros()-time)/1000.0);
        Serial.println(" ms");
    }
    else first_time_state = false;

    if (micros() > display_time + DISPLAY_TIME_US) {
        time_to_display = true;
        display_time = micros();
    }
    else time_to_display = false;

    switch(vario_state){
        case HOME_VARIO:
            if (first_time_state) displayMenu(HOME_VARIO);
            //if (time_to_display) displayMenu(HOME_VARIO); //displayStr(90, 80, "VARIO");
            break;
        case HOME_COMPASS:
            if (first_time_state) displayMenu(HOME_COMPASS);
            break;
        case HOME_GPS:
            if (first_time_state) displayMenu(HOME_GPS);
            break;
        case HOME_ALL:
            if (first_time_state) displayMenu(HOME_ALL);
            break;
        case HOME_SETTINGS:
            if (first_time_state) displayMenu(HOME_SETTINGS);
            break;
        case VARIO: 
            if (first_time_state) {
                displayStatusBar();
                displayVarioPage();
                barometer_time = micros();
            }
            float altitude = getAltitude();
            float ascent_rate = getAscentRate(last_time);
            float pressure = getAbsolutePressure();
            float temperature = getTemperature();
            
            Serial.print("Ascent rate: ");
            Serial.print(String(ascent_rate, 1));
            Serial.println(" m/s");

            float smoothed_ascent_rate = SMOOTHING_WEIGHT * ascent_rate + (1-SMOOTHING_WEIGHT) * last_smoother_ascent_rate;
            last_smoother_ascent_rate = smoothed_ascent_rate;
            avg_ascent_rate += smoothed_ascent_rate;
            avg_ascent_rate /= 2.0;

            Serial.print("Smoothed ascent rate: ");
            Serial.print(String(smoothed_ascent_rate, 1));
            Serial.println(" m/s");

            if (time_to_display){       
                
                if (micros() > barometer_time + BAROMETER_TIME_US) {      
                    // Test fit with max values
                    //displayVarioInfos(10080.0, 1085.0, -20.5, ascent_rate);
                    displayVarioInfos(altitude, pressure, temperature, avg_ascent_rate);
                    beep(avg_ascent_rate);
                    barometer_time = micros();
                } 
            }
            break;
        /*
        case GPS: {
            break;
        }
        case COMPASS: 
            {}
            break;
        case ALL: 
            {}
            break;
        case SETTINGS: 
            {}
            break;
        */
    }
    //}

    
    last_time = millis();
    //while(micros() < loop_time + LOOP_TIME_US) {}// wait 
    /*
    Serial.print("Loop time: ");
    Serial.print((micros() - loop_time)/1000.0);
    Serial.println("ms");
    */
}

JOYSTICK readButtons(int pin) {
    int c = analogRead(JOYSTICK_PIN);
    //Serial.println(c);
    JOYSTICK b = NONE;
    if(c > 2800) b = UP; 
    else if(c < 2700 && c > 2600) b = LEFT;
    else if(c < 2200 && c > 2100) b = DOWN;
    else if(c < 2000 && c > 1900) b = PRESS;
    else if(c < 1850 && c > 1750) b = RIGHT;

    if (joystick_previous_state != b){
        joystick_previous_state = b;
        return b;
    }
    else{
        joystick_previous_state = b;
        return NONE;
    }
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

            // 1/bps to get the duration, * 1000 to switch to ms$
            ledcWriteTone(BUZZER_CHANNEL, curr_freq);
            //tone(BUZZER_PIN, curr_freq, curr_waiting_time);
        } 
        else if (smoothed_ascent_rate < sink_threshold) {
            //tone(BUZZER_PIN, 1000, 200);
        }
        else{
            curr_waiting_time = 1000.0;
            ledcWriteTone(BUZZER_CHANNEL, 400);
            //tone(BUZZER_PIN, 400, curr_waiting_time);
        }
        time_started_tone = millis();
    }
}
