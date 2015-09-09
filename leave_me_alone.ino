/*

Leave-Me-Alone by Dahye Park
http://www.dahyepark.com/

Interactive Bench for Staying Calm and Alone

*/

/*

Sensor used;
Adafruit 9-DOF IMU Breakout - L3GD20H + LSM303
https://www.adafruit.com/products/1714

For libraries required, please refer;
https://learn.adafruit.com/adafruit-9-dof-imu-breakout/software

*/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_9DOF.h>

/* Assign a unique ID to the sensors */
Adafruit_9DOF                dof   = Adafruit_9DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);

/* Update this with the correct SLP for accurate altitude measurements */
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

/**************************************************************************/
/*!
 @brief  Initialises all the sensors used by this example
 */
/**************************************************************************/
void initSensors()
{
  if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
}


int vibe1 = 12;
//int vibe2 = 10;
//int vibe3 = 8;

unsigned long time;
boolean flat = false;

void setup(void)
{
  Serial.begin(115200);

  /* Initialise the sensors */
  initSensors();

  pinMode(vibe1, OUTPUT);
//  pinMode(vibe2, OUTPUT);
//  pinMode(vibe3, OUTPUT);
  
  digitalWrite(vibe1, LOW);
//  digitalWrite(vibe2, LOW);
//  digitalWrite(vibe3, LOW);
  
  time = 0;
}

void loop(void)
{
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_vec_t   orientation;

  /* Calculate pitch and roll from the raw accelerometer data */
  accel.getEvent(&accel_event);
  if (dof.accelGetOrientation(&accel_event, &orientation))
  {
    /* 'orientation' should have valid .roll and .pitch fields */
        Serial.print(F("Pitch: "));
        Serial.print(orientation.pitch);
        Serial.println(F("; "));

    if(orientation.pitch > -20) {
      if(!flat)
      {  
        flat = true;
        time = micros();
      } 
      else {
        if((micros() - time) > 20000000) {
          Serial.println("Time's up!");
          digitalWrite(vibe1, HIGH);
        }
      }
    } 
    else {
      Serial.println(orientation.pitch);
      Serial.println("Good to go.");
      flat = false;
    }    

  }
  delay(100);
  digitalWrite(vibe1, LOW);
}

