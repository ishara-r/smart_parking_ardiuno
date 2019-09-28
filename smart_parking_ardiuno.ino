// Include the Servo library 


/*Display*/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
/*Display end*/



#include <Servo.h> 
 
/* Port assign */
int rightRoadLedPin = A2;
int leftRoadLedPin = A3;

int entranceIr= 2; 
int entranceServoPin = 3; 

int parking1IRPin=4;
int parking1LEDPin=5;

int parking2IRPin = 6;
int parking2LEDPin = 7;

int parking3IRPin = 8;
int parking3LEDPin = 9;

int parking4IRPin = 11;
int parking4LEDPin = 12;


int exitServoPin=A1;
int exitIrPin=13;

/* Port assign end */


//initialize parking spaces
boolean parking[4]={false, false, false, false};

// Create a servo objects
Servo Servo1; 
Servo Servo2; 

void setup() { 
   
   Servo1.attach(entranceServoPin); 
   Servo2.attach(exitServoPin); 

   
   Serial.begin(9600);

  //entrance
  pinMode(entranceIr,INPUT);
  //exit
  pinMode(exitIrPin, INPUT);

  // right side road led
  pinMode(rightRoadLedPin, OUTPUT);
  // left side road led
  pinMode(leftRoadLedPin, OUTPUT);
  
  // Parking slot 1 
  pinMode(parking1LEDPin, OUTPUT);
  pinMode(parking1IRPin, INPUT);

  // Parking slot 2
  pinMode(parking2LEDPin, OUTPUT);
  pinMode(parking2IRPin, INPUT);
  
  // Parking slot 3
  pinMode(parking3LEDPin, OUTPUT);
  pinMode(parking3IRPin, INPUT);

  // Parking slot 4 
  pinMode(parking4LEDPin, OUTPUT);
  pinMode(parking4IRPin, INPUT);

  
  


  //Display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

}
void loop(){ 
  for ( int i = 0; i < 4; ++i ) {
        
        if(parking[i]==false){
          if(i==0){
            testdrawchar("Slot 1");
            break;
          }else if(i==1){
            testdrawchar("Slot 2");
            break;
          }else if(i==2){
            testdrawchar("Slot 3");
            break;
          }else if(i==3){
            testdrawchar("Slot 4");
            break;
          }
        }  
        if(i==3){
          if(parking[3]==true){
            testdrawchar("Full");
          }
        }        
   }

   
  char state=HIGH;
  if(digitalRead(entranceIr)==0){
    if(getAvailableSlotCount()>0){
      Servo1.write(0); 
    }
     
     for ( int i = 0; i < 4; ++i ) {
        if(parking[i]==false){
          if(i==0){
            digitalWrite(parking1LEDPin, state);
            digitalWrite(rightRoadLedPin, state);
            delay(2000); 
            break;
          }else if(i==1){
             digitalWrite(parking2LEDPin, state);
             digitalWrite(rightRoadLedPin, state);
             delay(2000);
             break;
          }else if(i==2){
            digitalWrite(parking3LEDPin, state);
            digitalWrite(leftRoadLedPin, state);
            delay(2000);
            break;
          }else if(i==3){
            digitalWrite(parking4LEDPin, state);
            digitalWrite(leftRoadLedPin, state);
            delay(2000);
            break;
          }
        }
      }
  }else{
      Servo1.write(90); 
  } 

  
  if(digitalRead(parking1IRPin)==0){
    digitalWrite(parking1LEDPin, !state);
    if(parking[0]==false){
      digitalWrite(rightRoadLedPin, !state);  
    }
    parking[0]=true;
  }else{
    parking[0]=false;    
  }
  
  if(digitalRead(parking2IRPin)==0){
    digitalWrite(parking2LEDPin, !state);
    if(parking[1]==false){
      digitalWrite(rightRoadLedPin, !state);  
    }
    parking[1]=true;
  }else{
    parking[1]=false;    
  }

  if(digitalRead(parking3IRPin)==0){
    digitalWrite(parking3LEDPin, !state);
    if(parking[2]==false){
      digitalWrite(leftRoadLedPin, !state);  
    }
    parking[2]=true;
  }else{
    parking[2]=false;    
  }

   if(digitalRead(parking4IRPin)==0){
    digitalWrite(parking4LEDPin, !state);
    if(parking[3]==false){
      digitalWrite(leftRoadLedPin, !state);  
    }
    parking[3]=true;
  }else{
    parking[3]=false;    
  }

  //exit
  if(digitalRead(exitIrPin)==0){
    Servo2.write(0); 
  }else{
    Servo2.write(90); 
  }
}

int getAvailableSlotCount(){
  int availableSlotCount=0;

  for ( int i = 0; i < 4; ++i ) {
    if(parking[i]==false){
        availableSlotCount=availableSlotCount+1;
    }  
  }
  return availableSlotCount;
}

void testdrawchar(String message) {
  display.clearDisplay();

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Welcome"));
  
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.println(F(""));
  
  display.setTextSize(1);   

  int availableSlotCount = getAvailableSlotCount();
  display.print(availableSlotCount);
  display.println(" slots available");

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.println(F(""));

  display.setTextColor(WHITE);        // Draw white text
  display.setTextSize(1);
  display.print(F("  "));
  display.setTextSize(3);             // Draw 2X-scale text

  display.println(message);

  display.display();
  delay(2000);

}
