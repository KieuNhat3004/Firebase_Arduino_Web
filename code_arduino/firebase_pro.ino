#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2); 

// Set these to run example.
#define FIREBASE_HOST "iotsmart-878c9-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "k4H6EGIY3JmxY24Rx8H40hsw09lCdQcJzgo2JQ7Q"
#define WIFI_SSID "LyBerry"
#define WIFI_PASSWORD "KPK@-.25072018"

//#define SCREEN_WIDTH 128 // OLED display width, in pixels
//#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Servo myservo;  // create servo object to control a servo


int ldrValue=0;
int pos=0;
int lastPos=0;
String ldr, distance, p;
const int red= D5;
const int green= D7;
const int trigPin = D3;
const int echoPin = D6;

void setup() {
  Serial.begin(115200);
  
  pinMode(A0, INPUT); 
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(trigPin, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);  // Sets the echoPin as an Input

  myservo.attach(D8);  // attaches the servo on D8 to the servo object

  //OLED SETUP
//  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); /* Initialize display with address 0x3C */
//  display.clearDisplay();  /* Clear display */
//  display.setTextSize(2);  /* Select font size of text. Increases with size of argument. */
//  display.setTextColor(WHITE);  /* Color of text*/
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  
  // connect to wifi.
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

void loop() {

  digitalWrite(red, Firebase.getInt("red-led"));
  digitalWrite(green, Firebase.getInt("green-led"));
  pos= Firebase.getInt("servo");
  if(pos != lastPos){
    Serial.println("SERVO POSITION: "+pos);
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    lastPos=pos;    
  }
  // set ultrasonic values
  int first = triggerRadar(trigPin,echoPin);
  Firebase.setInt("ultrasonic/first", first);
  delay(100);
  int second = triggerRadar(trigPin,echoPin);
  Firebase.setInt("ultrasonic/second", second);
  delay(100);
  int third = triggerRadar(trigPin,echoPin);
  Firebase.setInt("ultrasonic/third", third);
  delay(100);
  int fourth = triggerRadar(trigPin,echoPin);
  Firebase.setInt("ultrasonic/fourth", fourth);
  delay(100);
  int fifth = triggerRadar(trigPin,echoPin);
  Firebase.setInt("ultrasonic/fifth", fifth);
  delay(100);
  ldrValue = analogRead(A0);   // read the ldr input on analog pin 0
  Firebase.setInt("ldr", ldrValue);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting failed with error:");
      Serial.println(Firebase.error());  
      return;
  }
  ldr= String(ldrValue);
  distance= String(fifth);
  p = String(pos);
  Serial.println("LDR: "+ldr+" || Dist: "+distance+" || Pos: "+p);
  updateOLED(ldr,distance,p);/* Update OLED Every second and display */
}
void updateOLED(String ldr, String d, String pos){
  lcd.setCursor(0,0);
  lcd.print("LDR:"+ldr);
  
  lcd.setCursor(8, 0);
  lcd.print("DIST:"+d);

  lcd.setCursor(4, 1);
  lcd.print("POS: "+pos);
  lcd.display();

  if(d.toInt() < 100) 
  {
    lcd.setCursor(15, 0);
    lcd.print(" ");
  }

  if(pos.toInt() < 100) 
  {
    lcd.setCursor(11, 1);
    lcd.print(" ");
  }
}
int triggerRadar(int trigPin, int echoPin){
  String dist;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int d = duration*0.0343/2;
  return d;
}
