

void setUpRTC()
{
    setUpDS3231();
}

void setUpDS3231()
{
  rtc.begin();
  setClockDate(1, 1, 2016);
  setClockTime(12, 0, 0);   
}

void setClockTime(int h, int m, int s)
{
 rtc.setTime(h, m, s);  
}

void setClockDate(int d, int m, int y)
{
  rtc.setDate(d, m, y);
}

long getCurrentDateTime() 
{
  return rtc.getUnixTime(rtc.getTime());
} 

