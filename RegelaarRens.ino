void RegelaarRens() {
  float angle = mpu.GetAngZ();

  float error_hoek_rens = angle - sp_hoek_rens;
  d_error_hoek_rens = error_hoek_rens - error_hoek_oud_rens;
  error_hoek_som_rens += error_hoek_rens;
  float output = Kp_hoek_rens * error_hoek_rens;
  output += Ki_hoek_rens * error_hoek_som_rens * dt;
  output += Kd_hoek_rens * (d_error_hoek_rens / dt);

  output = constrain(output, Fmin, Fmax);

  StuurMotorenAan(-output, output);
  
}
