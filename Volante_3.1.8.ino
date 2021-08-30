#include "U8glib.h"
U8GLIB_ST7920_128X64_1X u8g(12, 11, 10);	// SPI Com: SCK = en = 12, MOSI = rw = 11, CS = di = 10
//Cronometro
unsigned long Anterior = 0;
int Horas, Minutos, Segundos, estado = 0, InicioCrn, cont = 0;
const int BotonCrn = 4;
//
//Voltimetro
const int Volt1 = A1; //PIN DE VOLTIMETRO
int Vmin = 42, Vmax= 65, valorVmin = ((Vmin * 83.466) * 0.2046), valorVmax = ((Vmax * 83.466) * 0.2046); //VALORES MMIN Y MAX DE LA BATERIA DEL AUTO
float Tension = 0;
byte Pbat; //PORCENTAJE DEL NIVEL DE LA BATERIA CON RESPECTO AL MIN Y MAX
//
//Amperimetro
float voltaje = 0, voltaje1 = 0, voltaje2 = 0, corrientesuma = 0, Corriente = 0;
const int Amp = A0; //PIN DE AMPERIMETRO
//
//Velocimetro
const int pin = 2; //PIN DE VELOCIMETRO
int Velocidad = 0, pulsos, contv;
byte velocidadkm;
float Distancia = 0;
const float perimetro = 0.8; //PERIMETRO DE LA RUEDA
const long diley = 1000;
unsigned long tiempoA, tiempoP;
//
int Initime = 5000; //Tiempo que permanece el logo de inicio en mili segundos

void setup(void) {
  pinMode(BotonCrn, INPUT); //pin de inicio/pausa del cronometro
  pinMode(pin, INPUT); //pin de velocimetro
  attachInterrupt(digitalPinToInterrupt(pin), contador, FALLING);
  Serial.begin(9600);
  u8g.setRot180();
}

void loop(void) {
  Funciones();
  u8g.firstPage();
  do {
    if (millis() <= Initime) {
      drawlogo();
    } else {
      draw();
    }
  } while ( u8g.nextPage() );
}

void draw(void) {
  drawCronometro(0, 64);
  drawVoltimetro(72, 19);
  drawVelocidad(0, 19);
  drawAmperimetro(72, 46);
  drawDistancia(85, 64);
}

void Funciones() {
  Cronometro();
  Velocimetro();
  Voltimetro();
  Amperimetro();
}

void BotonCrono () {
  if (digitalRead(BotonCrn) == HIGH) { //Cuenta el tiempo que se mantiene presionado el boton para reiniciar el cronometro o tambien la Distancia
    cont ++;
    if (cont >= 20) {
      estado = 0;
      Segundos = 0;
      Minutos = 0;
      Horas = 0;
      if (cont >= 40) {
        Distancia = 0;
      }
    }
  } else {
    cont = 0;
  }
  InicioCrn = detectaFlancoAscendente(BotonCrn); //Detecta el flanco para activar o pausar el cronometro
  if (InicioCrn == true) {
    if (estado == 1) {
      estado = 0;
    } else {
      estado = 1;
      Anterior = 0;
      Anterior = millis();
    }
  }

}

void Cronometro () {
  BotonCrono ();
  if (estado == 1) {
    if (millis() >= (Anterior)) {
      Anterior = Anterior + 1000;
      Segundos++ ;
      if (Segundos == 60) {
        Segundos = 0;
        Minutos++;
      }
      if (Minutos == 60) {
        Minutos = 0;
        Horas++;
      }
      if (Horas == 10) {
        Horas = 0;
      }
    }
  }
}

void Voltimetro() {
  Tension = (((analogRead(Volt1)) * Vmax) / 1024);// 
  /*if (Tension > Vmin && Tension < Vmax) {
    Pbat = map(analogRead(Volt1), valorVmin, valorVmax, 0, 100);
  } else if (Tension < Vmin) {
    Pbat = 0;
  } else if (Tension > Vmax) {
    Pbat = 100;
  }
  if (Pbat > 100) {
    Pbat = 100;
  }
  if (Tension < 1) {
    Tension = 0;
  }*/
}

void Amperimetro() {
  float voltaje = 0, voltaje1 = 0, voltaje2 = 0, corrientesuma = 0;
  for (int x = 0; x < 100; x++) {
    voltaje = analogRead(Amp) * 5.0 / 1023.0;
    corrientesuma += (voltaje - 2.56) / 0.032;//0.083
  }
  Corriente = (corrientesuma / 100);
  corrientesuma = 0;
  if (Corriente <= 0.5) {
    Corriente = 0.0;
  }
}

