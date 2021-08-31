#define sensor_velocidad  3            //PIN DE VELOCIMETRO (con interrupciones)
#define perimetro  2.2    //PERIMETRO DE LA RUEDA en m
#define interval 1000     //Tiempo en dar un valor

float velocidadms = 0;       //Velocidad redondeada
unsigned int pulsos = 0; 
byte velocidad = 0;

unsigned long previousMillis = 0; //Inicializa los millis

void inicia_velocimetro(){
pinMode(sensor_velocidad, INPUT); //pin de velocimetro
attachInterrupt(digitalPinToInterrupt(sensor_velocidad),cont, FALLING);
}
void cont(){
  pulsos++;
  }
int Velocimetro() {
  
    unsigned long currentMillis = millis();
  
    if (currentMillis - previousMillis >= interval) {
    
        previousMillis = currentMillis;               //Funcional para la repeticion
        velocidadms = ((pulsos/4) * perimetro) / (interval/1000);     // Multiplico cantidad de vueltas por el perimero
                                                       // Y dibido por la cantida de tiempo que le tomo hacer esa distacia
        velocidad = round(velocidadms * 3.600);
        pulsos = 0;                              //Reset de los pulsos
        return velocidad;             //Me da la velocidad
    }
}
