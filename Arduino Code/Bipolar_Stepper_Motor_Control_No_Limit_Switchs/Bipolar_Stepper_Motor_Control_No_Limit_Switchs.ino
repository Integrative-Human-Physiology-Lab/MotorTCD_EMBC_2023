/*
   Credits to Robin2 for his serial parsing tutorial, from which parts of
   this code regarding serial inputs are heavily derived from:
   https://forum.arduino.cc/t/serial-input-basics-updated/382007
*/

/*
   Motor cable order
   A_bar / A / B_bar / B
   Black / Red / Yellow / Green
*/
// #include "PinChangeInterrupt.h";

// for our up/down motor
#define A1motor_bar    A2                 // the pin connected to the wire A- of the coil A (or to the H-bridge pin controlling the same wire) 
#define A1motor        A3                  // the pin connected to the wire A of the coil A (or to the H-bridge pin controlling the same wire)
#define B1_bar    A4                  // the pin connected to the wire B- of the coil B (or to the H-bridge pin controlling the same wire)
#define B1        A5                  // the pin connected to the wire B of the coil B (or to the H-bridge pin controlling the same wire)
// for our left/right motor
#define A2motor_bar    9                  // the pin connected to the wire A- of the coil A (or to the H-bridge pin controlling the same wire) 
#define A2motor        10                  // the pin connected to the wire A of the coil A (or to the H-bridge pin controlling the same wire)
#define B2_bar    11                 // the pin connected to the wire B- of the coil B (or to the H-bridge pin controlling the same wire)
#define B2        12                 // the pin connected to the wire B of the coil B (or to the H-bridge pin controlling the same wire)
// motor controld variables
#define x        1000                // smaller values may make the motor produce more speed and less torque
// Note: approximate distance is 6.2um/step for current motors

#define xdirectionpin A0
#define ydirectionpin A1

int steps = 0;
float joystick_speed=0.1;
// Note: Change joystick speed as per your like. This is the ratio of movement of motors to movement of joystick. 
//So, the higher the ratio the faster the motor moves.


// int updownstroke=0;
// int leftrightstroke=0;
// int upperlimitswitch=0;
// int lowerlimitswitch=0;

/*
   Limit switch pin/interrupt instantiations
   Note: these pins vary depending on the board used. Validate at
   https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
*/
#define up 7
#define down 13 
#define right 5
#define left 6

volatile bool upTrigger = 0;
volatile bool downTrigger = 0;
volatile bool leftTrigger = 0;
volatile bool rightTrigger = 0;

//======== For limit switch interrupts ==========//


void limitUp() {
  if (digitalRead(up) == HIGH) {
    upTrigger = 0;
    Serial.println("Upper limit switch detects no collision");

  } else {
    steps=200;
    StepDown();
    upTrigger = 1;
    Serial.println("Upper limit switch detects collision");
    
  }
}

void limitDown() {
  if (digitalRead(down) == HIGH) {
    downTrigger = 0;
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Down limit switch detects no collision");
  } else {
    steps=200;
    StepUp();
    downTrigger = 1;
    Serial.println("Down limit switch detects collision");
  }
}

void limitLeft() {
  if (digitalRead(left) == HIGH) {
    leftTrigger = 0;
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Left limit switch detects no collision");
  } else {
    steps=200;
    StepRight();
    leftTrigger = 1;
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Left limit switch detects collision");
  }
}

void limitRight() {
  if (digitalRead(right) == HIGH) {
    rightTrigger = 0;
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Right limit switch detects no collision");
  } else {
    rightTrigger = 1;
    steps=200;
    StepLeft();
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Right limit switch detects collision");
  }
}



//============ for motor control ================//

void MotorsOn1() { // turns on motors, to start operating
  pinMode(A1motor, OUTPUT);
  pinMode(A1motor_bar, OUTPUT);
  pinMode(B1, OUTPUT);
  pinMode(B1_bar, OUTPUT);
}

void MotorsOff1() { // turns off motors, to stop current draw. Useful after moving, or when not operating
  pinMode(A1motor, INPUT);
  digitalWrite(A1motor, HIGH);
  pinMode(A1motor_bar, INPUT);
  digitalWrite(A1motor_bar, HIGH);
  pinMode(B1, INPUT);
  digitalWrite(B1, HIGH);
  pinMode(B1_bar, INPUT);
  digitalWrite(B1_bar, HIGH);
}

