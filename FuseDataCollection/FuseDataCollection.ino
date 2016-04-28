#include <Wire.h>
#include <SPI.h>
#include <QueueList.h>
#include <DS3231.h>
#include <Adafruit_BLE_UART.h>

DS3231  rtc(SDA, SCL);
  
QueueList <byte*> messageQueue;
#define BUTTON_PIN 7
#define BUZZER_PIN 5
#define MAX_QUEUE_SIZE 50
#define MESSAGE_LENGTH 16
bool encryptionEnabled = false;
struct GetHR
{
  bool shouldGetHR;
  int remainingSamples;
};
GetHR getHR;

void setup(void)
{
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  messageQueue.setPrinter(Serial);
  setUpEncryption();
  setUpRTC();
  setUpAccelerometer();
  setUpHeartRateSensor();
  setUpBluetooth();
  setUpECG();
}

void loop(void)
{
  processAccelerometer();
  processHeartRateSensor();
  processBluetooth();
  processAlert();
}




