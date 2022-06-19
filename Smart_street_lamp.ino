//BEFORE UPLOADING, MAKE SURE THAT YOUR DATABASE ".read" AND ".write" RULES ARE SET TO 'true'.

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>  //https://github.com/Rupakpoddar/ESP8266Firebase

#define WIFI_SSID "mardes home"
#define WIFI_PASSWORD "Prateek123@@"


#define projectID "warrior-bulls-default-rtdb" //Your Firebase Project ID; can be found in project settings.
#define FIREBASE_HOST "warrior-bulls-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "2JNTy9dXCRiq3Yyf78K9DqOhKZMzwnLeNIgZF5G3"

#define buzzer 13

#define device1 5 //D1
#define device2 4 //D2
#define device3 0 //D3
#define ldr3 14 //D5

#define ldr1 12 //D6
#define ldr2 15 //D8

bool inita = true;
bool isActive = false;

#define ldr2Pwr 16 //D0

int device_list[4] = {
  device1,
  device2,
  device3
};

void setup() {
  pinMode(device1, OUTPUT);
  pinMode(device2, OUTPUT);
  pinMode(device3, OUTPUT);
  pinMode(buzzer, OUTPUT);

  pinMode(ldr3, INPUT);
  pinMode(ldr2, INPUT);
  pinMode(ldr1, INPUT);

  pinMode(ldr2Pwr, OUTPUT);
  digitalWrite(ldr2Pwr, HIGH);

  pinMode(LED_BUILTIN, OUTPUT); //16
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);

 WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void work() {
   char result[50];
   float num = millis();
   sprintf(result, "%f", num);
  int ldrStatus = digitalRead(ldr1);
  int ldrStatus1 = digitalRead(ldr2);
  int ldrStatus2 = digitalRead(ldr3);

  if ((ldrStatus == 1 && ldrStatus1 == 1) ||
    (ldrStatus1 == 1 && ldrStatus2 == 1) ||
    (ldrStatus == 1 && ldrStatus2 == 1) ||
    (ldrStatus == 1 && ldrStatus1 == 1 && ldrStatus2 == 1)) {

    pinMode(device1, HIGH);
    pinMode(device2, HIGH);
    pinMode(device3, HIGH);

    Firebase.pushString("Times",result);
    Serial.println(ldrStatus);
    Serial.println(ldrStatus1);
    Serial.println(ldrStatus2);
    if (inita) {
      digitalWrite(buzzer, HIGH);
      inita = false;
    } else digitalWrite(buzzer, LOW);
    Serial.println("");

  } else {
    pinMode(device1, LOW);
    pinMode(device2, LOW);
    pinMode(device3, LOW);
    if (!inita) {
      digitalWrite(buzzer, HIGH);
      inita = true;
    } else {
      digitalWrite(buzzer, LOW);

    }
    Serial.println("Turned on ");

  }

}

void loop() {

  isActive = Firebase.getBool("master");

  Serial.println(isActive);

  if (!isActive) {
//    digitalWrite(stopLed, LOW);
    work();
  } else {
    //digitalWrite(buzzer, LOW);
    //digitalWrite(relay, HIGH);
    //digitalWrite(11, HIGH);
  }

    delay(3000);


}
