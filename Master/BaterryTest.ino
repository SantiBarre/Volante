boolean discrement = false;
unsigned long Ult_increment;
boolean Activate = false;


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
