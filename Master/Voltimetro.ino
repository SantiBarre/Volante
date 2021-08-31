#define sensor_vol A1
#define interval  1000 
#define res 16             //multiplicador de resistencias R2/(R1+R2)


unsigned long previousMillis2 = 500;        // Para que lea con diferencia de tiempo con respecto a amp
unsigned int lectura_vol = 0;
float volt_vol = 0;
float volt_fin = 0;
byte bateria = 0;

float vol(){

    unsigned long currentMillis2 = millis();
    
    if (currentMillis2 - previousMillis2 >= interval) {
         
          previousMillis2 = currentMillis2;
          
          lectura_vol = analogRead(sensor_vol);
          volt_vol = lectura_vol * 0.004887;
          volt_fin = volt_vol * res;

          return volt_fin;
    }

}