void MotorsOn2() { // turns on motors, to start operating
  pinMode(A2motor, OUTPUT);
  pinMode(A2motor_bar, OUTPUT);
  pinMode(B2, OUTPUT);
  pinMode(B2_bar, OUTPUT);
}

void MotorsOff2() { // turns off motors, to stop current draw. Useful after moving, or when not operating
  pinMode(A2motor, INPUT);
  digitalWrite(A2motor, HIGH);
  pinMode(A2motor_bar, INPUT);
  digitalWrite(A2motor_bar, HIGH);
  pinMode(B2, INPUT);
  digitalWrite(B2, HIGH);
  pinMode(B2_bar, INPUT);
  digitalWrite(B2_bar, HIGH);
}

void StepUp() {
  MotorsOn1();

  for (int i = 0; i < (steps / 4) ; i++) {
    if (upTrigger) {
      Serial.println("Upper limit switch detects collision");
      
      break;
    }
    digitalWrite(A1motor, HIGH);
    digitalWrite(A1motor_bar, LOW);
    digitalWrite(B1, HIGH);
    digitalWrite(B1_bar, LOW);
    delayMicroseconds (x);

    if (upTrigger) {
      Serial.println("Upper limit switch detects collision");
      break;
    }
    digitalWrite(A1motor, LOW);
    digitalWrite(A1motor_bar, HIGH);
    digitalWrite(B1, HIGH);
    digitalWrite(B1_bar, LOW);
    delayMicroseconds (x);

    if (upTrigger) {
      Serial.println("Upper limit switch detects collision");
      break;
    }
    digitalWrite(A1motor, LOW);
    digitalWrite(A1motor_bar, HIGH);
    digitalWrite(B1, LOW);
    digitalWrite(B1_bar, HIGH);
    delayMicroseconds (x);

    if (upTrigger) {
      Serial.println("Upper limit switch detects collision");
      break;
    }
    digitalWrite(A1motor, HIGH);
    digitalWrite(A1motor_bar, LOW);
    digitalWrite(B1, LOW);
    digitalWrite(B1_bar, HIGH);
    delayMicroseconds (x);
  }
  Serial.println("StepUp Subroutine Complete");
  upTrigger=0;
  MotorsOff1();
}

void StepDown() {
  MotorsOn1();
  for (int i = 0; i < (steps / 4); i++) {
    if (downTrigger) {
      Serial.println("Lower limit switch detects collision");
      
      break;
    }
    digitalWrite(A1motor, HIGH);
    digitalWrite(A1motor_bar, LOW);
    digitalWrite(B1, LOW);
    digitalWrite(B1_bar, HIGH);
    delayMicroseconds (x);

    if (downTrigger) {
      Serial.println("Lower limit switch detects collision");
    
      break;
    }
    digitalWrite(A1motor, LOW);
    digitalWrite(A1motor_bar, HIGH);
    digitalWrite(B1, LOW);
    digitalWrite(B1_bar, HIGH);
    delayMicroseconds (x);

    if (downTrigger) {
      Serial.println("Lower limit switch detects collision");
      
      break;
    }
    digitalWrite(A1motor, LOW);
    digitalWrite(A1motor_bar, HIGH);
    digitalWrite(B1, HIGH);
    digitalWrite(B1_bar, LOW);
    delayMicroseconds (x);

    if (downTrigger) {
      Serial.println("Lower limit switch detects collision");
      
      break;
    }
    digitalWrite(A1motor, HIGH);
    digitalWrite(A1motor_bar, LOW);
    digitalWrite(B1, HIGH);
    digitalWrite(B1_bar, LOW);
    delayMicroseconds (x);
  }
  Serial.println("StepDown Subroutine Complete");
  downTrigger=0;
  MotorsOff1();
}

void StepLeft() {
  MotorsOn2();
  for (int i = 0; i < (steps / 4) ; i++) {
    if (leftTrigger) {
      Serial.println("Left limit switch detects collision");

      break;
    }
    digitalWrite(A2motor, HIGH);
    digitalWrite(A2motor_bar, LOW);
    digitalWrite(B2, HIGH);
    digitalWrite(B2_bar, LOW);
    delayMicroseconds (x);

    if (leftTrigger) {
      Serial.println("Left limit switch detects collision");
      break;
    }
    digitalWrite(A2motor, LOW);
    digitalWrite(A2motor_bar, HIGH);
    digitalWrite(B2, HIGH);
    digitalWrite(B2_bar, LOW);
    delayMicroseconds (x);

    if (leftTrigger) {
      Serial.println("Left limit switch detects collision");
      break;
    }
    digitalWrite(A2motor, LOW);
    digitalWrite(A2motor_bar, HIGH);
    digitalWrite(B2, LOW);
    digitalWrite(B2_bar, HIGH);
    delayMicroseconds (x);

    if (leftTrigger) {
      Serial.println("Left limit switch detects collision");
      break;
    }
    digitalWrite(A2motor, HIGH);
    digitalWrite(A2motor_bar, LOW);
    digitalWrite(B2, LOW);
    digitalWrite(B2_bar, HIGH);
    delayMicroseconds (x);
  }
  Serial.println("StepLeft Subroutine Complete");
  leftTrigger=0;
  MotorsOff2();
}

