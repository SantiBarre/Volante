#define sensor_vol A1
#define interval  1000 
#define res 16             //multiplicador de resistencias R2/(R1+R2)
unsigned long previousMillis2 = 500;        // Para que lea con diferencia de tiempo con respecto a amp
float v_mili2 = 0;
float volt = 0;

float vol(){

    unsigned long currentMillis2 = millis();
    
    if (currentMillis2 - previousMillis2 >= interval) {
         
          previousMillis2 = currentMillis2;
          v_mili2 = (analogRead(sensor_vol) * 5000 ) / 1023;
          volt = v_mili * res;
          return volt;
    }

}
