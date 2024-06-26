/*
    Modify the values according to your LoRa configuration
    - Make a copy of this file as ./include/secrets.h
    - Do not forget to add ./include/secrets.h to .gitignore
    - To use the value in for ex. main.cpp, add #include "secrets.h"
    
    (i.e. just "secrets.h" and not "include/secrets.h". 
    The .include folder is in the include patha allready.)
*/ 

#define DEV_EUI 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC
#define APP_EUI 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC
#define APP_KEY 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC

#define WIFI_SSID 'mySSID'
#define WIFI_PASS 'pass.word'