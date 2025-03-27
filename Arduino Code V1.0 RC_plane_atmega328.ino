//Project created by Ghaith Mhamdi
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
RF24 radio(7, 8);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001";
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;
Servo servoR;
Servo servoL;
Servo servoB;

int escValue, servoRValue, servoLValue, servoBValue;
// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte j1PotX;
  byte j1PotY;
  byte j1Button;
  byte j2PotX;
  byte j2PotY;
  byte j2Button;
  byte pot1;
  byte pot2;
  byte tSwitch1;
  byte tSwitch2;
  byte button1;
  byte button2;
  byte button3;
  byte button4;
};
Data_Package data; //Create a variable with the above structure
void setup() {
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening(); //  Set the module as receiver
  resetData();
  servoR.attach(9);
  servoL.attach(5);
  servoB.attach(10);
}
void loop() {
  // Check whether we keep receving data, or we have a connection between the two modules
  currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone jas a throttle up, if we lose connection it can keep flying away if we dont reset the function
  }
  // Check whether there is data to be received
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    lastReceiveTime = millis(); // At this moment we have received the data
  }
  

//Pitch*****************************
if(data.j2PotY<=85){
  servoRValue = map(data.j2PotY, 85, 0, 90, 0);
  servoLValue = map(data.j2PotY, 85, 0, 90, 180);
}
else if(data.j2PotY>=170){
  servoRValue = map(data.j2PotY, 170, 255, 90, 180);
  servoLValue = map(data.j2PotY, 170, 255, 90, 0);
}

//Roll*****************************
if(data.j2PotX<=85){
  servoRValue = map(data.j2PotX, 85, 0, 90, 0);
  servoLValue = map(data.j2PotX, 85, 0, 90, 0);
}
else if(data.j2PotX>=170){
  servoRValue = map(data.j2PotX, 170, 255, 90, 180);
  servoLValue = map(data.j2PotX, 170, 255, 90, 180);
}
if ((data.j2PotX<170)&&(85<data.j2PotX)&&(data.j2PotY<170)&&(85<data.j2PotY)){
  servoRValue = 90;
  servoLValue = 90;
}


//Yaw*******************************
if(data.j1PotX<=85){
  servoBValue = map(data.j1PotX, 85, 0, 90, 180);
}
else if(data.j1PotX>=170){
servoBValue = map(data.j1PotX, 170, 255, 90, 36);
}
else{
  servoBValue = 90;
}

  
  servoR.write(servoRValue);
  servoL.write(servoLValue);
  servoB.write(servoBValue);
  analogWrite(6,data.pot2);
}
void resetData() {
  // Reset the values when there is no radio connection - Set initial default values
  data.j1PotX = 127;
  data.j1PotY = 127;
  data.j2PotX = 127;
  data.j2PotY = 127;
  data.j1Button = 1;
  data.j2Button = 1;
  data.pot1 = 17;
  data.pot2 = 1;
  data.tSwitch1 = 1;
  data.tSwitch2 = 1;
  data.button1 = 1;
  data.button2 = 1;
  data.button3 = 1;
  data.button4 = 1;
}
