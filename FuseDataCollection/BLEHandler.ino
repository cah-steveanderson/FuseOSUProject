#define ADAFRUITBLE_REQ 8
#define ADAFRUITBLE_RDY 2
#define ADAFRUITBLE_RST 6
#define ALARM 8
#define GET_HR 9
#define SET_TIME 10
#define SET_DATE 11
#define SET_DATETIME 12

Adafruit_BLE_UART uart = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);
aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED;
bool alarmToggled = false;

void aciCallback(aci_evt_opcode_t event)
{
  laststatus = event;
  switch(event)
  {
    case ACI_EVT_DEVICE_STARTED:
      Serial.println(F("Advertising started"));
      break;
    case ACI_EVT_CONNECTED:
      Serial.println(F("Connected!"));
      break;
    case ACI_EVT_DISCONNECTED:
      Serial.println(F("Disconnected or advertising timed out"));
      break;
    default:
      break;
  }
}

void rxCallback(uint8_t *buffer, uint8_t len)
{
  if(len == 1)
  {
    switch(buffer[0] - '0')
    {
      case ALARM:
          
        if (alarmToggled)
        {
          digitalWrite(BUZZER_PIN, LOW);
        }
        else
        {
          Serial.println(F("Buzzing!!"));
          digitalWrite(BUZZER_PIN, HIGH);
          processBuzzer();
        }
        alarmToggled = !alarmToggled;
        break;
      case GET_HR:
        getHR.shouldGetHR = true;
        getHR.remainingSamples = 20;
        break;
      default:
        break;
    }
  }
  if(len >= 2)
  {
    //Takes in a 2 digit integer command
    switch((buffer[0] - '0') * 10 + (buffer[1] - '0'))
    {
      case SET_TIME:
        if(len == 8)
        {
          int h = (buffer[2] - '0') * 10 + (buffer[3] - '0');
          int m = (buffer[4] - '0') * 10 + (buffer[5] - '0');
          int s = (buffer[6] - '0') * 10 + (buffer[7] - '0');
          setClockTime(h,m,s);
        }
        break;
      case SET_DATE:
        if(len == 10)
        {
          int d = (buffer[2] - '0') * 10 + (buffer[3] - '0');
          int m = (buffer[4] - '0') * 10 + (buffer[5] - '0');
          int y = (buffer[6] - '0') * 1000 + (buffer[7] - '0') * 100 + (buffer[8] - '0') * 10 + (buffer[9] - '0');
          setClockDate(d,m,y);
        }
        break;
      case SET_DATETIME:
        if(len == 16)
        {
          int d = (buffer[2] - '0') * 10 + (buffer[3] - '0');
          int m = (buffer[4] - '0') * 10 + (buffer[5] - '0');
          int y = (buffer[6] - '0') * 1000 + (buffer[7] - '0') * 100 + (buffer[8] - '0') * 10 + (buffer[9] - '0');
          setClockDate(d,m,y);
          int h = (buffer[10] - '0') * 10 + (buffer[11] - '0');
          int mn = (buffer[12] - '0') * 10 + (buffer[13] - '0');
          int s = (buffer[14] - '0') * 10 + (buffer[15] - '0');
          setClockTime(h,mn,s);
        }
        break;
      default: 
        break;
    }
  }
}

void setupBLE()
{ 
  //Serial.begin(9600);
  Serial.println(F("BLE Initialized"));

  uart.setRXcallback(rxCallback);
  // uart.setDeviceName("NEWNAME"); /* 7 characters max! */
  uart.begin();
}

void processBLE() 
{
  uart.pollACI();
  while(messageQueue.count() > 0 && uart.getState() == ACI_EVT_CONNECTED)
  {
    byte* ptr = messageQueue.pop();
    String message = ByteArrayToString(ptr);
    free(ptr);
    Serial.println(message);
    if (message.length() < 32)
    {
      //uint8_t buffer[32];
      //message.getBytes(buffer, message.length() + 1);
      Serial.print(F("About to write to BLE: "));
      //Serial.println(String((char *)buffer));
      Serial.println(message);
      //uart.write(buffer, 32);
      //uart.print(String((char*)buffer));
      uart.print(message);
    }
  }
}

