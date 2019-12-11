#include <SoftwareSerial.h>//incluimos SoftwareSerial
#include <TinyGPS.h>//incluimos TinyGPS

TinyGPS gps;//Declaramos el objeto gps
SoftwareSerial serialgps(11,10);//Declaramos el pin 11 Tx y 10 Rx

//Declaramos la variables para la obtención de datos
unsigned long chars;
unsigned short sentences, failed_checksum;

void setup()
{
  Serial.begin(9600);//Iniciamos el puerto serie
  serialgps.begin(9600);//Iniciamos el puerto serie del gps
  //Imprimimos:
  Serial.println("");
  Serial.println(" ---Buscando senal--- ");
  Serial.println("");
}

void loop()
{
  while(serialgps.available()) 
  {
    int c = serialgps.read();
 
    if(gps.encode(c))  
    {
      float latitude, longitude;
      gps.f_get_position(&latitude, &longitude);
      Serial.print("Latitud/Longitud: "); 
      Serial.print(latitude,5); 
      Serial.print(", "); 
      Serial.println(longitude,5);

      gps.stats(&chars, &sentences, &failed_checksum);  
    }
  }
}
