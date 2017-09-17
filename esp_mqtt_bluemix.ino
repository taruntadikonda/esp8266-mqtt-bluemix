#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char* ssid = "**********";   //your wifi ssid
const char* password = "*******";  //wifi password
#define mqtt_server  "mqtt.dioty.co"
WiFiClient espClient;
PubSubClient client(espClient);
int count,value;
char hello[50];
void setup_wifi()
{

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
int index()
{
  count=count+1;
  return count;
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
//  char* a=topic;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  } 
  Serial.println();
  
}


void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
     client.connect("clientId","mailid","password"); //enter your email id and password
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("/mailid/helloworld",hello);

      // ... and resubscribe
      //client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {

  if (!client.connected())
  {
    reconnect();
    
  }
  index();
  
client.loop();
snprintf(hello,50,"Helloworld=%d",count);
client.setCallback(callback);
client.publish("/mailid/helloworld",hello);
delay(1000);

}
