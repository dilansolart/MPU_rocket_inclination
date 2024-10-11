// CONEXIONES //
//GND - GND
//VCC - VCC
//SDA - Pin A4
//SCL - Pin A5
//Servo - Pin 2

#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

#include "Servo.h"

// Create a new servo object:
Servo myservo;
// Define the servo pin:
#define servoPin 2

const int mpuAddress = 0x68;  // Puede ser 0x68 o 0x69
MPU6050 mpu(mpuAddress);

int ax, ay, az;
int gx, gy, gz;

long tiempo_prev;
float dt;
float ang_x, ang_y;
float ang_x_prev, ang_y_prev;

void updateFiltered()
{
  dt = (millis() - tiempo_prev) / 1000.0;
  tiempo_prev = millis();

  //Calcular los ángulos con acelerometro
  float accel_ang_x = atan(ay / sqrt(pow(ax, 2) + pow(az, 2)))*(180.0 / 3.14);
  float accel_ang_y = atan(-ax / sqrt(pow(ay, 2) + pow(az, 2)))*(180.0 / 3.14);

  //Calcular angulo de rotación con giroscopio y filtro complementario
  ang_x = 0.98*(ang_x_prev + (gx / 131)*dt) + 0.02*accel_ang_x;
  ang_y = 0.98*(ang_y_prev + (gy / 131)*dt) + 0.02*accel_ang_y;

  ang_x_prev = ang_x;
  ang_y_prev = ang_y;
}

void setup()
{
  Serial.begin(9600);
  //Wire.begin();
  mpu.initialize();

  //Servo
  myservo.attach(servoPin);
  myservo.write(30);
}

void loop() 
{
  // Leer las aceleraciones y velocidades angulares
  mpu.getAcceleration(&ax, &ay, &az);
  mpu.getRotation(&gx, &gy, &gz);

  updateFiltered();

  Serial.print(F("Rotacion en X:  "));
  Serial.print(ang_x);
  if (ang_x >= 35 || ang_x <= -35  ){
    //Servo Disparar servo
    myservo.write(160);
  }
  Serial.print(F("\t Rotacion en Y: "));
  Serial.println(ang_y);
  if (ang_y >= 35 || ang_y <= -35  ){
    //Servo Disparar servo
    myservo.write(160);
  }

  delay(10);
}
