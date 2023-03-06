#include <analogWrite.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define ENA   33
#define IN1   25
#define IN2   26
#define IN3   27
#define IN4   14
#define ENB   12

BluetoothSerial SerialBT;

String bluetoothRead;
unsigned short x, y, area;
unsigned short strLength;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Object Follower");

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void loop() {
  register byte ndx = 0;
  char commandbuffer[50];

  if (SerialBT.available() > 0) {
    delay(10);
    while (SerialBT.available() && ndx < 50) {
      commandbuffer[ndx++] = SerialBT.read();
    }

    commandbuffer[ndx] = '\0';
    bluetoothRead = (char*)commandbuffer;
    strLength = bluetoothRead.length();
    Serial.println(bluetoothRead);
    Serial.println(strLength);

    if (bluetoothRead.substring(0, 1).equals("X")) {
      uint8_t pos, i = 1;

      while (bluetoothRead.substring(i, i+1) != ("Y")) {
        i++;
      }

      x = bluetoothRead.substring(1, i).toInt();
      pos = i+1;
      while (bluetoothRead.substring(i, i+1) != ("A")) {
        i++;
      }

      y = bluetoothRead.substring(pos, i).toInt();
      area = bluetoothRead.substring(i+1, strLength).toInt();
      Serial.println(x);
      Serial.println(y);
      Serial.println(area);

      if (x > 800) {
        //Serial.println("right");
        Right();
      }

      if (x < 400 ) {
        //Serial.println("left");
        Left();
      }

      if (x <= 800 && x >= 400) {
        if (area < 50) {
          //Serial.println("forward");
          Forward();
        }
        if (area >= 50 && area <= 130) {
          //Serial.println("stop");
          Stop();
        }
        if (area > 130) {
          //Serial.println("back");
          Back();
        }
      }

      if (x > 1200 || x < 20 || y > 700 || y < 20) {
        Stop();
      }
    }
  }
  Serial.flush();
}

void Left() {
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
  /*digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);*/
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void Right() {
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  /*digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);*/
}

void Forward() {
  analogWrite(ENA, 170);
  analogWrite(ENB, 170);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void Back() {
  analogWrite(ENA, 170);
  analogWrite(ENB, 170);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void Stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW); 
}
