
#define PINO_RX 13
#define PINO_TX 13
#define PINO_DADOS 10
#define PINO_CLOCK 9
#define BAUD_RATE 1
#define HALF_BAUD 1000/(2*BAUD_RATE)

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

// Executada uma vez quando o Arduino reseta
void setup(){
  //desabilita interrupcoes
  noInterrupts();
  // Configura porta serial (Serial Monitor - Ctrl + Shift + M)
  Serial.begin(9600);

  // Inicializa TX ou RX
  //>>>> Codigo Aqui <<<<
  // Configura timer
  //>>>> Codigo Aqui <<<<
  // habilita interrupcoes
  interrupts();
}

// O loop() eh executado continuamente (como um while(true))
void loop ( ) {
  //>>>> Codigo Aqui <<<<
}
