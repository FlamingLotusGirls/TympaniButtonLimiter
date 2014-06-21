
const byte iButtonPins[8] = {10, 11, 12, 13, 14, 15, 16, 17};
const byte oButtonPins[8] = {9, 8, 7, 6, 5, 4, 3, 2};
const byte iDelaySettingAnalogPin = 4;
const int decrementCheckInterval = 500;
const int decrementAmount = 400;
const int minTimeCountLimit = 900;
const int maxTimeCountLimit = 10000;

unsigned long onTimeCounts[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned long changeTimes[8] = {0, 0, 0, 0, 0, 0, 0, 0};
bool buttonStates[8] = {false, false, false, false, false, false, false, false};
bool enabledStates[8] = {true, true, true, true, true, true, true, true};
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
		if (!digitalRead(iButtonPins[i])){ //button pressed
			if (!buttonStates[i]){ //just pressed now
				changeTimes[i] = timeNow;
				buttonStates[i] = true;
			}
			if ((onTimeCounts[i] + timeNow - changeTimes[i]) > timeCountLimit){
				enabledStates[i] = false;
			}
		}
		else { //button not pressed
			if (buttonStates[i]){ //just released now
				onTimeCounts[i] += timeNow - changeTimes[i];
				onTimeCounts[i] = onTimeCounts[i] > timeCountLimit ? timeCountLimit : onTimeCounts[i];
				changeTimes[i] = timeNow;
				buttonStates[i] = false;
			}
		}
		if (buttonStates[i] && enabledStates[i]){
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
		if (onTimeCounts[i] == 0){
			enabledStates[i] = true;
		}
	}
}
int getTimeCountLimit(){
	unsigned long countAboveMin = (maxTimeCountLimit - minTimeCountLimit) / 32;
	countAboveMin = countAboveMin * (analogRead(iDelaySettingAnalogPin) / 32);
	int result = minTimeCountLimit + countAboveMin;
	return result;
}
