#include <M5Stack.h>
#include <WiFi.h>



#define num 10000
static float tableCos[num];



void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    while(!Serial){
        ; // wait for serial port to connect  
    }

    M5.begin();
    init_table();


//    for(int i = 0; i < num; i ++){
//        float rad = float(i)*PI*2.0 / float(num);
//        float c1 =  cos(rad);
//        float c2 = _cos(rad);
//        Serial.printf(" %6.4f : %6.4f, %6.4f   %6.4f \n", rad, c1, c2, c2-c1);
//    }

    
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



void init_table(){
    for(int i = 0; i < num; i ++){
        tableCos[i] = cos(float(i) * PI / 2.0 / float(num));
    }
}
float _cos(float rad){
    float sign = 1.0;
    int i;

    if(rad < 0.0) rad = -rad;
    rad = rad/PI;
    rad = (rad - int(rad/2.0)*2.0);

    if(rad > 1.0) {
        rad = 2.0 - rad;
    }
    if(rad > 0.5){
        rad = 1.0 - rad;
        sign = -1;
    }
    i = int(rad * 2.0 * num);

    return(sign * tableCos[i]);
}



void testAtan2(void *pvParameters){
    unsigned long tl, tn, tm;

    bool flg = true;
    disableCore0WDT();
    for(;;){
        if(flg){
            Serial.print("   cos  ");
            tn = micros();
            for(int i = 0; i < num; i ++){
                float val = cos(float(i)*PI*2.0/float(num));
            }
            M5.Lcd.setCursor(0, 20);
        }else{
            Serial.print("  _cos  ");
            tn = micros();
            for(int i = 0; i < num; i ++){
                float val = _cos(float(i)*PI*2.0/float(num));
            }
            M5.Lcd.setCursor(0, 40);
        }
        tm = micros();
        tl = tm-tn;        
        M5.Lcd.printf("%lu  ", tl);
        Serial.println(tl);
        flg = !flg;
        M5.update();
            
    }
}
