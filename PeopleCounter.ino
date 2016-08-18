#define trigPin1 11 // send ultrasonic from sensor 1
#define echoPin1 10 // receive echo on sensor 1
#define trigPin2 9 // send ultrasonic from sensor 2
#define echoPin2 8 // receive echo on sensor 2
#define servo 4   //servo
#define Buzzer 3 // speaker
#define TEDtest1 7 // led testing
#define TEDtest2 6 // led testing
#define button 2 // button for starting song

#include <Servo.h>
Servo servoMain;
int buzzFreq;
float duration1, duration2, distance1, distance2;
int set1 = 1, set2 = 1, counter = 0, ArrayI, pos = 60;
double timer1, timer2;
int notecount[400];

void setup() {
  Serial.begin (9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(TEDtest1, OUTPUT);
  pinMode(TEDtest2, OUTPUT);
  pinMode(button, INPUT);
  pinMode(servo, OUTPUT);
  pinMode(button, INPUT);
  servoMain.attach(servo); //servo pin4
  digitalWrite(button, LOW);
  digitalWrite(TEDtest1, LOW);
  digitalWrite(TEDtest2, LOW);
  servoMain.write(pos);
}

void loop() {
  if (set1 == 2) timer1 += 50;
  if (set2 == 2) timer2 += 50;

  // sensor 1
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  duration1 = pulseIn(echoPin1, HIGH, 100000);
  if (set1 == 2) timer1 += (duration1 + 12) / 1000;
  if (set2 == 2) timer2 += (duration1 + 12) / 1000;
  distance1 = (duration1 / 2 ) / 29.1;
  if (distance1 >= 50.0 && distance2 >= 50 && set1 == 0) set1 = 1; //privent double counts
  if (distance1 < 50.0 && set1 == 1 && set2 != 2) {     // when condition meets, start timer1
    set1 = 2;
    digitalWrite(TEDtest1, HIGH);
  }
  // someone exited the room, update variables, update output, reset
  if (distance1 < 50.0 && set2 == 2 && set1 == 1) {
    counter--;
    pos -= 3;
    servoMain.write(pos);
    buzzFreq = TimetoFreq(timer2);
    tone(Buzzer, buzzFreq, 300);
    notecount[ArrayI] = buzzFreq;
    ArrayI++;
    digitalWrite(TEDtest2, LOW);
    // print to console for test
    Serial.println("timer2: ");
    Serial.println(timer2);
    Serial.println("counter: ");
    Serial.println(counter);
    Serial.println(buzzFreq);
    // reset variables
    timer2 = 0;
    set1 = 0;
    set2 = 0;
    delay(500);
  }
  //sensor 2
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  duration2 = pulseIn(echoPin2, HIGH, 100000);
  if (set1 == 2) timer1 += (duration2 + 12) / 1000;
  if (set2 == 2) timer2 += (duration2 + 12) / 1000;
  distance2 = (duration2 / 2) / 29.1;
  if (distance2 >= 50.0 && distance1 >= 50 && set2 == 0) set2 = 1;
  if (distance2 < 50.0 && set2 == 1 && set1 != 2) {      // when condition meets, start timer2
    set2 = 2;
    digitalWrite(TEDtest2, HIGH);
  }

  // someone entered the room, update variables, update output, reset
  if (distance2 < 50.0 && set1 == 2 && set2 == 1) {
    counter++;
    pos += 3;
    servoMain.write(pos);
    buzzFreq = TimetoFreq(timer1);
    tone(Buzzer, buzzFreq, 300);
    notecount[ArrayI] = buzzFreq;
    ArrayI++;
    digitalWrite(TEDtest1, LOW);
    // print to console for test
    Serial.println("timer1: ");
    Serial.println(timer1);
    Serial.println("counter: ");
    Serial.println(counter);
    Serial.println(buzzFreq);
    // reset variables
    timer1 = 0;
    set1 = 0;
    set2 = 0;
    delay(500);
  }
  // print on monitor for testing
  Serial.print("ultra1: ");
  Serial.print(distance1);
  Serial.println(" cm");
  Serial.print("ultra2: ");
  Serial.print(distance2);
  Serial.println(" cm");
  Serial.println(set1);
  Serial.println(set2);
  delay(50);
  //play the music
  if (digitalRead(button) == HIGH || counter > 30 || counter < -10) {
    for (int i = 0; i <= ArrayI; i++) {
      tone(Buzzer, notecount[i], 12000 / sqrt(notecount[i]) );
    }
    pos = 60;
    servoMain.write(pos);
    counter = 0;
    ArrayI = 0;
    digitalWrite(button, LOW);
    tone(Buzzer, 100, 10);
  }
}

