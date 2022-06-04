#include <LedControl.h>
#include "numeros.h"

//// MOdificado

LedControl lc = LedControl(51, 52, 53, 1);

// Inicio variables para texto --- Agregado Gerson --------------------------------------------------------
int fil[] = {22, 23, 24, 25, 26, 27, 28, 29}; // Filas
int col[] = {32, 33, 34, 35, 36, 37, 38, 39}; // Columnas

int pot = A4; // Lee el potenciometro
int pot2 = A5; // Lee el potenciometro para el juego
int switchPresionado = 47; // Lee el switch
int estado = 0;
int boton; // guarda el estado del switch (presionado o no)
int del;   // guarda el valor del potenciomatro para hacer que el texto se traslade rapido/lento (delay)
// Fin variables para texto -----------------------------------------------------------
bool pausa = false;
int unidad = 0;
int decena = 0;
int columnas[] = {22, 23, 24, 25, 26, 27, 28, 29};
int filas[] = {32, 33, 34, 35, 36, 37, 38, 39};

int molde[][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

int btnDisparo = A0;
int btnIzq = A2;
int btnDer = A3;
int btnStart = A1;
bool btnIzqPres;
bool btnDerPres;
bool btnDisparoPres;
bool GAME_OVER = true;
byte cuadriculaLimpia[16] = {};
byte cuadriculaActual[16] = {};
const int MAX_NUMERO_DE_PROYECTILES = 25;
const int TODOS_LOS_COHETES_HAN_SIDO_USADOS = -1;
const int MAX_NUMERO_DE_ENEMIGOS = 55;
const int TODOS_LOS_ENEMIGOS_HAN_SIDO_USADOS = -1;


void setup() {

Serial.begin(9600);
  //inicializamos driver
  lc.shutdown(0, false); //inicia apagado - dispositivo 1
  lc.setIntensity(0, 15);
  lc.clearDisplay(0);

  // Indicando pines de salida
  for (int i = 0; i < 8; i++) {
    pinMode(fil[i], OUTPUT);
    pinMode(col[i], OUTPUT);
  }
  // Garantizando que se la matriz empiece apagada
  for (int i = 0; i < 8; i++) {
    digitalWrite(fil[i], HIGH);
    digitalWrite(col[i], LOW);
  }

  pinMode(switchPresionado, INPUT);
}

void loop() {
if (analogRead(btnStart) == 0) {
      //imprimir_conteo();
     // imprimir_sindriver_conteo();
      for (int fila = 0; fila < 8; fila++) {
    digitalWrite(filas[fila], HIGH);
    for (int columna = 0; columna < 8; columna++) {
       molde[columna][fila] = m0[columna][fila]; }}
       condriver();
delay(500);

     for (int fila = 0; fila < 8; fila++) {
    digitalWrite(filas[fila], HIGH);
    for (int columna = 0; columna < 8; columna++) {
       molde[columna][fila] = mm0[columna][fila]; }}
       condriver();
delay(500);

     for (int fila = 0; fila < 8; fila++) {
    digitalWrite(filas[fila], HIGH);
    for (int columna = 0; columna < 8; columna++) {
       molde[columna][fila] = m0[columna][fila]; }}
       sindriver();
delay(500);
     for (int fila = 0; fila < 8; fila++) {
    digitalWrite(filas[fila], HIGH);
    for (int columna = 0; columna < 8; columna++) {
       molde[columna][fila] = m0[columna][fila]; }}
       sindriver();
delay(500);

    
    
    
    } 



}




void sindriver() {
  for (int columna = 0; columna < 8; columna++) {
    //digitalWrite(filas[columna],HIGH);
    for (int fila = 0; fila < 8; fila++) {
      if (molde[columna][fila] == 1) {
        digitalWrite(columnas[fila], LOW);
      }
    }
    delay(4);
    digitalWrite(filas[columna], LOW);
    for (int j = 0; j < 8; j++) {
      digitalWrite(columnas[j], HIGH);
    }
  }
}


void condriver() {
  for (int fila = 0; fila < 8; fila++) {
    digitalWrite(filas[fila], HIGH);
    for (int columna = 0; columna < 8; columna++) {
       // molde[columna][fila] = m0[columna][fila];
        lc.setLed(0, fila, columna, molde[columna][fila]);
    }
  }
}


void paso() {
  for (int fila = 0; fila < 8; fila++) {
    digitalWrite(filas[fila], HIGH);
    for (int columna = 0; columna < 8; columna++) {
       molde[columna][fila] = m0[columna][fila];
        //lc.setLed(0, fila, columna, n1[columna][fila]);
    }
  }
}
