/*
Temperature and Humidity Sensor using IOT_644 combined 
with DHT11.
The data can be save to designated server.
every 1 minute sensing of temperature and humidity.
prepared by: OJT's
*/


#include <Ethernet.h>  //Library for Ethernet functions
#include <SPI.h>       //Library for Serial Peripheral Interface (SPI) for microcontrollers to communicate other devices quickly 
#include <Client.h>    //Library for client functions
#include <dht.h>


#define dht_dpin A0 //no ; here. Set equal to channel sensor is on

dht DHT;

int count = 0;
float hum1, hum2, hum3, hum4, hum5;  //temporary storages of sensed humidity
float humid;            //final variable storage for sensed humidity
float temp1, temp2, temp3, temp4, temp5;  //temporary storages of sensed temperature
float temp;             //final variable storage for sensed temperature
char tempchr[5];        //character value of temperature
char humidchr[5];       //character value of humidity


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  //IOT MAC address
byte ip[] = { 192,168,190,186};      //The Arduino device IP address
byte subnet[] = { 255,255,255,0};    //The Arduino device subnet
byte gateway[] = { 192,168,190,1};   //The Arduino device gateway
IPAddress server(192,168,0,9);       //IP address of server the Arduino sends data to
EthernetClient client;               //IOT-644 serves as a Client
bool connected = false;


void setup(void)
{

    Serial.begin(9600);
    delay(1000);
    if(Ethernet.begin(mac) == 0){
     Ethernet.begin(mac,ip);
    }
}

void loop()
{
 
    if (!connected)
    {
          client.connect(server,80); //connected as client to server port 80
          connected = true;
          delay(2000);
          DHT.read11(dht_dpin);      //change the "DHT.read22" to "DHT.read11" if used DHT11 sensor
           
          count = count + 1;
          
          switch (count)
          {
          case 1: temp1 = (float)DHT.temperature;
                  hum1 = (float)DHT.humidity;
          case 2: temp2 = (float)DHT.temperature;
                  hum2 = (float)DHT.humidity;
          case 3: temp3 = (float)DHT.temperature;
                  hum3 = (float)DHT.humidity;
          case 4: temp4 = (float)DHT.temperature;
                  hum4 = (float)DHT.humidity;
          case 5: temp5 = (float)DHT.temperature;
                  hum5 = (float)DHT.humidity;
          }
          delay(2000);
         
         if( count == 5){
           float thum = hum1 + hum2 + hum3 + hum4 + hum5;
           float ttemp = temp1 + temp2 + temp3 + temp4 + temp5;
           temp = ttemp / 5;     //calibration
           humid = thum / 5;     //calibration
           //temp = temp - 0.9;    //calibration
           humid = humid + 11.2;  //calibration
           dtostrf(temp,4,2,tempchr);    //convert float to character
           delay(4000);
           String gotcha = "GET /ARDUINOPHP/sensor.php?ip=192.168.190.186&temp=";
           gotcha += tempchr;
           gotcha += "&humid=";
           dtostrf(humid,4,2,humidchr);  //convert float to character
           gotcha += humidchr;
           gotcha += " HTTP/1.1";
           client.println(gotcha);
           client.println("Host: operator");
           client.println("Connection: close");
           client.println();
           delay(1000);
           count = 0;
           temp1 = 0;
           temp2 = 0;
           temp3 = 0;
           temp4 = 0;
           
           temp5 = 0;
           hum1 = 0;
           hum2 = 0;
           hum3 = 0;
           hum4 = 0;
           hum5 = 0;
           temp = 0;
           humid = 0;
          }
    }
    else
    {
        delay(1000);
        while (client.connected() && client.available())
        {
            char c = client.read();
            Serial.print(c);
        }
        Serial.println();
        client.stop();
        connected = false;
    }
    delay(3000);
 }
