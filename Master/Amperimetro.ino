#define sensor_amp  A0              //PIN DE AMPERIMETRO
#define valor_sensor 2500           //Valor del sensor sin corriente en mili voltios entre (2350(min)/2500(tipo)/2650(max))
#define ajuste_sensor 66          //Valor de sensivilidad del sensor entre (56(min)/66(tipico)/76(max))

unsigned int v_mili = 0;
float corrientesuma = 0;
float corriente = 0;
int x = 0;

float amp(){
 
  for (x=0; x < 100; x++) {
    
    v_mili = (analogRead(sensor_amp) * 5000 ) / 1023;
    corrientesuma += ((v_mili - valor_sensor) / ajuste_sensor);
    
  }
  corriente = (corrientesuma / 100);
  corrientesuma = 0;
  if (corriente <= 0.5) {
    corriente = 0;
  }
  return corriente;
}
