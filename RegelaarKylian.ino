void RegelaarZij(float motorkracht) {


  //Wanneer geen hysterese wenselijk is deze waarde 0 maken

  float x = AfstandZij()/1000; //Maak meters van de gemeten millimeters

  //error_oud is static omdat deze weer vergeleken moet worden met de nieuwe error
  error_afstand_zij = sp_afstand_zij - x;
  d_error_afstand_zij = error_afstand_zij - error_oud_afstand_zij;
  F = Kp_afstand_zij * error_afstand_zij + Kd_afstand_zij * d_error_afstand_zij / dt;

  //Zorg dat F niet hoger kan dan de maximale stuwkracht van de motoren.
  F = constrain(F, Fmin/2, Fmax/2);
  error_oud_afstand_zij = error_afstand_zij;
  
  //Delen door twee zodat per motor de pwm bepaald kan worden
  if((abs(x-sp_afstand_zij) < hysterese)){
    F = 0;
  }

  error_hoek = sp_hoek - Muurhoek();
  d_error_hoek = error_hoek - error_hoek_oud;
  
  float F_hoek = Kp_hoek * error_hoek + Kd_hoek * d_error_hoek / dt;
  
  F_hoek = constrain(F_hoek, Fmin/2, Fmax/2);
  error_hoek_oud = error_hoek;

  
  StuurMotorenAan(F_hoek + Fmax/2, -F_hoek + Fmax/2, -F);


}
