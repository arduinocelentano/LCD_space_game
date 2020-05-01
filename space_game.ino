#include <Wire.h> // Бібліотека для роботи з I2C пристроями
#include <LiquidCrystal_I2C.h> // бібліотека для роботи з LCD
#define BTNUP 2
#define BTNDOWN 3
#define ASTEROIDS_MAX 10

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); //параметри дисплею: 16x2, адреса 0x27
int shipY;
int asteroidsX[ASTEROIDS_MAX];
int asteroidsY[ASTEROIDS_MAX];
int asteroidsSpeed[ASTEROIDS_MAX];
int asteroidsCount;
int score;
bool gameover;

//Створення власних символів
//1 - зафарбований піксель, 0 - незафарбований
byte Ship1[] = {
  B11110,
  B01101,
  B01101,
  B11110,
  B00000,
  B00000,
  B00000,
  B00000
};
byte Ship2[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B11110,
  B01101,
  B01101,
  B11110
};
byte Asteroid1[] = {
  B00000,
  B01110,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte Asteroid2[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B01110,
  B01110,
  B00000
};

void setup() {
  lcd.init();// Ініціалізація LCD
  lcd.backlight();//Активація підсвітки
  //передача власних символів дисплею
  lcd.createChar(0, Ship1);
  lcd.createChar(1, Ship2);
  lcd.createChar(2, Asteroid1);
  lcd.createChar(3, Asteroid2);
  pinMode(BTNUP, INPUT_PULLUP);
  pinMode(BTNDOWN, INPUT_PULLUP);
  shipY = 0;
  randomSeed(analogRead(0));
  asteroidsCount = 1;
  asteroidsX[0] = 15;
  asteroidsY[0] = random(4);
  asteroidsSpeed[0] = random(2)+1;
  score = 0;
  gameover = false;
}

void loop() {
  if(gameover){
    lcd.setCursor(13,0);
    lcd.print(score);
    lcd.setCursor(3,1);
    lcd.print("GAME  OVER");
  }
  else{
    if (LOW==digitalRead(BTNUP)){
      shipY--;
      if (shipY<0)
        shipY=0;
    }
    if (LOW==digitalRead(BTNDOWN)){
      shipY++;
      if (shipY>3)
        shipY=3;
    }
    //  виведення ігрового поля
    lcd.clear();
    lcd.setCursor(0, shipY/2);
    lcd.write(byte(shipY%2));
    lcd.setCursor(13,0);
    lcd.print(score);
    for (int i = 0; i<asteroidsCount; i++){
      asteroidsX[i]-=asteroidsSpeed[i];
      if (0>=asteroidsX[i]){
        if(asteroidsY[i]==shipY)
          gameover = true;
        asteroidsX[i] = 16;
        asteroidsY[i] = random(4);
        asteroidsSpeed[i] = random(2)+1;
        score++;
        if ((0 == score %10)&&(asteroidsCount<ASTEROIDS_MAX)){
          asteroidsCount++;
          asteroidsX[asteroidsCount-1] = 15;
          asteroidsY[asteroidsCount-1] = random(4);
          asteroidsSpeed[asteroidsCount-1] = random(2)+1;
        }
      }
      lcd.setCursor(asteroidsX[i], asteroidsY[i]/2);
      lcd.write(byte(2+asteroidsY[i]%2));
    }
  }
  delay(100);
}
