
const int StepX = 2;
const int DirX = 5;
const int StepY = 3;
const int DirY = 6;
const int StepZ = 4;
const int DirZ = 7;


void setup() {
  pinMode(StepX,OUTPUT);
  pinMode(DirX,OUTPUT);
  pinMode(StepY,OUTPUT);
  pinMode(DirY,OUTPUT);
  pinMode(StepZ,OUTPUT);
  pinMode( DirZ,OUTPUT);
}

void loop() {
 for(int x = 0; x<200; x++) { // loop for 200 steps
  digitalWrite(StepX,HIGH);
  delayMicroseconds(500);
  digitalWrite(StepX,LOW); 
  delayMicroseconds(500);
 }
 delay(500);
 for(int x = 0; x<200; x++) { // loop for 200 steps
  digitalWrite(StepY,HIGH);
  delayMicroseconds(500);
  digitalWrite(StepY,LOW); 
  delayMicroseconds(500);
 }
 delay(500);
 for(int x = 0; x<200; x++) { // loop for 200 steps
  digitalWrite(StepZ,HIGH);
  delayMicroseconds(500);
  digitalWrite(StepZ,LOW); 
  delayMicroseconds(500);
 }
 delay(500);
}
