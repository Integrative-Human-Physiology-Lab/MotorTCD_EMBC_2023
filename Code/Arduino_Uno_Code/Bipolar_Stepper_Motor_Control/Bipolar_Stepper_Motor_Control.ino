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

// for our up/down motor
#define A1_bar    2                  // the pin connected to the wire A- of the coil A (or to the H-bridge pin controlling the same wire) 
#define A1        3                  // the pin connected to the wire A of the coil A (or to the H-bridge pin controlling the same wire)
#define B1_bar    4                  // the pin connected to the wire B- of the coil B (or to the H-bridge pin controlling the same wire)
#define B1        5                  // the pin connected to the wire B of the coil B (or to the H-bridge pin controlling the same wire)
// for our left/right motor
#define A2_bar    8                  // the pin connected to the wire A- of the coil A (or to the H-bridge pin controlling the same wire) 
#define A2        9                  // the pin connected to the wire A of the coil A (or to the H-bridge pin controlling the same wire)
#define B2_bar    10                 // the pin connected to the wire B- of the coil B (or to the H-bridge pin controlling the same wire)
#define B2        11                 // the pin connected to the wire B of the coil B (or to the H-bridge pin controlling the same wire)
// motor control variables
#define x        1000                // smaller values may make the motor produce more speed and less torque
// Note: approximate distance is 6.2um/step for current motors

/*
   Serial Parsing instantiations
   includes variables for storing data, holding parsed data, and new data input checks
*/

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];            // temporary array for use when parsing

char stepDirection[numChars] = {0};
int steps = 0;

boolean newData = false;

/*
   Limit switch pin/interrupt instantiations
   Note: these pins vary depending on the board used. Validate at
   https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
*/
byte up = 18;
byte down = 19;
byte left = 20;
byte right = 21;

bool upTrigger = 0;
bool downTrigger = 0;
bool leftTrigger = 0;
bool rightTrigger = 0;

