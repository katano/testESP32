#include <M5Stack.h>
#include <WiFi.h>

#define num 10000


void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    while(!Serial){
        ; // wait for serial port to connect  
    }

    M5.begin();
    
    xTaskCreatePinnedToCore(
        testAtan2
        ,  "testAtan2"   // A name just for humans
        ,  4096  // This stack size can be checked & adjusted by reading the Stack Highwater
        ,  NULL
        ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,  NULL 
        ,  0);   
}



void loop() {
  // put your main code here, to run repeatedly:

}





static inline float _atan2(float _y, float _x){
    float x = abs(_x);
    float y = abs(_y);
    float z;
    bool c;

    
    if(y < x){
      z = y/x;
      c = true;
    }else{
      z = x/y;
      c = false;
    }
    float a = 8.0928*z*z*z*z-19.657*z*z*z-0.9258*z*z+57.511*z-0.0083;
    if(_x == 0.0){
        if(_y > 0.0)    a = 90.0;
        else            a = -90.0;
    }
    if(c){    // a<1
        if(_x > 0.0){
            if(_y < 0.0)  a *= -1.0;
        }
        if(_x < 0.0){
            if(_y > 0.0)  a = 180.0 - a;
            if(_y < 0.0)  a = a - 180.0;
        }
    }
    if(!c){   // a>1
        if(_x > 0.0){
            if(_y > 0.0)  a = 90.0 - a;
            if(_y < 0.0)  a = a - 90.0;
        }
        if(_x < 0.0){
            if(_y > 0.0)  a = a + 90.0;
            if(_y < 0.0)  a = -a - 90.0;   
        }
    }
    return a/180.0;
}




void testAtan2(void *pvParameters){
    unsigned long tl, tn, tm;

    bool flg = false;
    disableCore0WDT();
    for(;;){
        if(flg){
            Serial.print("  _atan2  ");
            tn = micros();
            for(int i = 0; i < 100000; i ++){
                for(int j = 0; j < num; j ++){
                    float val = (float)i / 1000.0;
                    float rad = _atan2(0.5, val);
                }
            }
            M5.Lcd.setCursor(0, 40);
        }else{
            Serial.print("   atan2  ");
            tn = micros();
            for(int i = 0; i < 100000; i ++){
                float val = (float)i / 1000.0;
                float rad = atan2(0.5, val);
            }
            M5.Lcd.setCursor(0, 20);
        }
        tm = micros();
        tl = tm-tn;        
        M5.Lcd.printf("%lu  ", tl);
        Serial.println(tl);
        flg = !flg;
        M5.update();
            
    }
}
