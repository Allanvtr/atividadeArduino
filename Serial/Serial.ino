#define PINO_RTS 13
#define PINO_CTS 12
#define PINO_DADOS 10
#define PINO_CLOCK 9
#define BAUD_RATE 1

#include "Temporizador.h"

bool recebendo = false;
int contadorBit = 0;
char digito = 0;
int paridade;

// Calcula bit de paridade - Par ou impar
bool bitParidade(char dado){
  int contador = 0;

  for(int i = 0; i < 8; i++){
    if ((dado >> i) & 1 == 1) contador++;
  }

  return contador % 2 == 0;
}

// Rotina de interrupcao do timer1
// O que fazer toda vez que 1s passou?
ISR(TIMER1_COMPA_vect){
  if(!recebendo) return;

  if (digitalRead(PINO_CLOCK) == HIGH) {
    int bit = digitalRead(PINO_DADOS);

    if(contadorBit < 8) digito = (digito << 1) | bit;
    if(contadorBit == 8) paridade = bit;

    contadorBit++;
  }

  if(contadorBit > 8) {
    recebendo = false;
    contadorBit = 0;

    if(bitParidade(digito) == paridade) Serial.print(digito);
    else Serial.print("Erro");
  }
}

void setup(){
  noInterrupts();
  Serial.begin(9600); // setting up serial monitor (Serial Monitor - Ctrl + Shift + M)

  //Inicializating pins (TX or RX)
  pinMode(PINO_RTS, INPUT);   //Reading RTS
  pinMode(PINO_CTS, OUTPUT);  //Sending CTS
  pinMode(PINO_DADOS, INPUT); //Reading data bits
  pinMode(PINO_CLOCK, INPUT); // Reading clock

  digitalWrite(PINO_CTS, LOW); 

  configuraTemporizador(BAUD_RATE); //timer

  interrupts(); //enabling interrupts
}

void loop() {
  //RTS == HIGH, but we did'nt start receiving yet (CTS == LOW)
  if (digitalRead(PINO_RTS) == HIGH && digitalRead(PINO_CTS) == LOW) {
    recebendo = true;
    contadorBit = 0;
    digito = 0;
    
    digitalWrite(PINO_CTS, HIGH); //telling the emissor that we are ready
    iniciaTemporizador();
  }

  //RTS == LOW, but we are receiving (CTS == HIGH)
  if (digitalRead(PINO_RTS) == LOW && digitalRead(PINO_CTS) == HIGH) {
    paraTemporizador();
    digitalWrite(PINO_CTS, LOW);
  }
}
