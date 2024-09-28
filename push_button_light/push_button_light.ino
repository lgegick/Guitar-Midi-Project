// this code handles the push button and reflects the button press on a 7 segment light
#include <MIDI.h>
#include <hidboot.h>
#include "IRremote.h"

MIDI_CREATE_DEFAULT_INSTANCE();

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
int previousState = 0;

// constants for IR receiver
int receiver = A0;

void setup() {
  // put your setup code here, to run once:
  MIDI.begin(MIDI_CHANNEL_OMNI); // listen to all MIDI channels

  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);

  // setup the button reading
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  Serial.begin(9600); // initialize the serial communication
}

void loop() {

  // read the state of the button
  int buttonState1 = digitalRead(button1);
  int buttonState2 = digitalRead(button2);
  int buttonState3 = digitalRead(button3);

  if (buttonState1 == HIGH) {
    // set light to 0
    currentState = 0;
    Serial.println(currentState);
    delay(200);
  }
  else if (buttonState2 == HIGH) {
    // set light to 1
    currentState = 1;
    Serial.println(currentState);
    delay(200);
  }
  else if (buttonState3 == HIGH) {
    // set the light to 2
    currentState = 2;
    Serial.println(currentState);
    delay(200);
  }

  switch(currentState) {
    case 0:
      draw0();
      changeSnapshot(leadSnapshot);
      break;
    case 1:
      draw1();
      changeSnapshot(leadSnapshot2);
      break;
    case 2:
      draw2();
      changeSnapshot(rhythmSnapshot);
      break;
    default:
      draw8();
      break;
  }

  delay(50);
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

void changeSnapshot(int snapshotNumber) {
  // Send Control Change (CC#69) on Channel 1 to change snapshots
  MIDI.sendControlChange(69, snapshotNumber, 1);  // CC#69, value = snapshotNumber, channel = 1
}
