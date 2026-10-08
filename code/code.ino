/*
==================================================
SISTEMA MODULAR Y ESCALABLE DE SEMÁFOROS
==================================================

PREPARADO PARA:

- múltiples intersecciones
- intersecciones de 1 o 2 semáforos
- expansión futura
- sincronización
- sensores
- networking
- modos especiales

ARQUITECTURA:

INTERSECCIÓN
│
├── Semáforo A
└── Semáforo B (opcional)

==================================================
*/

#include <TimerOne.h>

// ==================================================
// CONFIGURACIÓN
// ==================================================

#define CANTIDAD_INTERSECCIONES 3

// ==================================================
// ENUMS
// ==================================================

typedef enum {

  SEM_APAGADO,

  SEM_ROJO,
  SEM_ROJO_AMARILLO,
  SEM_VERDE,
  SEM_AMARILLO

} estadoSemaforo;

typedef enum {

  CTRL_INICIO,

  CTRL_S1_ROJO_AMARILLO,
  CTRL_S1_VERDE,
  CTRL_S1_AMARILLO,

  CTRL_S2_ROJO_AMARILLO,
  CTRL_S2_VERDE,
  CTRL_S2_AMARILLO

} estadoControl;

// ==================================================
// ESTRUCTURA SEMÁFORO
// ==================================================

struct Semaforo {

  // Vehicular

  int pinRojo;
  int pinAmarillo;
  int pinVerde;

  // Peatonal

  int pinPeatonRojo;
  int pinPeatonVerde;  // Luz blanca de paso peatonal

  // Estado

  estadoSemaforo estado;

  // Tiempos

  int tiempoVerde;
  int tiempoAmarillo;

  // Existencia

  bool habilitado;
};

// ==================================================
// ESTRUCTURA INTERSECCIÓN
// ==================================================

struct Interseccion {

  Semaforo sem1;
  Semaforo sem2;

  // Permite intersecciones
  // de un solo semáforo

  bool tieneSemaforo2;

  estadoControl estado;

  unsigned long ultimoCambio;
};

// ==================================================
// TIMER GLOBAL
// ==================================================

volatile unsigned long ms = 0;

// ==================================================
// PROTOTIPOS
// ==================================================

void timer();

void iniciarInterseccion(
  Interseccion &inter);

void actualizarInterseccion(
  Interseccion &inter);

void actualizarSemaforo(
  Semaforo &sem,
  estadoSemaforo nuevoEstado);

bool temporizadorCumplido(
  Interseccion &inter,
  unsigned long tiempoObjetivo);

// ==================================================
// CONFIGURACIÓN DE LOS SEIS SEMÁFOROS
// ==================================================
// Fila de impares: 1, 3, 5. Fila de pares: 2, 4, 6.
// Cada intersección alterna entre un semáforo de cada fila.
// Orden de campos: rojo, amarillo, verde, peatón rojo, peatón blanco.

// Intersección 1: semáforos 1 y 2.
Interseccion inter1 = {
  {
    A2, A3, A4,  // Semáforo 1: rojo, amarillo, verde
    A5, A6,      // Peatón rojo, blanco
    SEM_APAGADO,
    10, 2,       // Segundos de verde y amarillo
    true
  },
  {
    3, 4, 5,     // Semáforo 2: rojo, amarillo, verde
    7, 6,        // Peatón rojo, blanco
    SEM_APAGADO,
    10, 2,
    true
  },
  true,
  CTRL_INICIO,
  0
};

// Intersección 2: semáforos 3 y 4.
Interseccion inter2 = {
  {
    18, 17, 16,  // Semáforo 3: rojo, amarillo, verde
    14, 15,      // Peatón rojo, blanco
    SEM_APAGADO,
    10, 2,
    true
  },
  {
    A8, A9, A10, // Semáforo 4: rojo, amarillo, verde
    A12, A11,    // Peatón rojo, blanco
    SEM_APAGADO,
    10, 2,
    true
  },
  true,
  CTRL_INICIO,
  0
};

// Intersección 3: semáforos 5 y 6.
Interseccion inter3 = {
  {
    35, 33, 31,  // Semáforo 5: rojo, amarillo, verde
    27, 29,      // Peatón rojo, blanco
    SEM_APAGADO,
    10, 2,
    true
  },
  {
    45, 43, 41,  // Semáforo 6: rojo, amarillo, verde
    37, 39,      // Peatón rojo, blanco
    SEM_APAGADO,
    10, 2,
    true
  },
  true,
  CTRL_INICIO,
  0
};

