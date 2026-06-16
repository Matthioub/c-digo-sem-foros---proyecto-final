
#define BLANCO 35 
#define NARANJA 33 
#define VERDE 31 
#define AMARILLO 29 
#define ROJO 27 

#define BLANCO2 A8 
#define NARANJA2 A9 
#define VERDE2 A10 
#define AMARILLO2 A11 
#define ROJO2 A12 

#define BLANCO3 A2 
#define NARANJA3 A3 
#define VERDE3 A4 
#define AMARILLO3 A5 
#define ROJO3 A6

#define BLANCO4 45
#define NARANJA4 43
#define VERDE4 41
#define AMARILLO4 39 
#define ROJO4 37 

void setup() {
  pinMode(ROJO, OUTPUT);
  pinMode(AMARILLO, OUTPUT);
  pinMode(VERDE, OUTPUT);
  pinMode(NARANJA, OUTPUT);
  pinMode(BLANCO, OUTPUT);

  pinMode(ROJO2, OUTPUT);
  pinMode(AMARILLO2, OUTPUT);
  pinMode(VERDE2, OUTPUT);
  pinMode(NARANJA2, OUTPUT);
  pinMode(BLANCO2, OUTPUT);

  pinMode(ROJO3, OUTPUT);
  pinMode(AMARILLO3, OUTPUT);
  pinMode(VERDE3, OUTPUT);
  pinMode(NARANJA3, OUTPUT);
  pinMode(BLANCO3, OUTPUT);

  pinMode(ROJO4, OUTPUT);
  pinMode(AMARILLO4, OUTPUT);
  pinMode(VERDE4, OUTPUT);
  pinMode(NARANJA4, OUTPUT);
  pinMode(BLANCO4, OUTPUT);
}

void loop() {
  analogWrite(ROJO, 1023);
  analogWrite(AMARILLO, 1023);
  analogWrite(VERDE, 1023);
  analogWrite(NARANJA, 1023);
  analogWrite(BLANCO, 1023);

  analogWrite(ROJO2, 1023);
  analogWrite(AMARILLO2, 1023);
  analogWrite(VERDE2, 1023);
  analogWrite(NARANJA2, 1023);
  analogWrite(BLANCO2, 1023);

  analogWrite(ROJO3, 1023);
  analogWrite(AMARILLO3, 1023);
  analogWrite(VERDE3, 1023);
  analogWrite(NARANJA3, 1023);
  analogWrite(BLANCO3, 1023);

  analogWrite(ROJO4, 1023);
  analogWrite(AMARILLO4, 1023);
  analogWrite(VERDE4, 1023);
  analogWrite(NARANJA4, 1023);
  analogWrite(BLANCO4, 1023);
}
