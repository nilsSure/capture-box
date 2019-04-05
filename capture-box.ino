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
int redCapture = 0;
int blueCapture = 0;

int MAX_CAP = 100;
int MIN_CAP = 0;
// INCREMENT & DECREMENT
int INC = 1;

const byte rows = 4;
const byte cols = 4;

char keys[rows][cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[rows] = {45,43,41,39};
byte colPins[cols] = {37,35,33,31};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

// LCD
LiquidCrystal_I2C lcd(0x3F,16,2); // set the LCD address to 0x3F for a 16 chars and 2 line display

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

void setup()
{
  // INPUT from buttons
  pinMode(redButton, INPUT);
  pinMode(blueButton, INPUT);
  
  // OUTPUT to LED
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  
  // OUTPUT to Speaker
  pinMode(buzzerPin, OUTPUT);
  
  // INIT the LCD
  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 

  lcd.setCursor(5,0);
  lcd.print("READY!");
}

void loop()
{
  // READ the INPUT from the buttons 
  redButtonValue = digitalRead(redButton);
  blueButtonValue = digitalRead(blueButton);
  
  
  if (redButtonValue != 0) {
    // Red button is being pressed
    
    redLight();
    sound();
    
    // If the BLUE capture is greater than MIN_CAP, decrease it as the RED button was pressed.
    if (blueCapture > MIN_CAP) {
      blueCapture -= INC; 
    }
    
    // If the BLUE capture is MIN_CAP and RED capture is not MAX_CAP yet increase RED capture
    if (blueCapture == MIN_CAP && redCapture < MAX_CAP) {
      redCapture += INC;
    }
  }
  else if (blueButtonValue != 0) {
    // Blue button is being pressed
    
    blueLight();
    sound();
    
    // If the RED capture is greater than MIN_CAP, decrease it as the BLUE button was pressed.
    if (redCapture > MIN_CAP) {
      redCapture -= INC; 
    }
    
    // If the RED capture is MIN_CAP and BLUE capture is not MAX_CAP yet increase BLUE capture
    if (redCapture == MIN_CAP && blueCapture < MAX_CAP) {
      blueCapture += INC; 
    }
  } else {
    // No buttons are being pressed
    noLight();
    noSound();
    
    // Decrease the RED capture
    if (redCapture > MIN_CAP && redCapture < MAX_CAP) {
      redCapture -= INC; 
    }
    
    // Decrease the BLUE capture
    if (blueCapture > MIN_CAP && blueCapture < MAX_CAP) {
      blueCapture -= INC; 
    }
  }
  
  // If RED capture is at MAX_CAP increase RED score
  if (redCapture == MAX_CAP) {
    redLight();
    redScore += INC; 
  }
  
  // If BLUE capture is at MAX_CAP increase BLUE score
  if (blueCapture == MAX_CAP) {
    blueLight();
    blueScore += INC; 
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
  delay(10);
}

void sound() {
    //Play first section
  firstSection();
 
  //Play second section
  secondSection();
 
  //Variant 1
  beep(f, 250);  
  beep(gS, 500);  
  beep(f, 350);  
  beep(a, 125);
  beep(cH, 500);
  beep(a, 375);  
  beep(cH, 125);
  beep(eH, 650);
 
  delay(500);
 
  //Repeat second section
  secondSection();
 
  //Variant 2
  beep(f, 250);  
  beep(gS, 500);  
  beep(f, 375);  
  beep(cH, 125);
  beep(a, 500);  
  beep(f, 375);  
  beep(cH, 125);
  beep(a, 650);  
 
  delay(650);
}

void noSound() {
  analogWrite(buzzerPin, 0); 
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

void noLight() {
 digitalWrite(greenLed, LOW);
 digitalWrite(blueLed, LOW);
 digitalWrite(redLed, LOW);
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

void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(buzzerPin, note, duration);
 
  //Play different LED depending on value of 'counter'
  if(counter % 2 == 0)
  {
    digitalWrite(redLed, HIGH);
    delay(duration);
    digitalWrite(redLed, LOW);
  }else
  {
    digitalWrite(blueLed, HIGH);
    delay(duration);
    digitalWrite(blueLed, LOW);
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
 
void secondSection()
{
  beep(aH, 500);
  beep(a, 300);
  beep(a, 150);
  beep(aH, 500);
  beep(gSH, 325);
  beep(gH, 175);
  beep(fSH, 125);
  beep(fH, 125);    
  beep(fSH, 250);
 
  delay(325);
 
  beep(aS, 250);
  beep(dSH, 500);
  beep(dH, 325);  
  beep(cSH, 175);  
  beep(cH, 125);  
  beep(b, 125);  
  beep(cH, 250);  
 
  delay(350);
}