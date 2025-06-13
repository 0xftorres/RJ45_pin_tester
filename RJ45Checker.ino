
#include <LiquidCrystal_I2C.h>

// Connector 1
#define PinC1_1 1
#define PinC1_2 0
#define PinC1_3 2
#define PinC1_4 3
#define PinC1_5 4
#define PinC1_6 5
#define PinC1_7 16
#define PinC1_8 17

// Connector 2 pin 12 no funcina
#define PinC2_1 8
#define PinC2_2 9
#define PinC2_3 10
#define PinC2_4 11
#define PinC2_5 7
#define PinC2_6 13
#define PinC2_7 14
#define PinC2_8 15

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// Array de pines para c/ conector
// Output
const int CO[] = {PinC1_1, PinC1_2, PinC1_3, PinC1_4, PinC1_5, PinC1_6, PinC1_7, PinC1_8};
// Input
const int CI[] = {PinC2_1, PinC2_2, PinC2_3, PinC2_4, PinC2_5, PinC2_6, PinC2_7, PinC2_8};

// Connections matrix for StraightThrough
const int StraightThrough[][3] = {
  {PinC1_1, PinC2_1, 0},
  {PinC1_2, PinC2_2, 0},
  {PinC1_3, PinC2_3, 0},
  {PinC1_4, PinC2_4, 0},
  {PinC1_5, PinC2_5, 0},
  {PinC1_6, PinC2_6, 0},
  {PinC1_7, PinC2_7, 0},
  {PinC1_8, PinC2_8, 0}
};

// Connections matrix for CrossOver
const int CrossOver[][3] = {
  {PinC1_1, PinC2_1, 0},
  {PinC1_2, PinC2_2, 0},
  {PinC1_3, PinC2_6, 0},
  {PinC1_4, PinC2_4, 0},
  {PinC1_5, PinC2_5, 0},
  {PinC1_6, PinC2_3, 0},
  {PinC1_7, PinC2_7, 0},
  {PinC1_8, PinC2_8, 0}
};

void setBitIO(int pin, int state){
  digitalWrite(pin, state);
  delay(10);
}

void setAllLow(int con[]){
  for(int i=0; i < 8; i++){
    setBitIO(con[i], LOW);
    delay(10);
  }
}

void setAllHigh(int con[]){
  for(int i=0; i < 8; i++){
    setBitIO(con[i], HIGH);
    delay(10);
  }
}

void setAllInput(int con[]){
  for(int i=0; i < 8; i++){
    pinMode(con[i], INPUT);
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
  lcd.print("Conectar cables RJ45");

   for (int i=0; i < 20; i++) {
    lcd.setCursor(i,2);
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
    if (digitalRead(CI[i]) == HIGH){
      return true;
    }
    delay(10);
  }
  return false;
}

void comparePins(int S[][3]){
  // set every output pin to low
  setAllLow(CO);

  // loop parameter
  for (int i=0; i < 8; i++){

    // set pin to 1
    setBitIO(S[i][0], HIGH);

    if (digitalRead(S[i][1]) == HIGH){
      // link is correct
      Serial.print(S[i][0]);
      Serial.print("-->");
      Serial.println(S[i][1]);
      
      lcd.print("| ");
      delay(50);
    } else{
      // link is incorrect
      lcd.print("X ");
      delay(50);
    }

    // set pin back to 0
    setBitIO(S[i][0], LOW);
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

