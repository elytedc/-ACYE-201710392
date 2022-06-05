#include <LedControl.h>
#include "letras.h"
#include "numeros.h"

//// MOdificado

LedControl lc = LedControl(51, 52, 53, 1);
long unsigned int contabilidad=0;
// Inicio variables para texto --- Agregado Gerson --------------------------------------------------------
int fil[] = {22, 23, 24, 25, 26, 27, 28, 29}; // Filas
int col[] = {32, 33, 34, 35, 36, 37, 38, 39}; // Columnas

int esta=0;
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


// NAVE ----------------------------------------------------------------------------------
class Nave {
  public:
    int coordX, velMov, velDisparo, score[2]; // [0]: Decenas, [1]; Unidades

    bool puedeMovIzq();
    bool puedeMovDer();
    //bool puedeDisparar();

    bool estaTocandoCoordenadas(int, int); // x, y

    void moverseIzq();
    void moverseDer();
    //void disparar();

  private:
    long unsigned int tiempoUltimoMov;
    //long unsigned int tiempoUltimoDisparo;
};

bool Nave::puedeMovIzq() {
  return (coordX > 1 && (millis() - tiempoUltimoMov) > velMov);
}

bool Nave::puedeMovDer() {
  return (coordX < 6 && (millis() - tiempoUltimoMov) > velMov);
}



bool Nave::estaTocandoCoordenadas(int x, int y) {
  bool estaTocandoLadoDer = ((x == coordX + 1) && (y == 0));
  bool estaTocandoLadoIzq = ((x == coordX - 1) && (y == 0));
  bool estaTocandoLadoFrontal = ((x == coordX) && (y == 1));
  return (estaTocandoLadoDer || estaTocandoLadoIzq  || estaTocandoLadoFrontal);
}

void Nave::moverseIzq() {
  if (coordX > 1) {
    coordX -= 1;
  }
  tiempoUltimoMov = millis();
}

void Nave::moverseDer() {
  if (coordX < 6) {
    coordX += 1;
  }
  tiempoUltimoMov = millis();
}



Nave nave;


// ENEMIGO ----------------------------------------------------------------------------------
class Enemigo {
  public:
    int coordX, coordY = 15, velMov = 2000; //Debe ser en una funcion

    bool puedeMoverse();
    void moverse();
    //void actvel();

  private:
    long unsigned int tiempoUltimoMov = 0;
};

bool Enemigo::puedeMoverse() {
  return (millis() - tiempoUltimoMov) > velMov;
}

Enemigo enemigosVivos[MAX_NUMERO_DE_ENEMIGOS] = {};
bool enemigosOcupanEspacio[MAX_NUMERO_DE_ENEMIGOS] = {};

void Enemigo::moverse() {
  coordY--;
  tiempoUltimoMov = millis();
}

void crearEnemigo() {
  Enemigo enemigo;
  enemigo.coordX = random(6);
  for (int idEnemigo = 0; idEnemigo < MAX_NUMERO_DE_ENEMIGOS; idEnemigo++) {
    if (!enemigosOcupanEspacio[idEnemigo]) {
      if (idEnemigo != 0) {
        if (enemigosVivos[idEnemigo - 1].coordY < 12) {
          if (millis()-contabilidad>=5000) {
            enemigosOcupanEspacio[idEnemigo] = true;
            contabilidad=0;
            contabilidad = millis();
            enemigosVivos[idEnemigo] = enemigo;
          }
          
          
        }
      } else {
        enemigosOcupanEspacio[idEnemigo] = true;
        enemigosVivos[idEnemigo] = enemigo;
      }
      break;
    }
  }
}

void actualizarVelEnemigo() {
  for (int idEnemigo = 0; idEnemigo < MAX_NUMERO_DE_ENEMIGOS; idEnemigo++) {
    if (enemigosOcupanEspacio[idEnemigo]) {
      enemigosVivos[idEnemigo].velMov = analogRead(pot2) * 4;
    }
  }
}

// JUEGO ------------------------------------------------------------------------------------
void dibujarCuadricula(byte * cuadricula) {
  int contadorM2 = 7;
  for (int i = 0; i < 16; i++) {
    if (i < 8) {
      digitalWrite(i + 32, HIGH);
      for (int j = 7; j >= 0; j--) {
        int a = cuadricula[i] >> j;
        bool valor = a & 1;
        if (valor) {
          digitalWrite(j + 22, LOW);
        } else {
          digitalWrite(j + 22, HIGH);
        }
      }
      delay(1);
      digitalWrite(i + 32, LOW);
      for (int j = 22; j < 30; j++) {
        digitalWrite(j, HIGH);
      }
    } else {
      lc.setColumn(0, contadorM2, cuadricula[i]);
      contadorM2--;
    }
  }
}

