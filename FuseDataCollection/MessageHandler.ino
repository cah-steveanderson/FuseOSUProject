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
    byte encryptedMessageBuffer[32]; 
    secureMessage(message, encryptedMessageBuffer);
    String securedMessage = ByteArrayToString(encryptedMessageBuffer);
    Serial.println(securedMessage);
    messageQueue.push((encryptedMessageBuffer));
  }
  else
  {
    byte* unsecuredMessageBuffer = (byte*)malloc(32*sizeof(byte));
    byte* unsecuredPaddedMessageBuffer = (byte*)malloc(32*sizeof(byte));
    getByteArray(message, unsecuredMessageBuffer);
    padBytes(unsecuredMessageBuffer, unsecuredPaddedMessageBuffer, message.length() + 1, 32);
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

String ByteArrayToString(byte byteArray[])
{
  //This is potentially dangerous if byte array does not end in null terminating character
  return String((char*)byteArray);
}







