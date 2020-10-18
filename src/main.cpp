/*
#include <WiFi.h>
#include <Arduino.h>

int masterLines = 10; //Change here to the number of lines of your Master Layer
int slaveLines = 7; //Change here to the number of lines of your Slave Layer
int matrixMaster[] = {32, 33, 25, 26, 27, 14, 12, 13, 15, 2}; //Put here the pins you connected the lines of your Master Layer 
int matrixSlave[] = {23, 22, 21, 19, 18, 5, 4}; //Put here the pins you connected the lines of your Slave Layer

int matrixScore[7][10] = {
  {27,36,15,60,3,54,12,39,18,30},
  {18,24,10,40,2,36,8,26,12,20},
  {9,12,5,20,1,18,4,13,6,10},
  {50,25,0,0,0,0,0,0,0,0},
  {14,11,8,16,7,19,3,17,2,15},
  {28,22,16,32,14,38,6,34,4,30},
  {42,33,24,48,21,57,9,51,6,45}
};

void setup() {     
    Serial.begin(9600);     
    for(int i = 0; i < slaveLines; i++){         
        pinMode(matrixSlave[i], INPUT_PULLUP);     
    }
   for(int i = 0; i < masterLines; i++){         
       pinMode(matrixMaster[i], OUTPUT);         
       digitalWrite(matrixMaster[i], HIGH);     
   } 
}

void loop() {     
    for(int i = 0; i < masterLines; i++){         
        digitalWrite(matrixMaster[i], LOW);         
        for(int j = 0; j < slaveLines; j++){             
            if(digitalRead(matrixSlave[j]) == LOW){  
                  Serial.print(matrixScore[j][i]);
                  Serial.print(" => ");
                  Serial.print(i);
                  Serial.print(",");
                  Serial.println(j);
                  delay(500);        
                  break;             
            }         
        }         
        digitalWrite(matrixMaster[i], HIGH);     
    } 
}
*/


/*
#################################################
#################################################
#################################################
#################################################
#################################################
*/


#include <WiFi.h>

int getShot();
void handleGame();
void displayHome(WiFiClient client);
void displayPlayers(WiFiClient client);
void displayConfirmPlayers(WiFiClient client);
void displayScores(WiFiClient client);
void getPlayerNames(String args);

// network credentials
const char* ssid     = "ESP32-Flechettes";
const char* password = "azerty12345";

// game variables
String mode = "";
String player1 = "";
String player2 = "";
String player3 = "";
String player4 = "";

int p1 = 0;
int p2 = 0;
int p3 = 0;
int p4 = 0;

int currentShot = 0;

