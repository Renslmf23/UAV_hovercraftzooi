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
  //  int indexC = buf.indexOf('c');
  Serial.println(buf.substring(indexA + 1));
  float aVal = buf.substring(indexA + 1).toInt();
  // int bVal = buf.substring(indexB + 1).toInt();
  if (aVal == 0) {
    aVal = lastHoekVal;
  }
  lastHoekVal = aVal;
  // aVal *= 0.01745;
  buf = "";
  Serial.println(aVal);
  //delay(500);
  return aVal;
}
