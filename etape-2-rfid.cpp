//============================================================================
// Name        : etape-2-rfid.cpp
// Author      : EQUIPE 5 - EPSI - I1 (DALENS, EFFROY, AFRI, TISSERAND)
// Copyright   : 2021
// Description : Led indication paired with rfid reader - red signalement if
//               card unknown, green otherwise.
//============================================================================

#include "CMFRC522.h"
#include "pigpio.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <string>
#include <wiringPi.h>
#include <csignal>

using namespace std;

// global flag used to exit from the main loop
bool RUNNING = true;

void light_both(int led1, int led2, int time) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    delay(time);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
}

void detection(int led, int time) {
    for(int i=0;i<10;i++) {
        digitalWrite(led, HIGH);
        delay(time);
        digitalWrite(led, LOW);
        delay(time);
    }
}

int main(int argc, char *argv[]) {
const std::string goodCard  = "72179213";
std:string tmpCard = "";
gpioInitialise();
wiringPiSetupGpio();

	CMFRC522 mfrc522;

    // Setup the pins
    int red = 3, green = 2;

    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);

    // Initializes PCD
    mfrc522.PCD_Init();
    std::cout << "Mise en marche" << std::endl;
    while(1) {
        if (mfrc522.isNewCardPresent()==0) { // Detect whether a new card is present
            std::cout << "New card is here..." << std::endl;
            mfrc522.RC522_Anticoll(mfrc522._id);
            tmpCard = "";
            for (uint8_t i = 2; i < 5; i++) {
                std::cout<<int(mfrc522._id[i])<<"";
                tmpCard = tmpCard + std::to_string(int(mfrc522._id[i]));    
            
            }
            if(tmpCard == goodCard){
                std::cout << "C'est la bonne carte" << std::endl;
                light_both(red, green, 500);
                detection(green, 50);
            }else{
                light_both(red, green, 500);
                detection(red, 50);
            }
        }

    }
}