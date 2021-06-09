void RegelaarHoek(float motorkracht){
  error_hoek = sp_hoek - Muurhoek();
  d_error_hoek = error_hoek - error_hoek_oud;
  
  float F_hoek = Kp_hoek * error_hoek + Kd_hoek * d_error_hoek / dt;
  
  F_hoek = constrain(F_hoek/2, Fmin/2, Fmax/2);
  error_hoek_oud = error_hoek;
  StuurMotorenAan(constrain(motorkracht+F_hoek, Fmin, Fmax), constrain(motorkracht-F_hoek, Fmin, Fmax));
}

void RegelaarHoek_aruco(float motorkracht){
  error_hoek = sp_hoek - hoekAruco();
  d_error_hoek = error_hoek - error_hoek_oud;
  
  float F_hoek = Kp_hoek * error_hoek + Kd_hoek * d_error_hoek / dt;
  
  F_hoek = constrain(F_hoek/2, Fmin/2, Fmax/2);
  error_hoek_oud = error_hoek;
  StuurMotorenAan(F_hoek, -F_hoek);
}
