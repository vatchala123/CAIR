//Initiates Child Identification Module
//Provides functions to test Child Identification Module

#define SERVOLOWERBOUND 30
#define SERVOHIGHERBOUND 135
//#define SERVOROTATEINCREMENTS 10

int ciTestDelay = 500;

int getCITestDelay(){
  return ciTestDelay;
}

void setCITestDelay(int i){
  ciTestDelay = i;
}

void initiateCI(){
  //motorSetup(); //prepares servos NOT NEEDED
  //temperature sensor pin
  thermalSensorSetup();//thermal sensor pin (I2C)
 // powerDetectionSetup(); //power detection pin PUT THIS LAST
}


void findChild(){ //rotates servos to take measurements with thermal sensor
  for(int x = 0; x < (getThermalTiltBufferSize() / 4); x++){
    moveTilt(120 - x * 30);
    for(int i = SERVOHIGHERBOUND; i >=  SERVOLOWERBOUND; i -= ((SERVOHIGHERBOUND - SERVOLOWERBOUND) / ((getThermalPanBufferSize() / 4) - 1))){
      setThermalTiltBufferPointer(x * 4);
      setThermalPanBufferPointer(((SERVOHIGHERBOUND - i) / ((SERVOHIGHERBOUND - SERVOLOWERBOUND) / ((getThermalPanBufferSize() / 4) - 1))) * 4);
      movePan(i);
      delay(getThermalReadDelay());
      readThermalSensor();
      Serial.print("Reading ");
      Serial.println(i);
    }  
  } 
  outputThermalData(); //outputs thermal data in tabular format; for MP2 demo purposes only
  //outputThermalDataP();
  
 // getCoord();
  
  if(childSearch()){
    //CALL FOR HELP
    Serial.println("Person detected!");
//    outputThermalData();
//    delay(60000);  
  //  printCoord(); //FOR MP3 DEMO ONLY
    //contactEmerg();
  }
  else{
    Serial.println("Person not detected!");
    //DO MOTION DETECTION GARBAGE
    //printCoord(); //FOR MP3 DEMO ONLY
  }
}


