
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
const int C1[] = {PinC1_1, PinC1_2, PinC1_3, PinC1_4, PinC1_5, PinC1_6, PinC1_7, PinC1_8};
const int C2[] = {PinC2_1, PinC2_2, PinC2_3, PinC2_4, PinC2_5, PinC2_6, PinC2_7, PinC2_8};

// Matriz de conecciones para StraightThrough
const int ST[][3] = {
  {PinC1_1, PinC2_1, 0},
  {PinC1_2, PinC2_2, 0},
  {PinC1_3, PinC2_3, 0},
  {PinC1_4, PinC2_4, 0},
  {PinC1_5, PinC2_5, 0},
  {PinC1_6, PinC2_6, 0},
  {PinC1_7, PinC2_7, 0},
  {PinC1_8, PinC2_8, 0}
};

// Matriz de conecciones para CrossOver
const int CO[][3] = {
  {PinC1_1, PinC2_1, 0},
  {PinC1_2, PinC2_2, 0},
  {PinC1_3, PinC2_6, 0},
  {PinC1_4, PinC2_4, 0},
  {PinC1_5, PinC2_5, 0},
  {PinC1_6, PinC2_3, 0},
  {PinC1_7, PinC2_7, 0},
  {PinC1_8, PinC2_8, 0}
};

void compararPines(int S[][3]){
  setearTodoBajo(C1);
  for (int i=0; i < 8; i++){
    setearIO(S[i][0], HIGH);
    if (digitalRead(S[i][1]) == HIGH){
      // Esta bien conectado
      Serial.print(S[i][0]);
      Serial.print("-->");
      Serial.println(S[i][1]);
      
      lcd.print("| ");
      delay(50);
    } else{
      // No esta bien conectado
      lcd.print("X ");
      delay(50);
    }
    setearIO(S[i][0], LOW);
    delay(10);
  }
}

void setearIO(int pin, int state){
  digitalWrite(pin, state);
  delay(10);
}

void setearTodoBajo(int con[]){
  for(int i=0; i < 8; i++){
    setearIO(con[0], LOW);
  }
}

void setearTodoAlto(int con[]){
  for(int i=0; i < 8; i++){
    setearIO(con[i], HIGH);
    delay(10);
  }
}

void setearTodosEntrada(int con[]){
  for(int i=0; i < 8; i++){
    pinMode(con[i], INPUT);
  }
}

void setearTodosSalida(int con[]){
  for(int i=0; i < 8; i++){
    pinMode(con[i], OUTPUT);
  }
}

bool detectarCables(){
  setearTodoAlto(C1);
  for(int i=0; i < 8; i++){
    if (digitalRead(C2[i]) == HIGH){
      return true;
    }
  }
  return false;
}

void inicializar(){
  setearTodosSalida(C1);
  setearTodosEntrada(C2);
}

void setup()
{
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  inicializar();
}

void loop()
{
  if(detectarCables()){

    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("1 2 3 4 5 6 7 8");
    lcd.setCursor(4,3);
    lcd.print("1 2 3 4 5 6 7 8");


    lcd.setCursor(1,1);
    lcd.print("ST ");
    lcd.setCursor(1,2);
    lcd.print("CO ");

    lcd.setCursor(4,1);
    compararPines(ST);

    lcd.setCursor(4,2);
    compararPines(CO);
    //lcd.print("CO | | | | | | | X");
    delay(10000);
  } else {
    setearTodoBajo(C1);
    lcd.clear();
    lcd.print("Conectar cables RJ45");

    for (int i=0; i < 20; i++){
      lcd.setCursor(i,2);
      if(i%2==0){
        lcd.print("-");
      }else{
        lcd.print("_");
      }
      delay(200);
    }
  }
}
