#include <WiFi.h>
#include <WiFiUdp.h>

const int ESP_PORT = 1234;

WiFiUDP udp;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi network
  WiFi.begin("", "");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi network");

  // Print IP address to serial console
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start listening for incoming UDP packets on ESP_PORT
  udp.begin(ESP_PORT);
  Serial.print("Listening on port ");
  Serial.println(ESP_PORT);
}

void loop() {
  // Wait for a UDP packet to arrive
  int packetSize = udp.parsePacket();
  if (packetSize) {
    Serial.println("Received UDP packet");

    // Read the incoming packet
    char buffer[packetSize];
    udp.read(buffer, packetSize);
    String command = String(buffer);

    // Send a response back to the sender
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.write((const uint8_t*)"Hello Python!", 13);
    udp.endPacket();

    Serial.println("Sent UDP response");
  }
}
