#include <Wire.h>
#include <VL53L0X.h>
#include <TinyMPU6050.h>

//Instanties van de ToF sensoren aanmaken
VL53L0X tof1 ;
VL53L0X tof2 ;
VL53L0X tof3 ;

//Instantie aanmaken van MPU
MPU6050 mpu (Wire);

//Shutdowns van de ToF sensoren
#define shdwnVoor 2
#define shdwnZijVoor 3
#define shdwnZijAchter 4

//Adressen voor de ToF sensoren
#define tofVoor 0x30
#define tofZijVoor 0x31
#define tofZijAchter 0x32

//Motor power en richting
#define mzijdir 8  //Motor zijkant achter direction
#define mradir 7  //Motor rechts achter direction
#define mladir 6  //Motor links achter direction

#define mzij 11   //Motor zijkant power
#define mra 10    //Motor rechts achter power 
#define mla 9     //Motor links achter power

#define blowers 5 //Blowers

#define TESTI2C 1

//Te gebruiken functies:
//returns void  -> StartSensoren() : Aanroepen in de setup, zet de adressen van de ToF sensoren wanneer de arduino opstart.
//returns float -> Muurhoek() : Geeft de hoek ten opzichte van de muur in graden. Rechtsom negatief, linksom positief.
//returns float -> ToF(int adres) : Returnt de afstand die de meegegeven sensor meet in mm.
//returns float -> AfstandVoor() : Returnt de afstand van de sensor aan de voorkant. Kan ipv deze functie ook ToF(tofVoor) aanroepen.
//returns void  -> RegelaarVoor() : Regelaar om op 300mm van een muur tot stilstand te komen.


//Belangrijke waardes die de meeste regelaars nodig hebben, vandaar globaal
const float m = 1.143;          // kg
const float Fmax = 0.20 * 2;    // N
const float Fmin = -0.26 * 2;   // N

long t_oud, t_nw;               // ms
float dt;
const long cyclustijd = 100;    // ms

//Regelaar waardes:
//Deze waarden aanpassen om de regelaar af te stellen.
//Beginwaarden waren sp = 0.3  Kp = 2.0  Kd = 3.0
//sp is het punt in m waar de hovercraft op moet stabiliseren
const float sp_afstand_voor = 0.3;
const float Kp_afstand_voor = 2.0;
const float Kd_afstand_voor = 2.0;
const float hysterese = 0.01;
float error_afstand_voor = 0.0, d_error_afstand_voor = 0.0, error_oud_afstand_voor = 0.0;

const float sp_afstand_zij = 0.3;
const float Kp_afstand_zij = 1.5;
const float Kd_afstand_zij = 1.5;
float error_afstand_zij = 0.0, d_error_afstand_zij = 0.0, error_oud_afstand_zij = 0.0;

const float sp_hoek = 0.0;
const float Kp_hoek = 2.0;
const float Kd_hoek = 2.0;
const float Ki_hoek = 1;

float error_hoek_som = 0;

float error_hoek, error_hoek_oud, d_error_hoek;

float sp_hoek_rens = 0.0;
const float Kp_hoek_rens = 2.0;
const float Kd_hoek_rens = 2.0;
const float Ki_hoek_rens = 1.0;

float error_hoek_som_rens = 0;

float error_hoek_oud_rens, d_error_hoek_rens;




float F = 0.0;

int lastHoekVal = 0; //last value send by py

static const float sensorSpacing = 328;

void StuurMotorenAan(float F1, float F2, float F3 = 0.0);

String inputString = "";

bool enabled = false;

enum State {
  regelaarVoor,
  regelaarHoek,
  regelaarZij,
  regelaarRosa,
  regelaarRens
};

State state = regelaarVoor;

String buf = "";

unsigned int errorCount = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();



  buf.reserve(100);

  //Zet de pinnen van de motoren en blowers als output
  pinMode(mra, OUTPUT);
  pinMode(mla, OUTPUT);
  pinMode(mzij, OUTPUT);
  pinMode(mradir, OUTPUT);
  pinMode(mladir, OUTPUT);
  pinMode(mzijdir, OUTPUT);
  pinMode(blowers, OUTPUT);

  //Voor testen zetten we de blowers aan en de fans uit
  digitalWrite(mzij, LOW);
  digitalWrite(mra, LOW);
  digitalWrite(mla, LOW);

  //ToF sensoren opstarten
  StartSensoren();
  delay(1000);
  Serial.println("Started!");

  t_oud = millis();
  //  enabled = true;
  //  state = regelaarRens;
  //  digitalWrite(blowers, HIGH);

}

void loop() {
  mpu.Execute();
  if (enabled) {
    t_nw = millis();
    if (t_nw - t_oud > cyclustijd)
    {
      dt = (t_nw - t_oud) * .001;
      t_oud = t_nw;
      if (state == regelaarRosa) {
        RegelaarHoek_aruco(0);
      } else if (state == regelaarRens) {
        RegelaarRens();
      } else {
        RegelaarVoor();
      }

      //hoekAruco();
    }
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      DecodeString();
    }
  }
}

void DecodeString() {
  switch (inputString.charAt(0)) {
    case 's':
      enabled = !enabled;
      SetEnabled();
      break;
    case 'r':
      state = regelaarVoor;
      analogWrite(mzij, 0);
      analogWrite(mra, 0);
      analogWrite(mla, 0);
      break;
    case 'v':
      state = regelaarZij;
      analogWrite(mzij, 0);
      analogWrite(mra, 0);
      analogWrite(mla, 0);
      break;
    case 'R':
      state = regelaarHoek;
      analogWrite(mzij, 0);
      analogWrite(mra, 0);
      analogWrite(mla, 0);
      break;
    case 't':
      state = regelaarRosa;
      analogWrite(mzij, 0);
      analogWrite(mra, 0);
      analogWrite(mla, 0);
      break;
    case 'h':
      state = regelaarRens;
      analogWrite(mzij, 0);
      analogWrite(mra, 0);
      analogWrite(mla, 0);
      break;
    case 'H':
      sp_hoek_rens = inputString.substring(1, 4).toInt();
      Serial.println(sp_hoek_rens);
      break;
    case 'C':
      mpu.Calibrate();
      Serial.println(F("Done"));
      break;
    default:
      Serial.print(F("Unknown command: "));
      Serial.println(inputString);
      break;
  }
  inputString = "";
}

void SetEnabled() {
  digitalWrite(blowers, enabled);
  analogWrite(mzij, 0);
  analogWrite(mra, 0);
  analogWrite(mla, 0);
}
