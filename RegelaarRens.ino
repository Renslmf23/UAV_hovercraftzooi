void RegelaarRens() {
  float angle = mpu.GetAngZ();
  Serial.println(angle);
  float error_hoek_rens = angle - sp_hoek_rens;
  d_error_hoek_rens = error_hoek_rens - error_hoek_oud_rens;
  error_hoek_som_rens += error_hoek_rens;
  error_hoek_som_rens = constrain(error_hoek_som_rens, -50, 50);
  float output = Kp_hoek_rens * error_hoek_rens;
  //output += Ki_hoek_rens * error_hoek_som_rens * dt;
  output += Kd_hoek_rens * (d_error_hoek_rens / dt);

  output = constrain(output/10, -Fmax/2, Fmax/2);
  //Serial.println(output);
  StuurMotorenAan(output, -output);
  error_hoek_oud_rens = error_hoek_rens;  
}
