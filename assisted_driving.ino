/* 
* Eckart Schneider
* 03/08/22
*/

#include <IRremote.hpp>
#include <Servo.h>

#define RECV_PIN 12

//Remote buttons
#define UP 16736925
#define DOWN 16754775
#define RIGHT 16761405
#define LEFT 16720605
#define STOP 16712445

#define HOLD 4294967295

#define ONE 16738455
#define TWO 16750695
#define THREE 16756815
#define FOUR 16724175
#define FIVE 16718055
#define SIX 16743045
#define SEVEN 16716015
#define EIGHT 16726215
#define NINE 16734885
#define TEN 16730805

#define ANTI_CRASH 16728765
#define ANTI_LINE 16732845

Servo myservo;      // create servo object to control servo

//Pins
int Echo = A4;
int Trig = A5;

#define ENB 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define ENA 6
int carSpeed = 200, lastNum = 10;
int rightDistance = 0, leftDistance = 0, middleDistance = 0;

#define LT_R !digitalRead(10)
#define LT_M !digitalRead(4)
#define LT_L !digitalRead(2)

IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long val;

boolean close = false, cross = false, line = true, wall = true;

void forward(){
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Forward");
}

void back() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Back");
}

void left() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Left");
}

void right() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Right");
}

void stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.println("Stop!");
}

int getDistance() {
    digitalWrite(Trig, LOW);
    delayMicroseconds(2);
    digitalWrite(Trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trig, LOW);
    return (int)pulseIn(Echo, HIGH) / 58;
}

void setup() {
  myservo.attach(3);  // attach servo on pin 3 to servo object
  Serial.begin(9600);
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  stop();
  myservo.write(90);  //setservo position according to scaled value
  irrecv.enableIRIn();
}

void loop() {
  carSpeed = map(lastNum, 1, 10, 100, 255); // set car speed to pressed button
    middleDistance = getDistance();

    //When close to the wall stop
    if(!close && wall && middleDistance <= 20) {
      stop();
      close = true;
    }
    //If current close, but now far away, now no longer close
    else if (close && middleDistance > 20 || !wall) {
      close = false;
    }
    
    // If any sensors detect a line, stop
    if(line && !cross && (LT_L || LT_M || LT_R)) {
      stop();
      cross = true;
    }
    //If no longer on a line, continue
    else if (cross && !(LT_L || LT_M || LT_R) || !line) {
      cross = false;
    }

  //Get input from IR remot
  if(irrecv.decode(&results)) {
    val = results.value;
    irrecv.resume();
    
    switch(val) {
      case UP: 
        Serial.println("UP");
        forward(); 
        break;
      case DOWN:
        Serial.println("DOWN");
        back();
        if(close || cross) {
          delay(300);
        }
         
        break;
      case LEFT: 
        Serial.println("LEFT");
        left(); 
        break;
      case RIGHT:
        Serial.println("RIGHT");
        right(); 
        break;
      case ANTI_CRASH:
        Serial.println("ANTI_CRASH");
        wall = !wall;
        break;
      case ANTI_LINE:
        Serial.println("ANTI_LINE");
        line = !line;
        break;
      case STOP:
        stop();
        break;
      case ONE:
        lastNum = 1;
        break;
      case TWO:
        lastNum = 2;
        break;
      case THREE:
        lastNum = 3;
        break;
      case FOUR:
        lastNum = 4;
        break;
      case FIVE:
        lastNum = 5;
        break;
      case SIX:
        lastNum = 6;
        break;
      case SEVEN:
        lastNum = 7;
        break;
      case EIGHT:
        lastNum = 8;
        break;
      case NINE:
        lastNum = 9;
        break;
      case TEN:
        lastNum = 10;
        break;
      default:
        Serial.println(val);
        break;
    }
  }
}