void Velocimetro() {
  tiempoA = millis();
  if (tiempoA - tiempoP >= diley) {
    tiempoP = tiempoA;
    
    velocidadkm = (pulsos * perimetro) * 3.6;
    pulsos = 0;                              //Reset de los pulsos

    Velocidad = round(velocidadkm);
  }
  if (Velocidad >= 99) {
    Velocidad = 99;
  }
  Serial.print("KM:   ");
  Serial.println(velocidadkm);
  Serial.print("velocidad");
  Serial.println(Velocidad);
}

void contador() {
  Distancia += (perimetro / 1000); //metros pasados a km ya que la distancia se mide en kilometros
  pulsos ++;
}

boolean detectaFlancoAscendente(int pin) {
  static boolean anterior_estado = digitalRead(pin);
  boolean eStado = digitalRead(pin);

  if ((anterior_estado != eStado) && (eStado == HIGH)) {
    anterior_estado = eStado;
    return true;
  }
  else {
    anterior_estado = eStado;
    return false;
  }
}

void drawVelocidad(int x, int y) {
  u8g.setFont(u8g_font_helvR18);
  if (Velocidad < 10) {
    u8g.setPrintPos(x + 13, y);
    u8g.print(Velocidad);
    //u8g.drawStr(x, y, " ");
  } else {
    u8g.setPrintPos(x, y);
    u8g.print(Velocidad);
  }
  u8g.setFont(u8g_font_courR10);
  u8g.drawStr(x + 28, y, "Km/h");
}

void drawDistancia(int x, int y) {
  u8g.setFont(u8g_font_6x12);
  u8g.drawStr(x - 20, y, "ODO");
  if (Distancia < 10) {
    u8g.setPrintPos(x + 9, y);
    u8g.print(Distancia, 1);
    u8g.drawStr(x - 1, y, "0");
    u8g.drawStr(x + 4, y, "0");
  } else if (Distancia < 100) {
    u8g.setPrintPos(x + 5, y);
    u8g.print(Distancia, 1);
    u8g.drawStr(x - 1, y, "0");
  } else {
    u8g.setPrintPos(x, y);
    u8g.print(Distancia, 1);
  }
  u8g.drawStr(x + 31, y, "Km");
}

void drawVoltimetro(int x, int y) {
  u8g.setFont(u8g_font_helvR18);
  if (Tension < 10) {
    u8g.setPrintPos(x + 13, y);
    u8g.print(Tension, 1);
    //u8g.drawStr(x, y, " ");
  } else {
    u8g.setPrintPos(x, y);
    u8g.print(Tension, 1);
  }
  u8g.setFont(u8g_font_courR10);
  u8g.drawStr(x + 46, y, "V");
}

void drawAmperimetro(int x, int y) {
  u8g.setFont(u8g_font_helvR18);
  if (Corriente < 10) {
    u8g.setPrintPos(x + 13, y);
    u8g.print(Corriente, 1);
    //u8g.drawStr(x, y, " ");
  } else {
    u8g.setPrintPos(x, y);
    u8g.print(Corriente, 1);
  }
  u8g.setFont(u8g_font_courR10);
  u8g.drawStr(x + 46, y, "A");
}

void drawCronometro(int x, int y) {
  u8g.setFont(u8g_font_fur14);
  u8g.setPrintPos(x, y);
  u8g.print(Horas);
  u8g.drawStr(x + 10, y - 2, ":");
  if (Minutos < 10) {
    u8g.setPrintPos(x + 25, y);
    u8g.print(Minutos);
    u8g.drawStr(x + 15, y, "0");
  } else {
    u8g.setPrintPos(x + 15, y);
    u8g.print(Minutos);
  }
  u8g.drawStr(x + 36, y - 2, ":");
  if (Segundos < 10) {
    u8g.setPrintPos(x + 51, y);
    u8g.print(Segundos);
    u8g.drawStr(x + 41, y, "0");
  } else {
    u8g.setPrintPos(x + 41, y);
    u8g.print(Segundos);
  }
}

