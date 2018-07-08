/* Allow communication via serial */
#include <SPI.h>

/* McGriff's Hats */
#define GREEN_HAT_LED   4
#define RED_HAT_LED     A5

/* McGriff's Bark */
#define VOICE_BOX       A2

/* McGriff's Tail */
#include <Servo.h>
#define TAIL            A3
Servo servo;

/* McGriff's Eyes */
#define trigPin         A4
#define echoPin         A1 // Help we need another PIN

/* McGriffs paw */
#define JOYSTICK_Y      A0

/* Default setting for the tail motor */
int servoAngle;   // servo position in degrees
#define startAngle      0
#define endAngle        100

/* McGriff's State */
uint8_t is_wagging = 0;

/**
 * Setup McGriff's body parts.
 */
void setup() {
  // Put on McGriff's hat so he can answer your questions
  setup_hat();

  // Let McGriff bark
  setup_voice();

  // Let McGriff wag his tail
  setup_tail();

  // Setup talking to the computer
  setup_serial();

  // Let McGriff see
  setup_eyes();

  // Setup the seven segment display
  setup_sev_seg();

  // Setup McGriff's paw
  setup_joystick();
}

/**
 * This is the main loop. It continously executes over and over.
 * 
 */
void loop() {
  // Blink the Hat LED's randomly
//  random_blink();
    
  // Leave the answer displayed for 5 seconds
  delay_for(5000);
  delay_in_loop();

  // Reset the hat LEDs
  hat_reset();

  // Delay for 0.5 seconds
  delay_for(500);
  delay_in_loop();
}

/* ---------------------------- McGriff's Hat --------------------------------- */

/**
 * Put on McGriff's hat.
 * This sets up the Red and Green LED's.
 */
void setup_hat(void) {
  // McGriff can't talk right now, help him be able to respond with his LED hat.
  // hints:
  //  - use the functions pinMode and digitalWrite
  //  - the Green Led is on pin GREEN_HAT_LED and Red LED on pin RED_HAT_LED

  /* Students fixed code: */
  pinMode(GREEN_HAT_LED, OUTPUT);
  pinMode(RED_HAT_LED, OUTPUT);
  /* End students code */
}

/**
 * McGriff answers yes to your question.
 */
void hat_correct(void) {
  // Turn the Green LED on
  digitalWrite(GREEN_HAT_LED, HIGH);
}

/**
 * McGriff answers no to your question.
 */
void hat_wrong(void) {
  // Turn the Red LED on
  digitalWrite(RED_HAT_LED, HIGH);
}

/**
 * Turn off both the hats LED's.
 */
void hat_reset(void) {
  // Turn off the Red and Green LEDs
  digitalWrite(RED_HAT_LED, LOW);
  digitalWrite(GREEN_HAT_LED, LOW);
}

/**
 * Oh no! McGriffs hat is blinking randomly.
 */
void random_blink(void) {
  // Choose randomly 0 or 1. 
  // The function used is random(min, max) this produces a random number whole 
  // number between min inclusive and max non-inclusive.
  if (random(0, 2) == 1) {
    hat_correct();
  } else {
    hat_wrong();
  }
}


/* ------------------------- McGriff's Voice Box --------------------------------- */

/**
 * Enable McGriff's voice box.
 */
void setup_voice() {
   pinMode(VOICE_BOX, OUTPUT);
}

/**
 * McGriff Barks at you!
 */
void bark() {
  arf();
  delay(80);
  arf();
  delay(2000);
}

/**
 * McGriff makes a "rufff" Sound.
 */
void arf() {
  playTone(890, 25);          // "a"    (short)
  for(int i = 890; i < 910; i += 2)    // "rrr"  (vary down)
     playTone(i, 5);
     
  playTone(4545, 80);         // intermediate
  playTone(12200, 70);        // "ff"   (shorter, hard to do)
}

/**
 * play tone on a piezo speaker.
 */
