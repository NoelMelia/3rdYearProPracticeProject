#include <Wire.h>//This library allows you to communicate with I2C / TWI devices
#include <LiquidCrystal_I2C.h>//Disply Screen. A library for I2C LCD displays.

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);// Used with the creen to display the info with the wires it alos connects to

#define CHOICE_OFF   0 //Used to control LEDs
#define CHOICE_NONE  0 //Used to check buttons
#define CHOICE_RED  (1 << 0)//<< (left shift) Takes two numbers, left shifts the bits of the first operand,
//                            the second operand decides the number of places to shift. Or in other words left 
//                            shifting an integer “x” with an integer “y” (x<<y) is equivalent to multiplying x with 2^y (2 raise to power y).
#define CHOICE_GREEN (1 << 1)
#define CHOICE_WHITE (1 << 2)


#define LED_RED    9//Led Lights and there Number in ports
#define LED_WHITE  8
#define LED_GREEN  3

// Button pin definitions and there ports in the uno
#define BUTTON_RED    7
#define BUTTON_GREEN  2
#define BUTTON_WHITE   6

// Buzzer and ports in uno
#define BUZZER1  10//10 Got Sick of Buzzer on
#define BUZZER2  11//11 Change Back for Testing Purposes

// Define game parameters
#define ENTRYTIME   5000 //Amount of time to press a button before game times out. 5000ms = 5 sec

//Levels before going to next level
#define LEVEL_1  3
#define LEVEL_2  5
#define LEVEL_3  100

// Game state variables
byte gameMode = 0; //By default, let's play the memory game
byte gameBoard[32]; //Contains the combination of buttons as we advance
byte gameRound = 0; //Counts the number of succesful rounds the player has made it through
byte currentMove = 0;
byte bestMove = 0;
byte highScore = 0;
byte level = 1;
void setup()
{
  //Setup hardware inputs/outputs
  //Enable pull ups on inputs
  pinMode(BUTTON_RED, INPUT_PULLUP);
  pinMode(BUTTON_GREEN, INPUT_PULLUP);
  pinMode(BUTTON_WHITE, INPUT_PULLUP);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_WHITE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  pinMode(BUZZER1, OUTPUT);
  pinMode(BUZZER2, OUTPUT);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  //Mode checking
  gameMode = 0; // By default, we're going to play the memory game
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  // Check to see if upper right button is pressed
  if (checkButton() == CHOICE_GREEN)
  {
    //Turn on the upper right (green) LED
    setLEDs(CHOICE_GREEN);
    toner(CHOICE_GREEN, 150);

    setLEDs(CHOICE_RED | CHOICE_WHITE ); // Turn on the other LEDs until you release button

    while(checkButton() != CHOICE_NONE) ; // Wait for user to stop pressing button
  }
  //Booting Up Display
    lcd.print("Loading:"); //print the text to the lcd
    for (int i = 0; i <= 100; i++){  // you can change the increment value here
    lcd.setCursor(8,0);
    if (i<100) lcd.print(" ");  //print a space if the percentage is < 100
    if (i<10) lcd.print(" ");  //print a space if the percentage is < 10
    lcd.print(i);
    lcd.print("%");
    delay(100);  //change the delay to change how fast the boot up screen changes
    }
  lcd.clear();
}

void loop()
{
  notPressed(); // Blink lights while waiting for user to press a button
  
  delay(1000);  //change the delay to change how fast the boot up screen changes
  //lcd.clear();
  // Play memory game and handle result
  // Indicate the start of game play
  setLEDs(CHOICE_RED | CHOICE_GREEN | CHOICE_WHITE ); // Turn all LEDs on
  delay(1000);
  setLEDs(CHOICE_OFF); // Turn off LEDs
  delay(250);

  if (gameMode == 0)//Only one game mode but wanted to make more
  {
    if (play() == true) 
    {
      //Congrats Message
      lcd.print("Congrats");
      delay(2000);
      lcd.clear();
    }
    else 
    {
      lcd.print("Hard Luck");
      delay(2000);
      lcd.clear();
    }
  }
  /*else if (gameMode == 1)*/
}

// Play the regular memory game
boolean play(void)
{
  lcd.clear();//Clear the screen
  randomSeed(millis()); // Random the seeds of the lights and pick one to light up
  gameRound = 0;// Reset the game to the beginning
  
  lcd.print("Level ");
  lcd.print( level);
  delay(2000);
  lcd.clear();
  gameRound = 0;
  while (gameRound < 100)
  {
      moves(); // Add a button to the current moves, then play them back
      playMoves(); // Play back the current game board
      // Then require the player to repeat the sequence.
      for (currentMove = 0 ; currentMove < gameRound ; currentMove++)
      {//Loop to go through to make sure the player puts in the correct button with light
          byte choice = wait(); // See what button the user presses
          
          if (choice == 0)
          {
            return false; // If wait timed out, player loses
          }
          if (choice != gameBoard[currentMove]) 
          {
            return false; // If the choice is incorect, player loses
          }
        
      }
      bestMove = currentMove;//Setting the move to current in screen
      lcd.setCursor(0,1);
      lcd.print("Score ");// Display the score in the below half of screen
      lcd.print(bestMove);
      
      if(currentMove >= LEVEL_2 && level != 3)
      {
          level = 2;
      }
      if(currentMove >= LEVEL_3 )
      {
          level = 3; 
      }
      lcd.setCursor(0,0);
      lcd.print("Level ");
      lcd.print( level);
      delay(2000); // Player was correct, delay before playing moves
      lcd.clear();
      if(highScore < bestMove)
      {//Set the current if higher then orignal high score
          highScore = bestMove;
            
      }
      
  }
  
  return true; // Player made it through all the rounds to win!
}  
// Plays the current contents of the game moves
void playMoves(void)
{
  for (byte currentMove = 0 ; currentMove < gameRound ; currentMove++) 
  {
    toner(gameBoard[currentMove], 150);
    delay(150); // 150 works well. Maybe Get faster as Time goes on
  }
}

