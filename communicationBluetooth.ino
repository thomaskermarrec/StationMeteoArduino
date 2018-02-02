#include <SoftwareSerial.h>

#define RxD 10
#define TxD 11
#define pinLum 0
#define pinTemp 1

SoftwareSerial BTSerie(RxD,TxD);

void setup() {
  InitComunicationSerie();
  Serial.println("Initialisation de la carte : OK");

  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);

  
  InitCommunicationBluetoothSerie();
}


void InitComunicationSerie(){
  Serial.begin(9600);
  while(!Serial){
  }
  Serial.println("Demarrage connexion serie : OK");
}


void InitCommunicationBluetoothSerie(){
  BTSerie.begin(9600);
  while(!BTSerie){
    Serial.println("Attente d'une réponse bluetooth");
  }
  Serial.println("Demarrage connexion bluetooth : OK");
}


String Lumiere(){
  String toReturn;
  toReturn.reserve(50);
  int photocellReading;
  
  photocellReading = analogRead(pinLum);
  Serial.print("Analog reading = \n");
  Serial.print(photocellReading); // the raw analog reading
  Serial.print("\n");
  // We'll have a few threshholds, qualitatively determined
  if (photocellReading < 10) {
    Serial.println(" - Noir\n");
    toReturn = "Noir";
  } else if (photocellReading < 200) {
    Serial.println(" - Sombre\n");
    toReturn = "Sombre";
  } else if (photocellReading < 500) {
    Serial.println(" - Lumiere\n");
    toReturn = "Lumiere";
  } else if (photocellReading < 800) {
    Serial.println(" - Lumineux\n");
    toReturn = "Lumineux";
  } else {
    Serial.println(" - Tres lumineux\n");
    toReturn = "Tres lumineux";
  }

  return toReturn;
}

String Temperature() {
 double Temp;
 int mesure;
 String toReturn;
 toReturn.reserve(50);
 
 mesure = analogRead(pinTemp);
 Temp = log(10000.0*((1024.0/mesure-1)));
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;            // Convert Kelvin to Celcius

 toReturn = String(Temp, DEC);
 
 return toReturn;
}


void loop() {
  String msg;
  String reponse;
  String lumiere = "lumiere";
  String temperature = "temperature";
  msg.reserve(50);
  reponse.reserve(50);

  if(BTSerie.available()){
    Serial.print("Reception de : ");
    msg = BTSerie.readString();
    Serial.print(msg);
    
    if(msg.equals(lumiere)){
      reponse = Lumiere();
      BTSerie.print(reponse + "\n");
    } else if(msg.equals(temperature)) {
      reponse = Temperature();
      BTSerie.print(reponse + "\n");
    } else {
      BTSerie.print("Commande incorrecte\n");
    }
  }
  
  delay(100);
}
