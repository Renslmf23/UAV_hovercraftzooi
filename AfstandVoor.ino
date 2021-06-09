//Vraag de afstand op aan de voorkant en return de afstand in mm
float AfstandVoor() {
  return ToF(tofVoor);
}

float AfstandZij() {
  return (ToF(tofZijVoor) + ToF(tofZijAchter)) / 2;
}
