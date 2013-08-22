#include <Wire.h>
#include <MAX17043.h>
#include <SoftwareSerial.h>

int motorSleepPin = 7;

MAX17043 fuelGauge(20,4);

/*This code controls a stepper motor with the 
EasyDriver board. It spins forwards and backwards
***************************/
int dirPin = 4;
int stepPin = 5;

boolean doorOpen = false;

SoftwareSerial xbee(2, 3); // RX, TX

void setup() 
{
   Serial.begin(9600);
  Serial.println("start");
  pinMode(motorSleepPin, OUTPUT);
  digitalWrite(motorSleepPin, LOW);
  
  
  Wire.begin();
   Serial.println("wire begin");
  xbee.begin(9600);
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  
  Serial.println("begin");
  fuelGauge.begin();

}

void loop()
{
  if(xbee.available()){
    getCommand();
  }else{
   
   delay(100); 
  }

}

void getCommand(){
  
 
  boolean readingCmd = false;
  
  char buffer[25];
  int bufferSize = 25;
  int currBufferSize = 0;
  
  while ( xbee.available() ) {
         
    char c = (char) xbee.read();
    
    if(!readingCmd && c == '@'){
       
       readingCmd = true;
       Serial.println("Reading cmd");
       continue;
       
    }
    
    if(readingCmd && c == '@'){
      
      //Got command 
      buffer[currBufferSize] = '\0';
      
      String cmd = String(buffer);
      
      if(cmd == "status"){
        sendStatus();
      }
      else if(cmd.substring(0,4) == "open"){
        Serial.println(cmd.substring(5));
        openDoor(cmd.substring(5).toInt());
      }
      else if(cmd.substring(0,5) == "close"){
        Serial.println(cmd.substring(5));
        closeDoor(cmd.substring(5).toInt());
        
      }
      else{
        Serial.println(buffer);
      }
      
      return;
    }
    
    if(currBufferSize < bufferSize){
     
       buffer[currBufferSize] = c;
       currBufferSize++; 
    }
    else{
     
       Serial.println("buffer overflow");
       return; 
    }
  }
  
}

void openDoor(int steps){
  
  if(!doorOpen){
    
    
    
    digitalWrite(motorSleepPin, HIGH);
  
    Serial.print("opening: ");
    Serial.println(steps,DEC);
    
    digitalWrite(dirPin, LOW);     // Set the direction.
    delay(100);
  
  
    for (int i = 0; i<steps; i++)       // Iterate for 4000 microsteps.
    {
      digitalWrite(stepPin, LOW);  // This LOW to HIGH change is what creates the
      digitalWrite(stepPin, HIGH); // "Rising Edge" so the easydriver knows when to step.
      delayMicroseconds(500);      // This delay time is close to top speed for this
    }      // particular motor. Any faster the motor stalls.
  
    digitalWrite(motorSleepPin, LOW);
    
    doorOpen = true;
  }
  
}


void closeDoor(int steps){
  
  if(doorOpen){
  
    digitalWrite(motorSleepPin, HIGH);
      
    Serial.print("closing: ");
    Serial.println(steps,DEC);
    
    digitalWrite(dirPin, HIGH);    // Change direction.
    delay(100);
  
  
    for (int i = 0; i<steps; i++)       // Iterate for microsteps
    {
      digitalWrite(stepPin, LOW);  // This LOW to HIGH change is what creates the
      digitalWrite(stepPin, HIGH); // "Rising Edge" so the easydriver knows to when to step.
      delayMicroseconds(500);      // This delay time is close to top speed for this
    }                           // particular motor. Any faster the motor stalls.
  
    digitalWrite(motorSleepPin, LOW);
    
    doorOpen = false;
    
  }
}

void sendStatus(){
  
  Serial.println("Sending status");
 
  float percent = fuelGauge.getBatteryPercentage();
  float volts = fuelGauge.getBatteryVoltage();
  int battery = percent;  //Get battery reading
  
  xbee.print("?");
  xbee.print(battery,DEC);
  //xbee.print(",");
  //xbee.print(volts,DEC);
  xbee.print("-");
  xbee.print(doorOpen,DEC);
  xbee.print("?");
  
}

void output() {
  Serial.print("Version: ");
  Serial.println(fuelGauge.getVersion());
  Serial.print("Alert Threshold: ");
  Serial.println(fuelGauge.getAlertThreshold());
  Serial.print("Alert Threshold Register Version: ");
  Serial.println(fuelGauge.getAlertThresholdRegister());
  Serial.print("Battery Voltage: ");
  Serial.println(fuelGauge.getBatteryVoltage());
  Serial.print("Battery Percentage: ");
  Serial.println(fuelGauge.getBatteryPercentage());
  Serial.print("Is Alerting? ");
  Serial.println(fuelGauge.isAlerting());
  Serial.print("Is Sleeping? ");
  Serial.println(fuelGauge.isSleeping());
  Serial.print("Is Sleeping Register Version? ");
  Serial.println(fuelGauge.isSleepingRegister()); 
  Serial.println("");
}