const uint8_t logo_bitmap[] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xfc,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x04,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x04,
  0x80, 0x12, 0x7b, 0xa2, 0xe0, 0x04,
  0x80, 0x1b, 0x56, 0xf7, 0xb0, 0x04,
  0x80, 0x3e, 0xd6, 0xef, 0xa0, 0x04,
  0x80, 0x20, 0x01, 0x00, 0x40, 0x04,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x04,
  0x80, 0x7e, 0x5e, 0xf1, 0x58, 0x04,
  0x80, 0x7b, 0x7f, 0xf9, 0xc8, 0x04,
  0x80, 0x7b, 0x4d, 0x7b, 0x70, 0x04,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x04,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x04,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xfc,
  0x80, 0x00, 0x03, 0xff, 0xff, 0xfc,
  0x80, 0x00, 0x03, 0xff, 0xff, 0xfc,
  0x80, 0x00, 0x03, 0xff, 0xff, 0xfc,
  0x80, 0x00, 0x03, 0xff, 0xff, 0xfc,
  0x83, 0xfc, 0x03, 0xff, 0x00, 0xfc,
  0x83, 0xfc, 0x03, 0xff, 0x00, 0xfc,
  0x83, 0x00, 0x03, 0xff, 0x3f, 0xfc,
  0x83, 0x00, 0x03, 0xff, 0x3f, 0xfc,
  0x83, 0xf8, 0x03, 0xff, 0x01, 0xfc,
  0x83, 0xf8, 0x03, 0xff, 0x01, 0xfc,
  0x83, 0x00, 0x03, 0xff, 0x3f, 0xfc,
  0x83, 0x00, 0x0f, 0xff, 0x3f, 0xfc,
  0x83, 0xfc, 0x30, 0x3f, 0x00, 0xfc,
  0x83, 0xfc, 0xc0, 0x0f, 0x00, 0xfc,
  0x80, 0x01, 0x80, 0x07, 0xff, 0xfc,
  0x80, 0x03, 0x00, 0x03, 0xff, 0xfc,
  0x80, 0x02, 0x03, 0x01, 0xff, 0xfc,
  0x80, 0x04, 0x07, 0x00, 0xff, 0xfc,
  0x80, 0x04, 0x0f, 0x00, 0xff, 0xfc,
  0x80, 0x04, 0x1f, 0x00, 0xff, 0xfc,
  0x80, 0x08, 0x03, 0x00, 0xff, 0xfc,
  0xff, 0xf8, 0x03, 0x00, 0xff, 0xfc,
  0xff, 0xf8, 0x03, 0x00, 0x80, 0x04,
  0xff, 0xf8, 0x03, 0x00, 0x80, 0x04,
  0xff, 0xfc, 0x03, 0x00, 0x80, 0x04,
  0xff, 0xfc, 0x03, 0x00, 0x80, 0x04,
  0xff, 0xfe, 0x03, 0x01, 0x00, 0x04,
  0xff, 0xfe, 0x00, 0x03, 0x00, 0x04,
  0xff, 0xff, 0x00, 0x06, 0x00, 0x04,
  0xff, 0x8f, 0x80, 0x0c, 0x00, 0x04,
  0xfe, 0x03, 0xe0, 0x19, 0xfe, 0x04,
  0xfc, 0x73, 0xff, 0xe1, 0xfe, 0x04,
  0xfc, 0x71, 0xfe, 0x00, 0x30, 0x04,
  0xfc, 0x7f, 0xfe, 0x00, 0x30, 0x04,
  0xfe, 0x0f, 0xfe, 0x00, 0x30, 0x04,
  0xff, 0x87, 0xfe, 0x00, 0x30, 0x04,
  0xff, 0xe3, 0xfe, 0x00, 0x30, 0x04,
  0xfc, 0xf1, 0xfe, 0x00, 0x30, 0x04,
  0xfc, 0x71, 0xfe, 0x00, 0x30, 0x04,
  0xfe, 0x03, 0xfe, 0x00, 0x30, 0x04,
  0xff, 0x07, 0xfe, 0x00, 0x00, 0x04,
  0xff, 0xff, 0xfe, 0x00, 0x00, 0x04,
  0xff, 0xff, 0xfe, 0x00, 0x00, 0x04,
  0xff, 0xff, 0xfe, 0x00, 0x00, 0x0c,
  0x7f, 0xff, 0xfe, 0x00, 0x00, 0x18,
  0x3f, 0xff, 0xfe, 0x3f, 0xff, 0xf0,
  0x00, 0x00, 0x1e, 0x60, 0x00, 0x00,
  0x00, 0x00, 0x0f, 0xc0, 0x00, 0x00,
  0x00, 0x00, 0x07, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
};

void drawlogo(void) {
  u8g.drawBitmapP( 0, 0, 6, 64, logo_bitmap);
  u8g.setFont(u8g_font_helvR08);
  u8g.drawStr(51, 11, "EEST N1");
  u8g.drawStr(51, 23, "Antonio Bermejo");
  u8g.drawStr(51, 35, "Junin(B)");
  u8g.drawStr(51, 47, "Desafio Eco");
  u8g.drawStr(51, 59, "2019");
}
