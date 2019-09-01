#include <Servo.h>

#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>
#include "OTA_PASSWORD.h"
#include <PubSubClient.h>
#include <PubSubClient.h>


const char* ssid = "revspace-pub-2.4ghz";
const char* mqtt_server = "mosquitto.space.revspace.nl";

Servo myservo;
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
	delay(10);
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	WiFi.begin(ssid, "");

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println();
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());

	ArduinoOTA.setHostname("maneki-neko");
	ArduinoOTA.setPassword(OTA_PASSWORD);

	ArduinoOTA.onStart([]() {
		Serial.println("Start");
	});

	ArduinoOTA.onEnd([]() {
		Serial.println("\nEnd");
		ESP.restart();
	});

	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
	});

	ArduinoOTA.onError([](ota_error_t error) {
		Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
		else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
		else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
		else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
		else if (error == OTA_END_ERROR) Serial.println("End Failed");
	});

	ArduinoOTA.begin();
}

void callback(char* topic, byte* payload, unsigned int length) {
	char buf[50] = "";

	for (unsigned int i = 0; i < length; i++) {
		buf[i] = payload[i];
	}

	String message(buf);
	Serial.println("Message arrived: " + message);

	if (message == "open") {
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
			client.subscribe("revspace/state");
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
	myservo.detach();

	Serial.begin(115200);
	setup_wifi();

	client.setServer(mqtt_server, 1883);
	client.setCallback(callback);
}

void wink() {
	Serial.println("Wink!");
	myservo.attach(D3);
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
	myservo.detach();
}

void loop() {
	if (!client.connected()) {
		reconnect();
	}

	ArduinoOTA.handle();
	client.loop();
}
