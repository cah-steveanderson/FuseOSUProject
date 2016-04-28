int count = 1;

void saveMessage(String message)
{
  if(messageQueue.count() > MAX_QUEUE_SIZE - 1)
  {
    while(messageQueue.count() > 0)
    {
      byte* ptr = messageQueue.pop();
      free(ptr);
    }
  }
  
  message = getCurrentDateTime() + message;
  
  if(encryptionEnabled)
  {
    byte* encryptedMessageBuffer = (byte*)malloc(MESSAGE_LENGTH * 2 *sizeof(byte)); 
    secureMessage(message, encryptedMessageBuffer);
    tackOnIV(encryptedMessageBuffer);
    String securedMessage = ByteArrayToString(encryptedMessageBuffer, MESSAGE_LENGTH*2);
    Serial.println(securedMessage);
    messageQueue.push((encryptedMessageBuffer));
  }
  else
  {
    byte* unsecuredMessageBuffer = (byte*)malloc(MESSAGE_LENGTH * 2 *sizeof(byte));
    padBytes(message, unsecuredMessageBuffer, MESSAGE_LENGTH);
    tackOnIV(unsecuredMessageBuffer);
    Serial.println(message);
    messageQueue.push(unsecuredMessageBuffer);
  }
  
  Serial.println(String(messageQueue.count()) + "=" + message);
}

void saveMessage(int messageType, int messageData)
{
  byte messageBuffer[16];
  fillMessageDateTime(messageBuffer, getCurrentDateTime());
  messageBuffer[4] = messageType;
  messageBuffer[5] = messageData;
  for(int i = 6; i < 16; i++)
  {
    messageBuffer[i] = 0;
  }
}

byte* fillMessageDateTime(byte message[], long t)
{
  message[0] = (int)((t >> 24) & 0xFF) ;
  message[1] = (int)((t >> 16) & 0xFF) ;
  message[2] = (int)((t >> 8) & 0XFF);
  message[3] = (int)((t & 0XFF));
  return message;
}

String ByteArrayToString(byte byteArray[], int len)
{
  String s = "";
  for(int i = 0; i < len; i++)
  {
    s += (char)byteArray[i];
  }
  return s;
}







