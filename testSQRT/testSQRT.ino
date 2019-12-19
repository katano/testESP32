#include <M5Stack.h>
#include <WiFi.h>


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




static inline float _sqrtinv(float a){

    float x, h, g;
    int e;

    // enhalf the exponent for a half digit initial accuracy 
    frexp(a, &e);
    x = ldexp(1.0, -e >> 1);
//    Serial.printf("    a=%6.4f  e = %6.4f   x = %6.4f \n", a, e, x);
    
    // 1/sqrt(a) 4th order convergence
    g = 1.0;
    while(fabs(h=1.0-a*x*x) < fabs(g)){
        x += x * (h * (8.0 + h * (6.0 + 5.0 * h)) / 16.0);
        g = h;
//        Serial.printf("     G=%6.4f  x = %6.4f \n", g, x);
    }
    return(x);
}

static inline float _sqrt(float a){
    float ret;

    if(a < 0){
        return 0.0;
    }
    ret = a * _sqrtinv(a);
//    Serial.println(ret);
    
    return (ret);
}







void testAtan2(void *pvParameters){
    unsigned long tl, tn, tm;

    bool flg = true;
    disableCore0WDT();
    for(;;){
        if(flg){
            Serial.print("   sqrt  ");
            tn = micros();
            for(int i = 0; i < 100000; i ++){
                float rad = sqrt(double(i));
            }
            M5.Lcd.setCursor(0, 20);
        }else{
            Serial.print("  _sqrt  ");
            tn = micros();
            for(int i = 0; i < 100000; i ++){
                float rad = _sqrt(double(i));
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
