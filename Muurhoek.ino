//Vraag de hoek in graden ten opzichte van de muur. 0 graden is parallel, positief is linksom, negatief rechtsom.
float Muurhoek() {
  float voor = ToF(tofZijVoor);
  float achter = ToF(tofZijAchter);
  float hoek = 0;
  if (voor > achter) {
    hoek = atan((voor - achter) / sensorSpacing);
  }
  else if (voor < achter) {
    hoek = -1 * atan((achter - voor) / sensorSpacing);
  }
  else {
    hoek = 0;
  }
  return hoek;
}

float hoekAruco()
{
  Wire.beginTransmission(9);
  Wire.write("p");
  Wire.endTransmission();
  delay(15);
  Wire.requestFrom(9, 6);
  while (Wire.available()) {
    char c = Wire.read();
    if (c == '\n') {
      break;
    }
    buf += c;

  }
  int indexA = buf.indexOf('a');
  if (errorCount > 100) {
    enabled = false;
    errorCount = 0;
    Serial.println(F("No marker found!"));
    return 0;
  }
  if (indexA == -1) {
    buf = "";
    errorCount++;
    return 0;
  }
  errorCount = 0;
  //  int indexC = buf.indexOf('c');
  float aVal = (0.5 * buf.substring(indexA + 1).toInt()) + (0.5 * lastHoekVal);
  buf = "";
  // int bVal = buf.substring(indexB + 1).toInt();
  lastHoekVal = aVal;
  //aVal *= 0.01745;
  //  //delay(500);
  Serial.println(aVal);
  return aVal;
}
