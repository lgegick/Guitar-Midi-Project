// this code handles the push button and reflects the button press on a 7 segment light

#include <hidboot.h>
#include <usbh_midi.h>
#include <usbhub.h>
#include <SPI.h>
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
int previousState = 0;

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

  // Setup the USB MIDI communication
  if (Usb.Init() == -1) {
    Serial.println("USB Host Shield initialization failed.");
    while (1);  // Freeze execution if USB initialization fails
  }
  Serial.println("USB Host Shield initialized.");
}

void loop() {
  // handle USB communication continuously
  Usb.Task();

  int decodedNumber = 8;
  if (irrecv.decode()) {
    decodedNumber = translateIR();
  }

  // read the state of the button
  int buttonState1 = digitalRead(button1);
  int buttonState2 = digitalRead(button2);
  int buttonState3 = digitalRead(button3);

  if (buttonState1 == HIGH || decodedNumber == 0) {
    // set light to 0
    currentState = 0;
    Serial.println(currentState);
    displayRedLed();
    changeSnapshot(69, 0);
    delay(200);
  }
  else if (buttonState2 == HIGH || decodedNumber == 1) {
    // set light to 1
    currentState = 1;
    Serial.println(currentState);
    displayYellowLed();
    changeSnapshot(69, 1);
    delay(200);
  }
  else if (buttonState3 == HIGH || decodedNumber == 2) {
    // set the light to 2
    currentState = 2;
    Serial.println(currentState);
    displayGreenLed();
    changeSnapshot(69, 4);
    delay(200);
  }

  switch(currentState) {
    case 0:
      draw0();
      // changeSnapshot(leadSnapshot);
      break;
    case 1:
      draw1();
      // changeSnapshot(leadSnapshot2);
      break;
    case 2:
      draw2();
      // changeSnapshot(rhythmSnapshot);
      break;
    default:
      draw8();
      break;
  }
  irrecv.resume();
  delay(50);
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
  // Check if it is a repeat IR code 
  if (irrecv.decodedIRData.flags)
  {
    //set the current decodedRawData to the last decodedRawData 
    irrecv.decodedIRData.decodedRawData = last_decodedRawData;
    return last_decodedRawData;
  } 
  int decodedData = 8;

  //map the IR code to the remote key
  switch (irrecv.decodedIRData.decodedRawData)
  {
    case 0xE916FF00: decodedData = 0;    break;
    case 0xF30CFF00: decodedData = 1;    break;
    case 0xE718FF00: decodedData = 2;    break;
    default:
      decodedData = 8;
  }

  //store the last decodedRawData
  last_decodedRawData = decodedData;
  return decodedData;
  delay(500); // Do not get immediate repeat
}

void changeSnapshot(byte ccNumber, byte value) {
  // MIDI Control Change message consists of three bytes
  uint8_t midiMessage[3];
    
  // 0xB0 = Control Change message on channel 1
  midiMessage[0] = 0xB0 | 0x00;  // Control Change on channel 1 (0xB0)
  midiMessage[1] = ccNumber;     // Control Change number (snapshot number)
  midiMessage[2] = value;        // Control Change value (snapshot value 0-7)

  // display the data to the serial monitor
  Serial.print("MIDI CC Sent: ");
  Serial.print(ccNumber);
  Serial.print(" Value: ");
  Serial.println(value);
}
