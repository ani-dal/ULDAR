/*
System to measure distance in varying temperature conditions
*/


#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define ECHO 9  //echo pin
#define TRIG 10 // trigger pin

LiquidCrystal_I2C lcd(32, 16, 2);
Servo AZMTH,ELV;

/*
Velocity of Sound is directly proportional to the temperature
ref: https://www.engineeringtoolbox.com/air-speed-sound-d_603.html
speed of sound (v) = 20.05(273.16 + temperature)^1/2
*/
signed temp; // temperature range is from -40C to 125C
int TMP36_val; //the value of TMP36 ranges from 20 to 358 
double vSnd; //velocity of sound

int potVal;

int azmth = 0; // azimuth angle
int elv = 0; // elevation angle

int distance;
long duration;

bool sweepDir;

void setup()
{
 Serial.begin(9600);
  
 pinMode(A0,INPUT);
 pinMode(A1,INPUT);
  
 AZMTH.attach(3);
 ELV.attach(5);
  
 pinMode(TRIG, OUTPUT);
 pinMode(ECHO, INPUT);
  
 lcd.init();  
 lcd.backlight();
}

void loop()
{
  
 if (azmth == 0) sweepDir = true;
 else if (azmth == 180) sweepDir = false;
 lcd.clear();
 lcd.setCursor(0,0);
 
 digitalWrite(TRIG, LOW);
 delayMicroseconds(2);
 digitalWrite(TRIG, HIGH);
 delayMicroseconds(10);
 digitalWrite(TRIG, LOW);
 duration = pulseIn(ECHO, HIGH); 
 
 TMP36_val = analogRead(A0);
 temp  =   map(((TMP36_val - 20) * 3.04), 0, 1023, -40, 125);
 lcd.print("T=");
 lcd.print(temp);
 lcd.print("C ");
 
 vSnd = (pow(temp + 273.16,0.5)*20.05);
 
 //display the distance
 lcd.setCursor(7,0);
 lcd.print("s=");
 distance = (((vSnd/10000) * duration))/2;
 lcd.print(distance);
 lcd.print("cm");
 //Seting the elevation of the UL-DAR
 potVal = analogRead(A1);
 elv = map(potVal,0,1023,0,90);
 ELV.write(elv);
  
 //Sweep azimuth 10 deg of incrment
 AZMTH.write(azmth);
 azmth = azmth + ((sweepDir)?10:-10);
 lcd.setCursor(0,1);
 lcd.print("E=");
 lcd.print(elv);
 lcd.print(" A=");
 lcd.print(azmth);
 delay(500);
}