void playTone(uint16_t tone1, uint16_t duration) {
  for (long i = 0; i < duration * 1000L; i += tone1 * 2) {
     digitalWrite(VOICE_BOX, HIGH);
     delayMicroseconds(tone1);
     digitalWrite(VOICE_BOX, LOW);
     delayMicroseconds(tone1);
  }     
}


/* ----------------------------- McGriff's Tail --------------------------------- */

/**
 * Allows McGriff's tail to wag.
 */
void setup_tail() {
  // Specify the TAIL pin as a servo motor
  servo.attach(TAIL);
}

/**
 * McGriff wags his tail.
 */
void wag_tail() {
  //if you change the delay value (from example change 50 to 10), the speed of the servo changes
  int increment = random(1, 10) * 5;
  
  for(servoAngle = startAngle; servoAngle < endAngle; servoAngle += increment)  //move the micro servo from 0 degrees to 180 degrees
  {
    // Move the tail to the given angle. 
    servo.write(servoAngle);
    delay(20);               
  }

  for(servoAngle; servoAngle > 0; servoAngle -= increment)  //now move back the micro servo from 0 degrees to 180 degrees
  { 
    // Move the tail to the given angle. 
    servo.write(servoAngle);       
    delay(20);
  }
}

/**
 * Wag McGriff's tail a number of times.
 */
void multiple_tail_wag(int numberOfWags) {
  is_wagging = numberOfWags;
}


/* --------------------------- Communicating with the computer ---------------------------- */

/**
 * Setup the serial communication with the computer
 */
void setup_serial() {
  Serial.begin(9600);
  Serial.println("Uryyb gurer zl ibvpr obk naq ung frrz gb or oebxra. Svk zl ung svefg fb v pna nafjre lbhe dhrfgvbaf.");
}

/**
 * Communicate with McGruff via the serial connection.
 */
void communicate_via_serial() {
  // Read all the messages from serial
  while(Serial.available()) {
    String message = Serial.readString();
    checkMessage(message);
  }
}

/**
 * Jumble the message.
 */
String jumble(String message) {
  char jumbled[message.length()];
  for (int i = 0; i < message.length(); i++) {
    if (message[i] == ' ') {
      jumbled[i] = ' ';
    } else {
      jumbled[i] = random(33,126);
    }
  }
  return jumbled;
}

/* -------------------------- McGriffs eyes ------------------------- */

/**
 * Setup McGriff's eyes to watch for a wave.
 */
