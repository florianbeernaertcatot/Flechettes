/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <WiFi.h>

void displayHomePage(WiFiClient client);
void display301Page(WiFiClient client);
void display301GamePage(WiFiClient client);


// Replace with your network credentials
const char* ssid     = "ESP32-Flechettes";
const char* password = "123456789";
const char* mode = "";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;


void setup() {
  Serial.begin(9600);
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n')
        { // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          // WEB CONTROLLER WHICH REDIRECTS BY URL
          if (currentLine.length() == 0)
          {
            if (header.indexOf("GET /301") >= 0){
              display301Page(client);
              mode = "301";
            }
            else if (header.indexOf("GET /301/game") >= 0){
              display301GamePage(client);
              mode = "301";
            }
            else {
              displayHomePage(client);
              mode = "";
            }
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
        else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void displayHomePage(WiFiClient client){
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 My Dart Game</h1>");
            
            // Display play mode with link
            client.println("<p><h1>301 :</h1> Chaque joueur tire a tour de role avec un score de base de 301. On soustrait les scores realises aux totaux des joueurs au fur et a mesure de la partie. Le but du jeu est alors d’etre le premier a atteindre zero pour remporter la partie!</p>");
            client.println("<p><a href=\"/301\"><button class=\"button\">Jouer au 301</button></a></p>");
            
            // The HTTP response ends with another blank line
            client.println();
}

void display301Page(WiFiClient client){
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // HTML Skeleton + HEAD
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // body
            client.println("<body><h1>ESP32 My Dart Game</h1>");
            client.println("<p><h1>301 :</h1> Bienvenue au 301 </p>");
            client.println("<br><p>Ajoutez vos joueurs :</p>");
            client.println("<p>Joueur 1");
            client.println("<input type=\"text\" id=\"player1\" required minlength=\"3\" maxlength=\"8\" size=\"10\"><br>");
            client.println("<p>Joueur 2");
            client.println("<input type=\"text\" id=\"player2\" required minlength=\"3\" maxlength=\"8\" size=\"10\"><br>");
            client.println("<p>Joueur 3");
            client.println("<input type=\"text\" id=\"player3\" required minlength=\"3\" maxlength=\"8\" size=\"10\"><br>");
            client.println("<p>Joueur 4");
            client.println("<input type=\"text\" id=\"player4\" required minlength=\"3\" maxlength=\"8\" size=\"10\"><br>");
            client.println("<a href=\"/301/game\">Jouer !</a>/");
            client.println();
}

void display301GamePage(WiFiClient client){
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // HTML Skeleton + HEAD
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // body
            client.println("<body><h1>ESP32 My Dart Game</h1>");
            client.println("<p><h1>301 :</h1> Vous pouvez jouer !</p>");
            client.println("<p style=\"color:red\">Suivez les scores sur l'écran du board !</p>");
            client.println();
}