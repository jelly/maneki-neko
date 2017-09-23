#include <Servo.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>


const char* ssid = "revspace-pub-2.4ghz";
const char* mqtt_server = "mosquitto.space.revspace.nl";

Servo myservo;
WiFiClient espClient;
PubSubClient client(espClient);

#define TIMEOUT_MS 60000

unsigned long last_message = 0;

void setup_wifi() {
	delay(10);
	// We start by connecting to a WiFi network
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	WiFi.begin(ssid, "");

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
	unsigned long now = millis();
	Serial.print("Message arrived [");
	for (int i = 0; i < length; i++) {
		Serial.print((char)payload[i]);
	}
	Serial.print("] now: ");
	Serial.print(now);
	Serial.print(" last_message: ");
	Serial.print(last_message);
	Serial.println();

	if (last_message == 0) {
		Serial.println("first message");
		wink();
		return;
	}

	if (now - TIMEOUT_MS > last_message) {
		Serial.println("new message");
		wink();
	}
}

void reconnect() {
	// Loop until we're reconnected
	while (!client.connected()) {
		Serial.print("Attempting MQTT connection...");
		// Attempt to connect
		if (client.connect("Winkcat")) {
			Serial.println("connected");
			// Once connected, publish an announcement...
			// ... and resubscribe
			client.subscribe("revspace/winkcat");
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
	myservo.attach(D3);
	myservo.write(90);

	Serial.begin(115200);
	setup_wifi();

	client.setServer(mqtt_server, 1883);
	client.setCallback(callback);
}

void wink() {
	Serial.println("Wink!");
	// Stop position
	myservo.write(90);
	delay(500);
	// Wink position
	myservo.write(50);
	delay(500);
	myservo.write(90);
	delay(10000);

	// Slowly stop the cat
	for (int pos = 90; pos > 50; pos--) {
		myservo.write(pos);
		delay(15);
	}
	delay(1000);
	Serial.println("Stopped winking!");
	last_message = millis();
}

void loop() {
	if (!client.connected()) {
		reconnect();
	}
	client.loop();
}
