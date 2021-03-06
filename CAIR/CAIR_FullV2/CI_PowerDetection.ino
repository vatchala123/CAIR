//reads the voltage coming from the car battery to determine whether the car is on

#define CARPIN A9
#define RLARGE 299500
#define RSMALL 98200
#define VADJUST 0.00
#define CARPOWERSHUTOFFPIN 42
#define MOSFETON 1
#define MOSFETOFF 0
#define VOLTAGETHRESHOLD 13.25
#define CARSTATPIN 40

double carVoltage;
bool carVoltageStatus;
int carVoltageReadDelay = 100;

void powerDetectionSetup(){
  pinMode(CARPIN, INPUT);
  pinMode(CARPOWERSHUTOFFPIN, OUTPUT);
  pinMode(CARSTATPIN, OUTPUT);
  controlPower(checkCarVoltageStatus()); 
  if(getCarVoltageStatus()){ //accounts for if car is on upon activation of CAIR
    digitalWrite(CARPOWERSHUTOFFPIN, MOSFETON);
    return;
  }
  digitalWrite(CARPOWERSHUTOFFPIN, MOSFETOFF); //if car was off continue to operate CAIR normally and don't jump to child identification
}

//Gets and sets
double getCarVoltage(){
  return carVoltage; 
}

void setCarVoltage(double i){
  carVoltage = i; 
}

void setCarVoltageReadDelay(int i){
  carVoltageReadDelay = i; 
}

int getCarVoltageReadDelay(){
  return carVoltageReadDelay; 
}

void setCarVoltageStatus(bool s){
  carVoltageStatus = s;
  digitalWrite(CARSTATPIN, s);
}

bool getCarVoltageStatus(){
  return carVoltageStatus; 
}

//Most important functions
bool checkCarVoltageStatus(){ //Return values: 1 car is on, 0 car is off.
  if(getCarVoltageStatus()){ //if 1, car was on. If 0, car was off.
    for(int i = 0; i < 45; i++){
      readCarVoltage();
      if(getCarVoltage() >= VOLTAGETHRESHOLD){
        setCarVoltageStatus(1);
        return 1; 
      }
      delay(getCarVoltageReadDelay());
    }
    controlPower(MOSFETOFF);
    setCarVoltageStatus(0);
    return 0; 
  }
  else{
    for(int i = 0; i < 45; i++){
      readCarVoltage();
      if(getCarVoltage() < VOLTAGETHRESHOLD){
        setCarVoltageStatus(0);
        return 0; 
      }
      delay(getCarVoltageReadDelay());
    }
    controlPower(MOSFETON);
    setCarVoltageStatus(1);
    return 1; 
  }
} 

void controlPower(bool stat){ //shuts off car power from UPS when car is off
  if(stat == 0){
    digitalWrite(CARPOWERSHUTOFFPIN, LOW);
  }
  else if(stat == 1){
    digitalWrite(CARPOWERSHUTOFFPIN, HIGH);
  }
}

//Mildly useful functions
void readCarVoltage(){
  setCarVoltage(analogRead(CARPIN));
//  Serial.println("ANALOGREAD: "); //testing
//  Serial.println(getCarVoltage()); 
//  delay(1000);
//  setCarVoltage(map(carVoltage, 0, 1023, 0, 5000)); //map uses integer values. 3 decimal points are needed so the mapped-to value is scaled up by 1000
//  Serial.println("MAP: "); //testing
//  Serial.println(getCarVoltage()); 
//  delay(1000);
//  setCarVoltage((getCarVoltage()/(double)1000)*(double)4); //scales voltage back down (e.g. 3125 -> 3.125) and converts to voltage at car battery (0-20)
  setCarVoltage((map(getCarVoltage(), 0, 1023, 0, 5000)/(double)1000)*(double)(RLARGE + RSMALL)/(double)RSMALL + VADJUST); //scales voltage back down (e.g. 3125 -> 3.125) and converts to voltage at car battery (0-20). Also acounts for error)
}

//Testing
void displayCarVoltage(){
   Serial.print("Car voltage is currently: ");
   Serial.print(getCarVoltage());
   Serial.println("V"); 
}

int testPowerDetection(){
  while(!checkCarVoltageStatus()){ //waiting for car to turn on
    Serial.println("Car is OFF");
    displayCarVoltage();
    delay(1000);
  }  
  while(checkCarVoltageStatus()){ //waiting for car to turn off
    Serial.print("Car is ON");
    displayCarVoltage();
    delay(1000);
  }  
  Serial.print("Car is OFF");
  displayCarVoltage();
  delay(1000);
  return 0;
}

void testReadVoltage(){
  readCarVoltage();
//  Serial.print("Divided Voltage is currently: ");
//  Serial.print(getCarVoltage()*(double)RSMALL/(double)(RSMALL+RLARGE));
  displayCarVoltage();
}
