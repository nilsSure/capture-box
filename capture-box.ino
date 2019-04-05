#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// Button pins
int redButton = 4;
int blueButton = 2;

// LED pins
int greenLed = 48;
int blueLed = 50;
int redLed = 52;

// Speaker pin
int buzzerPin = 12;

// Button values (whether or not they are pressed)
int redButtonValue = 0;
int blueButtonValue = 0;

// Team scores
int redScore = 0;
int blueScore = 0;

// Team captures
static int redCapture = 0;
static int blueCapture = 0;

int MAX_CAP = 100;
int MIN_CAP = 0;
// INCREMENT & DECRREMENT
int CAP = 2;
int SCORE = 1;

//const byte rows = 4;
//const byte cols = 4;
//
//char keys[rows][cols] = {
//  {'1','2','3','A'},
//  {'4','5','6','B'},
//  {'7','8','9','C'},
//  {'*','0','#','D'}
//};

//byte rowPins[rows] = {45,43,41,39};
//byte colPins[cols] = {37,35,33,31};

//sound constants
const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;

int counter = 0;

//Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

// LCD
LiquidCrystal_I2C lcd(0x3F,16,2); // set the LCD address to 0x3F for a 16 chars and 2 line display



void setup() {
  Serial.begin(9600);
    // INPUT from buttons
  pinMode(redButton, INPUT_PULLUP);
  pinMode(blueButton, INPUT_PULLUP);
  
  // OUTPUT to LED
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  
  // OUTPUT to Speaker
//  pinMode(buzzerPin, OUTPUT);
  
  // INIT the LCD
  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 

  startSequence();
}

void loop()
{
  // READ the INPUT from the buttons 
  redButtonValue = digitalRead(redButton);
  blueButtonValue = digitalRead(blueButton);
  
  
  if (redButtonValue != LOW) {
    // Red button is being pressed
    noLight();
    redLight();
    redCapSound();
    
    // If the BLUE capture is greater than MIN_CAP, DECRrease it as the RED button was pressed.
    if (blueCapture > MIN_CAP) {
      blueCapture -= CAP; 
    }
    
    // If the BLUE capture is MIN_CAP and RED capture is not MAX_CAP yet increase RED capture
    if (blueCapture == MIN_CAP && redCapture < MAX_CAP) {
      Serial.print("Value of red capture before: ");
      Serial.println(redCapture);
      redCapture += CAP;
      Serial.print("Value of red capture: ");
      Serial.println(redCapture);
    }
  } else if (blueButtonValue != LOW) {
    
    // Blue button is being pressed
    noLight();
    blueLight();
    blueCapSound();
    
    // If the RED capture is greater than MIN_CAP, DECRrease it as the BLUE button was pressed.
    if (redCapture > MIN_CAP) {
      redCapture -= CAP; 
    }
    
    // If the RED capture is MIN_CAP and BLUE capture is not MAX_CAP yet increase BLUE capture
    if (redCapture == MIN_CAP && blueCapture < MAX_CAP) {
      blueCapture += CAP;
      Serial.print("Value of blue capture: ");
      Serial.println(blueCapture);
    } 
  } else {
    // No buttons are being pressed
    noLight();
    greenLight();
    noSound();
    
    // DECRrease the RED capture
    if (redCapture > MIN_CAP && redCapture < MAX_CAP && blueCapture == MIN_CAP) {
      redCapture -= CAP; 
    }
    
    // DECRrease the BLUE capture
    if (blueCapture > MIN_CAP && blueCapture < MAX_CAP && redCapture == MIN_CAP) {
      blueCapture -= CAP; 
    }
  }
  
  // If RED capture is at MAX_CAP increase RED score
  if (redCapture == MAX_CAP) {
    noLight();
    redLight();
//    redCapSound();
    delay(1000);
    redScore += SCORE; 
  }
  
  // If BLUE capture is at MAX_CAP increase BLUE score
  if (blueCapture == MAX_CAP) {
    noLight();
    blueLight();
//    blueCapSound();
    delay(1000);
    blueScore += SCORE; 
  }
  
  // Display the RED score
  lcd.setCursor(0, 0);
  lcd.print("R=");
  lcd.setCursor(2, 0);
  lcd.print(redScore);
  
  // Display the space between the SCORE and CAPTURE
  int rScoreChars = getNumberLength(redScore);
  int rCapChars = getNumberLength(redCapture);
  // Start from 'X=' + the written score
  // End after 16 chars (total) - capture - '%'
  for (int i = 2 + rScoreChars; i < 16 - rCapChars - 1; i++) {
    lcd.print("."); 
  }
  
  // Display the RED capture
  lcd.print(redCapture);
  lcd.print("%");
 
  
  // Display the BLUE score
  lcd.setCursor(0, 1);
  lcd.print("B=");
  lcd.setCursor(2, 1);
  lcd.print(blueScore);
  
  // Display the space between the SCORE and CAPTURE
  int bScoreChars = getNumberLength(blueScore);
  int bCapChars = getNumberLength(blueCapture);
  // Start from 'X=' + the written score
  // End after 16 chars (total) - capture - '%'
  for (int i = 2 + bScoreChars; i < 16 - bCapChars - 1; i++) {
    lcd.print("."); 
  }
  
  // Display the BLUE capture
  lcd.print(blueCapture);
  lcd.print("%");
 
  // Delay a bit
  delay(40);
}