void dibujarNave(int coord) {
  cuadriculaActual[12] = cuadriculaActual[12] | (B01000000 >> coord - 1);
  cuadriculaActual[13] = (B11100000 >> coord - 1);
  cuadriculaActual[14] = (B01000000 >> coord - 1);
  cuadriculaActual[15] = (B11100000 >> coord - 1);
  // este es de 3 B11100000
  // este es de 1 B01000000
}



void dibujarEnemigos() {
  for (int idEnemigo = 0; idEnemigo < MAX_NUMERO_DE_ENEMIGOS; idEnemigo++) {
    Enemigo enemigo = enemigosVivos[idEnemigo];
    if (enemigosOcupanEspacio[idEnemigo]) {
      if (enemigo.coordY == 15) {
        cuadriculaActual[0] = cuadriculaActual[0] | (B01000000 >> enemigo.coordX - 1);
      } else {
        if (enemigo.coordY == 14) {
          cuadriculaActual[0] = cuadriculaActual[0] | (B01000000 >> enemigo.coordX - 1);
          cuadriculaActual[1] = cuadriculaActual[1] | (B11100000 >> enemigo.coordX - 1);
        } else {
          if (enemigo.coordY == -1) {
            cuadriculaActual[14] = cuadriculaActual[14] | (B11100000 >> enemigo.coordX - 1);
            cuadriculaActual[15] = cuadriculaActual[15] | (B01000000 >> enemigo.coordX - 1);
          } else {
            if (enemigo.coordY == -2) {
              cuadriculaActual[15] = cuadriculaActual[15] | (B11100000 >> enemigo.coordX - 1); // es tope del tablero---
            } else {
              cuadriculaActual[13 - enemigo.coordY] = cuadriculaActual[13 - enemigo.coordY] | (B11100000 >> enemigo.coordX - 1);
              cuadriculaActual[14 - enemigo.coordY] = cuadriculaActual[14 - enemigo.coordY] | (B01000000 >> enemigo.coordX - 1);
              cuadriculaActual[15 - enemigo.coordY] = cuadriculaActual[15 - enemigo.coordY] | (B11100000 >> enemigo.coordX - 1);
            }
          }
        }
      }
      if (enemigo.puedeMoverse()) {
        enemigosVivos[idEnemigo].moverse();
        if (enemigosVivos[idEnemigo].coordY + 2 < 0) {
          enemigosOcupanEspacio[idEnemigo] = false;
        }
      }
    }
  }
}

bool estaPresionado(int idBtn) {
  int estadoBtn = digitalRead(idBtn);
  return (estadoBtn == HIGH);
}

void verificarEnemigoTocaNave() {
  for (int idEnemigo = 0; idEnemigo < MAX_NUMERO_DE_ENEMIGOS; idEnemigo++) {
    Enemigo enemigo = enemigosVivos[idEnemigo];
    bool tocaDer = nave.estaTocandoCoordenadas(enemigo.coordX - 1, enemigo.coordY + 1);
    bool tocaIzq = nave.estaTocandoCoordenadas(enemigo.coordX + 1, enemigo.coordY + 1);
    bool tocaFrente = nave.estaTocandoCoordenadas(enemigo.coordX, enemigo.coordY);
    if (enemigosOcupanEspacio[idEnemigo] && (tocaDer || tocaIzq || tocaFrente)) {
      reiniciarJuego();
    }
  }
}



void reiniciarJuego() {
  GAME_OVER = true;
  dibujarCuadricula(cuadriculaLimpia);
  nave.coordX = 3;
  nave.velMov = 50;
  nave.velDisparo = 200;
  unidad = 0;
  decena = 0;
  // Enemigos
  for (int idEnemigo = 0; idEnemigo < MAX_NUMERO_DE_ENEMIGOS; idEnemigo++) {
    enemigosOcupanEspacio[idEnemigo] = false;
  }
}

// SETUP ------------------------------------------------------------------------------------
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

  reiniciarJuego();
}