//======== For limit switch interrupts ==========//
void limitUp() {
  if (digitalRead(up) == HIGH) {
    upTrigger = 0;
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    upTrigger = 1;
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void limitDown() {
  if (digitalRead(down) == HIGH) {
    downTrigger = 0;
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    downTrigger = 1;
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void limitLeft() {
  if (digitalRead(left) == HIGH) {
    leftTrigger = 0;
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    leftTrigger = 1;
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void limitRight() {
  if (digitalRead(right) == HIGH) {
    rightTrigger = 0;
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    rightTrigger = 1;
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

//========== For detecting start/end markers in Serial input ===========//
void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();
    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      } else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    } else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

//=========== For parsing Serial input data ============//
void parseData() {      // split the data into its parts
  char * strtokIndx; // this is used by strtok() as an index
  strtokIndx = strtok(tempChars, ",");
  strcpy(stepDirection, strtokIndx);
  strtokIndx = strtok(NULL, ",");
  steps = atoi(strtokIndx);
}

//============= To show parsed Serial inputs ==============//
void showParsedData() {
  Serial.print("Direction ");
  Serial.println(stepDirection);
  Serial.print("Steps ");
  Serial.println(steps);
}



//============ for motor control ================//

void MotorsOn1() { // turns on motors, to start operating
  pinMode(A1, OUTPUT);
  pinMode(A1_bar, OUTPUT);
  pinMode(B1, OUTPUT);
  pinMode(B1_bar, OUTPUT);
}

void MotorsOff1() { // turns off motors, to stop current draw. Useful after moving, or when not operating
  pinMode(A1, INPUT);
  digitalWrite(A1, HIGH);
  pinMode(A1_bar, INPUT);
  digitalWrite(A1_bar, HIGH);
  pinMode(B1, INPUT);
  digitalWrite(B1, HIGH);
  pinMode(B1_bar, INPUT);
  digitalWrite(B1_bar, HIGH);
}

void MotorsOn2() { // turns on motors, to start operating
  pinMode(A2, OUTPUT);
  pinMode(A2_bar, OUTPUT);
  pinMode(B2, OUTPUT);
  pinMode(B2_bar, OUTPUT);
}

void MotorsOff2() { // turns off motors, to stop current draw. Useful after moving, or when not operating
  pinMode(A2, INPUT);
  digitalWrite(A2, HIGH);
  pinMode(A2_bar, INPUT);
  digitalWrite(A2_bar, HIGH);
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
    digitalWrite(A1, HIGH);
    digitalWrite(A1_bar, LOW);
    digitalWrite(B1, HIGH);
    digitalWrite(B1_bar, LOW);
    delayMicroseconds (x);

    if (upTrigger) {
      Serial.println("Upper limit switch detects collision");
      break;
    }
    digitalWrite(A1, LOW);
    digitalWrite(A1_bar, HIGH);
    digitalWrite(B1, HIGH);
    digitalWrite(B1_bar, LOW);
    delayMicroseconds (x);

    if (upTrigger) {
      Serial.println("Upper limit switch detects collision");
      break;
    }
    digitalWrite(A1, LOW);
    digitalWrite(A1_bar, HIGH);
    digitalWrite(B1, LOW);
    digitalWrite(B1_bar, HIGH);
    delayMicroseconds (x);

    if (upTrigger) {
      Serial.println("Upper limit switch detects collision");
      break;
    }
    digitalWrite(A1, HIGH);
    digitalWrite(A1_bar, LOW);
    digitalWrite(B1, LOW);
    digitalWrite(B1_bar, HIGH);
    delayMicroseconds (x);
  }
  Serial.println("StepUp Subroutine Complete");
  MotorsOff1();
}

void StepDown() {
  MotorsOn1();
  for (int i = 0; i < (steps / 4); i++) {
    if (downTrigger) {
      Serial.println("Lower limit switch detects collision");
      break;
    }
    digitalWrite(A1, HIGH);
    digitalWrite(A1_bar, LOW);
    digitalWrite(B1, LOW);
    digitalWrite(B1_bar, HIGH);
    delayMicroseconds (x);

    if (downTrigger) {
      Serial.println("Lower limit switch detects collision");
      break;
    }
    digitalWrite(A1, LOW);
    digitalWrite(A1_bar, HIGH);
    digitalWrite(B1, LOW);
    digitalWrite(B1_bar, HIGH);
    delayMicroseconds (x);

    if (downTrigger) {
      Serial.println("Lower limit switch detects collision");
      break;
    }
    digitalWrite(A1, LOW);
    digitalWrite(A1_bar, HIGH);
    digitalWrite(B1, HIGH);
    digitalWrite(B1_bar, LOW);
    delayMicroseconds (x);

    if (downTrigger) {
      Serial.println("Lower limit switch detects collision");
      break;
    }
    digitalWrite(A1, HIGH);
    digitalWrite(A1_bar, LOW);
    digitalWrite(B1, HIGH);
    digitalWrite(B1_bar, LOW);
    delayMicroseconds (x);
  }
  Serial.println("StepDown Subroutine Complete");
  MotorsOff1();
}

void StepLeft() {
  MotorsOn2();
  for (int i = 0; i < (steps / 4) ; i++) {
    if (leftTrigger) {
      Serial.println("Left limit switch detects collision");
      break;
    }
    digitalWrite(A2, HIGH);
    digitalWrite(A2_bar, LOW);
    digitalWrite(B2, HIGH);
    digitalWrite(B2_bar, LOW);
    delayMicroseconds (x);

    if (leftTrigger) {
      Serial.println("Left limit switch detects collision");
      break;
    }
    digitalWrite(A2, LOW);
    digitalWrite(A2_bar, HIGH);
    digitalWrite(B2, HIGH);
    digitalWrite(B2_bar, LOW);
    delayMicroseconds (x);

    if (leftTrigger) {
      Serial.println("Left limit switch detects collision");
      break;
    }
    digitalWrite(A2, LOW);
    digitalWrite(A2_bar, HIGH);
    digitalWrite(B2, LOW);
    digitalWrite(B2_bar, HIGH);
    delayMicroseconds (x);

    if (leftTrigger) {
      Serial.println("Left limit switch detects collision");
      break;
    }
    digitalWrite(A2, HIGH);
    digitalWrite(A2_bar, LOW);
    digitalWrite(B2, LOW);
    digitalWrite(B2_bar, HIGH);
    delayMicroseconds (x);
  }
  Serial.println("StepLeft Subroutine Complete");
  MotorsOff2();
}

void StepRight() {
  MotorsOn2();
  for (int i = 0; i < (steps / 4); i++) {
    if (rightTrigger) {
      Serial.println("Right limit switch detects collision");
      break;
    }
    digitalWrite(A2, HIGH);
    digitalWrite(A2_bar, LOW);
    digitalWrite(B2, LOW);
    digitalWrite(B2_bar, HIGH);
    delayMicroseconds (x);

    if (rightTrigger) {
      Serial.println("Right limit switch detects collision");
      break;
    }
    digitalWrite(A2, LOW);
    digitalWrite(A2_bar, HIGH);
    digitalWrite(B2, LOW);
    digitalWrite(B2_bar, HIGH);
    delayMicroseconds (x);

    if (rightTrigger) {
      Serial.println("Right limit switch detects collision");
      break;
    }
    digitalWrite(A2, LOW);
    digitalWrite(A2_bar, HIGH);
    digitalWrite(B2, HIGH);
    digitalWrite(B2_bar, LOW);
    delayMicroseconds (x);

    if (rightTrigger) {
      Serial.println("Right limit switch detects collision");
      break;
    }
    digitalWrite(A2, HIGH);
    digitalWrite(A2_bar, LOW);
    digitalWrite(B2, HIGH);
    digitalWrite(B2_bar, LOW);
    delayMicroseconds (x);
  }
  Serial.println("StepRight Subroutine Complete");
  MotorsOff2();
}

//=================== Arduino Setup ===================//
void setup() {
  // setting our motor pin states as internal pull-up until ready to actuate
  MotorsOff1();
  MotorsOff2();

  // For Serial Monitor
  Serial.begin(9600);
  Serial.println("Enter commands in the form of <stepDirection,steps>");
  Serial.println("valid stepDirections are {up,down,left,right}");
  Serial.println();

  // For limit switch interrupts
  digitalWrite(up,    HIGH);
  digitalWrite(down,  HIGH);
  digitalWrite(left,  HIGH);
  digitalWrite(right, HIGH);

  // These require different syntax for arduino uno rev2 wifi, differ per board. Check:
  // https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
  attachInterrupt(digitalPinToInterrupt(up), limitUp,    CHANGE);
  attachInterrupt(digitalPinToInterrupt(down), limitDown,  CHANGE);
  attachInterrupt(digitalPinToInterrupt(left), limitLeft,  CHANGE);
  attachInterrupt(digitalPinToInterrupt(right), limitRight, CHANGE);

  // Built-in LED to flash when we hit any of our limit switches
  pinMode(LED_BUILTIN, OUTPUT);
}

//============= Main loop ================//
void loop() {
  // Scrape for data from Serial Monitor
  recvWithStartEndMarkers();
  // If new input, begin subroutine
  if (newData == true) {
    strcpy(tempChars, receivedChars);
    // this temporary copy is necessary to protect the original data
    //   because strtok() used in parseData() replaces the commas with \0
    parseData();
    showParsedData();
    newData = false; // ensures we don't have infinite loop

    // Comparing input to determine direction of operation (if any) and instigating operation
    if (strcmp(stepDirection, "up") == 0) {
      StepUp();
    }
    if (strcmp(stepDirection, "down") == 0) {
      StepDown();
    }
    if (strcmp(stepDirection, "left") == 0) {
      StepLeft();
    }
    if (strcmp(stepDirection, "right") == 0) {
      StepRight();
    }
  }
}
