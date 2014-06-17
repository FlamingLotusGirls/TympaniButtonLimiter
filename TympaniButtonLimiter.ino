
const byte iButtonPins[8] = {10, 11, 12, 13, 14, 15, 16, 17};
const byte oButtonPins[8] = {9, 8, 7, 6, 5, 4, 3, 2};
const byte iDelaySettingAnalogPin = 4;
const int decrementCheckInterval = 500;
const int decrementAmount = 300;
const int minTimeCountLimit = 1000;
const int maxTimeCountLimit = 10000;

unsigned long onTimeCounts[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned long changeTimes[8] = {0, 0, 0, 0, 0, 0, 0, 0};
bool buttonStates[8] = {false, false, false, false, false, false, false, false};
unsigned long nextDecrementCheckTime = 0;

void setup(){
  for (int i=0; i<8; i++){
    pinMode(iButtonPins[i], INPUT);
    pinMode(oButtonPins[i], OUTPUT);
  }

}


void loop(){
  unsigned long timeNow = millis();
  int timeCountLimit = getTimeCountLimit();
  if (timeNow > nextDecrementCheckTime){
    nextDecrementCheckTime = timeNow + decrementCheckInterval;
    doDecrement();
  }
  for (int i=0; i<8; i++){
    if (!digitalRead(iButtonPins[i])){
      if (!buttonStates[i]){
        changeTimes[i] = timeNow;
        buttonStates[i] = true;
      }
    }
    else {
      if (buttonStates[i]){
        onTimeCounts[i] += timeNow - changeTimes[i];
        onTimeCounts[i] = onTimeCounts[i] > maxTimeCountLimit ? maxTimeCountLimit : onTimeCounts[i];
        changeTimes[i] = timeNow;
        buttonStates[i] = false;
      }
    
    }
    if (buttonStates[i] && (onTimeCounts[i] + timeNow - changeTimes[i]) < timeCountLimit){
      digitalWrite(oButtonPins[i], HIGH);
    }
    else {
      digitalWrite(oButtonPins[i], LOW);
    }
  }
}
void doDecrement(){
  for (int i=0; i<8; i++){
    if (onTimeCounts[i] > 0 && !buttonStates[i]){
      onTimeCounts[i] = onTimeCounts[i] > decrementAmount ? onTimeCounts[i] - decrementAmount : 0;
    }
  }
}
int getTimeCountLimit(){
  long countAboveMin = ((maxTimeCountLimit - minTimeCountLimit) * analogRead(iDelaySettingAnalogPin)) / 1024;
  int result = minTimeCountLimit + countAboveMin;
  return result;
}
