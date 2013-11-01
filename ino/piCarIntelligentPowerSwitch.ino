
// pin definitions
int carACC = 10;
int powerButton = 9;
int powerLed = 8;

int piRelay = A0;
int piPowerOnIn = A1;
int piPowerOffOut = A2;


int powerLedCounter;

int powerButtonCounter;

boolean carACCStatus;
int carACCCounter;

byte powerState;
// 0 shutdown, can be only powered up by power button
// powerled is off

// 1 stand by, can power up by power button single push or carACC signal, holding power button goes into shutdown mode 
// powerled blinks sofly

// 2 powering up state, can be shutdown by holding power button, returning to stand by, waits for feedback from the pi
// powerled glows up

// 3 power up, single push or car acc going low can issue a soft shutdown, holding will hard shutdown leaving into standby, 
// car acc will issue a soft shutdown
// powerled stays on 

// 4 softshutdown, will leave state of stand by
// powerled glows down

byte nextPowerState;
// the setup routine runs once when you press reset:
void setup() {                
  // initialize
  // pins
  pinMode(piRelay, OUTPUT);
  digitalWrite(piRelay, LOW);
  pinMode(piPowerOnIn, INPUT); 
  pinMode(piPowerOffOut, OUTPUT);
  digitalWrite(piPowerOffOut, LOW);
  
  pinMode(carACC, INPUT);
  pinMode(powerButton, INPUT);
  pinMode(powerLed, OUTPUT);
  analogWrite(powerLed, 0);
  
  // vars
  powerState=0;
  nextPowerState=0;
  
  powerLedCounter=0;
  powerButtonCounter=0;  
  carACCStatus=false;
  carACCCounter=0;

}

// the loop routine runs over and over again forever:
void loop() {
  
  delay(10);
  switch (powerState){
    // shutdown
    case 0:
      digitalWrite(piRelay, LOW);
      digitalWrite(piPowerOffOut, LOW);
      ledUpdate();
      if (powerButtonRead() == 1){
        powerState=1;
        
        delay(100);
      }
      break;
    // standby
    case 1:
      digitalWrite(piRelay, LOW);
      digitalWrite(piPowerOffOut, LOW);
      ledUpdate();
      if (powerButtonRead() == 2 ){
        powerState = 0;
        powerButtonCounter = 0; 
        ledUpdate();
        delay(2000); 
      }
      if (carACCRead() == 1){
        powerState = 2;
        delay(100); 
      }  
      break;    
    // powering up
    case 2:
      digitalWrite(piRelay, HIGH);
      digitalWrite(piPowerOffOut, LOW);
      ledUpdate();
      if (digitalRead(piPowerOnIn) == HIGH){
        powerState = 3;
        delay(100);
      }
      if (powerButtonRead() == 2 ){
        powerState=0;
        ledUpdate();
        powerButtonCounter = 0;
        powerButtonCounter = 0; 
        delay(2000); 
      }
      break;
    
    //  power on
    case 3:
      digitalWrite(piRelay, HIGH);
      digitalWrite(piPowerOffOut, LOW);
      ledUpdate();
      if (powerButtonRead() == 2 ){
        powerState=0;
        ledUpdate();
        powerButtonCounter = 0;
        ledUpdate();
        delay(2000);
      }
      // this does not work, i do not know why
      if(powerButtonRead() == 1){
        nextPowerState = 0;
        powerState = 4; 
        delay(100);
      }
      
      if (carACCRead() == 0){
        nextPowerState = 1;
        powerState = 4;
        delay(100); 
      }
      break;
      
    // soft shutdown
    case 4:
      digitalWrite(piRelay, HIGH);
      digitalWrite(piPowerOffOut, HIGH);
      ledUpdate();
      if (digitalRead(piPowerOnIn) == LOW){
        powerState = nextPowerState;
        ledUpdate();
        delay(100);
      }
      if (powerButtonRead() == 2 ){
        powerState = 0;
        powerButtonCounter = 0; 
        ledUpdate();
        delay(1000);
      }
      break;
      
    default:
      powerState=0;
      ledUpdate();
      digitalWrite(piRelay, LOW);
      digitalWrite(piPowerOffOut, LOW);
  }
  
  
  
    
  
}

// returns 0 if car ACC is off
// returns 1 if car ACC is on

int carACCRead(){
  if (digitalRead(carACC) == LOW && carACCStatus == false ){
     if (carACCCounter > 700 ){
       carACCStatus = true;
       carACCCounter = 0 ;
       return 1;
     }
     carACCCounter++;
     return carACCStatus;
     
  } 
  
  if (digitalRead(carACC) == HIGH && carACCStatus == true ){
     if (carACCCounter > 700 ){
       carACCStatus = false;
       carACCCounter = 0 ;
       return 0;
     }
     carACCCounter++;
     return carACCStatus;
     
  }
  
  carACCCounter = 0;
  return carACCStatus;
  
  
}

// returns 0 for nothing
// returns 1 for single click
// returns 2 for hold 

