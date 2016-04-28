#define ECG_PIN A1
int rawECGdata = 0;

void setUpECG() {
  pinMode(29, INPUT); // Setup for leads off detection LO +
  pinMode(28, INPUT); // Setup for leads off detection LO -
}

void processECG() {
  if ((digitalRead(29) == 1)||(digitalRead(28) == 1)){
        saveMessage("ECG is not connected");
  } else {
        rawECGdata = analogRead(A1);
        saveMessage("16" + String(rawECGdata));
  }
}
