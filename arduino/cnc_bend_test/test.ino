void blinkSetup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void blinkLoop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
}