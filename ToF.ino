//Als je meer wilt returnen een struct terug sturen ipv een float
float ToF(int sensorAdres) {

  float afstand;

  switch (sensorAdres) {
    case tofVoor:
      afstand = tof1.readRangeSingleMillimeters();
      break;
    case tofZijVoor:
      afstand = tof2.readRangeSingleMillimeters();
      break;
    case tofZijAchter:
      afstand = tof3.readRangeSingleMillimeters();
      break;
  }

  if (afstand == 0) {
    Serial.println("error");
    ToF(sensorAdres);
  }
  if (afstand >= 5000) {
    Serial.println(" Out of range ");
  }
//  Serial.print("Afstand (mm): ");
//  Serial.println(afstand);

  return (afstand);
}