// ==================================================
// ARRAY DE INTERSECCIONES
// ==================================================

Interseccion *intersecciones[CANTIDAD_INTERSECCIONES] = {
  &inter1,
  &inter2,
  &inter3
};

// ==================================================
// SETUP
// ==================================================

void setup() {

  Serial.begin(9600);

  // ==============================================
  // TIMER
  // ==============================================

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timer);

  // ==============================================
  // INICIALIZAR INTERSECCIONES
  // ==============================================

  for (int i = 0; i < CANTIDAD_INTERSECCIONES; i++) {

    iniciarInterseccion(
      *intersecciones[i]);
  }
}

// ==================================================
// LOOP
// ==================================================

void loop() {

  // ==============================================
  // ACTUALIZAR TODAS LAS INTERSECCIONES
  // ==============================================

  for (int i = 0; i < CANTIDAD_INTERSECCIONES; i++) {

    actualizarInterseccion(
      *intersecciones[i]);
  }
}

// ==================================================
// TIMER
// ==================================================

void timer() {

  ms++;
}

// ==================================================
// INICIALIZAR INTERSECCIÓN
// ==================================================

void iniciarInterseccion(
  Interseccion &inter) {

  // ==============================================
  // SEMÁFORO 1
  // ==============================================

  if (inter.sem1.habilitado) {

    pinMode(inter.sem1.pinRojo, OUTPUT);
    pinMode(inter.sem1.pinAmarillo, OUTPUT);
    pinMode(inter.sem1.pinVerde, OUTPUT);

    pinMode(inter.sem1.pinPeatonRojo, OUTPUT);
    pinMode(inter.sem1.pinPeatonVerde, OUTPUT);

    actualizarSemaforo(
      inter.sem1,
      SEM_ROJO);
  }

  // ==============================================
  // SEMÁFORO 2
  // ==============================================

  if (
    inter.tieneSemaforo2 && inter.sem2.habilitado) {

    pinMode(inter.sem2.pinRojo, OUTPUT);
    pinMode(inter.sem2.pinAmarillo, OUTPUT);
    pinMode(inter.sem2.pinVerde, OUTPUT);

    pinMode(inter.sem2.pinPeatonRojo, OUTPUT);
    pinMode(inter.sem2.pinPeatonVerde, OUTPUT);

    actualizarSemaforo(
      inter.sem2,
      SEM_ROJO);
  }
}

// ==================================================
// FSM INTERSECCIÓN
// ==================================================

void actualizarInterseccion(
  Interseccion &inter) {

  switch (inter.estado) {

      // ==============================================
      // INICIO
      // ==============================================

    case CTRL_INICIO:

      actualizarSemaforo(
        inter.sem1,
        SEM_ROJO);

      if (inter.tieneSemaforo2) {

        actualizarSemaforo(
          inter.sem2,
          SEM_ROJO);
      }

      inter.ultimoCambio = ms;

      inter.estado =
        CTRL_S1_ROJO_AMARILLO;

      break;

      // ==============================================
      // SEMÁFORO 1
      // ==============================================

    case CTRL_S1_ROJO_AMARILLO:

      actualizarSemaforo(
        inter.sem1,
        SEM_ROJO_AMARILLO);

      if (inter.tieneSemaforo2) {

        actualizarSemaforo(
          inter.sem2,
          SEM_ROJO);
      }

      if (
        temporizadorCumplido(
          inter,
          2000)) {

        inter.ultimoCambio = ms;

        inter.estado =
          CTRL_S1_VERDE;
      }

      break;

    case CTRL_S1_VERDE:

      actualizarSemaforo(
        inter.sem1,
        SEM_VERDE);

      if (inter.tieneSemaforo2) {

        actualizarSemaforo(
          inter.sem2,
          SEM_ROJO);
      }

      if (
        temporizadorCumplido(
          inter,
          inter.sem1.tiempoVerde * 1000UL)) {

        inter.ultimoCambio = ms;

        inter.estado =
          CTRL_S1_AMARILLO;
      }

      break;

    case CTRL_S1_AMARILLO:

      actualizarSemaforo(
        inter.sem1,
        SEM_AMARILLO);

      if (inter.tieneSemaforo2) {

        actualizarSemaforo(
          inter.sem2,
          SEM_ROJO);
      }

      if (
        temporizadorCumplido(
          inter,
          inter.sem1.tiempoAmarillo * 1000UL)) {

        inter.ultimoCambio = ms;

        // ==========================================
        // SI EXISTE SEMÁFORO 2
        // ==========================================

        if (inter.tieneSemaforo2) {

          inter.estado =
            CTRL_S2_ROJO_AMARILLO;
        }

        // ==========================================
        // SI NO EXISTE
        // ==========================================

        else {

          inter.estado =
            CTRL_S1_ROJO_AMARILLO;
        }
      }

      break;

      // ==============================================
      // SEMÁFORO 2
      // ==============================================

    case CTRL_S2_ROJO_AMARILLO:

      actualizarSemaforo(
        inter.sem1,
        SEM_ROJO);

      actualizarSemaforo(
        inter.sem2,
        SEM_ROJO_AMARILLO);

      if (
        temporizadorCumplido(
          inter,
          2000)) {

        inter.ultimoCambio = ms;

        inter.estado =
          CTRL_S2_VERDE;
      }

      break;

    case CTRL_S2_VERDE:

      actualizarSemaforo(
        inter.sem1,
        SEM_ROJO);

      actualizarSemaforo(
        inter.sem2,
        SEM_VERDE);

      if (
        temporizadorCumplido(
          inter,
          inter.sem2.tiempoVerde * 1000UL)) {

        inter.ultimoCambio = ms;

        inter.estado =
          CTRL_S2_AMARILLO;
      }

      break;

    case CTRL_S2_AMARILLO:

      actualizarSemaforo(
        inter.sem1,
        SEM_ROJO);

      actualizarSemaforo(
        inter.sem2,
        SEM_AMARILLO);

      if (
        temporizadorCumplido(
          inter,
          inter.sem2.tiempoAmarillo * 1000UL)) {

        inter.ultimoCambio = ms;

        inter.estado =
          CTRL_S1_ROJO_AMARILLO;
      }

      break;
  }
}