// Adds a new random button
void moves(void)
{
  byte newButton = random(0, 3);//3 buttons in it

  // We have to convert this number, 0 to 3, to CHOICEs
  if(newButton == 0){ 
    newButton = CHOICE_RED
  };
  else if(newButton == 1){
    newButton = CHOICE_GREEN
  };
  else if(newButton == 2){
    newButton = CHOICE_WHITE
  };

  gameBoard[gameRound++] = newButton; // Add this new button to the game array
}

// Lights a given LEDs
void setLEDs(byte leds)
{
  if ((leds & CHOICE_RED) != 0){
    digitalWrite(LED_RED, HIGH);// The different flashes of high and low in the Led lights
  }
  else{
    digitalWrite(LED_RED, LOW);
  }
  if ((leds & CHOICE_GREEN) != 0){
    digitalWrite(LED_WHITE, HIGH);
  }
  else
  {
    digitalWrite(LED_WHITE, LOW);
  }  
  if ((leds & CHOICE_WHITE) != 0){
    digitalWrite(LED_GREEN, HIGH);
  }
  else{
    digitalWrite(LED_GREEN, LOW);
  }
}

// Wait for a button to be pressed to Start the game
byte wait(void)
{
  long startTime = millis();// Number of milliseconds passed since the program began

  while ( (millis() - startTime) < ENTRYTIME) // Loop until too much time has passed
  {
    byte button = checkButton();
    if (button != CHOICE_NONE)
    { 
      toner(button, 150); // Play the button the user just pressed
      while(checkButton() != CHOICE_NONE) ;  // Now let's wait for player to release button
      delay(10); // This helps with pressing button twice. Alot of People always double tap
      return button;
    }
  }
  return CHOICE_NONE; // Time Out
}
// Checking the move is the same as this so the player can continue or not.
byte checkButton(void)
{
  if (digitalRead(BUTTON_RED) == 0) return(CHOICE_RED); 
  else if (digitalRead(BUTTON_GREEN) == 0) return(CHOICE_GREEN); 
  else if (digitalRead(BUTTON_WHITE) == 0) return(CHOICE_WHITE); 

  return(CHOICE_NONE); // If no button is pressed, return none
}
// Sound of Buzzer and Length depending on button 
// pressed for light and button to work
void toner(byte which, int buzzLen)
{
  setLEDs(which); //Turn on a given LED
  switch(which) 
  {
    case CHOICE_RED:
      sound(buzzLen, 1100); 
      break;
    case CHOICE_GREEN:
      sound(buzzLen, 500); 
      break;
    case CHOICE_WHITE:
      sound(buzzLen, 300); 
      break;
  }// I had to really miss around with these as i couldnt get working for a long time
  setLEDs(CHOICE_OFF); // Turn off all LEDs
}
// Timing was everything in this. The amount of time the tone play before it became annoying
void sound(int buzzLen, int buzzDelay)
{
  long buzzLength = buzzLen * (long)1000;
  while (buzzLength > (buzzDelay * 2))
  {
    buzzLength -= buzzDelay * 2; //Decrease
    //Speed low High 
    digitalWrite(BUZZER1, LOW);
    digitalWrite(BUZZER2, HIGH);
    delayMicroseconds(buzzDelay);
    //Speed High low
    digitalWrite(BUZZER1, HIGH);
    digitalWrite(BUZZER2, LOW);
    delayMicroseconds(buzzDelay);
  }
}

// Flash Lights while Waiting
// Easy Sequence as just Red, Green and White
void notPressed(void)
{
  while(1) 
  {// Keep it in the loop until something is pressed
    // Text to Display
    lcd.print("Press Button");
    lcd.setCursor(0,1);
    lcd.print("to Begin");//High Score At Home Screen
    delay(2000);
    lcd.clear();
    setLEDs(CHOICE_RED);
    delay(500);
    if (checkButton() != CHOICE_NONE) return;
    setLEDs(CHOICE_WHITE);
    delay(500);
    if (checkButton() != CHOICE_NONE) return;
    setLEDs(CHOICE_GREEN);
    delay(500);
    if (checkButton() != CHOICE_NONE) return;
    // High Score to be Stored and Displayed
    lcd.print("High Score ");
    lcd.print(highScore);
    delay(2000);
    lcd.clear();
  }
}
