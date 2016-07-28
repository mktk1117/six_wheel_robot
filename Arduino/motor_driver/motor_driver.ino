#include <string.h>

// pin settings
int pwmPin0 = 3;
int pwmPin1 = 5;
int pwmPin2 = 6;
int pwmPin3 = 9;
int pwmPin4 = 10;
int pwmPin5 = 11;
int INaPin0 = 2;
int INbPin0 = 4;
int INaPin1 = 7;
int INbPin1 = 8;
int INaPin2 = 12;
int INbPin2 = 13;
int INaPin3 = A5;
int INbPin3 = A4;
int INaPin4 = A3;
int INbPin4 = A2;
int INaPin5 = A1;
int INbPin5 = A0;

void setup() {
    Serial.begin(115200);

    // set pins to output
    pinMode(pwmPin0, OUTPUT);
    pinMode(pwmPin1, OUTPUT);
    pinMode(pwmPin2, OUTPUT);
    pinMode(pwmPin3, OUTPUT);
    pinMode(pwmPin4, OUTPUT);
    pinMode(pwmPin5, OUTPUT);
    pinMode(INaPin0, OUTPUT);
    pinMode(INbPin0, OUTPUT);
    pinMode(INaPin1, OUTPUT);
    pinMode(INbPin1, OUTPUT);
    pinMode(INaPin2, OUTPUT);
    pinMode(INbPin2, OUTPUT);
    pinMode(INaPin3, OUTPUT);
    pinMode(INbPin3, OUTPUT);
    pinMode(INaPin4, OUTPUT);
    pinMode(INbPin4, OUTPUT);
    pinMode(INaPin5, OUTPUT);
    pinMode(INbPin5, OUTPUT);
}


// function to control motor
// speed is how fast the motor rotates
// Please set pwmPin, InaPin and INbPin for the motor you want to drive
void control_motor(int speed, int pwmPin, int INaPin, int INbPin){
    if(speed > 0){
        analogWrite(pwmPin, speed);
        digitalWrite(INaPin, HIGH);
        digitalWrite(INbPin, LOW);
    }
    else if(speed < 0){
        analogWrite(pwmPin, -speed);
        digitalWrite(INaPin, LOW);
        digitalWrite(INbPin, HIGH);
    }
    else{
        digitalWrite(INaPin, LOW);
        digitalWrite(INbPin, LOW);
    }
}

// In time loop, receives from serial and control 6 motors
void loop() {
    static int speed[6];
    static char buff[30];
    int counter = 0;

    // read command from raspberry pi
    while(Serial.available()){
        buff[counter] = Serial.read();
        if (counter > 30 || buff[counter] == '*') {
            buff[counter] = '\0';
            speed[0]=atoi(strtok(buff,","));
            speed[1]=atoi(strtok(NULL,","));
            speed[2]=atoi(strtok(NULL,","));
            speed[3]=atoi(strtok(NULL,","));
            speed[4]=atoi(strtok(NULL,","));
            speed[5]=atoi(strtok(NULL,","));
        }
        else{
            counter++;
        }
    }

    // control motors
    control_motor(speed[0], pwmPin0, INaPin0, INbPin0);
    control_motor(speed[1], pwmPin1, INaPin1, INbPin1);
    control_motor(speed[2], pwmPin2, INaPin2, INbPin2);
    control_motor(speed[3], pwmPin3, INaPin3, INbPin3);
    control_motor(speed[4], pwmPin4, INaPin4, INbPin4);
    control_motor(speed[5], pwmPin5, INaPin5, INbPin5);

    // send message to raspberry pi
    Serial.print(speed[0]); Serial.print(",");
    Serial.print(speed[1]); Serial.print(",");
    Serial.print(speed[2]); Serial.print(",");
    Serial.print(speed[3]); Serial.print(",");
    Serial.print(speed[4]); Serial.print(",");
    Serial.println(speed[5]);

    delay(100);
}
