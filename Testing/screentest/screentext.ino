
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 
//Code Got off internet Techydiy is the reference 
void setup()
{
  lcd.begin(16, 2);

  lcd.setCursor(0,0); //set cursor to top left corner
  lcd.print("Booting:"); //print the text to the lcd

  for (int i = 0; i <= 100; i++){  // you can change the increment value here
    lcd.setCursor(8,0);
    if (i<100) lcd.print(" ");  //print a space if the percentage is < 100
    if (i<10) lcd.print(" ");  //print a space if the percentage is < 10
    lcd.print(i);
    lcd.print("%");
    delay(100);  //change the delay to change how fast the boot up screen changes
  }

  lcd.setCursor(0,0);
  // Print a message to the LCD.
  lcd.print("Welcome, player!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis()/1000);
}
