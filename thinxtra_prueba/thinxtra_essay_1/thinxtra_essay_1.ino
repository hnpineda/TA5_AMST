

void setup() {
  Serial.begin(9600);
  Serial.println("AT$RC");
  delay(100);
  Serial.println("AT$SF=0123CAFE");
}

void loop() {}
