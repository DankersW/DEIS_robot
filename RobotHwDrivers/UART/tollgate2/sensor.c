void setup() 
{
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial); // wait for serial port to connect. Needed for native USB port only

    Serial.println("Goodnight moon!");
}
  
// this loops runs repeatedly and keeps polling sensors.
void loop() { // run over and over

    if (Serial.available()) {
        Serial.write("ALL\0");
        Serial.write("5,5,5,5\0");
    }
}