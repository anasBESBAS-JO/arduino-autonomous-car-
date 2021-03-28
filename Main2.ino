#include <HCSR04.h>   // used to control HCSR04 Sensors.
#include <IRremote.h> // used to control the IR reciver and remote.
#include <Servo.h>    // used to control the motors.
#include "DHT.h"      // used to control DHT sensor.

// IR recever pin and configuration
const int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;    // store read value from the remote

// LEDs from different checks and signs
byte LEDONE = 7;          // LED for the proximity  function
byte LED    = 9;          // led for blinking function
byte LEDFR  = 19;         // indicate a call for frontRight sensor
byte LEDFL  = 14;         // indicate a call for frontLeft sensor
byte LEDF   = 15;         // indicate a call for front sensor
byte LEDB   = 16;         // indicate a call for Back sensor
byte LEDR   = 17;         // indicate a call for Right sensor
byte LEDL   = 18;         // indicate a call for Left sensor

//Create ESC drive motor 'motor' and steering servo 'steer'
Servo motor;
Servo steer;

int direct     = steer.read(); // read actual servo direction
int speedMicro = motor.read(); // read actual motor speed

//Motor Speed control -> PWM signal
const int speedCenter = 1500;    // new speed center 1500
const int forwardSpeed = 1400;    // Min Speed front new speed min 1400 new
                                  // speed max 1280
const int forwardSpeedMax = 1280;    // Max Speed front
const int reverseSpeed = 1960;    // reverse //1570 1550 //  new one for min :
                                  // 1620 // max new 1960
const int reverseSpeedMin = 1620;    //Min Speed reverse

// tmp sensor
#define DHTPIN 4      //define the digital pin for DHT temprature Sensor
#define DHTTYPE DHT11 // define the type of DHT Sensor
DHT dht(DHTPIN, DHTTYPE); // Creat DHT object to Initialize DHT sensor

//Servo position control values
const int neutral = 1480; // Steering servo positions for straight, left
                          // turns, and right turns
const int steerRight = 1000;
const int steerLeft = 1960;

// Sonars pins & initialisations by creating an object from type HCSR04
HCSR04 fSonic(37, 2); //initialisation class HCSR04 for the front sensor (trig
                      // pin , echo pin)
HCSR04 flSonic(39, 5); //initialisation class HCSR04 for the frontleft
                       // sensor(trig pin , echo pin)
HCSR04 frSonic(38, 8); //initialisation class HCSR04 for the frontright sensor
                       // (trig pin , echo pin)
HCSR04 baSonic(40, 6); //initialisation class HCSR04 for the back sensor(trig
                       // pin , echo pin)

// infarad pins
#define Right  A0 //Pins for sonarR()
#define Left   A1  //Pins for sonarL()
#define InfaradFRight A2 //Pins for sonarFRSide()
#define BackRight A3 //Pins for sonarBR()

//Thresholds
const int frontThreshold = 66;    //Thresholds for the different 'types' of
                                  // sonar sensors front,sides, and Back.In
                                  // centimeters.
const int sideThreshold = 25;
const int backThreshold = 40;
const int frontSideThreshold = 45;

float distance;          // vars for sonar readings.
float distance2;
float distance3;
float distance4;
float distance5;

void setup() {
  Serial.begin(9600);                                  // Start serial
                                                       // communication at 9600
                                                       // Baud
  motor.attach(3);                                     //Attaches motor to pin 3
  Serial.println("MOTOR ATTACHED");
  speedCenterN();                                      // Set motor to neutral
  delay(500);
  steer.attach(12);                                    //Attaches steering servo
                                                       // to pin 12
  Serial.println("Steer ATTACHED");
  delay(500);
  steer.write(neutral);                                // Set steering to
                                                       // straight
  irrecv.enableIRIn();                                 //Enable IR reciver
  irrecv.blink13(true);                                // Blink LED13 every time
                                                       // IR reciver recive a
                                                       // signal
  pinMode(LEDONE, OUTPUT);                            // LEDS initialisation
  pinMode(LED   , OUTPUT);
  pinMode(LEDF  , OUTPUT);
  pinMode(LEDB  , OUTPUT);
  pinMode(LEDFR , OUTPUT);
  pinMode(LEDFL , OUTPUT);
  pinMode(LEDL  , OUTPUT);
  pinMode(LEDR  , OUTPUT);
  
  digitalWrite(LEDONE, LOW);
  blinking();                                         // Blink the top led to
                                                      // indecate car is ready
  turnOffLED();                                       // turn off all leds
  dht.begin();                                        // start dht sensor comunication
}

void loop() { 
  steer.write(neutral);                  // set steer to neutral
  motor.writeMicroseconds(forwardSpeed); // run motor to front direction (start the car)
  distance = sonarFront();              // get front distance 
  Serial.print("Forward Reading: ");
  Serial.println(distance);
  delay(800);                            //  quadruple checks to make
                                         //  sure that an obstacle
                                         //  is present before turning. 
  if (distance < frontThreshold && distance != 0) {
    distance = sonarFront();
    Serial.print("Forward Reading: ");
    Serial.println(distance);
    delay(8);
    if (distance < frontThreshold && distance != 0) {
      distance = sonarFront();
      Serial.print("Forward Reading: ");
      Serial.println(distance);
      delay(8);
      if (distance < frontThreshold && distance != 0) {
        distance = sonarFront();
        Serial.print("Forward Reading: ");
        Serial.println(distance);
        delay(8);
        if (distance > frontThreshold) { // || distance == 0
          motor.writeMicroseconds(forwardSpeed);
          steer.write(neutral);
        } else {          //Here is the passage of the quadruple
                          // check, indicating an obstacle. Below, the car reads
                          // the two sides and decides which one has more room.
          distance3 = sonarR();   
          distance2 = sonarL();
          //checkFront();
          if (distance2 > distance3 && distance2 != 0) {
            leftTurn();
            //   checkFront();
          } else if (distance3 > distance2 && distance3 != 0) {
            rightTurn();
            // checkFront();
          } else if (distance2 < 7) {
            rightTurn();
            // checkFront();
          } else if (distance3 < 7) {
            leftTurn();
            //   checkFront();
          }
          checkFront(); // imp
        }
      } else {
        readSidesAndFront();    //Here, if the multiple checks for a wall in
                                // front fail at any point, the car reads the
                                // rest of the sensors for minor adjustments. It
                                // calls the function mentioned above.
      }
    } else {
      readSidesAndFront();
    }
  } else {
    readSidesAndFront();
  }
}