void redLight() {
  digitalWrite(redLed, HIGH);
}

void blueLight() {
  digitalWrite(blueLed, HIGH);
}

void greenLight() {
  digitalWrite(greenLed, HIGH);
}

void startSequence() {
  sound();
  digitalWrite(redLed, HIGH);
  delay(500);
  digitalWrite(redLed, LOW);
  delay(500);
  digitalWrite(greenLed, HIGH);
  delay(500);
  digitalWrite(greenLed, LOW);
  delay(500);
  digitalWrite(blueLed, HIGH);
  delay(500);
  digitalWrite(blueLed, LOW);
  delay(500);
  lcd.setCursor(5,0);
  lcd.print("READY!");
  delay(100);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("R=0...........0%");
  lcd.setCursor(0,1);
  lcd.print("B=0...........0%");
}

void noLight() {
 digitalWrite(greenLed, LOW);
 digitalWrite(blueLed, LOW);
 digitalWrite(redLed, LOW);
}

void noSound() {
  analogWrite(buzzerPin, LOW); 
}

int getNumberLength(int x) {
    if(x>=1000000000) return 10;
    if(x>=100000000) return 9;
    if(x>=10000000) return 8;
    if(x>=1000000) return 7;
    if(x>=100000) return 6;
    if(x>=10000) return 5;
    if(x>=1000) return 4;
    if(x>=100) return 3;
    if(x>=10) return 2;
    return 1;
}

void sound() {
  //Play first section
  firstSection();
}

void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(buzzerPin, note, duration);
 
  //Play different LED depending on value of 'counter'
  if(counter % 2 == 0)
  {
//    digitalWrite(redLed, HIGH);
    delay(duration);
//    digitalWrite(redLed, LOW);
  }else
  {
//    digitalWrite(blueLed, HIGH);
    delay(duration);
//    digitalWrite(blueLed, LOW);
  }

  if (redButtonValue != 0) {
    redLight();
  }

  if (blueButtonValue != 0) {
    blueLight();
  }
 
  //Stop tone on buzzerPin
  noTone(buzzerPin);
 
  delay(50);
 
  //Increment counter
  counter++;
}
 
void firstSection()
{
  beep(a, 500);
  beep(a, 500);    
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);  
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
 
  delay(500);
 
  beep(eH, 500);
  beep(eH, 500);
  beep(eH, 500);  
  beep(fH, 350);
  beep(cH, 150);
  beep(gS, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
 
  delay(500);
}

void redCapSound()
{
  beep(cH, 150);  
  beep(a, 500);
  beep(f, 350);
}

void blueCapSound()
{
  beep(f, 350);
  beep(a, 500);
  beep(cH, 150); 
}
