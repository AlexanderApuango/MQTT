
#include <WiFi.h>
#include <PubSubClient.h>
#include <Separador.h>
#include <Wire.h>

//WIFICONFIG
const char* ssid = "Familia Apuango";
const char* password =  "Gabrielitayalex594621648@#22@#";
//const char* ssid = "NETLIFE-Foxcorp";
//const char* password =  "Thaisita2019";
//const char* ssid = "POCO X3 Pro";
//const char* password = "A123456789";


WiFiClient espClient;
PubSubClient client(espClient);

//VARIABLES A TRANSMITIR
float tempt;
float I;
float T;
float H;
int alertnum = 0;
float tiemp;
String PT100;

// CONEXION MQTT CON HIVEMQ
const char *SERVIDOR = "broker.hivemq.com";
const char *USUARIO = "";
const char *CLAVE = "";
const char *TOPIC_SUB = "SUB";
const char *TOPIC_PUB = "PUB";
const int PUERTO = 1883;

//VARIABLES DE MENSAJE
char MENSS[100];
char sms[50];

//VARIABLES DE DATOS A COMUNICAR
String ts;
char a;
boolean TransmisionCompleta = false;

//BUCLES
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
void setup_wifi();

//RELE
int pin_led = 26;
int pin_led2 = 25;

//SEPARADOR
Separador sep;


#define RXD2 16
#define TXD2 17


int n = 0;

void setup()
{
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(115200);
  //Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial Txd is on pin: " + String(TX));
  Serial.println("Serial Rxd is on pin: " + String(RX));
  setup_wifi();
  client.setServer(SERVIDOR, PUERTO);
  client.setCallback(callback);
  pinMode(pin_led, OUTPUT);
  pinMode(pin_led2, OUTPUT);
}

void setup_wifi()
{

  delay(10);
  // Nos conectamos a nuestra red Wifi
  Serial.println();
  //Serial.print("Conectando a ssid: ");
  //Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado a red WiFi!");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}
void reconnect() {

  while (!client.connected()) {
    Serial.print("Intentando conexión Mqtt...");
    // Creamos un cliente ID
    String clientId = "TEST";
    clientId += String(random(0xffff), HEX);
    // Intentamos conectar
    if (client.connect(clientId.c_str(), USUARIO, CLAVE)) {
      Serial.println("Conectado!");
      // Nos suscribimos
      if (client.subscribe(TOPIC_SUB)) {
        Serial.println("Suscripcion ok");
      } else {
        Serial.println("fallo Suscripción");
      }
    } else {
      Serial.print("falló :( con error -> ");
      Serial.print(client.state());
      Serial.println(" Intentamos de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

//*****************************
//***       CALLBACK        ***
//*****************************

void callback(char* topic, byte * payload, unsigned int length)
{
  String incoming = "";
  Serial.print("Mensaje recibido desde -> ");
  Serial.print(topic);
  Serial.println("");
  for (int i = 0; i < length; i++) {
    incoming += (char)payload[i];
  }
  incoming.trim();
  Serial.println("Mensaje -> " + incoming);
  if (incoming == "ON")
  {
    Serial.println("Activa compresor");
    digitalWrite(pin_led, HIGH);
    digitalWrite(pin_led2, HIGH);

  }
  if (incoming == "OFF")
  {
    Serial.println("Desactiva compresor");
    digitalWrite(pin_led, LOW);
    digitalWrite(pin_led2, LOW);

  }
  if (incoming == "REPARADO")
  {
    Serial.println("Activa compresor");
    Serial2.println("a");

  }



}
void loop()
{
  serialEvent();
  String valor1 = sep.separa(ts, '-', 0);
  tempt = valor1.toFloat();
  //Serial.println(tempt);
  String valor2 = sep.separa(ts, '-', 1);
  I = valor2.toFloat();;
  //Serial.println(flujo);
  String valor3 = sep.separa(ts, '-', 2);
  T = valor3.toFloat();
  //Serial.println(T);
  String valor4 = sep.separa(ts, '-', 3);
  H = valor4.toFloat();
  //Serial.println(flujo);
  String valor5 = sep.separa(ts, '-', 4);
  tiemp = valor5.toFloat();
  String valor6 = sep.separa(ts, '-', 5);
  alertnum = valor6.toInt();
  //Serial.println(flujo);


  if (TransmisionCompleta)
  {
    ts = "";
    TransmisionCompleta = false;
  }
  if (!client.connected()) {
    reconnect();
  }
  if (client.connected())
  {
    String s = ",";
    String str = tempt + s + I + s + T + s + H + s + tiemp + s + alertnum;
    str.toCharArray(sms, 50);
    client.publish(TOPIC_PUB, sms);
    Serial.println(sms );
    delay(2000);
    if (alertnum ==1)
    {
      
      digitalWrite(pin_led, LOW);
      digitalWrite(pin_led2, LOW);
    }
  }

  client.loop();
}

void serialEvent() {
  while (Serial2.available())
  {
    char b = Serial2.read();
    //Serial.print(a);
    ts += b;
    //n++;
    if (b == '\n')
    {
      TransmisionCompleta = true;
    }
  }

  //Serial.println(ts);
  delay(5000);
}
