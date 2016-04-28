#include <ADXL345.h>

ADXL345 accelerometer;
long DoubleTapTime;
long InactivityTime;
long ActivityTime;
long SingleTapTime;
long FreeFallTime;
long DTTime;
enum AccelerometerAction {DoubleTap, SingleTap, Inactivity, Activity, FreeFall};
AccelerometerAction lastAction;

void setUpAccelerometer() {
  saveMessage("50"); // Initialize ADXL345

  lastAction = Inactivity;
  
  while (!accelerometer.begin(false))
  {
    saveMessage("99"); //Could not find a valid ADXL345 sensor, check wiring!
    delay(500);
  }

  setUpFreeFallConfiguration(0.35, 0.05);
  setUpActivityDetectionConfiguration();
  setUpTapDetectionConfiguration();
  accelerometer.useInterrupt(ADXL345_INT1);
}

void validateAccelerometerSetup() {
  if (!accelerometer.begin(false))
  {
    saveMessage("99"); //Could not find a valid ADXL345 sensor, check wiring!
    delay(20);
  }
}

void setUpFreeFallConfiguration(float freeFallAccelThresh, float freeFallDurationThresh) {
  // Values for Free Fall detection
  accelerometer.setFreeFallThreshold(freeFallAccelThresh); // Recommended 0.3 -0.6 g
  accelerometer.setFreeFallDuration(freeFallDurationThresh);   // Recommended 0.1 s
}

void setUpActivityDetectionConfiguration() {
  // Values for Activity and Inactivity detection
  accelerometer.setActivityThreshold(1.2);    // Recommended 1.2 g
  accelerometer.setInactivityThreshold(1.2);  // Recommended 1.2 g
  accelerometer.setTimeInactivity(5);         // Recommended 5 s

  // Set activity detection only on X,Y,Z-Axis
  accelerometer.setActivityXYZ(1);         // Check activity on X,Y,Z-Axis
  // or
  // accelerometer.setActivityX(1);        // Check activity on X_Axis
  // accelerometer.setActivityY(1);        // Check activity on Y-Axis
  // accelerometer.setActivityZ(1);        // Check activity on Z-Axis

  // Set inactivity detection only on X,Y,Z-Axis
  accelerometer.setInactivityXYZ(1);       // Check inactivity on X,Y,Z-Axis
  // or
  // accelerometer.setInactivityX(1);      // Check inactivity on X_Axis
  // accelerometer.setInactivityY(1);      // Check inactivity on Y-Axis
  // accelerometer.setInactivityZ(1);      // Check inactivity on Z-Axis 
}

void setUpTapDetectionConfiguration() {
  // Set tap detection on Z-Axis
  //accelerometer.setTapDetectionX(0);       // Don't check tap on X-Axis
  //accelerometer.setTapDetectionY(0);       // Don't check tap on Y-Axis
  //accelerometer.setTapDetectionZ(1);       // Check tap on Z-Axis
  // or
   accelerometer.setTapDetectionXYZ(1);  // Check tap on X,Y,Z-Axis

  accelerometer.setTapThreshold(2.5);      // Recommended 2.5 g
  accelerometer.setTapDuration(0.02);      // Recommended 0.02 s
  accelerometer.setDoubleTapLatency(0.10); // Recommended 0.10 s
  accelerometer.setDoubleTapWindow(0.30);  // Recommended 0.30 s
}

void processAccelerometer() {
  long time = micros();

  // Read values for activities
  delay(50);

  Vector norm = accelerometer.readNormalize();

  // Read activities
  Activites activ = accelerometer.readActivites();
  
  if (activ.isFreeFall)
  {
    processFreeFallAction();
  }

  if (activ.isDoubleTap)
  {
    processDoubleTapAction();
  } 
  else if (activ.isTap)
  {
    processSingleTapAction();
  }

  if (activ.isInactivity)
  {
    processInactivityAction();
  }

  if (activ.isActivity)
  {
    processActivityAction();
  }
}

void processFreeFallAction() {
  FreeFallTime = micros();
  saveMessage("13"); //Freeeeeeee Faaaaaaaaaaaling
  lastAction = FreeFall;
}

void processDoubleTapAction() {
  DoubleTapTime = micros();
  if (isEnoughTimeSinceLastFreeFallDT()) {
    saveMessage("12"); //Double Tap
    lastAction = DoubleTap;    
  }
}

void processSingleTapAction() {
  SingleTapTime = micros();
  if (isEnoughTimeSinceLastFreeFallST()) {
    saveMessage("11"); //Single Tap
    lastAction = SingleTap;
  }
}

void processInactivityAction() {
  InactivityTime = micros();
  saveMessage("00"); //Inactive
  lastAction = Inactivity;
}

void processActivityAction() {
  ActivityTime = micros();
  saveMessage("01"); //Active
  lastAction = Activity;
}

 bool isEnoughTimeSinceLastFreeFallST() {
  return SingleTapTime - FreeFallTime > 1000000;
 }

bool isEnoughTimeSinceLastFreeFallDT() {
  return DoubleTapTime - FreeFallTime > 1000000;
 }

