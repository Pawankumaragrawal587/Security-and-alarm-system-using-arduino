#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

//for ultrasonic sensor
const int trigPin = 4;
const int echoPin = 3;
long duration;

const int buzz = 13;

//for 4X4 matrix keypad
const byte ROWS = 4; /* four rows */
const byte COLS = 4; /* four columns */
/* define the symbols on the buttons of the keypads */
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}};
byte rowPins[ROWS] = {12, 11, 10, 9}; /* connect to the row pinouts of the keypad */
byte colPins[COLS] = {8, 7, 6, 5}; /* connect to the column pinouts of the keypad */
/* initialize an instance of class NewKeypad */
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

bool lock = true;
bool alarm = true;
long long t =0;
String curr_password = "1234";

// return the readings of ultrasonic sensor
int readUltrasonic(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  float  d= duration*0.034/2;              //d=distance
  Serial.println(d);
  return d;
}

// return the 4 char entered password
String enter_password(){
  t = millis();
  lcd.setCursor(0, 0);
  lcd.print("Enter password:  ");
  lcd.setCursor(0, 1);
  lcd.print("->                 ");
  String pass = "";
  int i=0;
  while( i < 4){
    char customKey = customKeypad.getKey();
    if( customKey){
      pass += customKey;
      i++;
    }
    lcd.setCursor(0, 1);
    lcd.print("-> "+ pass + "            ");
    
    if(millis()- t > 5000){
      return("");
    }
  }
  delay(500);
  return(pass);
}

//variry the current password, return true when verified, 3 attempt is given
bool verify_password(){ 
  int i=0;
  while(i<2){
    String pass = enter_password();
    if(pass == curr_password){
      return(true);
    }
    i++;
  }
  return(false);
}


void securty_breach(){
  
  while(1){ 
    lcd.setCursor(0, 0);
    lcd.print("securty Breached");
    lcd.setCursor(0, 1);
    lcd.print("   WARNING!!!   ");
    digitalWrite(buzz, HIGH);
    delay(1000);

    char customKey = customKeypad.getKey();
    if( customKey == 'D'){
      bool flag = verify_password();
      if(flag){
        lock = false;
        lcd.setCursor(0, 1);
        lcd.print("                 ");
      digitalWrite(buzz, LOW);
        break;
      }
    }
    
    lcd.setCursor(0, 1);
    lcd.print("                 ");
    digitalWrite(buzz, LOW);
    delay(500);

  }
  lcd.setCursor(0, 0);
  lcd.print("Alarm reset      ");
  delay(500);
}


void personEncountered(){
  bool flag = verify_password();
  if(flag){
    lock = false;
  }
  else{
    securty_breach();
  }
}


void reset_password(){
  lcd.setCursor(0, 0);
  lcd.print(" Enter old pass   ");
  lcd.setCursor(0, 1);
  lcd.print("##################");
  delay(1000);
  if(verify_password()){
    lcd.setCursor(0, 0);
    lcd.print(" Enter New pass   ");
    lcd.setCursor(0, 1);
    lcd.print("##################");
    delay(1000);
    String pass = enter_password();
    
    lcd.setCursor(0, 0);
    lcd.print("Re-enter password  ");
    lcd.setCursor(0, 1);
    lcd.print("##################");
    delay(1000);
    String temp = enter_password();
    if(temp == pass){
      curr_password = pass;
      lcd.setCursor(0, 0);
      lcd.print("Password Updated  ");
      lcd.setCursor(0, 1);
      lcd.print("##################");
      delay(1000);
      return(NULL);
    }
  }
  lcd.setCursor(0, 0);
  lcd.print("Update Failed!!!     ");
  lcd.setCursor(0, 1);
  lcd.print("Try again            ");
  delay(1000);
}

// main menu
void enterToMainMenu(){
  t = millis();
  int temp = 1;
  lcd.setCursor(0, 0);
  lcd.print("->Reset Password     ");
  lcd.setCursor(0, 1);
  if(alarm){
    lcd.print("  Off alarm        ");
  }
  else{
    lcd.print("  On alarm         ");
  }
  
  while(1){
    if(millis()-t>20000){
      break;
    }
    char customKey = customKeypad.getKey();
    if( customKey == 'A'){
      temp = 1;
    }
    if( customKey == 'C'){
      temp = 2;
    }
    if( customKey == 'B'){
      if(temp ==1){
        reset_password();
      }
      else if(temp ==2){
        if(alarm){
          lcd.setCursor(0, 0);
          lcd.print("                    ");
          lcd.setCursor(0, 1);
          lcd.print("Alarm OFF          ");
          alarm = false;
        }
        else{
          lcd.setCursor(0, 0);
          lcd.print("                   ");
          lcd.setCursor(0, 1);
          lcd.print("Alarm ON           ");
          alarm = true;
        }
        delay(1000);
      }
    }
    if( customKey == 'D'){
      return(NULL);
    }
    if(temp ==1){
      lcd.setCursor(0, 0);
      lcd.print("->Reset Password  ");
      lcd.setCursor(0, 1);
      if(alarm){
        lcd.print("  Off alarm          ");
      }
      else{
        lcd.print("  On alarm          ");
      }
    }
    else if(temp==2){
      lcd.setCursor(0, 0);
      lcd.print("  Reset Password     ");
      lcd.setCursor(0, 1);
      if(alarm){
        lcd.print("->Off alarm        ");
      }
      else{
        lcd.print("->On alarm         ");
      }
    }
  }
}


void setup(){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzz, OUTPUT);
  
  Serial.begin(9600);
  
  lcd.begin();  
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Pawan K. Agrawal");
}


void loop(){
  lcd.setCursor(0, 0);
  lcd.print("Pawan K. Agrawal     ");
  if(lock == true){
    lcd.setCursor(0, 1);
    lcd.print("  HOME LOCKED!       ");
  }
  else{
    lcd.setCursor(0, 1);
    lcd.print("  Welcome Home      ");
  }
  
  float d = readUltrasonic();
  if(d<=30 && lock == true && alarm == true){  // person detected
    personEncountered();
    t = millis();
  }

  char customKey = customKeypad.getKey();
  if( customKey == 'D' && (lock == false || alarm == false)){  // going to main menu
    enterToMainMenu();
  }

  if(lock == false && millis()- t > 10000){  // activating the allarm after 10sec
    lock = true;
  }

}