int powerButtonRead(){
  if (digitalRead (powerButton) == LOW ){
    if (powerButtonCounter > 300){
      powerButtonCounter=0;
      return 2;
    }
    powerButtonCounter++;
    return 0;
  } 
  
  if (powerButtonCounter < 1){
    powerButtonCounter=0;
    return 0;
  }
  
  if (powerButtonCounter > 2 && powerButtonCounter <= 300){
    powerButtonCounter=0;
    return 1;
  }
  
  return 0;
 
}




void ledUpdate(){
  switch (powerState){
    case 0:
      powerLedCounter=0;
      analogWrite(powerLed, powerLedCounter);
      break;
    
    case 1:
      powerLedCounter++;
      if (powerLedCounter > 100 ){
        powerLedCounter = 0;
      }
      if (powerLedCounter < 5){
        analogWrite(powerLed, 100);
      } else {
        analogWrite(powerLed, 0);
      }
      break;
      
    case 2:
      powerLedCounter++;
      if (powerLedCounter > 255 ){
        powerLedCounter = 0;
      }
      analogWrite(powerLed, powerLedCounter);
      break;
    
    case 3:
      powerLedCounter=255;
      analogWrite(powerLed, powerLedCounter);
      break;
      
    case 4:
      powerLedCounter--;
      if (powerLedCounter < 0 ){
        powerLedCounter = 255;
      }
      analogWrite(powerLed, powerLedCounter);
      break;
      
    default:
      powerLedCounter=0;
      analogWrite(powerLed, powerLedCounter);
      break;
    
  }
  
}


/*

USELESS bits

/*
boolean poweringUp;
boolean shuttingDown;
boolean powerOn;


  poweringUp=false;
  shuttingDown=false;
  powerOn=false;



  if(digitalRead(carACC)==LOW && powerOn == false){
    carACCcount++;
  } 
  else {
    carACCcount=0;
  }
  
  if (carACCcount > 500 && carACCfiltered == false && powerOn == false){
    carACCfiltered=true;
    carACCcount=0;
     
  }
  
  
  if(digitalRead(carACC)==HIGH && powerOn == true){
    carACCcount++;
  } 
  else {
    carACCcount=0;
  }
  
  if (carACCcount > 500 && carACCfiltered == true && powerOn == true ){
    carACCfiltered=false;
    carACCcount=0; 
  }
  
  
  if ( (carACCfiltered == true || digitalRead(powerButton) == LOW) && powerOn == false ){
    poweringUp=true;
    digitalWrite(piPowerOffOut, LOW);
    digitalWrite(piRelay, HIGH);
    delay(100);
    while(powerOn == false ){
      if (digitalRead(piPowerOnIn) == HIGH){
        powerOn = true;
        poweringUp = false;
        ledUpdate();
      }
      if (digitalRead(powerButton) == LOW ){
        powerButtonCount++;
      }
      else{
        powerButtonCount=0;
      } 
      if (powerButtonCount > 300){
        powerButtonCount = 0;
        powerOn = false;
        poweringUp = false;
        shuttingDown = false;
        ledUpdate();
        digitalWrite(piPowerOffOut, LOW);
        digitalWrite(piRelay, LOW);
        delay(1000);
        break;
      } 
      ledUpdate();
      delay(10);
    }
    delay(100);
  }

  if ( (carACCfiltered == false && digitalRead(powerButton) == LOW) && powerOn == true ){
    shuttingDown=true;
    digitalWrite(piPowerOffOut, HIGH);
    delay(100);
    while(powerOn == true ){
      if (digitalRead(piPowerOnIn) == LOW){
        powerOn = false;
        shuttingDown = false;
        ledUpdate();
        delay(3000);
        digitalWrite(piRelay, LOW);
      }
      if (digitalRead(powerButton) == LOW ){
        powerButtonCount++;
      }
      else{
        powerButtonCount=0;
      } 
      if (powerButtonCount > 300){
        powerButtonCount = 0;
        powerOn = false;
        poweringUp = false;
        shuttingDown = false;
        ledUpdate();
        digitalWrite(piPowerOffOut, LOW);
        digitalWrite(piRelay, LOW);
        delay(1000);
        break;
      }
      ledUpdate();
      delay(10);
    }
    delay(100);
  }

void ledUpdate(){
  
  if (poweringUp == true && powerOn == false){
    powerLedCounter++;
    if (powerLedCounter > 255 ){
      powerLedCounter = 0;
    }
    analogWrite(powerLed, powerLedCounter);    
    return;
  }
 
  if (shuttingDown == true && powerOn == true){
    powerLedCounter--;
    if (powerLedCounter < 0 ){
      powerLedCounter = 255;
    }
    analogWrite(powerLed, powerLedCounter);    
    return;
  }
  
  if (powerOn == true){
    powerLedCounter = 255;
    analogWrite(powerLed, powerLedCounter);
    return; 
  }
  
  if (powerOn == false){
    powerLedCounter = 0;
    analogWrite(powerLed, powerLedCounter);
    return; 
  }
}

*/
