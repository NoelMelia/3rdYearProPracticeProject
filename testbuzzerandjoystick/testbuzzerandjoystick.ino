
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

const int buzzer = 4;
const int yellow = 12;
const int red = 11;
const int green = 10;
const int blue = 9;

const int SW_pin = 2;
const int X_pin = 0;
const int Y_pin = 1;

const int MAX_LEVEL = 100;
int sequence[MAX_LEVEL];
int your_sequence[MAX_LEVEL];
int level = 1;

int velocity = 1000;

void setup() {  
  pinMode(buzzer, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  //randomSeed(analogRead(A0));
 // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  //lcd.setCursor(0,0); //set cursor to top left corner
  /*
  lcd.print("Booting:"); //print the text to the lcd

  //Booting Up Display
  
  for (int i = 0; i <= 100; i++){  // you can change the increment value here
    lcd.setCursor(8,0);
    if (i<100) lcd.print(" ");  //print a space if the percentage is < 100
    if (i<10) lcd.print(" ");  //print a space if the percentage is < 10
    lcd.print(i);
    lcd.print("%");
    delay(100);  //change the delay to change how fast the boot up screen changes
  }
  */
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  digitalWrite(green, LOW);
  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(blue, LOW);
  // Print a message to the LCD.
  
  Serial.begin(9600);
  lcd.print("Welcome, Player!");
  delay(2000);  //change the delay to change how fast the boot up screen changes
  lcd.clear();
}

void loop() {
  //Testing the X and Y for joystick
  /*Serial.print("Switch:  ");
  Serial.print(digitalRead(SW_pin));
  Serial.print("\n");
  Serial.print("X-axis: ");
  Serial.print(analogRead(X_pin));
  Serial.print("\n");
  Serial.print("Y-axis: ");
  Serial.println(analogRead(Y_pin));
  Serial.print("\n\n");
  delay(100);  //change the delay to change how fast the boot up screen changes
  */
  if (level == 1){
    generate_sequence();//generate a sequence;
    if (analogRead(2) == LOW || level != 1) //If start button is pressed or you're winning
    {
      
      //lcd.clear();
      show_sequence();    //show the sequence
      get_sequence();     //wait for your sequence
    }

    lcd.setCursor(0,0); 
    lcd.print("Press Button");
    lcd.setCursor(0,1); 
    lcd.print("To Begin");
    
    delay(2000);
    lcd.clear();
  }
}
void show_sequence()
{
  digitalWrite(blue, LOW);
  digitalWrite(green, LOW);
  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);

  for (int i = 0; i < level; i++)
  {
    digitalWrite(sequence[i], HIGH);
    delay(velocity);
    digitalWrite(sequence[i], LOW);
    delay(200);
  }
}
void get_sequence()
{
  lcd.print("New Game");
  delay(2000);
  int PX = analogRead(X_pin);
  int PY = analogRead(Y_pin);
  int switch_status = digitalRead(SW_pin);
  int flag = 0; //this flag indicates if the sequence is correct
  
  for (int i = 0; i < level; i++)
  {
    flag = 0;
    while(flag == 0)
    {
      if (PY <=500) {
        digitalWrite(yellow, HIGH);
        your_sequence[i] = 5;
        flag = 1;
        delay(200);
        if (your_sequence[i] != sequence[i])
        {
          wrong_sequence();
          return;
        }
        digitalWrite(yellow, LOW);
      }
    
      if (PY >= 520) {
        digitalWrite(green, HIGH);
        your_sequence[i] = 4;
        flag = 1;
        delay(200);
        if (your_sequence[i] != sequence[i])
        {
          wrong_sequence();
          return;
        }
        digitalWrite(green, LOW);
      }
    
      if (PX >= 520) {
      
        digitalWrite(blue, HIGH);
        your_sequence[i] = 3;
        flag = 1;
        delay(200);
        if (your_sequence[i] != sequence[i])
        {
          wrong_sequence();
          return;
        }
        digitalWrite(blue, LOW);
      }
    
      if (PX <= 500) {
        digitalWrite(red, HIGH);
        your_sequence[i] = 2;
        flag = 1;
        delay(200);
        if (your_sequence[i] != sequence[i])
        {
        wrong_sequence();
        return;
        }
        digitalWrite(red, LOW);
      }
    
    }
  }
  right_sequence();
}

void generate_sequence()
{
  randomSeed(millis()); //in this way is really random!!!
  
  for (int i = 0; i < MAX_LEVEL; i++)
  {
    sequence[i] = random(9,12);
  }
}


void wrong_sequence()
{
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(blue, HIGH);
    digitalWrite(green, HIGH);
    digitalWrite(red, HIGH);
    digitalWrite(yellow, HIGH);
    delay(250);
    digitalWrite(blue, LOW);
    digitalWrite(green, LOW);
    digitalWrite(red, LOW);
    digitalWrite(yellow, LOW);
    delay(250);
  }
  level = 1;
  velocity = 1000;
}

void right_sequence()
{
  digitalWrite(blue, LOW);
  digitalWrite(green, LOW);
  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);
  delay(250);
  
  digitalWrite(blue, HIGH);
  digitalWrite(green, HIGH);
  digitalWrite(red, HIGH);
  digitalWrite(yellow, HIGH);
  delay(500);
  digitalWrite(blue, LOW);
  digitalWrite(green, LOW);
  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);
  delay(500);
  
  if (level < MAX_LEVEL);
  level++;
  
  velocity -= 50; //increase difficulty
}
