#include <SoftwareSerial.h>                             //WARNING: Do not use the last byte of any array
#define rxPin 10
#define txPin 11
SoftwareSerial SSerial(rxPin, txPin);                   //Define software serial pins

char Input;                                             //Input from bluetooth
char Output;                                            //Output from bluetooth
int InputHistory[16];                                   //Records the last 16 * 8 bits of data recieved from bluetooth
int Password[10];                                       //Password of 10 * 8 bits of length
char DCString[] = {"OK+LOST"};                          //String outputted by the HM-19 bluetooth module on device disconnect
int DCMatchCounter = 0;                                 //Counter
int PasswordUpdateNeeded = 1;                           //Bool
int LEDRelightNeeded = 1;                               //Bool
int ActionNeeded = 0;                                   //Bool
int DelayNeeded = 0;                                    //Bool
int Verification = 0;                                   //Bool
int AuthState = 0;                                      //Bool 0 = no access, 1 = access
int i = 0;                                              //Counter
int j = 0;                                              //Counter

void setup() {
  randomSeed(analogRead(A0));                           //Use the noise from an analog port as the seed for the random function
  Serial.begin(9600);                                   //Hardware serial, used for USB communication
  SSerial.begin(9600);                                  //Software serial, emulated hardware serial used for bluetooth communication
  for(i = 0; i < 16; i++){InputHistory[i] = 1;}         //Set the input history to a number or letter other than 0 or 1
  for(j = 2; j < 10; j++){pinMode(j, OUTPUT);}          //Set the LED pins to output
  for(j = 12; j < 14; j++){pinMode(j, OUTPUT);}         //Set the LED pins to output
  Serial.print("\n");
}

void loop(){
  if(PasswordUpdateNeeded == 1){
    PasswordUpdateNeeded = 0;
    LEDRelightNeeded = 1;
    for(i = 0; i < 10; i++){Password[i] = random(2);}   //Randomize the password
    //Serial.print("\nThe Password is ");
    //for(i = 0; i < 10; i++){Serial.print(Password[i]);} //Print the password to hardware serial
    //Serial.print("\n");
  }

  if(LEDRelightNeeded == 1){
    LEDRelightNeeded = 0;
    for(j = 2; j < 10; j++){digitalWrite(j, LOW);}      //Reset LED pins to low
    for(j = 12; j < 14; j++){digitalWrite(j, LOW);}     //Reset LED pins to low
    i = 0;
    for(j = 2; j < 10; j++){if(Password[i] == 1){digitalWrite(j, HIGH);}    //Set LED pins to high depending on the password
      i++;
    }
    for(j = 12; j < 14; j++){if(Password[i] == 1){digitalWrite(j, HIGH);}   //Set LED pins to high depending on the password
      i++;
    }
  }

  while(SSerial.available()){
    Input = SSerial.read();                                               //Get the input
    for(i = 0; i < 15; i++){InputHistory[i] = InputHistory[i+1];}         //Push the input history back by 1 byte
    InputHistory[15] = Input;                                             //Update latest input into input history
    ActionNeeded = 1;                                                     //Enable the command handler
    DelayNeeded = 1;
  }

  if(DelayNeeded == 1){
    DelayNeeded == 0;
    delay(0.5);                                                           //Delay of 0.5ms incase a string was inputted
  }

  while(Serial.available()){
    Output = Serial.read();                                              //Get the output
    SSerial.print(Output);                                               //Output the output to software serial
  }

  if(ActionNeeded == 1){
    ActionNeeded = 0;
    if(AuthState == 0){
      Verification = 1;                                   //Give temporary verification
      for(i = 0; i < 10; i++){
        if (InputHistory[i + 6] - '0' != Password[i]) {   //Check if the last 10 items in the input history are equal to the password
          Verification = 0;                               //If any of the last 10 are different from the password, remove the verification
        }
      }
      if(Verification == 1){
        SSerial.print("Access Granted");
        Serial.print("\nAccess Granted");
        AuthState = 1;                                     //Enable authenticated permissions
        PasswordUpdateNeeded = 1;                          //Get new password
        Verification = 0;                                  //Line does nothing but here for safety incase code breaks
      }
    }
    
    else if(AuthState == 1){
      for(i = 0; i < 8; i++){
        if (InputHistory[i + 8] == DCString[i]) {
          DCMatchCounter++;
        }
      }
      if(DCMatchCounter >= 7){
        Serial.print("\nDevice disconnected");
        AuthState = 0;
      }
      DCMatchCounter == 0;
      if(AuthState == 1){                                   //Authentication might have been lost so therefore reverify
        Serial.print(Input);                                //Output the incoming inputs to hardware serial
      }
    }
  }
}