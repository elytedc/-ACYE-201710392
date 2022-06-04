#include <LedControl.h>
#include "numeros.h"
#include "letras.h"

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
//      for (int fila = 0; fila < 8; fila++) {
//    digitalWrite(filas[fila], HIGH);
//    for (int columna = 0; columna < 8; columna++) {
//       molde[columna][fila] = p1[columna][fila]; }}
//       condriver();
//delay(500);
mostrarTexto_AbajoArriba();
    
    
    
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
       molde[columna][fila] = p1[columna][fila];
        //lc.setLed(0, fila, columna, n1[columna][fila]);
    }
  }
}



void mostrarTexto_AbajoArriba() {

  if (estado == 0) {
    //mostrarCaracter(ASTERISCO);
    mostrarCaracterDriver(ESPACIO);
    mostrarCaracter(ASTERISCO);
    delay(del);
    LimpiarMatriz();
    estado = 20; // reseteando los estados (esto para que cuando el texto termine, se vuelva a mostrar el mismo texto desde el inicio)
  } else if (estado == 1) {
    mostrarCaracterDriver(ASTERISCO);
    mostrarCaracter(P);
    delay(del);
    LimpiarMatriz();
    estado = 0;
  } else if (estado == 2) {
    mostrarCaracterDriver(P);
    mostrarCaracter(UNO);
    delay(del);
    LimpiarMatriz();
    estado = 1;
  } else if (estado == 3) {
    mostrarCaracterDriver(UNO);
    mostrarCaracter(GUION);
    delay(del);
    LimpiarMatriz();
    estado = 2;
  } else if (estado == 4) {
    mostrarCaracterDriver(GUION);
    mostrarCaracter(G);
    delay(del);
    LimpiarMatriz();
    estado = 3;
  } else if (estado == 5) {
    mostrarCaracterDriver(G);
    mostrarCaracter(R);
    delay(del);
    LimpiarMatriz();
    estado = 4;
  } else if (estado == 6) {
    mostrarCaracterDriver(R);
    mostrarCaracter(U);
    delay(del);
    LimpiarMatriz();
    estado = 5;
  } else if (estado == 7) {
    mostrarCaracterDriver(U);
    mostrarCaracter(P);
    delay(del);
    LimpiarMatriz();
    estado = 6;
  } else if (estado == 8) {
    mostrarCaracterDriver(P);
    mostrarCaracter(O);
    delay(del);
    LimpiarMatriz();
    estado = 7;
  } else if (estado == 9) {
    mostrarCaracterDriver(O);
    mostrarCaracter(CUATRO);
    delay(del);
    LimpiarMatriz();
    estado = 8;
  } else if (estado == 10) {
    mostrarCaracterDriver(CUATRO);
    mostrarCaracter(GUION);
    delay(del);
    LimpiarMatriz();
    estado = 9;
  } else if (estado == 11) {
    mostrarCaracterDriver(GUION);
    mostrarCaracter(S);
    delay(del);
    LimpiarMatriz();
    estado = 10;
  } else if (estado == 12) {
    mostrarCaracterDriver(S);
    mostrarCaracter(E);
    delay(del);
    LimpiarMatriz();
    estado = 11;
  } else if (estado == 13) {
    mostrarCaracterDriver(E);
    mostrarCaracter(C);
    delay(del);
    LimpiarMatriz();
    estado = 12;
  } else if (estado == 14) {
    mostrarCaracterDriver(C);
    mostrarCaracter(C);
    delay(del);
    LimpiarMatriz();
    estado = 13;
  } else if (estado == 15) {
    mostrarCaracterDriver(C);
    mostrarCaracter(I);
    delay(del);
    LimpiarMatriz();
    estado = 14;
  } else if (estado == 16) {
    estado = 15;
    mostrarCaracterDriver(I);
    mostrarCaracter(O);
    delay(del);
    LimpiarMatriz();
  } else if (estado == 17) {
    estado = 16;
    mostrarCaracterDriver(O);
    mostrarCaracter(N);
    delay(del);
    LimpiarMatriz();
  } else if (estado == 18) {
    mostrarCaracterDriver(N);
    mostrarCaracter(B);
    delay(del);
    LimpiarMatriz();
    estado = 17;
  } else if (estado == 19) {
    mostrarCaracterDriver(B);
    mostrarCaracter(ASTERISCO);
    delay(del);
    LimpiarMatriz();
    estado = 18;
  } else if (estado == 20) {
    mostrarCaracterDriver(ASTERISCO);
    mostrarCaracter(ESPACIO);
    delay(del);
    LimpiarMatriz();
    estado = 19; // reseteando los estados
  }
}



void LimpiarMatriz() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(fil[i], HIGH);
    digitalWrite(col[i], LOW);
  }
}




void mostrarCaracter(int letra[8][8]) {
  //int fil_aux=0;
  int col_aux = 7;
  for (int repe = 0; repe < 30; repe++) {
    for (int i = 0; i < 8; i++) {
      //digitalWrite(fil[i],LOW);
      digitalWrite(col[i], HIGH);
      for (int j = 0; j < 8; j++) {
        if (letra[i][j] == 1) {
          //digitalWrite(fil[j], HIGH);
          digitalWrite(fil[col_aux], LOW);
        }
        col_aux--;
      }
      col_aux = 7;
      delay(2);
      //digitalWrite(fil[i],HIGH);
      digitalWrite(col[i], LOW);
      for (int j = 0; j < 8; j++) {
        //digitalWrite(col[j],LOW);
        digitalWrite(fil[j], HIGH);
      }
    }
  }
}





void mostrarCaracterDriver(int letra[8][8]) {
  int fil_aux = 0;
  int col_aux = 7;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      lc.setLed(0, fil_aux, col_aux, letra[i][j]);
      fil_aux++;
    }
    fil_aux = 0;
    col_aux--;
  }
}
