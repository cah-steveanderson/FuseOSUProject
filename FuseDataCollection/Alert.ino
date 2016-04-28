
void processAlert()
{
  if(digitalRead(BUTTON_PIN) == LOW)
  {
    saveMessage("14"); //ALERT ALERT!!!!!!
  }
}