// ==================================================
// FSM INDIVIDUAL SEMÁFORO
// ==================================================

void actualizarSemaforo(
  Semaforo &sem,
  estadoSemaforo nuevoEstado) {

  // ==============================================
  // SI NO EXISTE
  // ==============================================

  if (!sem.habilitado) {
    return;
  }

  // ==============================================
  // EVITAR REESCRIBIR
  // ==============================================

  if (sem.estado == nuevoEstado) {
    return;
  }

  sem.estado = nuevoEstado;

  switch (nuevoEstado) {

      // ==============================================
      // ROJO
      // ==============================================

    case SEM_ROJO:

      digitalWrite(sem.pinRojo, HIGH);
      digitalWrite(sem.pinAmarillo, LOW);
      digitalWrite(sem.pinVerde, LOW);

      digitalWrite(sem.pinPeatonRojo, HIGH);
      digitalWrite(sem.pinPeatonVerde, LOW);

      break;

      // ==============================================
      // ROJO + AMARILLO
      // ==============================================

    case SEM_ROJO_AMARILLO:

      digitalWrite(sem.pinRojo, HIGH);
      digitalWrite(sem.pinAmarillo, HIGH);
      digitalWrite(sem.pinVerde, LOW);

      digitalWrite(sem.pinPeatonRojo, HIGH);
      digitalWrite(sem.pinPeatonVerde, LOW);

      break;

      // ==============================================
      // VERDE
      // ==============================================

    case SEM_VERDE:

      digitalWrite(sem.pinRojo, LOW);
      digitalWrite(sem.pinAmarillo, LOW);
      digitalWrite(sem.pinVerde, HIGH);

      digitalWrite(sem.pinPeatonRojo, LOW);
      digitalWrite(sem.pinPeatonVerde, HIGH);

      break;

      // ==============================================
      // AMARILLO
      // ==============================================

    case SEM_AMARILLO:

      digitalWrite(sem.pinRojo, LOW);
      digitalWrite(sem.pinAmarillo, HIGH);
      digitalWrite(sem.pinVerde, LOW);

      digitalWrite(sem.pinPeatonRojo, HIGH);
      digitalWrite(sem.pinPeatonVerde, LOW);

      break;

      // ==============================================
      // APAGADO
      // ==============================================

    case SEM_APAGADO:

      digitalWrite(sem.pinRojo, LOW);
      digitalWrite(sem.pinAmarillo, LOW);

      digitalWrite(sem.pinVerde, LOW);

      digitalWrite(sem.pinPeatonRojo, LOW);
      digitalWrite(sem.pinPeatonVerde, LOW);

      break;
  }
}
// ==================================================
// TEMPORIZADOR
// ==================================================

bool temporizadorCumplido(
  Interseccion &inter,
  unsigned long tiempoObjetivo) {

  return (
    (ms - inter.ultimoCambio)
    >= tiempoObjetivo);
}