void loop() {
  //Serial.println(analogRead(btnStart));

  if (!GAME_OVER) {
    // PAUSA 
    if (analogRead(btnStart) == 0) {
      //imprimir_conteo();
      //imprimir_sindriver_conteo();
      esta=0;
    } else {
      if (nave.puedeMovIzq()) {
        if (!estaPresionado(btnIzq) ) {
          btnIzqPres = true;
        }
        if (estaPresionado(btnIzq) && btnIzqPres) {
          nave.moverseIzq();
          btnIzqPres = false;
        }
      }
      if (nave.puedeMovDer()) {
        if (!estaPresionado(btnDer) ) {
          btnDerPres = true;
        }
        if (estaPresionado(btnDer) && btnDerPres) {
          nave.moverseDer();
          btnDerPres = false;
        }
      }
//      if (nave.puedeDisparar()) {
//        if (!estaPresionado(btnDisparo) ) {
//          btnDisparoPres = true;
//        }
//        if (estaPresionado(btnDisparo) && btnDisparoPres) {
//          nave.disparar();
//          btnDisparoPres = false;
//        }
//      }

      crearEnemigo();
      actualizarVelEnemigo();

      memcpy(cuadriculaActual, cuadriculaLimpia, 16);

      dibujarNave(nave.coordX);
      dibujarEnemigos();
      dibujarCuadricula(cuadriculaActual);
      //verificarEnemigoTocaNave();
      //verificarProyectilTocaEnemigo();
    }
  } else {



    if (estaPresionado(btnStart)) {
      GAME_OVER = false;
    }


    else {
      boton = digitalRead(switchPresionado);
      del = analogRead(pot);
      if (boton == 1) {
        AS(); 
      } else {
        DES(); 
      }
    }
    delay(250);

  }
}





