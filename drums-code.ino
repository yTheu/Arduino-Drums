const int threshold = 9;
unsigned long startTime[5] = {0, 0, 0, 0, 0};
boolean active[5] = {0, 0, 0, 0, 0};
unsigned long offTime = 150;
boolean firstHit[5] = {1, 1, 1, 1, 1};
boolean setTime[5] = {0, 0, 0, 0, 0};
const int piezoPins[5] = {A0, A1, A2, A3, A4};
const byte piezoPitches[5] = {48, 52, 55, 69, 60};

void midiMsg(byte cmd, byte pitch, byte velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}

void handlePiezo(int index) {
  int val = analogRead(piezoPins[index]);

  if (val > threshold) {
    if (firstHit[index]) {
      if (!setTime[index]) {
        startTime[index] = micros();
        setTime[index] = 1;
      }

      if (micros() - startTime[index] > 800) {
        firstHit[index] = 0;
        setTime[index] = 0;
      }
    } else {
      if (!active[index]) {
        val = map(val, threshold, 1023, 50, 127);

        midiMsg(0x99, piezoPitches[index], val);
        active[index] = 1;
        startTime[index] = millis();
      }
    }
  }

  if (active[index]) {
    if (millis() - startTime[index] > offTime) {
      active[index] = 0;
      midiMsg(0x89, piezoPitches[index], 0);
    }
  }
}

void setup() {
  Serial.begin(115200); 
}

void loop() {
  for (int i = 0; i < 5; i++) {
    handlePiezo(i);
  }
}