void StepRight() {
  MotorsOn2();
  for (int i = 0; i < (steps / 4); i++) {
    if (rightTrigger) {
      Serial.println("Right limit switch detects collision");
      break;
    }
    digitalWrite(A2motor, HIGH);
    digitalWrite(A2motor_bar, LOW);
    digitalWrite(B2, LOW);
    digitalWrite(B2_bar, HIGH);
    delayMicroseconds (x);

    if (rightTrigger) {
      Serial.println("Right limit switch detects collision");
      break;
    }
    digitalWrite(A2motor, LOW);
    digitalWrite(A2motor_bar, HIGH);
    digitalWrite(B2, LOW);
    digitalWrite(B2_bar, HIGH);
    delayMicroseconds (x);

    if (rightTrigger) {
      Serial.println("Right limit switch detects collision");
      break;
    }
    digitalWrite(A2motor, LOW);
    digitalWrite(A2motor_bar, HIGH);
    digitalWrite(B2, HIGH);
    digitalWrite(B2_bar, LOW);
    delayMicroseconds (x);

    if (rightTrigger) {
      Serial.println("Right limit switch detects collision");
      break;
    }
    digitalWrite(A2motor, HIGH);
    digitalWrite(A2motor_bar, LOW);
    digitalWrite(B2, HIGH);
    digitalWrite(B2_bar, LOW);
    delayMicroseconds (x);
  }
  Serial.println("StepRight Subroutine Complete");
  rightTrigger=0;
  MotorsOff2();
}



//=================== Arduino Setup ===================//
void setup() {
  // setting our motor pin states as internal pull-up until ready to actuate
  MotorsOff1();
  MotorsOff2();

  // For Serial Monitor
  Serial.begin(9600);
  Serial.println("The setup is ready. Use joystick for movement.");

  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);
  pinMode(left, INPUT_PULLUP);

  // if(updownstroke==0){
  //   Serial.println("Homing starting");

  //    while(digitalRead(up) == HIGH){
  //       steps=10;
  //       StepUp();  
  //    }

  //   while(digitalRead(down) == HIGH){
  //     steps=10;
  //     StepDown();
  //     updownstroke=updownstroke+steps;
  //   }
  //   while(digitalRead(right) == HIGH){
  //       steps=20;
  //       StepRight();  
  //    }

  //   while(digitalRead(left) == HIGH){
  //     steps=10;
  //     StepLeft();
  //     leftrightstroke=leftrightstroke+steps;
  //   }
    
  // }
  // Serial.println("updownstroke");
  // Serial.println(updownstroke);
  // Serial.println("leftrightstroke");
  // Serial.println(leftrightstroke);
  // steps=updownstroke/2;
  // StepUp();
  // steps=leftrightstroke/2;
  // StepRight();
  // attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(up), limitUp, FALLING);
  // attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(down), limitDown, FALLING);
  // attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(right), limitRight, FALLING);
  // attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(left), limitLeft, FALLING);

  

}

//============= Main loop ================//
void loop() {
  // read data from joystick and use that for movement.
  
   int xdirection=analogRead(xdirectionpin);
   int ydirection=analogRead(ydirectionpin);
  
    int xdirection_difference= xdirection-510;
    int ydirection_difference= ydirection-505;

    if (ydirection_difference < -5) {
      steps=joystick_speed*abs(ydirection_difference);
      Serial.println(steps);
      StepUp();
    }
    if (ydirection_difference >5 ) {
      steps=joystick_speed*abs(ydirection_difference);
      Serial.println(steps);
      StepDown();
    }
    if (xdirection_difference < -5) {
      steps=joystick_speed*abs(xdirection_difference);
      StepRight();
    }
    if (xdirection_difference > 5) {
      steps=joystick_speed*abs(xdirection_difference);
      StepLeft();
    }
  
}
