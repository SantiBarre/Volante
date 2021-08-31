#include<SoftwareSerial.h>
#include<TimerOne.h>

SoftwareSerial sw(9,8);   //rx, tx

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
  
}

volatile int Encoder_value = 0;
boolean Activate = false;

void timerInterrupt () {     //Cada 250 ms se dedica a llamar al metodo update_values_bluetooth();
  update_values_bluetooth();
  }

  
boolean discrement = false;
unsigned long Ult_increment;

void loop() {
Battery_test_actions();
Read_bluetooth_signals_and_check_conecction();

//Insertar nuevo codigo abajo


}


void Battery_test_actions(){    //Este metodo unicamente se dedica a sumarle o restarle gradualmente todas las variables a la vez cada 25 milisegundos.
  if(Activate){
  if(millis() - Ult_increment > 25){
  if(!discrement){
    Values[0]+=1;
    Values[1]+=2;
    Values[2]+=2;
    Values[3]+=2;
    } else {
      Values[0]-=1;
      Values[1]-=2;
      Values[2]-=2;
      Values[3]-=2;
      }

   if(discrement && Values[1] == 0) {
    discrement = false;
    } else if(!discrement && Values[1] == 1024) {
    discrement = true;  
      }
      Ult_increment = millis();
  } }
  }

unsigned long Ultime_check_connection_time = 0;
const int Init_read = 80;
const int Stop_read = 60;
const int Cheack_connect_read = 40;
void Read_bluetooth_signals_and_check_conecction(){
  if(sw.available() > 0) {
    int Bluetooth_read = sw.read();
    Serial.println(Bluetooth_read);
    if(Bluetooth_read == Init_read){
      Activate = true;
      Ultime_check_connection_time = millis();
      }

    if(Bluetooth_read == Stop_read){
      Activate = false;
      }
      
    if(Bluetooth_read == Cheack_connect_read){
      if(Activate){
        Ultime_check_connection_time = millis();
        }
      }
    }
    if(Activate && millis() - Ultime_check_connection_time > 2000) {   //Basicamente si durante 2 segundos no recibio el codigo de conexion es que perdio la conexion
      Serial.println("Conexion perdida");
      Activate = false;
      }
  }

void update_values_bluetooth(){
    const int Regulated_value[4] = {
      map(Values[0], 0, 512, 0, 300),
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
