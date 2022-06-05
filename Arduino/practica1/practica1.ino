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
    
    mostrarCaracterDriver(P1);
    mostrarCaracter(ESPACIO);
    delay(del);
    LimpiarMatriz();
    estado = 1; // reseteando los estados (esto para que cuando el texto termine, se vuelva a mostrar el mismo texto desde el inicio)
  } else if (estado == 1) {
    
    mostrarCaracterDriver(P2);
    mostrarCaracter(ESPACIO);
    delay(del);
    LimpiarMatriz();
    estado = 2;
  } else if (estado == 2) {
    
    mostrarCaracterDriver(PR);
    mostrarCaracter(P1);
    delay(del);
    LimpiarMatriz();
    estado = 3;
  }
  else if (estado == 3) {
    
    mostrarCaracterDriver(R1);
    mostrarCaracter(P2);
    delay(del);
    LimpiarMatriz();
    estado = 4;
  }  

  else if (estado == 4) {
    
    mostrarCaracterDriver(RA);
    mostrarCaracter(PR);
    delay(del);
    LimpiarMatriz();
    estado = 5;
  } 
else if (estado == 5) {
    
    mostrarCaracterDriver(AA1);
    mostrarCaracter(R1);
    delay(del);
    LimpiarMatriz();
    estado = 6;
  } 
  else if (estado == 6) {
    
    mostrarCaracterDriver(AC);
    mostrarCaracter(RA);
    delay(del);
    LimpiarMatriz();
    estado = 7;
  }

  else if (estado == 7) {
    
    mostrarCaracterDriver(C1);
    mostrarCaracter(AA1);
    delay(del);
    LimpiarMatriz();
    estado = 8;
  }
  else if (estado == 8) {
    
    mostrarCaracterDriver(CT);
    mostrarCaracter(AC);
    delay(del);
    LimpiarMatriz();
    estado = 9;
  }

else if (estado == 9) {
    
    mostrarCaracterDriver(T1);
    mostrarCaracter(C1);
    delay(del);
    LimpiarMatriz();
    estado = 10;
  }


  else if (estado == 10) {
    
    mostrarCaracterDriver(TI);
    mostrarCaracter(CT);
    delay(del);
    LimpiarMatriz();
    estado = 11;
  }

else if (estado == 11) {
    
    mostrarCaracterDriver(I1);
    mostrarCaracter(T1);
    delay(del);
    LimpiarMatriz();
    estado = 12;
  }


else if (estado == 12) {
    
    mostrarCaracterDriver(IC);
    mostrarCaracter(TI);
    delay(del);
    LimpiarMatriz();
    estado = 13;
  }


  else if (estado == 13) {
    
    mostrarCaracterDriver(C1);
    mostrarCaracter(I1);
    delay(del);
    LimpiarMatriz();
    estado = 14;
  }

  else if (estado == 14) {
    
    mostrarCaracterDriver(CA);
    mostrarCaracter(IC);
    delay(del);
    LimpiarMatriz();
    estado = 15;
  }
  else if (estado == 15) {
    
    mostrarCaracterDriver(AA1);
    mostrarCaracter(C1);
    delay(del);
    LimpiarMatriz();
    estado = 16;
  }

else if (estado == 16) {
    
    mostrarCaracterDriver(AUNO);
    mostrarCaracter(CA);
    delay(del);
    LimpiarMatriz();
    estado = 17;
  }

else if (estado == 17) {
    
    mostrarCaracterDriver(UNO);
    mostrarCaracter(AA1);
    delay(del);
    LimpiarMatriz();
    estado = 18;
  }

else if (estado == 18) {
    
    mostrarCaracterDriver(UNODIA);
    mostrarCaracter(AUNO);
    delay(del);
    LimpiarMatriz();
    estado = 19;
  }

else if (estado == 19) {
    mostrarCaracterDriver(DIA);
    mostrarCaracter(UNO);
    delay(del);
    LimpiarMatriz();
    estado = 20;
  }

else if (estado == 20) {//7777777777777777777777777777777777777777777777
    mostrarCaracterDriver(DOS);
    mostrarCaracter(DIA);
    delay(del);
    LimpiarMatriz();
    estado = 21;
  }

else if (estado == 21) {
    mostrarCaracterDriver(DOSCERO);
    mostrarCaracter(CABEZADOS);
    delay(del);
    LimpiarMatriz();
    estado = 22;
  }

  else if (estado == 22) {
    mostrarCaracterDriver(CERO);
    mostrarCaracter(DOS);
    delay(del);
    LimpiarMatriz();
    estado = 23;
  }

  else if (estado == 23) {
    mostrarCaracterDriver(CERO1);
    mostrarCaracter(DOSCERO);
    delay(del);
    LimpiarMatriz();
    estado = 24;
  }

  else if (estado == 24) {
    mostrarCaracterDriver(UNO);
    mostrarCaracter(DOS);
    delay(del);
    LimpiarMatriz();
    estado = 25;
  }

  else if (estado == 25) {
    mostrarCaracterDriver(UNO7);
    mostrarCaracter(CERO1);
    delay(del);
    LimpiarMatriz();
    estado = 26;
  }

  else if (estado == 26) {
    mostrarCaracterDriver(SIETE);
    mostrarCaracter(UNO);
    delay(del);
    LimpiarMatriz();
    estado = 27;
  }

  else if (estado == 27) { 
    mostrarCaracterDriver(SIETE1);
    mostrarCaracter(UNO7);
    delay(del);
    LimpiarMatriz();
    estado = 28;
  }
///AQUI
else if (estado == 28) { 
    mostrarCaracterDriver(UNO);
    mostrarCaracter(SIETE);
    delay(del);
    LimpiarMatriz();
    estado = 29;
  }

else if (estado == 29) { 
    mostrarCaracterDriver(UNOCERO);
    mostrarCaracter(SIETE1);
    delay(del);
    LimpiarMatriz();
    estado = 30;
  }

  else if (estado == 30) { 
    mostrarCaracterDriver(CERO);
    mostrarCaracter(UNO);
    delay(del);
    LimpiarMatriz();
    estado = 31;
  }

  else if (estado == 31) { 
    mostrarCaracterDriver(CERO2);
    mostrarCaracter(UNOCERO);
    delay(del);
    LimpiarMatriz();
    estado = 32;
  }


  else if (estado == 32) { 
    mostrarCaracterDriver(TRES);
    mostrarCaracter(CERO);
    delay(del);
    LimpiarMatriz();
    estado = 33;
  }


  else if (estado == 33) { 
    mostrarCaracterDriver(TRES9);
    mostrarCaracter(CERO2);
    delay(del);
    LimpiarMatriz();
    estado = 34;
  }

  else if (estado == 34) { 
    mostrarCaracterDriver(NUEVE);
    mostrarCaracter(TRES);
    delay(del);
    LimpiarMatriz();
    estado = 35;
  }


  else if (estado == 35) { 
    mostrarCaracterDriver(NUEVE2);
    mostrarCaracter(TRES9);
    delay(del);
    LimpiarMatriz();
    estado = 36;
  }


  else if (estado == 36) { 
    mostrarCaracterDriver(DOS);
    mostrarCaracter(NUEVE);
    delay(del);
    LimpiarMatriz();
    estado = 0;
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
