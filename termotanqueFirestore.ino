#include <dummy.h>

 
  /***************para otro sensor*************
   * 
   * #include <DHT.h>
#include <DHT_U.h>

DHT dht (D3, DHT11);
float temp, hume;
int pause = 800;

void leerTemperatura(){
  hume =dht.readHumidity();
  temp =dht.readTemperature();

  Serial.println("Temperatura: "+String(temp)+", Humedad: "+String(hume));
  }

void setup(){
  dht.begin();
  Serial.begin(115200);
  
  }

void loop(){
  leerTemperatura();
  delay(pause);
  
  
  
  }*/
/*
#include <OneWire.h>
#include <DallasTemperature.h>

const int ONE_WIRE_BUS = 2;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensor(&oneWire);

int tiempo = 1;
int contador = 0;

void setup()
{
  Serial.begin(115200);

  Serial.print("t (s)\t");
  Serial.println("T (grados C)");
  Serial.println("===================");

  sensor.begin();
}

void loop()
{
  sensor.requestTemperatures();
  float temperatura = sensor.getTempCByIndex(0);

  Serial.print(contador);
  Serial.print("\t");
  Serial.println(temperatura, 1);

  delay(tiempo * 1000);
  contador += tiempo;
}
***********************************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiManager.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>

// Definir constantes
#define ANCHO_PANTALLA 128 // ancho pantalla OLED
#define ALTO_PANTALLA 64 // alto pantalla OLED

#define FIREBASE_HOST "domotikapp-b7876-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "7re5tvl3YMqlRKaB8SDsVJnUjSZu3dE15DDQXqkj"
//#define WIFI_SSID "enertik"
//#define WIFI_PASSWORD "lalalala"
const int ONE_WIRE_BUS = 2;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensor(&oneWire);
float Recuperacion;
float RecuFloat;
float ControlTempInicio;
float ControlTempFin;
float ControlNivelInicio;
bool ControlANivelEstado;
bool ControlATempEstado;
bool NivelEstado;
bool TempEstado;
String RecuString;
int tiempo = 1;
int contador = 0;
String Tiempo="";
String Fecha="";
String Hora="";
String Minuto="";
float temperatura=0;
float Nivel=0;
float temperaturaMax=0;
float temperaturaMin=1000;
float promedio=0;
bool ResetMinyMax=false;
String path = "/Dispositivo5";
//Define un objeto de Firebase
FirebaseData firebaseData;
// Objeto de la clase Adafruit_SSD1306
Adafruit_SSD1306 display(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, -1);
void printResult(FirebaseData &data);
void CausaError(void);
void InforSetLuzSensor(void);

void setup() {


  Serial.begin(115200);

  WiFiManager wifiManager;

  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  //wifiManager.resetSettings();

  wifiManager.autoConnect("ControlTemp");
  
  //WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  //Serial.print("Conectando a ....");
  
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Conectado con la IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //Establezca el tiempo de espera de lectura de la base de datos en 1 minuto (máximo 15 minutos)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //Tamaño y  tiempo de espera de escritura, tiny (1s), small (10s), medium (30s) and large (60s).
  //tiny, small, medium, large and unlimited.
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
  Serial.print("t (s)\t");
  Serial.println("T (grados C)");
  Serial.println("===================");
  
  Fecha = getFecha();
  //CArgar Fecha Anodo
  //if(Firebase.getString(firebaseData,path+"/Anodo/Fecha",&RecuString)){
   // }else{RecuString="00-00-00";}
  //delay(1000);
  //if (Firebase.setString(firebaseData, path + "/Anodo/Fecha" , RecuString) ){InforSetLuzSensor();}else{CausaError();}
  ////CArgar Fecha Cambio Anodo
  //if(Firebase.getString(firebaseData,path+"/Anodo/FechaCambio",&RecuString)){
    //}else{RecuString="00-00-00";}
  //delay(1000);
  //if (Firebase.setString(firebaseData, path + "/Anodo/FechaCambio" , RecuString) ){InforSetLuzSensor();}else{CausaError();}


  //Recuperacion Control automatico nivel
  if (Firebase.getBool(firebaseData,path+"/ControlANivel/Estado",&ControlANivelEstado)){}else{ControlANivelEstado=false;}
  delay(1000);
  if (Firebase.setBool(firebaseData,path+"/ControlANivel/Estado",ControlANivelEstado)){InforSetLuzSensor();}else{CausaError();}

  if (Firebase.getFloat(firebaseData,path+"/ControlANivel/Inicio",&ControlNivelInicio)){}else{ControlNivelInicio=0;}
  delay(1000);
  if (Firebase.setFloat(firebaseData,path+"/ControlANivel/Inicio",ControlNivelInicio)){InforSetLuzSensor();}else{CausaError();}
  
  //REcuperacion Control Automatico TEmperatura
  if (Firebase.getBool(firebaseData,path+"/ControlATemp/Estado",&ControlATempEstado)){}else{ControlATempEstado=false;}
  delay(1000);
  if (Firebase.setBool(firebaseData,path+"/ControlATemp/Estado",ControlATempEstado)){InforSetLuzSensor();}else{CausaError();}

  if (Firebase.getFloat(firebaseData,path+"/ControlATemp/Inicio",&ControlTempInicio)){}else{ControlTempInicio=0;}
  delay(1000);
  if (Firebase.setFloat(firebaseData,path+"/ControlATemp/Inicio",ControlTempInicio)){InforSetLuzSensor();}else{CausaError();}
  
  if (Firebase.getFloat(firebaseData,path+"/ControlATemp/Fin",&ControlTempFin)){}else{ControlTempFin=0;}
  delay(1000);
  if (Firebase.setFloat(firebaseData,path+"/ControlATemp/Fin",ControlTempFin)){InforSetLuzSensor();}else{CausaError();}

  //REcuperacion estado de Relay Nivel
  if (Firebase.getBool(firebaseData,path+"/NivelRelay/Estado",&NivelEstado)){}else{NivelEstado=false;}
  delay(1000);
  if (Firebase.setBool(firebaseData,path+"/NivelRelay/Estado",NivelEstado)){InforSetLuzSensor();}else{CausaError();}
  
  
  //Recuperacion estado de Relay TEmperatura
  if (Firebase.getBool(firebaseData,path+"/Resistencia/Estado",&TempEstado)){}else{TempEstado=false;}
  delay(1000);
  if (Firebase.setBool(firebaseData,path+"/Resistencia/Estado",TempEstado)){InforSetLuzSensor();}else{CausaError();}


  //REcuperacion Fechas y horas
  if(Firebase.getFloat(firebaseData,path + "/Fecha/" + Fecha + "/Hora/00",&Recuperacion)){
   
    }else{Recuperacion=0;};
  delay(1000);
  if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/00" , Recuperacion) ){InforSetLuzSensor();}else{CausaError();}
  if(Firebase.getFloat(firebaseData,path + "/Fecha/" + Fecha + "/Hora/02",&Recuperacion)){
   
    }else{Recuperacion=0;};
  delay(1000);
  if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/02" , Recuperacion)){InforSetLuzSensor();}else{CausaError();}
  if(Firebase.getFloat(firebaseData,path + "/Fecha/" + Fecha + "/Hora/04",&Recuperacion)){
   
    }else{Recuperacion=0;};
  delay(1000);
  if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/04" , Recuperacion)){InforSetLuzSensor();}else{CausaError();}
  if(Firebase.getFloat(firebaseData,path + "/Fecha/" + Fecha + "/Hora/06",&Recuperacion)){
   
    }else{Recuperacion=0;};
  delay(1000);
  if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/06" , Recuperacion)){InforSetLuzSensor();}else{CausaError();}

  if(Firebase.getFloat(firebaseData,path + "/Fecha/" + Fecha + "/Hora/08",&Recuperacion)){
   
    }else{Recuperacion=0;};
  delay(1000);
  if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/08" , Recuperacion)){InforSetLuzSensor();}else{CausaError();}

  if(Firebase.getFloat(firebaseData,path + "/Fecha/" + Fecha + "/Hora/10",&Recuperacion)){
   
    }else{Recuperacion=0;};
  delay(1000);
  if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/10" , Recuperacion)){InforSetLuzSensor();}else{CausaError();}

  if(Firebase.getFloat(firebaseData,path + "/Fecha/" + Fecha + "/Hora/12",&Recuperacion)){
   
    }else{Recuperacion=0;};
  delay(1000);
  if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/12" , Recuperacion)){InforSetLuzSensor();}else{CausaError();}

  if(Firebase.getFloat(firebaseData,path + "/Fecha/" + Fecha + "/Hora/14",&Recuperacion)){
   
    }else{Recuperacion=0;};
  delay(1000);
  if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/14" , Recuperacion)){InforSetLuzSensor();}else{CausaError();}

  if(Firebase.getFloat(firebaseData,path + "/Fecha/" + Fecha + "/Hora/16",&Recuperacion)){
   
    }else{Recuperacion=0;};
  delay(1000);
  if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/16" , Recuperacion)){InforSetLuzSensor();}else{CausaError();}

  if(Firebase.getFloat(firebaseData,path + "/Fecha/" + Fecha + "/Hora/18",&Recuperacion)){
   
    }else{Recuperacion=0;};
  delay(1000);
  if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/18" , Recuperacion)){InforSetLuzSensor();}else{CausaError();}

  if(Firebase.getFloat(firebaseData,path + "/Fecha/" + Fecha + "/Hora/20",&Recuperacion)){
   
    }else{Recuperacion=0;};
  delay(1000);
  if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/20" , Recuperacion)){InforSetLuzSensor();}else{CausaError();}

  if(Firebase.getFloat(firebaseData,path + "/Fecha/" + Fecha + "/Hora/22",&Recuperacion)){
   
    }else{Recuperacion=0;};
  delay(1000);
  if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/22" , Recuperacion)){InforSetLuzSensor();}else{CausaError();}
  sensor.begin();
#ifdef __DEBUG__
  
  delay(100);
  Serial.println("Iniciando pantalla OLED");
#endif
 
  // Iniciar pantalla OLED en la dirección 0x3C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
#ifdef __DEBUG__
    Serial.println("No se encuentra la pantalla OLED");
#endif
    while (true);
  }
  sensor.requestTemperatures();
  temperatura = sensor.getTempCByIndex(0);
  // Limpiar buffer
  display.clearDisplay();
  String Time = getTime();
  Serial.println(Time);
  // Tamaño del texto
  display.setTextSize(1);
  // Color del texto
  display.setTextColor(SSD1306_WHITE);
  // Posición del texto
  display.setCursor(10, 32);
  // Escribir texto
  display.println("Temperatura: "+String(temperatura));
 
  // Enviar a pantalla
  display.display();
  delay( 15000);
  contador += tiempo;
};
 
void loop() {
  
  sensor.requestTemperatures();
  temperatura = sensor.getTempCByIndex(0);
  Fecha = getFecha();
  Hora = getHora();
  Minuto= getMinutos();
  
  
  //RElay de Nivel
  if(NivelEstado){
    digitalWrite(5,HIGH);
    //Prender rele de nivel
  }else{
    digitalWrite(5,LOW);
    //Apagar rele de nivel  
  }

  
  //Relay de Resistencia de temperatura
  if(TempEstado){
    digitalWrite(4,HIGH);
    //Prender rele de temperatura
    //Inicia temporizador
  }else{
    digitalWrite(4,LOW);
    //Apagar rele de temperatura
    //Reinicia temporizador  
  }

  
  //Control automatico de Temperatura
  if(ControlATempEstado){
    //Prender rele de nivel
    if(temperatura<ControlTempInicio){
      //Prender rele de TEmperatura
        digitalWrite(4,HIGH);
        
    }
    if(temperatura>ControlTempFin){
      digitalWrite(4,LOW);
      }
      
      
      
  }else{
    digitalWrite(4,LOW);
    //Apagar rele de temperature 
  }

  
  //Control automatico de nivel
  if(ControlANivelEstado){
    if(Nivel<ControlNivelInicio){
      //Prender rele de nivel
      digitalWrite(5,HIGH);
    }
    if( Nivel>76){
      digitalWrite(5,LOW);
      }
    
  }else{
    digitalWrite(5,LOW);
    //Apagar rele de nivel  
  }
  
  Serial.println(Tiempo);
  if (temperatura>=temperaturaMax){
    temperaturaMax=temperatura;
    }
  if (temperatura<=temperaturaMin){
    temperaturaMin=temperatura;
    }
  display.clearDisplay();
 
  // Tamaño del texto
  display.setTextSize(1);
  // Color del texto
  display.setTextColor(SSD1306_WHITE);
  // Posición del texto
  display.setCursor(10, 32);
  // Escribir texto
  display.println("Temperatura: "+String(temperatura));
  
  // Enviar a pantalla
  display.display();
  if(Hora.toInt()==0){
    if(Minuto.toInt()==5 && ResetMinyMax){
         temperaturaMax=0;
         temperaturaMin=1000;
         ResetMinyMax=false;
         if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/00" , 0)){InforSetLuzSensor();}else{CausaError();}
         if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/02" , 0)){InforSetLuzSensor();}else{CausaError();}
         if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/04" , 0)){InforSetLuzSensor();}else{CausaError();}
         if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/06" , 0)){InforSetLuzSensor();}else{CausaError();}
         if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/08" , 0)){InforSetLuzSensor();}else{CausaError();}
         if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/10" , 0)){InforSetLuzSensor();}else{CausaError();}
         if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/12" , 0)){InforSetLuzSensor();}else{CausaError();}
         if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/14" , 0)){InforSetLuzSensor();}else{CausaError();}
         if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/16" , 0)){InforSetLuzSensor();}else{CausaError();}
         if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/18" , 0)){InforSetLuzSensor();}else{CausaError();}
         if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/20" , 0)){InforSetLuzSensor();}else{CausaError();}
         if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/22" , 0)){InforSetLuzSensor();}else{CausaError();}
         
      }
    }
  if(Hora.toInt()==0){
    if(Minuto.toInt()==12){
        ResetMinyMax=true;
        if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/"+ Hora , temperatura)){InforSetLuzSensor();}else{CausaError();}
      }
  }else if((Hora.toInt()%2)==0){
    if(Minuto.toInt()==12){
        if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Hora/"+ Hora , temperatura)){InforSetLuzSensor();}else{CausaError();}
      }
    
    }
  promedio=Promedios(promedio,temperatura);
  if (Firebase.setFloat(firebaseData, path + "/Sensores/Temperatura", temperatura)){InforSetLuzSensor();}else{CausaError();}
  if (Firebase.setFloat(firebaseData, path + "/Fecha/" + Fecha + "/Temperatura", promedio)){InforSetLuzSensor();}else{CausaError();}
  if (Firebase.setFloat(firebaseData, path + "/Sensores/TemperaturaMax", temperaturaMax)){InforSetLuzSensor();}else{CausaError();}
  if (Firebase.setFloat(firebaseData, path + "/Fecha/" +  Fecha + "/TemperaturaMax", temperaturaMax)){InforSetLuzSensor();}else{CausaError();}
  if (Firebase.setFloat(firebaseData, path + "/Sensores/TemperaturaMin", temperaturaMin)){InforSetLuzSensor();}else{CausaError();}
  if (Firebase.setFloat(firebaseData, path + "/Fecha/" +  Fecha + "/TemperaturaMin", temperaturaMin)){InforSetLuzSensor();}else{CausaError();}
  delay(tiempo * 10000);
  contador += tiempo;}

  void InforSetLuzSensor(void)
{
  Serial.println("Aprobado");
  String Time = getTime();
  Serial.println(Time);
  Serial.println("Ruta: " + firebaseData.dataPath());
  Serial.println("Tipo: " + firebaseData.dataType());
  Serial.println("ETag: " + firebaseData.ETag());
  Serial.print("Valor: ");
  printResult(firebaseData);
  Serial.println("------------------------------------");
  Serial.println(); 
}
void CausaError(void)
{
  Serial.println("ERROR");
  Serial.println("RAZON: " + firebaseData.errorReason());
  Serial.println("------------------------------------");
  Serial.println();
}
void printResult(FirebaseData &data)
{
    if (data.dataType() == "int")
        Serial.println(data.intData());
    else if (data.dataType() == "float")
        Serial.println(data.floatData(), 5);
    else if (data.dataType() == "double")
        printf("%.9lf\n", data.doubleData());
    else if (data.dataType() == "boolean")
        Serial.println(data.boolData() == 1 ? "true" : "false");
    else if (data.dataType() == "string")
        Serial.println(data.stringData());
}

String getTime(){
  WiFiClient client;
  HTTPClient http;
  String FechaS ="fecha";
  String timeS ="no anda";
  String HoraS="u";
  String MinutoS="e";
  http.begin(client, "http://worldtimeapi.org/api/timezone/America/Argentina/Buenos_Aires");
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY){
    String payload = http.getString();
    int beginS = payload.indexOf("datetime");
    int endS = payload.indexOf("T");
    
    int endSh = payload.indexOf("T");
    //Serial.println(payload);
    FechaS = payload.substring(beginS + 11, endS);
    timeS = payload.substring(endSh+1 ,endSh+9);
    HoraS = payload.substring(endSh+1 ,endSh+3);
    MinutoS=payload.substring(endSh+4 ,endSh+6);
    Serial.println("Fecha: "+FechaS);
    Serial.println("Tiempo: "+timeS);
    Serial.println("Hora: "+HoraS);
    Serial.println("Minuto: "+MinutoS);  
    
       
    
  }else{
    Serial.println(httpCode);
    timeS = httpCode;
    }
    
  return timeS;

  
}
String getFecha(){
  WiFiClient client;
  HTTPClient http;
  String FechaS ="fecha";
  String timeS ="no anda";
  String HoraS="u";
  String MinutoS="e";
  http.begin(client, "http://worldtimeapi.org/api/timezone/America/Argentina/Buenos_Aires");
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY){
    String payload = http.getString();
    int beginS = payload.indexOf("datetime");
    int endS = payload.indexOf("T");
    
    int endSh = payload.indexOf("T");
    //Serial.println(payload);
    FechaS = payload.substring(beginS + 11, endS);
    timeS = payload.substring(endSh+1 ,endSh+9);
    HoraS = payload.substring(endSh+1 ,endSh+3);
    MinutoS=payload.substring(endSh+4 ,endSh+6);
    Serial.println("Fecha: "+FechaS);
    Serial.println("Tiempo: "+timeS);
    Serial.println("Hora: "+HoraS);
    Serial.println("Minuto: "+MinutoS);  
    
       
    
  }else{
    Serial.println(httpCode);
    timeS = httpCode;
    }
    
  return FechaS;

  
}
String getHora(){
  WiFiClient client;
  HTTPClient http;
  String FechaS ="fecha";
  String timeS ="no anda";
  String HoraS="u";
  String MinutoS="e";
  http.begin(client, "http://worldtimeapi.org/api/timezone/America/Argentina/Buenos_Aires");
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY){
    String payload = http.getString();
    int beginS = payload.indexOf("datetime");
    int endS = payload.indexOf("T");
    
    int endSh = payload.indexOf("T");
    //Serial.println(payload);
    FechaS = payload.substring(beginS + 11, endS);
    timeS = payload.substring(endSh+1 ,endSh+9);
    HoraS = payload.substring(endSh+1 ,endSh+3);
    MinutoS=payload.substring(endSh+4 ,endSh+6);
    Serial.println("Fecha: "+FechaS);
    Serial.println("Tiempo: "+timeS);
    Serial.println("Hora: "+HoraS);
    Serial.println("Minuto: "+MinutoS);  
    
       
    
  }else{
    Serial.println(httpCode);
    timeS = httpCode;
    }
    
  return HoraS;

  
}
String getMinutos(){
  WiFiClient client;
  HTTPClient http;
  String FechaS ="fecha";
  String timeS ="no anda";
  String HoraS="u";
  String MinutoS="e";
  http.begin(client, "http://worldtimeapi.org/api/timezone/America/Argentina/Buenos_Aires");
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY){
    String payload = http.getString();
    int beginS = payload.indexOf("datetime");
    int endS = payload.indexOf("T");
    
    int endSh = payload.indexOf("T");
    //Serial.println(payload);
    FechaS = payload.substring(beginS + 11, endS);
    timeS = payload.substring(endSh+1 ,endSh+9);
    HoraS = payload.substring(endSh+1 ,endSh+3);
    MinutoS=payload.substring(endSh+4 ,endSh+6);
    Serial.println("Fecha: "+FechaS);
    Serial.println("Tiempo: "+timeS);
    Serial.println("Hora: "+HoraS);
    Serial.println("Minuto: "+MinutoS);  
    
       
    
  }else{
    Serial.println(httpCode);
    timeS = httpCode;
    }
    
  return MinutoS;

  
}



  float Promedios (float prom,float temp){
    if (prom==0){
      prom=temp;
      return (prom+temp)/2;  
    } else{ 
      return (prom+temp)/2;  
    }
    }
