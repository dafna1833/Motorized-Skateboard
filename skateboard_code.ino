# include <IRremote.h>

#define oneButt 12495
#define twoButt 6375
#define threeButt 31365
#define fourButt 4335
#define fiveButt 14535
#define sixButt 23205
#define plus 43095
#define minus 57375
#define repeat 65325

const int controlPin1 = 2;
const int controlPin2 = 3;
const int enablePin = 9;
const int potPin = A0;

int IRSwitchOnOff = 0;
int previousIRSwitchOnOff = 0;
int IRSwitchDirection = 0;
int previousIRSwitchDirection = 0;


int motorEnabled = 0;
int motorSpeed = 0;
int motorDirection = 1;

const int IRpin = 11;
IRrecv receiver(IRpin); // "create" IR sensor object
decode_results IRoutput;

// declare initial photoresistor (PR) variables 
int PRpin = 1;
int PRvalue = 0; // value read by the PR
int PRsens = 20; // sensitivity of PR


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(controlPin1, OUTPUT);
  pinMode(controlPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(7, OUTPUT); // LED is connected to pin 3 (PMW capable)
  
  digitalWrite(enablePin, LOW); 

  receiver.enableIRIn();

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(300);

 unsigned int currentSetting;
  if (receiver.decode(&IRoutput)){
    unsigned int IRvalue = IRoutput.value;
    switch(IRvalue){ // the following switch case is irrelevant? fix this
      case repeat:
          currentSetting = currentSetting;
          break; 
      case oneButt:
          currentSetting = IRvalue; break;
      case twoButt:
          currentSetting = IRvalue; break;
      case threeButt:
          currentSetting = IRvalue; break;
      case plus:
          currentSetting = IRvalue; break;
      case minus:
          currentSetting = IRvalue; break;
      default:
          currentSetting = currentSetting; // might not work 
          break; 
    } 

  }

  IRSwitchOnOff = currentSetting;
  
  Serial.println(IRSwitchOnOff);
  motorSpeed = analogRead(potPin)/4;

  Serial.print("Here\n");

   if (IRSwitchOnOff != previousIRSwitchOnOff) {
    if (IRSwitchOnOff == oneButt) {
    motorEnabled = !motorEnabled; 
   }
   }
   if (IRSwitchOnOff != previousIRSwitchOnOff) {
    if (IRSwitchOnOff == twoButt) {
      motorDirection = !motorDirection;
    }

   }

  if (motorDirection == 1) {
    digitalWrite(controlPin1, HIGH);
    digitalWrite(controlPin2, LOW);
  }

  else {
    digitalWrite(controlPin1, LOW);
    digitalWrite(controlPin2, HIGH);
  }

  if (motorEnabled == 1){
    analogWrite(enablePin, motorSpeed);
  }
  else {
    analogWrite(enablePin, 0);
  }

  previousIRSwitchOnOff = IRSwitchOnOff;
  previousIRSwitchDirection = IRSwitchDirection;

   delay(100);
   
   PRvalue = analogRead(PRpin); // read data from PR
  Serial.println(PRvalue);
  //Serial.println(PRvalue); // print resistor value  
  
  // turn LED on if the value read by the PR is less than the 
  // set sensitivity of the PR (PRsens)
  if (PRvalue < PRsens){
    if (currentSetting == fourButt){ // regular lights
        digitalWrite(7,HIGH);
        delay(100); 
        Serial.print("regular lights on, setting = "); 
        Serial.print(currentSetting);  
        Serial.print(",  PRvalue = ");
        Serial.println(PRvalue); 
    } /*
    else if (currentSetting == twoButt){ // breathing lights 
      breathingEffect();  
      Serial.print("breathing effect, setting = "); 
      Serial.println(currentSetting); }
    */
    else if (currentSetting == fiveButt){ // strobe 
      analogWrite(7,255);
      delay(100);
      analogWrite(7,1);
      delay(100); 
      Serial.print("strobe lights on, setting = "); 
      Serial.print(currentSetting);  
      Serial.print(",  PRvalue = ");
      Serial.println(PRvalue); 
      
    } 
    else{ digitalWrite(7,HIGH); Serial.println("default");}
  }
  else{
    digitalWrite(7, LOW);
  }

   
  receiver.resume();
    delay(500); 
  currentSetting = 0;

}
