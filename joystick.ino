#include "Joystick.h"

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK, 16, 0,
   true,  true,  true, false, false, false,  true, true, false, false, false);
//    X,     Y,     Z,    RX,    RY,    RZ,  rudr, throt, accel, brake, steer);


#define JOYAX1_PIN  A1 // Arduino pin connected to VRX pin
#define JOYAX2_PIN  A2 // Arduino pin connected to VRY pin
#define JOYAX3_PIN  A4 // Arduino pin connected to VRY pin
#define JOYAX4_PIN  A3 // Arduino pin connected to VRY pin
#define JOYAX5_PIN  A0 // Arduino pin connected to VRY pin
#define HIGHPIN 13
#define CHKPIN 12

#define buttchanA 2
#define buttchanB 3
#define buttchanC 4
#define buttchanD 5

#define buttreadA 8
#define buttreadB 9
#define buttreadC 7
#define buttreadD 6

int ButtonState[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int reRange(int inval, int inmin, int inmax, int neg){
  int range=inmax-inmin;
  float oldval=inval;
  int newval=(((oldval-inmin)*1023)/range);
  newval=min(newval,1023);
  newval=max(newval,0);
  if (neg<0) {
    newval=1023-newval;
  }
  return int(newval);
}

void setup() {
  //Serial.begin(9600);
  pinMode(JOYAX1_PIN, INPUT_PULLUP);
  pinMode(JOYAX2_PIN, INPUT_PULLUP);
  pinMode(JOYAX3_PIN, INPUT_PULLUP);
  pinMode(JOYAX4_PIN, INPUT_PULLUP);
  pinMode(JOYAX5_PIN, INPUT_PULLUP);
  
  pinMode(HIGHPIN, OUTPUT);
  digitalWrite(HIGHPIN, HIGH);
  pinMode(buttchanA, OUTPUT);
  pinMode(buttchanB, OUTPUT);
  pinMode(buttchanC, OUTPUT);
  pinMode(buttchanD, OUTPUT);
  digitalWrite(buttchanA, HIGH);
  digitalWrite(buttchanB, HIGH);
  digitalWrite(buttchanC, HIGH);
  digitalWrite(buttchanD, HIGH);
  pinMode(buttreadA, INPUT_PULLUP);
  pinMode(buttreadB, INPUT_PULLUP);
  pinMode(buttreadC, INPUT_PULLUP);
  pinMode(buttreadD, INPUT_PULLUP);
  pinMode(CHKPIN, INPUT); 
  Joystick.begin();
}

void loop() {
  int joy_ax1_val = analogRead(JOYAX1_PIN); // X axis, right stick regx
  int joy_ax2_val = analogRead(JOYAX2_PIN); // y axis, right stick regy
  int joy_ax3_val = analogRead(JOYAX3_PIN); // X axis, right stick rudder
  int joy_ax4_val = analogRead(JOYAX4_PIN); // Y axis, right stick throttle
  int joy_ax5_val = analogRead(JOYAX5_PIN); // Z axis, slider for trim               

  int pinW;
  int pinR;
  for (int idxW = 0; idxW < 4; idxW++){
    if (idxW == 0) {pinW=buttchanA;}
    if (idxW == 1) {pinW=buttchanB;}
    if (idxW == 2) {pinW=buttchanC;}
    if (idxW == 3) {pinW=buttchanD;}
    digitalWrite(pinW, LOW);
    for (int idxR = 0; idxR < 4; idxR++){
      if (idxR == 0) {pinR=buttreadA;}
      if (idxR == 1) {pinR=buttreadB;}
      if (idxR == 2) {pinR=buttreadC;}
      if (idxR == 3) {pinR=buttreadD;}
      int ChkButtonState=!digitalRead(pinR);
      int butnum=(idxW*4)+idxR;
      if (ChkButtonState != ButtonState[butnum]){
        Joystick.setButton(butnum, ChkButtonState);
        ButtonState[butnum] = ChkButtonState;
      }
    }
    digitalWrite(pinW, HIGH);
  }

  joy_ax1_val=reRange(joy_ax1_val,92,977,1);
  joy_ax2_val=reRange(joy_ax2_val,64,928,1);
  joy_ax3_val=reRange(joy_ax3_val,88,985,1);
  joy_ax4_val=reRange(joy_ax4_val,47,920,-1);
  joy_ax5_val=reRange(joy_ax5_val,15,1023,-1);
  /*Serial.println( String(joy_ax1_val)+" "+
                  String(joy_ax2_val)+" "+
                  String(joy_ax3_val)+" "+
                  String(joy_ax4_val)+" "+
                  String(joy_ax5_val)+" ");
  */
  Joystick.setXAxis(joy_ax1_val);
  Joystick.setYAxis(joy_ax2_val);
  Joystick.setRudder(joy_ax3_val);
  Joystick.setThrottle(joy_ax4_val);
  Joystick.setZAxis(joy_ax5_val);

  if (digitalRead(CHKPIN) == LOW){
    Joystick.end();
  }
  delay(5);
}
