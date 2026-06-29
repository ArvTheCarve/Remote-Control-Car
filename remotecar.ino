#include <IRremote.hpp>

/* WELCOME TO MY CAR PROJECT!!!

TO USE THIS CAR, 2 is forward, 6 is right, 4 is left, 
8 is backwards, and 5 is stop.

DEFAULT SPEED IS 0 SO MAKE SURE TO USE VOL+ TO INCREASE IT !!!
*/



// Motor control pins
const int controlPin1 = 3;  // Left motor negative
const int controlPin2 = 2;  // Left motor positive
const int controlPin3 = 7;  // Right motor positive
const int controlPin4 = 8;  // Right motor negative
const int enablePin = 9;    // Enable pin for left motor
const int enablePin2 = 6;   // Enable pin for right motor
const int IR_RECEIVE_PIN = 11;  // IR receiver pin

// Variables
int motorSpeed = 0;
bool motorEnabled = false;

void setup() {
    // Set motor control pins as outputs
    pinMode(controlPin1, OUTPUT);
    pinMode(controlPin2, OUTPUT);
    pinMode(controlPin3, OUTPUT);
    pinMode(controlPin4, OUTPUT);
    pinMode(enablePin, OUTPUT);
    pinMode(enablePin2, OUTPUT);
    
    // Start serial for debugging
    Serial.begin(9600);
    
    // Initialize IR receiver with specific protocol
    IrReceiver.begin(IR_RECEIVE_PIN);
    
    // Initially stop motors
    stop();
}

void loop() {
    if (IrReceiver.decode()) {

        Serial.print("Received IR command: 0x"); //shortens the Hex code
        Serial.println(IrReceiver.decodedIRData.command, HEX);
        
        // Movement commands
        if (IrReceiver.decodedIRData.command == 0x11) { // UP button
            motorEnabled = true;
            forwards();
        } else if (IrReceiver.decodedIRData.command == 0x19) { // DOWN button
            motorEnabled = true;
            backwards();
        } else if (IrReceiver.decodedIRData.command == 0x14) { // LEFT button
            motorEnabled = true;
            left();
        } else if (IrReceiver.decodedIRData.command == 0x16) { // RIGHT button
            motorEnabled = true;
            right();
        } else if (IrReceiver.decodedIRData.command == 0x15) { // STOP/OK button
            motorEnabled = false;
            stop();
        }
        
        // Speed adjustment
        if (IrReceiver.decodedIRData.command == 0x1) { // Increase speed
            motorSpeed = min(motorSpeed + 25, 255);  // Cap speed at 255
            Serial.print("Increased speed to: ");
            Serial.println(motorSpeed);
            analogWrite(enablePin, motorSpeed);
            analogWrite(enablePin2, motorSpeed);
        } else if (IrReceiver.decodedIRData.command == 0x9) { // Decrease speed
            motorSpeed = max(motorSpeed - 25, 0);  // Ensure speed doesn’t go below 0
            Serial.print("Decreased speed to: ");
            Serial.println(motorSpeed);
            analogWrite(enablePin, motorSpeed);
            analogWrite(enablePin2, motorSpeed);
        }
        
        IrReceiver.resume();
    }
}

void forwards() {
    // Both motors forward
    digitalWrite(controlPin1, LOW);   // Left motor -
    digitalWrite(controlPin2, HIGH);  // Left motor +
    digitalWrite(controlPin3, HIGH);  // Right motor +
    digitalWrite(controlPin4, LOW);   // Right motor -
}

void backwards() {
    // Both motors backward
    digitalWrite(controlPin1, HIGH);  // Left motor -
    digitalWrite(controlPin2, LOW);   // Left motor +
    digitalWrite(controlPin3, LOW);   // Right motor +
    digitalWrite(controlPin4, HIGH);  // Right motor -
}

void left() {
    // Left motor off, Right motor forward
    digitalWrite(controlPin1, LOW);  // Left motor -
    digitalWrite(controlPin2, LOW);   // Left motor +
    digitalWrite(controlPin3, HIGH);  // Right motor +
    digitalWrite(controlPin4, LOW);   // Right motor -
}

void right() {
    // Left motor forward, Right motor off
    digitalWrite(controlPin1, LOW);   // Left motor -
    digitalWrite(controlPin2, HIGH);  // Left motor +
    digitalWrite(controlPin3, LOW);   // Right motor +
    digitalWrite(controlPin4, LOW);   // Right motor -
}

void stop() {
    // All pins LOW to stop both motors
    digitalWrite(controlPin1, LOW);
    digitalWrite(controlPin2, LOW);
    digitalWrite(controlPin3, LOW);
    digitalWrite(controlPin4, LOW);
    motorEnabled = false;
}
