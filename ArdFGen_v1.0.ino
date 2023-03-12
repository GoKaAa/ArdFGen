/*
 * ArdFGen (Arduino Based Function Generator)
 * Copyright (c) 2022, ASLab. All Rights Reserved.
 * 
 * Max Freq -> 1 MHz
 */
#include <LiquidCrystal.h> //Liquid Crystal Library

//Liquid Crystal Library Settings
const int rs = 9, en = 11, d4 = 12, d5 = 13, d6 = SDA1, d7 = SCL1;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int BLEN = 8; //Black Light Enable.
int LPEN = 10; //LCD Display Power Enable

//Serial UART Debug Port Configuration
#define ARDFGEN_DEBUG Serial
#define ARDFGEN_BAUD_BPS 115200

//Function Generator Settings
int FGen_Out            = 7;          //Function Generator Output Pin
float t                 = 0.00;        //Time (pW+sW)
float pWus              = 0.00;        //Pulse Width in ms
float sWus              = 0.00;        //Space Width in ms
float pW                = 0.00;        //Pulse Width in s
float sW                = 0.00;        //Space Width in s
float f                 = 0.00;        //Frequency

void init_signal() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("AfG     Signal on D7");
  lcd.setCursor(0, 1);
  lcd.print("pW(us) - ");
  lcd.setCursor(10, 1);
  lcd.print(pWus);
  lcd.setCursor(0, 2);
  lcd.print("sW(us) - ");
  lcd.setCursor(10, 2);
  lcd.print(sWus);
  lcd.setCursor(0, 3);
  lcd.print("Freq - ");
  lcd.setCursor(12, 3);
  lcd.print(f);
  digitalWrite(FGen_Out, HIGH);
  delayMicroseconds(pWus);
  digitalWrite(FGen_Out, LOW);
  delayMicroseconds(sWus);
  init_signal();
}

void setup() {
  pinMode(FGen_Out, OUTPUT);
  pinMode(BLEN, OUTPUT);
  pinMode(LPEN, OUTPUT);
  digitalWrite(BLEN, HIGH);
  digitalWrite(LPEN, HIGH);
  lcd.begin(20, 4);
  ARDFGEN_DEBUG.begin(ARDFGEN_BAUD_BPS);
  ARDFGEN_DEBUG.println("ArdFGen v1.0 Started!");
  lcd.setCursor(0, 1);
  lcd.print("ArdFGen v1.0 Started");
  delay(1000);
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("AfG        No Signal");
  lcd.setCursor(0, 1);
  lcd.print("pW(us) - ");
  lcd.setCursor(10, 1);
  lcd.print(pWus);
  lcd.setCursor(0, 2);
  lcd.print("sW(us) - ");
  lcd.setCursor(10, 2);
  lcd.print(sWus);
  lcd.setCursor(0, 3);
  lcd.print("Freq(Hz) - ");
  lcd.setCursor(12, 3);
  lcd.print(f);
  delay(10);
  if (ARDFGEN_DEBUG.available()) 
        {   
        String ARDFGEN_INPUT = ARDFGEN_DEBUG.readString();
        ARDFGEN_INPUT.trim();
        if (ARDFGEN_INPUT.equals("set pulse-w")){
          ARDFGEN_DEBUG.println("");
          ARDFGEN_DEBUG.print("Please enter your Signal's Pulse Width (in us) -> ");
          while(ARDFGEN_DEBUG.available() == 0) {
            
          }
          int gpW = ARDFGEN_DEBUG.parseInt();
          pWus    = gpW;
          pW      = pWus/1000000;
          t = pW+sW;
          f = 1/t;
          ARDFGEN_DEBUG.print(pWus);
          ARDFGEN_DEBUG.println(" us (Micro Seconds)");
          ARDFGEN_DEBUG.print("Logical Pulse Width of your Signal in Seconds (pW(us)/(10^6)) -> ");
          ARDFGEN_DEBUG.print(pW);
          ARDFGEN_DEBUG.println(" s (Seconds)");
     }
     if (ARDFGEN_INPUT.equals("set space-w")){
          ARDFGEN_DEBUG.println("");
          ARDFGEN_DEBUG.print("Please enter your Signal's Space Width (in us) -> ");
          while(ARDFGEN_DEBUG.available() == 0) {
            
          }
          int gsW = ARDFGEN_DEBUG.parseInt();
          sWus    = gsW;
          sW      = sWus/1000000;
          t = pW+sW;
          f = 1/t;
          ARDFGEN_DEBUG.print(sWus);
          ARDFGEN_DEBUG.println(" us (Micro Seconds)");
          ARDFGEN_DEBUG.print("Logical Space Width of your Signal in Seconds (sW(us)/(10^6)) -> ");
          ARDFGEN_DEBUG.print(sW);
          ARDFGEN_DEBUG.println(" s (Seconds)");
     }
     if (ARDFGEN_INPUT.equals("init sig")){
          ARDFGEN_DEBUG.println("");
          ARDFGEN_DEBUG.print("Initializing your Signal...");
          init_signal();
     }
   }
}
