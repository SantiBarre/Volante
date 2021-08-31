#define sensor_amp  A0              //PIN DE AMPERIMETRO
#define valor_sensor 2.500           //Valor del sensor sin corriente en mili voltios entre (2.350(min)/2.500(tipo)/2.650(max))
#define ajuste_sensor 0.066          //Valor de sensivilidad del sensor entre (0.056(min)/0.066(tipico)/0.076(max))

float corrientesuma = 0;
float corriente = 0;
float volt_amp = 0;
unsigned int lectura_amp = 0;
int x = 0;

float amp(){
 
  for (x=0; x < 100; x++) {
    
    lectura_amp = analogRead(sensor_amp);
    volt_amp = lectura_amp * 0.004887;
  
    if (volt_amp > valor_sensor){
      corrientesuma += ((volt_amp - valor_sensor) / ajuste_sensor);
    }else{
        corrientesuma += 0;
    }
    
  }
  
  corriente = (corrientesuma / 100);
  corrientesuma = 0;
  if (corriente <= 0.5) {
    corriente = 0;
  }
  return corriente;
}
