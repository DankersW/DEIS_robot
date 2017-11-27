const int maxValueSpeedArray = 30;
int intaSpeed[maxValueSpeedArray][1] = {0};
int speedArrayCounter = 0;

void setup() {
  Serial.begin(9600);
  for(int i = 0; i < 30; i++){
    Serial.print(i);
    intaSpeed[i][0] = 75;
    intaSpeed[i][1] = 75;
    Serial.println(": " + String(intaSpeed[i][0]) + "," + String(intaSpeed[i][1]) + String(intaSpeed[i][3]));
  }
}

void printList(int a[30][2]){
  for(int i = 0; i <= 30; i++){
    Serial.println(a[i][0]);
  }
}

void loop() {
  
  if(speedArrayCounter == maxValueSpeedArray){
    speedArrayCounter = 0;
    printList(intaSpeed);
    delay(100000);
  }
  intaSpeed[speedArrayCounter][0] = speedArrayCounter;
  Serial.println(intaSpeed[speedArrayCounter][1]);
  speedArrayCounter++;
}



