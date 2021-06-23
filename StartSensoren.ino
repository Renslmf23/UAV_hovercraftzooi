void StartSensoren() {
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  pinMode(shdwnVoor, OUTPUT);
  pinMode(shdwnZijVoor, OUTPUT);
  pinMode(shdwnZijAchter, OUTPUT);

  digitalWrite(shdwnVoor, LOW);
  digitalWrite(shdwnZijVoor, LOW);
  digitalWrite(shdwnZijAchter, LOW);
  delay(10);
  digitalWrite(shdwnVoor, HIGH);
  digitalWrite(shdwnZijVoor, HIGH);
  digitalWrite(shdwnZijAchter, HIGH);
  delay(10);

  //Serial.println("hoi");

  //Adres van sensor 1 zetten
  digitalWrite(shdwnVoor, HIGH);
  digitalWrite(shdwnZijVoor, LOW);
  digitalWrite(shdwnZijAchter, LOW);
  delay(10);
  if (!tof1.init()) {
    Serial.println(F("Eerste ToF sensor start niet"));
    //while (1);
  }
  //Serial.println("hoi2");
  tof1.setAddress(tofVoor);
  delay(10);

  //Adres van sensor 2 zetten
  digitalWrite(shdwnZijVoor, HIGH);
  if (!tof2.init()) {
    Serial.println(F("Tweede ToF sensor start niet"));
    //while (1);
  }
  tof2.setAddress(tofZijVoor);
  delay(10);

  //Adres van sensor 3 zetten
  digitalWrite(shdwnZijAchter, HIGH);
  if (!tof3.init()) {
    Serial.println(F("Derde ToF sensor start niet"));
    //while (1);
  }
  tof3.setAddress(tofZijAchter);
  Serial.println(F("ToF started"));
#ifndef TESTI2C
  mpu.Initialize();
  mpu.Calibrate();
#endif

}
