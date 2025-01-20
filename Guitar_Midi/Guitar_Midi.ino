// this code handles the push button and reflects the button press on a 7 segment light

#include <usbh_midi.h>
#include <usbhub.h>
#include "IRremote.h"

int rhythmSnapshot = 3;
int leadSnapshot = 1;
int leadSnapshot2 = 2;

// MACROS for use in the 7 segment light
#define A 2
#define B 3
#define C 4
#define D 5
#define E 6
#define F 7
#define G 8

// MACROS for use of the push button
#define button1 10
#define button2 11
#define button3 12
int currentState = 0;
int previousState = -1;

// IR receiver variables
int receiver = A0;
IRrecv irrecv(receiver);
uint32_t last_decodedRawData = 0;

// values for the different LED's
#define redLed A5
#define yellowLed A4
#define greenLed A3

// Create the objects for handling USB output
USB Usb;
USBH_MIDI Midi(&Usb);

void setup() {
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);

  // setup the button reading
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  Serial.begin(9600); // initialize the serial communication
  irrecv.enableIRIn();
  draw8();

  // Initialize the USB Host Shield
  if (Usb.Init() == -1) {
    Serial.println("USB Host Shield initialization failed.");
    delay(1000);
  }
  Serial.println(Usb.getUsbTaskState());
  Serial.println("USB Host Shield initialized.");
}

void loop() {
  Serial.println();

  // handle USB communication continuously
  Usb.Task();
  Serial.println(Usb.getUsbTaskState());


  int decodedNumber = 8;
  /*
  if (irrecv.decode()) {
    decodedNumber = translateIR();
  } */


  // read the state of the button
  int buttonState1 = digitalRead(button1);
  int buttonState2 = digitalRead(button2);
  int buttonState3 = digitalRead(button3);

  Serial.println("buttonState1: " + String(buttonState1) + "\tbuttonState2: " + String(buttonState2) + "\tbuttonState3: " + String(buttonState3));

  if (buttonState1 == LOW || decodedNumber == 0) {
    // set light to 0
    currentState = 0;
    Serial.println("BUTTON PRESSED");
    delay(200);
  }
  else if (buttonState2 == LOW || decodedNumber == 1) {
    // set light to 1
    currentState = 1;
    Serial.println("BUTTON PRESSED");
    delay(200);
  }
  else if (buttonState3 == LOW || decodedNumber == 2) {
    // set the light to 2
    currentState = 2;
    Serial.println("BUTTON PRESSED");
    delay(200);
  } 

  Serial.println("USB Task State: " + String(static_cast<bool>(Usb.getUsbTaskState())));
  if (currentState != previousState && Usb.getUsbTaskState() == 144) {
    switch(currentState) {
      case 0: {
        draw0();
        displayRedLed();
        uint8_t snapshot1[] = {0xB0, 69, 0};
        Midi.SendData(snapshot1, sizeof(snapshot1));
        Serial.println("CURRENT SNAPSHOT 0");
        break;
      }
      case 1: {
        draw1();
        displayYellowLed();
        uint8_t snapshot2[] = {0xB0, 69, 1};
        Midi.SendData(snapshot2, sizeof(snapshot2));
        Serial.println("CURRENT SNAPSHOT 1");
        break;
      }
      case 2: {
        draw2();
        displayGreenLed();
        uint8_t snapshot3[] = {0xB0, 69, 4};
        Midi.SendData(snapshot3, sizeof(snapshot3));
        Serial.println("CURRENT SNAPSHOT 4");
        break;
      }
      default: {
        draw8();
        break;
      }
    }

    previousState = currentState;
  }

  delay(1000);
}

void displayRedLed() {
  digitalWrite(redLed, HIGH);
  digitalWrite(greenLed, LOW);
  digitalWrite(yellowLed, LOW);
}

void displayGreenLed() {
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, HIGH);
  digitalWrite(yellowLed, LOW);
}

void displayYellowLed() {
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(yellowLed, HIGH);
}

void draw0() {
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(F, HIGH);
  digitalWrite(G, LOW);
}

void draw1() {
  digitalWrite(A, LOW);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
}

void draw2() {
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, LOW);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(F, LOW);
  digitalWrite(G, HIGH);
}

void draw3() {
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, HIGH);
}

void draw4() {
  digitalWrite(A, LOW);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
}

void draw5() {
  digitalWrite(A, HIGH);
  digitalWrite(B, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, LOW);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
}

void draw6() {
  digitalWrite(A, HIGH);
  digitalWrite(B, LOW);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
}

void draw7() {
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
}

void draw8() {
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
}

void draw9() {
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
}

int translateIR() {
  if (irrecv.decode()) {
    // Check if it is a repeat IR code 
    if (irrecv.decodedIRData.flags) {
      // set the current decodedRawData to the last decodedRawData 
      irrecv.decodedIRData.decodedRawData = last_decodedRawData;
    } else {
      // Store the new raw data
      last_decodedRawData = irrecv.decodedIRData.decodedRawData;
    }

    // Map the IR code to the remote key
    int decodedData = 8;  // Default to 8 for unrecognized codes
    switch (last_decodedRawData) {
      case 0xE916FF00: decodedData = 0; break;  // Remote button 0
      case 0xF30CFF00: decodedData = 1; break;  // Remote button 1
      case 0xE718FF00: decodedData = 2; break;  // Remote button 2
      default: decodedData = 8;
    }

    // Print the decoded value to the serial monitor for debugging
    Serial.print("IR Decoded Data: ");
    Serial.println(decodedData);

    // Prepare to receive the next value
    irrecv.resume();
    return decodedData;  // Return the decoded data for use in loop()
  }
  return 8;  // Default value when no IR code is detected
}

