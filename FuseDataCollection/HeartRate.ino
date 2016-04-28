#define DEFAULT_SAMPLES 10
#define DEFAULT_SLEEP_SAMPLES 20

//PulseSensor Vars
int pulsePin = 0;                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin = 13;                // pin to blink led at each beat
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin
// these variables are volatile because they are used during the interrupt service routine!
volatile int BPM;                   // used to hold the pulse rate
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, the Inter-Beat Interval
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.
//Allows the device to sleep for 20 samples and then give 10 consecutive samples
int samples = DEFAULT_SAMPLES;
int sleepSamples = DEFAULT_SLEEP_SAMPLES;




void setUpHeartRateSensor() {
  interruptSetup();
}

//TODO fix this logic
void processHeartRateSensor() {
  if(QS == true)
  {
    if (getHR.shouldGetHR && getHR.remainingSamples > 0)
    {
      //saveMessage(getCurrentDateTime() + "Heart Rate: " + BPM);
      saveMessage("15" + String(BPM));
      QS = false; 
      if (getHR.remainingSamples <= 1)
      {
        getHR.shouldGetHR = false;
        sleepSamples = DEFAULT_SLEEP_SAMPLES;
        samples = DEFAULT_SAMPLES;
      }
      getHR.remainingSamples--;
    }
    else if (sleepSamples <= 0){
      //saveMessage(getCurrentDateTime() + "Heart Rate: " + BPM);
      BPM = (BPM > 999) ? 999 : BPM;
      String bpmStr = (BPM < 100) ? ((BPM < 10) ? "00" + String(BPM) : "0" + String(BPM)) : String(BPM);
      saveMessage("15" + bpmStr);
      QS = false; 
      if(samples <= 1)
        sleepSamples = DEFAULT_SLEEP_SAMPLES;
      samples--;
    }
    else
    {
      if(sleepSamples > 0)
        sleepSamples--;
      samples = DEFAULT_SAMPLES;
    }
  }
}


