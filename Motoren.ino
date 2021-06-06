void StuurMotorenAan(float F1, float F2, float F3) {

  digitalWrite(mladir, F1 <= 0);
  digitalWrite(mradir, F2 <= 0);
  digitalWrite(mzijdir, F3 <= 0);
  F1 = abs(F1);
  F2 = abs(F2);
  F3 = abs(F3);

  //omrekenen naar PWM:
  int pwm1 = NewtonToPWM(F1);
  int pwm2 = NewtonToPWM(F2);
  int pwm3 = NewtonToPWM(F3);

  analogWrite(mra, pwm1);
  analogWrite(mla, pwm2);
  analogWrite(mzij, 0);

}

int NewtonToPWM(float F) {
  float pwm = ((F + 0.027) / 0.002);
  if (pwm >= 100) {
    pwm = 100;
  }
  if (pwm <= 30) {
    pwm = 30;
  }
  pwm *= 2.55;
  return pwm;
}