void AS() {

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



void DES() {

  if (estado == 36) {
    mostrarCaracterDriver(ESPACIO);
    mostrarCaracter(DOS);
    delay(del);
    LimpiarMatriz();
    estado = 35; 
  }
if (estado == 35) {
    mostrarCaracterDriver(FINALDOS);
    mostrarCaracter(NUEVE2);
    delay(del);
    LimpiarMatriz();
    estado = 34; 
  }

  else if (estado == 34) { 
    mostrarCaracterDriver(DOS);
    mostrarCaracter(NUEVE);
    delay(del);
    LimpiarMatriz();
    estado = 33;
  }

else if (estado == 33) { 
    mostrarCaracterDriver(NUEVE2);
    mostrarCaracter(TRES9);
    delay(del);
    LimpiarMatriz();
    estado = 32;
  }

else if (estado == 32) { 
    mostrarCaracterDriver(NUEVE);
    mostrarCaracter(TRES);
    delay(del);
    LimpiarMatriz();
    estado = 31;
  }//////////////////////////////////////
else if (estado == 31) { 
    mostrarCaracterDriver(TRES9);
    mostrarCaracter(CERO2);
    delay(del);
    LimpiarMatriz();
    estado = 30;
  }

  else if (estado == 30) { 
    mostrarCaracterDriver(TRES);
    mostrarCaracter(CERO);
    delay(del);
    LimpiarMatriz();
    estado = 29;
  }

  else if (estado == 29) { 
    mostrarCaracterDriver(CERO2);
    mostrarCaracter(UNOCERO);
    delay(del);
    LimpiarMatriz();
    estado = 28;
  }

  else if (estado == 28) { 
    mostrarCaracterDriver(CERO);
    mostrarCaracter(UNO);
    delay(del);
    LimpiarMatriz();
    estado = 27;
  }

  else if (estado == 27) { 
    mostrarCaracterDriver(UNOCERO);
    mostrarCaracter(SIETE1);
    delay(del);
    LimpiarMatriz();
    estado = 26;
  }

  else if (estado == 26) { 
    mostrarCaracterDriver(UNO);
    mostrarCaracter(SIETE);
    delay(del);
    LimpiarMatriz();
    estado = 25;
  }
        
else if (estado == 26) { 
    mostrarCaracterDriver(SIETE1);
    mostrarCaracter(UNO7);
    delay(del);
    LimpiarMatriz();
    estado = 25;
  }
  
else if (estado == 25) { 
    mostrarCaracterDriver(SIETE);
    mostrarCaracter(UNO);
    delay(del);
    LimpiarMatriz();
    estado = 24;
  }

  else if (estado == 24) { 
    mostrarCaracterDriver(UNO7);
    mostrarCaracter(CERO1);
    delay(del);
    LimpiarMatriz();
    estado = 23;
  }

  else if (estado == 23) { 
    mostrarCaracterDriver(UNO);
    mostrarCaracter(CERO);
    delay(del);
    LimpiarMatriz();
    estado = 22;
  }

  else if (estado == 22) { 
    mostrarCaracterDriver(CERO1);
    mostrarCaracter(DOSCERO);
    delay(del);
    LimpiarMatriz();
    estado = 21;
  }

  else if (estado == 21) { 
    mostrarCaracterDriver(CERO);
    mostrarCaracter(DOS);
    delay(del);
    LimpiarMatriz();
    estado = 20;
  }

  else if (estado == 20) { 
    mostrarCaracterDriver(DOS);
    mostrarCaracter(DIA);
    delay(del);
    LimpiarMatriz();
    estado = 19;
  }

  else if (estado == 19) { 
    mostrarCaracterDriver(CABEZADOS);
    mostrarCaracter(UNODIA);
    delay(del);
    LimpiarMatriz();
    estado = 18;
  }

  else if (estado == 18) { 
    mostrarCaracterDriver(DIA);
    mostrarCaracter(UNO);
    delay(del);
    LimpiarMatriz();
    estado = 17;
  }

  else if (estado == 17) { 
    mostrarCaracterDriver(UNODIA);
    mostrarCaracter(AUNO);
    delay(del);
    LimpiarMatriz();
    estado = 16;
  }

  else if (estado == 16) { 
    mostrarCaracterDriver(UNO);
    mostrarCaracter(AA1);
    delay(del);
    LimpiarMatriz();
    estado = 15;
  }

else if (estado == 15) { 
    mostrarCaracterDriver(AUNO);
    mostrarCaracter(CA);
    delay(del);
    LimpiarMatriz();
    estado = 14;
  }

  else if (estado == 14) { 
    mostrarCaracterDriver(AA1);
    mostrarCaracter(C1);
    delay(del);
    LimpiarMatriz();
    estado = 13;
  }

  else if (estado == 13) { 
    mostrarCaracterDriver(CA);
    mostrarCaracter(IC);
    delay(del);
    LimpiarMatriz();
    estado = 12;
  }

  else if (estado == 12) { 
    mostrarCaracterDriver(C1);
    mostrarCaracter(I1);
    delay(del);
    LimpiarMatriz();
    estado = 11;
  }

  else if (estado == 11) { 
    mostrarCaracterDriver(IC);
    mostrarCaracter(TI);
    delay(del);
    LimpiarMatriz();
    estado = 10;
  }

  else if (estado == 10) { 
    mostrarCaracterDriver(I1);
    mostrarCaracter(T1);
    delay(del);
    LimpiarMatriz();
    estado =9;
  }

  else if (estado == 9) { 
    mostrarCaracterDriver(TI);
    mostrarCaracter(CT);
    delay(del);
    LimpiarMatriz();
    estado = 8;
  }

  else if (estado == 8) { 
    mostrarCaracterDriver(T1);
    mostrarCaracter(C1);
    delay(del);
    LimpiarMatriz();
    estado = 7;
  }

  else if (estado == 7) { 
    mostrarCaracterDriver(CT);
    mostrarCaracter(AC);
    delay(del);
    LimpiarMatriz();
    estado = 6;
  }

  else if (estado == 6) { 
    mostrarCaracterDriver(C1);
    mostrarCaracter(AA1);
    delay(del);
    LimpiarMatriz();
    estado = 5;
  }

else if (estado == 5) { 
    mostrarCaracterDriver(AC);
    mostrarCaracter(RA);
    delay(del);
    LimpiarMatriz();
    estado = 4;
  }

  else if (estado == 4) { 
    mostrarCaracterDriver(AA1);
    mostrarCaracter(R1);
    delay(del);
    LimpiarMatriz();
    estado = 3;
  }

  else if (estado == 3) { 
    mostrarCaracterDriver(RA);
    mostrarCaracter(PR);
    delay(del);
    LimpiarMatriz();
    estado = 2;
  }

  else if (estado == 2) { 
    mostrarCaracterDriver(R1);
    mostrarCaracter(P2);
    delay(del);
    LimpiarMatriz();
    estado = 1;
  }

  else if (estado == 1) { 
    mostrarCaracterDriver(PR);
    mostrarCaracter(P1);
    delay(del);
    LimpiarMatriz();
    estado = 0;
  }

  else if (estado == 0) { 
    mostrarCaracterDriver(P2);
    mostrarCaracter(ESPACIO);
    delay(del);
    LimpiarMatriz();
    estado = 36;
  }

  }






void LimpiarMatriz() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(fil[i], HIGH);
    digitalWrite(col[i], LOW);
  }
}




void mostrarCaracter(int letras[8][8]) {
  //int fil_aux=0;
  int col_aux = 7;
  for (int repe = 0; repe < 30; repe++) {
    for (int i = 0; i < 8; i++) {
      //digitalWrite(fil[i],LOW);
      digitalWrite(col[i], HIGH);
      for (int j = 0; j < 8; j++) {
        if (letras[i][j] == 1) {
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





void mostrarCaracterDriver(int letras[8][8]) {
  int fil_aux = 0;
  int col_aux = 7;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      lc.setLed(0, fil_aux, col_aux, letras[i][j]);
      fil_aux++;
    }
    fil_aux = 0;
    col_aux--;
  }
}
