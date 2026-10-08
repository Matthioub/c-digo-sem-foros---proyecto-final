// Prueba de cableado: enciende las cinco luces de los seis semáforos.
// Fila de impares: 1, 3, 5. Fila de pares: 2, 4, 6.
const int CANTIDAD_SEMAFOROS = 6;
const int LUCES_POR_SEMAFORO = 5;

// Pines en el orden proporcionado para cada semáforo.
const int pinesSemaforos[CANTIDAD_SEMAFOROS][LUCES_POR_SEMAFORO] = {
  {A2, A3, A4, A5, A6},    // Semáforo 1
  {3, 4, 5, 6, 7},         // Semáforo 2
  {18, 17, 16, 15, 14},    // Semáforo 3
  {A8, A9, A10, A11, A12}, // Semáforo 4
  {35, 33, 31, 29, 27},    // Semáforo 5
  {45, 43, 41, 39, 37}     // Semáforo 6
};

void setup() {
  for (int semaforo = 0; semaforo < CANTIDAD_SEMAFOROS; semaforo++) {
    for (int luz = 0; luz < LUCES_POR_SEMAFORO; luz++) {
      pinMode(pinesSemaforos[semaforo][luz], OUTPUT);
      digitalWrite(pinesSemaforos[semaforo][luz], HIGH);
    }
  }
}

void loop() {
  // Las salidas mantienen HIGH: todas las luces quedan encendidas.
}
