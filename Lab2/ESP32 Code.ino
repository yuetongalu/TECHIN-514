void setup() {
  // Initialize serial communication at a baud rate of 9600:
  Serial.begin(9600);
}

void loop() {
  // Read the input on analog pin A0:
  int sensorValue = analogRead(A0);
  
  // Convert the analog reading (which goes from 0 - 4095) to a voltage (0 - 3.3V):
  float voltage = sensorValue * (3.3 / 4095.0);

  // Print out the value in volts:
  Serial.println(voltage);

  // Wait for 100 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(100);
}