// Expose server on port X.X.X.X:80
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

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n')
        {
          // WEB CONTROLLER WHICH REDIRECTS BY URL
          if (currentLine.length() == 0)
          {
            if(header.indexOf("GET /scores") >= 0){
              displayScores(client);
            } else
            if (header.indexOf("GET /confirmPlayers?") >= 0)
            {
              // getting first line of HTTP header
              String args = header.substring(0, header.indexOf("\n"));
              // splitting args like ?arg1=value1&arg2=value2&...
              // then getting values
              getPlayerNames(args);
              displayConfirmPlayers(client);
            } else
            if (header.indexOf("GET /301") >= 0)
            {
              mode = "301";
              player1 = "";
              player2 = "";
              player3 = "";
              player4 = "";
              p1 = 301;
              p2 = 301;
              p3 = 301;
              p4 = 301;
              displayPlayers(client);
            } else
            if (header.indexOf("GET /501") >= 0)
            {
              mode = "501";
              player1 = "";
              player2 = "";
              player3 = "";
              player4 = "";
              p1 = 501;
              p2 = 501;
              p3 = 501;
              p4 = 501;
              displayPlayers(client);
            } else
            if (header.indexOf("GET /701") >= 0)
            {
              mode = "701";
              player1 = "";
              player2 = "";
              player3 = "";
              player4 = "";
              p1 = 701;
              p2 = 701;
              p3 = 701;
              p4 = 701;
              displayPlayers(client);
            } else
            {
              mode = "";
              displayHome(client);
            }
            break;
          } else
          { // if you got a newline, then clear currentLine
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

void getPlayerNames(String args){
  String name = "";
  int begin = 0;
  int end = 0;
  for (size_t i=0; i < args.length(); i++){
    if(args.charAt(i) == '='){
      begin = i;
    }
    if(args.charAt(i) == '&' || (begin != 0 && args.charAt(i) == ' ')){
      end = i;
      name = args.substring(begin+1, end);
      if(player1 == ""){
        player1 = name;
      } else
      if(player2 == ""){
        player2 = name;
      } else
      if(player3 == ""){
        player3 = name;
      } else  
      if(player4 == ""){
        player4 = name;
      }
    }
  }
}

void displayHome(WiFiClient client){
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
            
            client.println("<body><a style=\"color:#4CAF50;text-decoration:none;\" href=\"/\"><h1>ESP32 My Dart Game</h1></a>");
            
            client.println("<h2>Choisissez votre mode de jeu :</h2>");
            client.println("<p><a href=\"/301\"><button class=\"button\">Jouer au 301</button></a></p>");
            client.println("<p><a href=\"/501\"><button class=\"button\">Jouer au 501</button></a></p>");
            client.println("<p><a href=\"/701\"><button class=\"button\">Jouer au 701</button></a></p>");
            
            // The HTTP response ends with another blank line
            client.println();
}

void displayPlayers(WiFiClient client){
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
            client.println("<body><a style=\"color:#4CAF50;text-decoration:none;\" href=\"/\"><h1>ESP32 My Dart Game</h1></a>");
            client.println("<h2>Bienvenue au mode " + mode + " </h2>");
            client.println("<br><p>Ajoutez vos joueurs :</p>");
            client.println("<form action=\"/confirmPlayers\" method=\"GET\">");
            client.println("<p>Joueur 1");
            client.println("<input type=\"text\" name=\"player1\"><br>");
            client.println("<p>Joueur 2");
            client.println("<input type=\"text\" name=\"player2\"><br>");
            client.println("<p>Joueur 3");
            client.println("<input type=\"text\" name=\"player3\"><br>");
            client.println("<p>Joueur 4");
            client.println("<input type=\"text\" name=\"player4\"><br>");
            client.println("<input type=\"submit\"/>");
            client.println("</from>");
            client.println();
}

void displayScores(WiFiClient client){
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
            client.println("<meta http-equiv=\"refresh\" content=\"1\">");
            // CSS
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // body
            client.println("<body><a style=\"color:#4CAF50;text-decoration:none;\" href=\"/\"><h1>ESP32 My Dart Game</h1></a>");
            client.println("<p>Vous pouvez jouer  au <b>" + mode + "</b> !</p>");
            client.println("<p style=\"color:red\">Suivez les scores sur l'écran du board !</p>");
            if(player1 != "")
              client.println("<br><p>" + player1 + ": " + p1 + " points");
            if(player2 != "")
              client.println("<br><p>" + player2 + ": " + p2 + " points");
            if(player3 != "")
              client.println("<br><p>" + player3 + ": " + p3 + " points");
            if(player4 != "")
              client.println("<br><p>" + player4 + ": " + p4 + " points");

            client.println();
}

void displayConfirmPlayers(WiFiClient client){
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
            client.println("<body><a style=\"color:#4CAF50;text-decoration:none;\" href=\"/\"><h1>ESP32 My Dart Game</h1></a>");
            client.println("<p>Vous pouvez jouer  au <b>" + mode + "</b> !</p>");
            if(player1 != "")
              client.println("<br><p>" + player1 + ": " + p1 + " points");
            if(player2 != "")
              client.println("<br><p>" + player2 + ": " + p2 + " points");
            if(player3 != "")
              client.println("<br><p>" + player3 + ": " + p3 + " points");
            if(player4 != "")
              client.println("<br><p>" + player4 + ": " + p4 + " points");
            client.println("<p><a href=\"/scores\"><button class=\"button\">Confirmer</button></a></p>");

            client.println();
}

void handleGame(){
  while(p1 != 0 && p2 != 0 && p3 != 0 && p4 != 0){
    currentShot = getShot();
    if(currentShot !=0 ){
      p1 = currentShot;
    }

    currentShot = getShot();
    if(currentShot !=0 ){
      p2 = currentShot;
    }

    currentShot = getShot();
    if(currentShot !=0 ){
      p3 = currentShot;
    }

    currentShot = getShot();
    if(currentShot !=0 ){
      p4 = currentShot;
    }
  }
}

int getShot(){
  // return current shot point ortherwise return 0
  return 0;
}