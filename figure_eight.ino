//www.elegoo.com

#include <Servo.h>  //servo library
Servo myservo;      // create servo object to control servo

int Echo = A4;
int Trig = A5;

#define LT_R !digitalRead(10)
#define LT_M !digitalRead(4)
#define LT_L !digitalRead(2)

#define ENB 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define ENA 6
#define carSpeed 80
#define timeNeeded 6

int idealDistance = 40;
int distance = 0, difference = 0;

void forward() {
  //analogWrite(ENA, carSpeed);
  //analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Forward");
}

void back() {
  //analogWrite(ENA, carSpeed);
  //analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Back");
}

void left() {
  //analogWrite(ENA, carSpeed);
  //analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Left");
}

void right() {
  //analogWrite(ENA, carSpeed);
  //analogWrite(ENB, carSpeed);
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

//Ultrasonic distance measurement Sub function
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
  forward();
  myservo.write(180);  //setservo position according to scaled value
}

void loop() {
  distance = getDistance();
  //If no object detected
  if (distance > 70 && distance < 85) {
    //If crossed the line then turn right otherwise left
    if(!passed) {
      left();
    analogWrite(ENA, difference);
    analogWrite(ENB, difference);
    } else {
      right();
    analogWrite(ENA, difference);
    analogWrite(ENB, difference);
    }
    
  }
  //Object detected
  else {
    difference = distance - idealDistance;
    forward();
    
    //If crossed the line then turn right otherwise left
    if(!passed) {
      analogWrite(ENA, carSpeed + difference);
      analogWrite(ENB, carSpeed - difference);
    } else {
      analogWrite(ENA, carSpeed - difference);
      analogWrite(ENB, carSpeed + difference);
    }
  
  }
  
  //If all three sensors are triggered, that means line has been crossed
  if (LT_M && LT_R && LT_L) {
    stop();
    delay(3000);
    // Switch servo to the right
    myservo.write(0);
    passed = true;

  }

}
