#include <WiFi.h>
#include <WebServer.h>
#include <Servo.h>

const char* ssid = "Internet_Name"; //Needs to be changed to the WiFi network you are using
const char* password = "Internet_Password"; //Needs to be changed to WiFi network you are using

WebServer server(80);

Servo servo1; // create servo object to control a servo

// GPIO the servo is attached to
static const int servoPin = 23;
static const int rotOutput = 22;
static const int rotInput = 21;

//Basic HTML written to display webpage to allow for door opening command
void handleRoot() {
  server.send(
  200, 
  "text/html", 
  "<!DOCTYPE html><html><head>
  <title>Enter Password to Open Door
  </title></head><body><h2>Enter Password to Open Door</h2><label 
  for='password'>Password:</label><input type='password' id='password' name='password'><button onclick='rotateServo()'>
  Submit</button><script>function rotateServo() {var password = document.getElementById('password').value;
  if (password === 'jfio') {var xhr = new XMLHttpRequest();xhr.open('GET', '/rotate_servo', true);xhr.send();alert('Opening Door! Best, -J');} 
  else {alert('Incorrect password!');}}</script></body></html>");
}

//Servo logic that is called when the correct password is entered
void handleRotateServo() {
  Serial.println("Opening Door"); //Print to debug port 
  servo1.write(90); //servo angles to depress button
  delay(3000); //give a 3 second delay to get in
  servo1.write(140); //servo angles to unpress buttom
  server.send(200, "text/plain", "Servo rotated"); //send a message back to server to give user an update
}

//Setup of ESP-32 IO pins and baud rate for communication to console
void setup() {
  Serial.begin(115200);
  servo1.attach(servoPin);
  pinMode(rotOutput, OUTPUT);
  pinMode(rotInput, INPUT);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); //input location specific WiFi params
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  server.on("/", handleRoot);
  server.on("/rotate_servo", handleRotateServo);

  server.begin();
  Serial.println("HTTP server started");
  servo1.write(140);
}

//ESP-32 sits in this loop forever to host the webpage and wait for clients to request the door to open
void loop() {
  //constantly check for updates from client
  server.handleClient();
  //functionality to detect when system offline not currently added
}

