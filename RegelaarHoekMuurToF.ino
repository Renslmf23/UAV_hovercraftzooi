void RegelaarHoek(float motorkracht){
  error_hoek = sp_hoek - Muurhoek();
  d_error_hoek = error_hoek - error_hoek_oud;
  error_hoek_som += error_hoek;
  error_hoek_som = constrain(error_hoek_som, -6, 6); //constrain max sum to roughly +- 360 deg
  
  float F_hoek = Kp_hoek * error_hoek + Kd_hoek * d_error_hoek / dt + Ki_hoek * error_hoek_som * dt;
  
  F_hoek = constrain(F_hoek/2, Fmin/2, Fmax/2);
  error_hoek_oud = error_hoek;
  StuurMotorenAan(constrain(motorkracht+F_hoek, Fmin, Fmax), constrain(motorkracht-F_hoek, Fmin, Fmax));
}
