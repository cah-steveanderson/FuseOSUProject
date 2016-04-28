#include <AES.h>

AES aes ;

byte key[] = 
{
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

byte my_iv[] = 
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
};

int bits = 128;
int blocks = 1;

void setUpEncryption()
{
  randomSeed(analogRead(0));
}

void setRandomIV()
{
  for(int i = 0; i <16; i++)
  {
    my_iv[i] = getRandomByte();
  }
}

byte getRandomByte()
{
  return random(0,255);
}

//For now, max message length is 32
void secureMessage(String message, byte encryptedBytesBuffer[])
{
  setRandomIV();
  encryptMessage(message, encryptedBytesBuffer);
}

void encryptMessage(String message, byte encryptedBytesBuffer[])
{
  byte bytes[message.length() + 1];
  getByteArray(message, bytes);
  byte paddedBytes[MESSAGE_LENGTH];
  padBytes(bytes, paddedBytes, message.length() + 1, MESSAGE_LENGTH);
  encryptBlocks(paddedBytes, encryptedBytesBuffer, blocks);
}

void encryptBlocks(byte unencryptedBytes[], byte encryptedBytes[], int numBlocks)
{
  byte iv [N_BLOCK];
  byte succ = aes.set_key (key, bits) ;
  //Encrypt
  for (byte i = 0 ; i < 16 ; i++)
      iv[i] = my_iv[i] ;
  succ = aes.cbc_encrypt (unencryptedBytes, encryptedBytes, numBlocks, iv) ;
}

void decryptBlocks(byte unencryptedBytes[], byte encryptedBytes[], int numBlocks)
{
  byte iv [N_BLOCK];
  byte succ = aes.set_key (key, bits) ;

  //Decrypt
  for (byte i = 0 ; i < 16 ; i++)
      iv[i] = my_iv[i] ;
   succ = aes.cbc_decrypt (encryptedBytes, unencryptedBytes, numBlocks, iv) ;
}

void getByteArray(String message, byte bytes[])
{
  message.getBytes(bytes, message.length() + 1);
}

void padBytes(byte bytes[], byte paddedBytes[], int byteArrLen, int desiredLength)
{
  for(int i = 0; i < byteArrLen; i++)
  {
    paddedBytes[i] = bytes[i];
  }
  for(int i = byteArrLen; i < desiredLength; i++)
  {
    paddedBytes[i] = '\0';
  }
}


