#define sensor_velocidad  2            //PIN DE VELOCIMETRO (con interrupciones)
#define perimetro  0.8    //PERIMETRO DE LA RUEDA en m
#define interval 1000     //Tiempo en dar un valor
byte velocidadms = 0;       //Velocidad redondeada
byte pulsos = 0; 
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
        velocidadms = (pulsos * perimetro) / (interval/1000);     // Multiplico cantidad de vueltas por el perimero
                                                       // Y dibido por la cantida de tiempo que le tomo hacer esa distacia
        velocidad = round(velocidadms * 3.6);
        pulsos = 0;                              //Reset de los pulsos
        return velocidad;             //Me da la velocidad
    }
}
