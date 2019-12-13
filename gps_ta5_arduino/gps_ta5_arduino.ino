#include <SoftwareSerial.h>//incluimos SoftwareSerial
#include <TinyGPS.h>//incluimos TinyGPS

TinyGPS gps;//Declaramos el objeto gps
SoftwareSerial serialgps(50,51);//Declaramos el pin 11 Tx y 10 Rx

//Declaramos la variables para la obtención de datos
unsigned long chars;
unsigned short sentences, failed_checksum;
const long interval = 5000;
unsigned long previousMillis = 0;
float latitude, longitude;

void setup()
{
  Serial.begin(9600);//Iniciamos el puerto serie
  serialgps.begin(9600);//Iniciamos el puerto serie del gps
  //Imprimimos:
  Serial.println("");
  Serial.println(" ---Buscando senal--- ");
  Serial.println("");
//////////////////////////
  //Serial.begin(9600);
//  Serial.println("AT$RC");
//  delay(100);
//  Serial.println("AT$SF=0123CAFE");
//////////////////////////
  
}

void loop()
{
  
  //unsigned long currentMillis = millis();
  //if (currentMillis - previousMillis >= interval) 
  //{
  //previousMillis = currentMillis;
  //Serial.print("entro cada 5 segundos\n"); 
    while(serialgps.available()) 
    {
      int c = serialgps.read();
     
      if(gps.encode(c))  
      {
        //float latitude, longitude;
        gps.f_get_position(&latitude, &longitude);
        Serial.print("Latitud/Longitud: "); 
        Serial.print(latitude,4); 
        Serial.print(", "); 
        Serial.println(longitude,4);
        delay(5000); 

        gps.stats(&chars, &sentences, &failed_checksum);  
      }
    //}
  }
}
