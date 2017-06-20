//________  __________ .__                                                                   
//\_____  \ \______   \|  |__  _____     ______  ____                                        
//  _(__  <  |     ___/|  |  \ \__  \   /  ___/_/ __ \                                       
// /       \ |    |    |   Y  \ / __ \_ \___ \ \  ___/                                       
///______  / |____|    |___|  /(____  //____  > \___  >                                      
//       \/                 \/      \/      \/      \/                                       
//__________ .____     ________   _________    ________           .__                        
//\______   \|    |    \______ \  \_   ___ \   \______ \  _______ |__|___  __  ____  _______ 
// |    |  _/|    |     |    |  \ /    \  \/    |    |  \ \_  __ \|  |\  \/ /_/ __ \ \_  __ \
// |    |   \|    |___  |    `   \\     \____   |    `   \ |  | \/|  | \   / \  ___/  |  | \/
// |______  /|_______ \/_______  / \______  /  /_______  / |__|   |__|  \_/   \___  > |__|   
//        \/         \/        \/         \/           \/                         \/         

// Kyle Hess
// 05-29-2017
// University of Washington
// EE 449

//Controls the switching of a three-phase inverter bridge
//for driving a BLDC three-phase motor. This code is 
//proprietary, and meant to be used with the custom DF45
//motor driver board for QB^3.

//SPEED
float RPM = 0.0;
float stepCount = 0.0;
int prevTime = 0;

// MUX BIT OUTPUTS
const int muxA0 = 5; //PD5
const int muxA1 = 6; //PD6
const int muxAe = 11; //PB3
const int muxB0 = 7; //PD7

const int muxB1 = 8; //PB0
const int muxBe = 12; //PB4
const int muxC0 = 9; //PB1
const int muxC1 = 10; //PB2
const int muxCe = 13; //PB5

// HALL SENSOR INPUTS
const int HallA = 2; //PD2
const int HallB = 3; //PD3
const int HallC = 4; //PD4

// HALL EFFECT READING VARS
char my_varA = 0;
char my_varB = 0;
char my_varC = 0;
int A;
int B;
int C;

// MOTOR STEP LOCATION
long curStep = 0;
long prevStep = 0;
long prevStep2 = 0;

void setup() {
  //Initialize Hall effect input pins
  pinMode(HallA, INPUT);
  pinMode(HallB, INPUT);
  pinMode(HallC, INPUT);

  //Initialize outputs/inputs
  DDRD = 0b11100000;
  DDRB = 0b11111111;

  //Detect the initial rotor step location
  A = digitalRead(HallA);
  B = digitalRead(HallB);
  C = digitalRead(HallC);
  if (A == 1 && B == 0 && C == 0) {
    curStep = 1;
  } else if (A == 1 && B == 1 && C == 0) {
    curStep = 2;
  } else if (A == 0 && B == 1 && C == 0) {
    curStep = 3;
  } else if (A == 0 && B == 1 && C == 1) {
    curStep = 4;
  } else if (A == 0 && B == 0 && C == 1) {
    curStep = 5;
  } else {
    curStep = 6;
  }

  //Initialize all MOSFETs LOW!
  turnOff();
}

void loop() {
  runMotor();
}

void runMotor() {
  // READ & GET THE HALL SENSOR STATES
  A = (PIND & (1 << PD2));
  B = (PIND & (1 << PD3));
  C = (PIND & (1 << PD4));

  prevStep = curStep;
  if (A == 4 && B == 0 && C == 0) {
    PORTD = 0b00000000;
    PORTB = 0b00010011;
    curStep = 1;
  } else if (A == 4 && B == 8 && C == 0) {
    PORTD = 0b01000000;
    PORTB = 0b00001010;
    curStep = 2;
  } else if (A == 0 && B == 8 && C == 0) {
    PORTD = 0b00100000;
    PORTB = 0b00100100;
    curStep = 3;
  } else if (A == 0 && B == 8 && C == 16) {
    PORTD = 0b00100000;
    PORTB = 0b00010001;
    curStep = 4;
  } else if (A == 0 && B == 0 && C == 16) {
    PORTD = 0b11000000;
    PORTB = 0b00001000;
    curStep = 5;
  } else if (A == 4 && B == 0 && C == 16) {
    PORTD = 0b10000000;
    PORTB = 0b00100100;
    curStep = 6;
  }
}

void turnOff() {
  PORTD = 0b01001110;
  PORTB = 0b00111101;
}

void getRpm() {
  // CALCULATE RPM
  if (curStep != prevStep) {
    stepCount++;
  }
  prevStep = curStep;
  if (millis() - prevTime >= 250) {
    RPM = (stepCount / 48.0) * 240.0;
    stepCount = 0.0;
    prevTime = millis();
  }
}