void setup_eyes() {
  // trigPin and echoPin are used by the ultrasonic sensor to determine distance
  // to an object.
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

/**
 * McGriff looks for an object in front of his eyes.
 * 
 */
void look_for_object() {
  long distance = get_distance();

  // Check if an object is closer then 20 cm.
  if (distance < 20) {
    Serial.println("Thanks for putting me back together again!");
    bark();
    Serial.println("I’m beginning to feel better.");
    bark();
    Serial.println("I know who the killer is - dfshdafshdjkfasasdfhjdsafklhj Whoops!");
    bark();
    Serial.println("You need my password. Fix my paw and I’ll give it to you.");
    multiple_tail_wag(4);
  }
}

/**
 * Get a distance measurement from the ultrasonic sensor and convert it to cm.
 * 
 */
long get_distance() {
  long duration, distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

  return distance;
}


/* ------------------------- McGriff PAW ---------------------------*/
/**
 * Setup McGriff's Paw.
 * 
 */
void setup_joystick() {
    pinMode(JOYSTICK_Y, INPUT);
}


// Where McGriffs paw currently is
int paw_state = 0;

// The password: Up is 1, down is 2
#define UP                1
#define DOWN              2
#define PASSWORD_LENGTH   4
int password[] = {UP, DOWN, UP, DOWN};
int lastState = 0;

/**
 * Get the location of McGriff's paw and check for the password.
 */
void get_paw_location() {
  int joystick_y = analogRead(JOYSTICK_Y);

  if (password[paw_state] != lastState) {
   /* Can you help decode my password? 
   *  Create if statements to ask for a paw UP if the 
   *  next part of the password is 1 or DOWN for 2.
   */

    /* Student code goes here */
    if (password[paw_state] == UP) {
      Serial.println("Shake my paw UP!");
    } 
    
    else if (password[paw_state] == DOWN) {
      Serial.println("Shake my paw DOWN!");
    }
    /* End students code */

    lastState = password[paw_state];
  }
  
  // Convert the value to either up or down
  int location = joystick_at_location(joystick_y);

  // Check if the password is correct
  if (location == password[paw_state]) {
    bark();
    paw_state += 1;
  } else if (location != 0) {
    paw_state = 0;
  }

  // Check if the password is correct
  if (paw_state >= PASSWORD_LENGTH) {
    bark();
    multiple_tail_wag(4);
    Serial.println("Woof! Woof!");
    bark();
    Serial.println("The killer was Adam Jones.");
    bark();
    Serial.println("Thanks for fixing me and helping me fight crime.");
    bark();
    multiple_tail_wag(10);
    
    paw_state = 0;
  }
}

/**
 * Convert the digital joystick value to either UP or DOWN. 
 * 
 */
int joystick_at_location(int joystick_y) {
  /* Can you help fix McGriff's paw? He's lost feeling and doesn't know
   *  if his paw is up or down.
   *  Hint: use the function Serial.println(joystick_y) to see the current value.
   */

  /* 
    Students change the values here from 10000 to approx 800 and 0 to approx 200
  */
  // Check if McGriff's paw is up
  if (joystick_y > 10000) {
  return UP;
  }

  // Check if McGriff's paw is down
  if (joystick_y < 0) {
  return DOWN;
  }
  
  return 0;
}

/* Use to control the seven segment display */
int pinmap[] = {12, 8, 5, 3, 2, 11, 6};
int select[] = {13, 10, 9, 7};

#define SEGMENT_ON    LOW
#define SEGMENT_OFF   HIGH
#define SEGMENTS      7
#define SELECTORS     4

/* Mappings of the letters on the seven seg */
#define P "1100111"
#define SIX "1011111"
#define EIGHT "1111111"
#define NINE "1111011"
#define ZERO "1111110"
#define NONE "0000000"
#define E "1001111"
#define L "0001110"
#define T "0001111"
#define O "1111110"
#define N "0010101"
#define G "1011110"
#define FIVE "1011011"
#define U "0111110"
#define THREE "1111001"
#define D "0111101"
#define W "0101010"
#define ONE "0110000"
#define TWO "1101101"
#define FOUR "0110011"

/* Seven segment display */
// Oh no! My language cortex has been scrabbled.
// Please help me fix it.

//   A_
// F|G_ |B
// E| _ |C
//    D

// Remember each of these numbers has the format
// "ABCDEFG"
#define R     "1101010"
#define SEVEN "0100011"
#define Y     "1001000"
#define S     "0011100"

/* Students fix these letters to: 
#define Y     "0111011"
#define S     "1011011"
#define R     "0000101"
#define SEVEN "1110000"
End of student work */

// This is used to store the current seven segment values
char multi[SELECTORS][SEGMENTS + 1] = {NONE, NONE, NONE, NONE};

/**
 * Setup the seven segment display.
 * 
 */
void setup_sev_seg() {
  // Setup the seven segment display
  // There are two mistakes in the following for loops! 
  // Can you correct them for me?
  /* Incorrect code 
  for (int i = 0; i < SEGMENTS; i--) {
    pinMode(pinmap[i], OUTPUT);
  }
  */

  /* Corrected code */
   for (int i = 0; i < SEGMENTS; i++) {
    pinMode(pinmap[i], OUTPUT);
  }
  /* End corrected code */

  // Setup the select
  /* Incorrect code
  for (int i = 0; i < SELECTORS; i++) {
    pinMode(select[i], INPUT);
  }*/

  /* Corrected code */
  for (int i = 0; i < SELECTORS; i++) {
    pinMode(select[i], OUTPUT);
  }
  /* End corrected code */
  
  // Calls an initialization function for the screen.
  initScreen();
}







/* ------------- Don't touch below here ----------------- */





/* Use to synchronise McGriff's different body parts */
uint32_t delay_until = 0;
long sees_object = 0;
long feels_object = 0;

/**
 * Delay for the specified about of time. 
 * 
 */
void delay_for(int waitTime) {
  delay_until = max(millis() + waitTime, delay_until);
}

/** 
 *  Busy waiting function that still allows for most of McGriff's body to function.
 *  
 */
void delay_in_loop() {
  while (millis() < delay_until) {
    // Check for a wag
    // McGriff wags his tail
    if (is_wagging > 0) {
      wag_tail();
      is_wagging--;
    }

    // McGriff Looks for objects
    if (millis() > sees_object + 500) {
      look_for_object();
      sees_object = millis();
    }

    // Check his paw
    if (millis() > feels_object + 200) {
      get_paw_location();
      feels_object = millis();
    }
    
    // McGriff listens for a message
    communicate_via_serial();

    // Display on the 7 segment display
    displayMulti();
  }
}

/** 
 * Turn off all segments.
 * 
 */
void initScreen() {
  for (int i = 0; i < SEGMENTS; i++) {
    digitalWrite(pinmap[i], SEGMENT_OFF); 
  }
}

// Store the next segment to light up
int nextSeg = 0;

/**
 * Display the next segment of the seven segment display.
 * 
 */
void displayMulti() {
  display(multi[nextSeg], nextSeg);
  nextSeg += 1;
  nextSeg = nextSeg % 4;
}

/**
 * Display a letter on the next segment of the seven segment display.
 * 
 */
void display(const char segs[], int pos) {
  int num = 0;

  int error[] = {1, 0, 0, 1, 1, 1, 1};

  selectorsLow();
  segmentsHigh();
  digitalWrite(select[pos], HIGH);

  for (int i = 0; i < SEGMENTS; i++) {
    if (i >= strlen(segs) || (segs[i] != '0' && segs[i] != '1')) {
      num = error[i];
    } else {
      num = segs[i] - '0';
    }

    digitalWrite(pinmap[i], !num);
    
  }
}

/**
 * Set all select pins to low.
 * 
 */
void selectorsLow() {
  for (int i = 0; i < SELECTORS; i++) {
    digitalWrite(select[i], LOW);
  }
}

/**
 * Set all select pins to high.
 * 
 */
void segmentsHigh() {
  for (int i = 0; i < SEGMENTS; i++) {
    digitalWrite(pinmap[i], SEGMENT_OFF);
  }
}

/**
 * Check if a message is correct.
 */
void checkMessage(String message) {
  if (message.equals("What time was the murder?")) {
    char answer[][SEGMENTS + 1] = {NONE, SEVEN, THREE, ZERO};
    memcpy(multi, answer, SELECTORS * (SEGMENTS + 1));
  }

  else if (message.equals("What colour were their clothes?")) {
    char answer[][SEGMENTS + 1] = {NONE, R, E, D};
    memcpy(multi, answer, SELECTORS * (SEGMENTS + 1));

  }

  else if (message.equals("Were they wearing a hat?")) {
    char answer[][SEGMENTS + 1] = {NONE, Y, E, S};
    memcpy(multi, answer, SELECTORS * (SEGMENTS + 1));
  }

  else if (message.equals("Are you feeling OK?")) {
    hat_wrong();
  }

  else if (message.equals("Do you know the murderer?")) {
    hat_correct();
  } 
  
  else {
    Serial.println(jumble(message));
  }

  bark();
}

