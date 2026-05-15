// include the library code:
#include <LiquidCrystal.h>
// #include <string>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2, 3, 8, 9, 10, 11);

// set up a constant for the tilt switch pin

const int switchPin1 = 12;
const int switchPin2 = 13;
const int switchPin3 = 6;
const int switchPin4 = 7;

const int switchPin5 = 5;

unsigned long last_time = 0;
const unsigned long AMOUNT = 3000; // 3 seconds

bool change_input = false;

// variable to hold the value of the switch pin
int switchState1 = 0;
int switchState2 = 0;
int switchState3 = 0;
int switchState4 = 0;

int switchState5 = 0;//idk



int col_state = 0;
int prev_col_state = 0;

bool is_timer = false;

// variable to hold previous value of the switch pin
int prevSwitchState = 0;

// a variable to choose which reply from the crystal ball
int reply;

char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
int alpha_index = 0;

int curser_index = 0;

String input_str = "                ";
String prev_input_str = "                ";

void sendCommand(String cmd, int waitTime){
  Serial1.println(cmd);
  delay(waitTime);

  while(Serial1.available()){
    Serial.write(Serial1.read()); // print ESP response
  }
}

void setup() {
  // set up the number of columns and rows on the LCD
  lcd.begin(16, 2);
  // set up the switch pin as an input
  pinMode(switchPin1, INPUT);
  pinMode(switchPin2, INPUT);
  pinMode(switchPin3, INPUT);
  pinMode(switchPin4, INPUT);

  pinMode(switchPin5, INPUT);

  // Print a message to the LCD.
  lcd.print("Use Arduino To");
  // set the cursor to column 0, line 1
  // line 1 is the second row, since counting begins with 0
  lcd.setCursor(0, 1);
  lcd.cursor();

  // print to the second line
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  // check the status of the switch

  if(Serial1.available()){
    Serial.write(Serial1.read());
  }

  switchState1 = digitalRead(switchPin1);
  switchState2 = digitalRead(switchPin2);
  switchState3 = digitalRead(switchPin3);
  switchState4 = digitalRead(switchPin4);
  switchState5 = digitalRead(switchPin5);

  //add together
  //Serial.println("here2");

  col_state = switchState1 + switchState2 + switchState3 + switchState4 + switchState5;

  if(col_state > 0){
    col_state = 1;
  }
  // if switch_state != prevSwitchState

  if (col_state != prev_col_state) {
  
  if (switchState1 == HIGH){
    alpha_index--;

    if (alpha_index == -1){
      alpha_index = 25;
    }

    //set last, can display 
    last_time = millis();
    is_timer = true;
  }

  if (switchState2 == HIGH){
    alpha_index++;

    if(alpha_index == 26){
      alpha_index = 0;
    }

    last_time = millis();
    is_timer = true;
  }

  if (switchState3 == HIGH){
    curser_index--;

    if (curser_index == -1){
      curser_index = 16;
    }
  }
  
   if (switchState4 == HIGH){
    curser_index++;

    if(curser_index == 17){
      curser_index = 0;
    }
  }

  if (switchState1 == HIGH || switchState2 == HIGH){
    lcd.clear();
    input_str[curser_index] = alphabet[alpha_index];

    lcd.print(input_str);
  }

  if (switchState5 == HIGH){
    lcd.clear();
    input_str[curser_index] = ' ';
    
    lcd.print(input_str);
  }

  // display on 2nd line

  prev_col_state = col_state;
  }

  if(prev_input_str != input_str){
    change_input = true;
  }

  if(is_timer && (millis() - last_time) >= AMOUNT && change_input){
    change_input = false;

    lcd.setCursor(0, 1);
    lcd.print(input_str);
    //send string to ESP

    Serial.println("Sending to ESP:");
    Serial.println(input_str);
    
    //Serial.println(input_str);
    Serial1.println(input_str);
    
    lcd.setCursor(0, 0);
  }

  prev_input_str = input_str;
}