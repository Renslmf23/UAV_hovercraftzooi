void RegelaarVoor() {


  //Wanneer geen hysterese wenselijk is deze waarde 0 maken

  float x = AfstandVoor()/1000; //Maak meters van de gemeten millimeters

  //error_oud is static omdat deze weer vergeleken moet worden met de nieuwe error
  error_afstand_voor = sp_afstand_voor - x;
  d_error_afstand_voor = error_afstand_voor - error_oud_afstand_voor;
  F = Kp_afstand_voor * error_afstand_voor + Kd_afstand_voor * d_error_afstand_voor / dt;
  //Zorg dat F niet hoger kan dan de maximale stuwkracht van de motoren.
  F = constrain(F, Fmin, Fmax);
  error_oud_afstand_voor = error_afstand_voor;
  
  //Delen door twee zodat per motor de pwm bepaald kan worden
  float motorkracht = F / 2;
  if((abs(x-sp_afstand_voor) < hysterese)){
    motorkracht = 0;
  }
//  Serial.print("Motorkracht is: "); Serial.println(motorkracht);
  if(state == regelaarVoor){
    StuurMotorenAan(motorkracht, motorkracht, 0);
  } else if(state == regelaarZij){
      RegelaarZij(motorkracht);

  } else if(state == regelaarHoek){
    RegelaarHoek(motorkracht);
  }
}
