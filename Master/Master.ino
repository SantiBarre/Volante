#include<SoftwareSerial.h>
#include<TimerOne.h>

SoftwareSerial sw(9,8);   //rx, tx

#define maxbat 12       //Maximo voltaje de la bateria        
#define minbat 11.6     //Minimo voltaje de la bateria



int volatile Values[4] = { 
  0,   //Bateria
  0,   //Velocimetro
  0,   //Voltimetro
  0   //Amperimetro
  };
int volatile Index_Select = 0;
/*
Nazareno Molinari 
Ultima modificacion: 02/07/2020
Este codigo implementa el metodo final de comunicacion bluetooth con el telefono y funciona de la siguiente forma con la libreria
timerOne genera una interrupcion virtual cada 250 ms y envia por bluetooth 4 variables del tipo int (No se pueden enviar valores negativos)
Estas mismas variables forman un vector de 4 posiciones, este metodo junto con su vector se puede reciclar en cualquier codigo, y se puede manipular
este vector con sus variables como a uno se le de la gana.
La funcion del bluetooth cuenta con su propio metodo de limitar las variables (Para que se puedan mostrar bien en pantalla), 
en Regulated_values, habria que modificar estas limitaciones segun las necesidaes que se presenten.
*/

void setup() {
  Serial.begin(115200);
  sw.begin(9600);
  Timer1.initialize(); 
  Timer1.attachInterrupt(timerInterrupt, 250000);   //Interrupciones virtuales
  inicia_velocimetro();
}

volatile int Encoder_value = 0;


void timerInterrupt () {     //Cada 250 ms se dedica a llamar al metodo update_values_bluetooth();
  update_values_bluetooth();
  }


void loop() {
Battery_test_actions();
Read_bluetooth_signals_and_check_conecction();

//Insertar nuevo codigo abajo
int volatile Values[4] = { 
  0,   //Bateria
  0,   //Velocimetro
  0,   //Voltimetro
  0   //Amperimetro
  };
Values[0] = vol();
Values[1] = Velocimetro();
Values[2] = vol();
Values[3] = amp();
}


void update_values_bluetooth(){
    const int Regulated_value[4] = {
      map(Values[0], minbat, maxbat, 0, 300),
      map(Values[1], 0, 1024, 0, 240),
      map(Values[2], 0, 1024, 0, 60),
      map(Values[3], 0, 1024, 0, 60)
      };
    int bytes[2] = {0,0};
    bytes[0] =  Regulated_value[0] & 0xFF;
    bytes[1] = ( Regulated_value[0] >> 8) & 0xFF;
    sw.write(bytes[0]);
    sw.write(bytes[1]);
    
    bytes[0] =  Regulated_value[1] & 0xFF;
    bytes[1] = ( Regulated_value[1] >> 8) & 0xFF;
    sw.write(bytes[0]);
    sw.write(bytes[1]);

    bytes[0] =  Regulated_value[2] & 0xFF;
    bytes[1] = ( Regulated_value[2] >> 8) & 0xFF;
    sw.write(bytes[0]);
    sw.write(bytes[1]);

    bytes[0] =  Regulated_value[3] & 0xFF;
    bytes[1] = ( Regulated_value[3] >> 8) & 0xFF;
    sw.write(bytes[0]);
    sw.write(bytes[1]);
  }
