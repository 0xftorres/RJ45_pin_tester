#include <LiquidCrystal_I2C.h>

// Connector 1
#define PinC1_1 1
#define PinC1_2 0
#define PinC1_3 2
#define PinC1_4 3
#define PinC1_5 4
#define PinC1_6 5
#define PinC1_7 6
#define PinC1_8 7

// Connector 2
#define PinC2_1 8
#define PinC2_2 9
#define PinC2_3 10
#define PinC2_4 11
#define PinC2_5 12
#define PinC2_6 13
#define PinC2_7 14
#define PinC2_8 15

#define HIGH_PullUp LOW
#define LOW_PullUp HIGH

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// Pin array for each connector
// Output
const int CO[] = {PinC1_1, PinC1_2, PinC1_3, PinC1_4, PinC1_5, PinC1_6, PinC1_7, PinC1_8};
// Input
const int CI[] = {PinC2_1, PinC2_2, PinC2_3, PinC2_4, PinC2_5, PinC2_6, PinC2_7, PinC2_8};

// Connections matrix for StraightThrough
const int StraightThrough[][2] = {
  {PinC1_1, PinC2_1},
  {PinC1_2, PinC2_2},
  {PinC1_3, PinC2_3},
  {PinC1_4, PinC2_4},
  {PinC1_5, PinC2_5},
  {PinC1_6, PinC2_6},
  {PinC1_7, PinC2_7},
  {PinC1_8, PinC2_8}
};

// Connections matrix for CrossOver
const int CrossOver[][2] = {
  {PinC1_1, PinC2_3},
  {PinC1_2, PinC2_6},
  {PinC1_3, PinC2_1},
  {PinC1_4, PinC2_7},
  {PinC1_5, PinC2_8},
  {PinC1_6, PinC2_2},
  {PinC1_7, PinC2_4},
  {PinC1_8, PinC2_5}
};

void setBitIO(int pin, int state){
  digitalWrite(pin, state);
  delay(10);
}

void setAllLow(int con[]){
  for(int i=0; i < 8; i++){
    setBitIO(con[i], LOW_PullUp);
    delay(10);
  }
}

void setAllHigh(int con[]){
  for(int i=0; i < 8; i++){
    setBitIO(con[i], HIGH_PullUp);
    delay(10);
  }
}

void setAllInput(int con[]){
  for(int i=0; i < 8; i++){
    pinMode(con[i], INPUT_PULLUP);
    delay(10);
  }
}

void setAllOutput(int con[]){
  for(int i=0; i < 8; i++){
    pinMode(con[i], OUTPUT);
    delay(10);
  }
}

void cableNotDetectedScreen(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Cable not connected!");

  lcd.setCursor(1,3);
  lcd.print("Made by @0xftorres");

  for (int i=0; i < 20; i++) {
    lcd.setCursor(i,1);
    if(i%2==0) {
      lcd.print("-");
    } else{
      lcd.print("_");
    }
    delay(200);
  }
}

bool detectCable(){
  setAllHigh(CO);
  for(int i=0; i < 8; i++){
    if (digitalRead(CI[i]) == HIGH_PullUp){
      return true;
    }
    delay(10);
  }
  return false;
}

void comparePins(int S[][2]){
  // set every output pin to low
  setAllLow(CO);

  // loop parameter
  for (int i=0; i < 8; i++){

    // set pin to 1
    setBitIO(S[i][0], HIGH_PullUp);

    if (digitalRead(S[i][1]) == HIGH_PullUp){
      // link is correct
      
      lcd.print("| ");
      delay(50);
    } else{
      // link is incorrect

      Serial.print(S[i][0]);
      Serial.print("-->");
      Serial.println(S[i][1]);
      lcd.print("X ");
      delay(50);
    }

    // set pin back to 0
    setBitIO(S[i][0], LOW_PullUp);
  }
}

void setup()
{
  // serial for debugging
  Serial.begin(9600);

  // lcd setup
  lcd.init();
  lcd.backlight();

  // initial state for pins
  setAllOutput(CO);
  setAllInput(CI);
}

void loop()
{
  if(detectCable()){

    lcd.clear();

    // print numbers
    lcd.setCursor(4,0);
    lcd.print("1 2 3 4 5 6 7 8");
    lcd.setCursor(4,3);
    lcd.print("1 2 3 4 5 6 7 8");

    // print labels
    lcd.setCursor(1,1);
    lcd.print("ST ");
    lcd.setCursor(1,2);
    lcd.print("CO ");

    // print straight-through
    lcd.setCursor(4,1);
    comparePins(StraightThrough);

    // print straight-through
    lcd.setCursor(4,2);
    comparePins(CrossOver);

    delay(10000);
  }
  
  else {
    setAllLow(CO);
    cableNotDetectedScreen();
  }
}
