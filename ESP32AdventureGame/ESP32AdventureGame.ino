/*
 * Project: ESP32 Adventure Game
 * Author: annoyedmilk
 *
 * Description:
 * This project creates a simple text-based RPG game hosted on an ESP32. Players can connect to the ESP32 via Wi-Fi
 * and interact with the game through a web browser. The game features exploration, combat, and character progression
 * elements. It utilizes the ESP32's WebServer library to serve web pages that allow players to navigate through the game's
 * world, make choices, and view their character's status. The project demonstrates handling HTTP requests, dynamic content generation,
 * and basic game design principles on a microcontroller platform.
 * 
 * Revision History:
 * - Version 1.0: Initial release. Basic game framework with exploration and combat.
 * - Version 1.1: Added inventory and character progression systems.
 * - Version 1.2: Improved game mechanics and introduced responsive web design for mobile compatibility.
 */

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "ESP32-Adventure"; // Change to your SSID
const char* password = "adventure"; // Change to your password

WebServer server(80);

// Player State
struct Player {
  String location;
  bool hasSword;
  int health;
  int maxHealth;
  int gold;
  int experience;
} player;

// Initializes player with default values
void setupPlayer() {
  player.location = "start";
  player.hasSword = false;
  player.health = 100;
  player.maxHealth = 100;
  player.gold = 0;
  player.experience = 0;
}

String generateStatus() {
  String status = "<p>Health: " + String(player.health) + "/" + String(player.maxHealth);
  status += " | Gold: " + String(player.gold);
  status += " | Experience: " + String(player.experience) + "</p>";
  return status;
}

void sendHtmlResponse(String content) {
  String page = "<!DOCTYPE html><html><head><title>ESP32 Adventure Game</title>";
  page += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  page += "<style>body { font-family: Arial, sans-serif; text-align: center; padding: 20px; } a { display: inline-block; margin: 10px; padding: 10px; background: #007bff; color: white; text-decoration: none; border-radius: 5px; }</style>";
  page += "</head><body>";
  page += "<h2>ESP32 Adventure Game</h2>";
  page += generateStatus();
  page += content;
  page += "</body></html>";
  server.send(200, "text/html", page);
}

void handleRoot() {
  String content = "<p>Welcome to the ESP32 Adventure Game! What would you like to do?</p>";
  content += "<a href=\"/explore\">Explore</a><br>";
  content += "<a href=\"/rest\">Rest</a><br>";
  sendHtmlResponse(content);
}

void handleExplore() {
  // Example exploration logic
  player.location = "forest";
  String content = "<p>You venture into the forest and find a wild creature!</p>";
  content += "<a href=\"/fight\">Fight</a><br>";
  content += "<a href=\"/flee\">Flee</a><br>";
  sendHtmlResponse(content);
}

void handleFight() {
  // Example fight logic
  if (player.hasSword) {
    player.experience += 10;
    player.gold += 5;
    String content = "<p>You bravely fight the creature and win! You find some gold.</p>";
    content += "<a href=\"/\">Return</a><br>";
    sendHtmlResponse(content);
  } else {
    player.health -= 10;
    String content = "<p>You attempt to fight the creature but take damage. It might be wise to find a weapon.</p>";
    content += "<a href=\"/\">Return</a><br>";
    sendHtmlResponse(content);
  }
}

void handleRest() {
  // Example rest logic
  player.health = player.maxHealth;
  String content = "<p>You rest and recover your health.</p>";
  content += "<a href=\"/\">Return</a><br>";
  sendHtmlResponse(content);
}

void handleFlee() {
  // Example flee logic
  String content = "<p>You manage to flee back to safety.</p>";
  content += "<a href=\"/\">Return</a><br>";
  sendHtmlResponse(content);
}

void handleNotFound() {
  sendHtmlResponse("<p>Page not found. <a href=\"/\">Return</a></p>");
}

void setup() {
  Serial.begin(115200);
  setupPlayer(); // Initialize player
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.println("Access point started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/explore", handleExplore);
  server.on("/fight", handleFight);
  server.on("/rest", handleRest);
  server.on("/flee", handleFlee);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}