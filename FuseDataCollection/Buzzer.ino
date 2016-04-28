int timesToBeep = 15;
void processBuzzer() {
  if (digitalRead(BUTTON_PIN) == HIGH) {
    for(int i = 0; i < timesToBeep; i++)
    {
      beep(100);
    }
  }
}

void beep(unsigned char delayms) {
  analogWrite(BUZZER_PIN, 200);     // ant value can be used expect 0 and 255
  delay(delayms);
  analogWrite(BUZZER_PIN, 0);       // 0 truns it off
  delay(delayms);
}

