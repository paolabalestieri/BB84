//////////////////////////////////////////////////////////
/* BobArduino.ino                                       */
/* Código que recebe as bases e bits enviados pelo      */
/* outro arduino (Alice) e compara com a base escolhida */
/* na interface feita em processing, "simulando" uma    */
/* medida quantica da polarização.                      */
/*                                                      */
/* Projeto de extensão: Protocolo BB84                  */
/* Aluna: Paola dos Santos Balestieri                   */
/* Orientador: Bruno Gouvea Taketani                    */
//////////////////////////////////////////////////////////

#include "Wire.h"

#define myAdress 0x08

const int N = 8; // Número de qbits
int i, nrand, flag, countBob=0, countAlice=0;
boolean basisAlice[N], bitsAlice[N];
boolean basisBob[N], bitsBob[N];
char val;

void setup() {
  Wire.begin(myAdress); // Inicia a comunicação i2c (entre arduinos)
  Wire.onReceive(receiveEvent); // Quando Alice enviar algo, chame a função receiveEvent()
  Serial.begin(9600); // Inicia a comunicação serial (computador-arduino)
}

/* Pega informações passadas pela Alice (i2c) */
void receiveEvent(int howMany) {
  if (Wire.available()) {
    char received = Wire.read(); // Guarda o valor recebido pela Alice numa variável

    if (received == 'R') { // Se for 'R', reinicia a contagem
      for (i = 0; i < N; i++) { 
        basisAlice[i] = 0;
        bitsAlice[i] = 0;
      }
      countAlice = 0;
      flag = 1;
    }

    /* Cada bit/base é enviado individualmente por isso precisa do contador */
    else { // Se não for 'R' então é uma base/bit
      if (countAlice >= 16) { countAlice = 0; } // Quando a contagem passar de 16, zera e recomeça
      if (countAlice < 8) { // Os primeiros 8 serão bases, conforme programado em AliceArduino.c
        if (received == 1) {
          basisAlice[countAlice] = HIGH;
        }
        else {
          basisAlice[countAlice] = LOW;
        }
      }
      else { // Depois, de 8 a 16 são bits
        if (received == 1) {
          bitsAlice[countAlice - 8] = HIGH;
        }
        else {
          bitsAlice[countAlice - 8] = LOW;
        }
      }
      countAlice++;
    }
  }
}


void loop() {
  if (Serial.available()) { // Se o computador mandar alguma coisa
    val = Serial.read(); // Pega a mensagem

    if (val == 'R') { // Processing envia 'R' para reiniciar
      for (i = 0; i < N; i++) { // Zera tudo do Bob
        basisBob[i] = 0;
        bitsBob[i] = 0;
      }
      countBob = 0; // E reinicia o contador dele
    }

    else if (val == 'S') { // Processing envia 'S' para dizer que pode medir
      for (i = 0; i < N; i++) { // Zera tudo do bob
        basisBob[i] = 0;
        bitsBob[i] = 0;
      }
      countBob = 0;
      flag = 1; // e liga a flag, para entrar no processo de "medição"
    }

    else { // Se não for 'R' e nem 'S', então a serial ta mandando a base escolhida por Bob na interface
      if (countBob >= 8) { countBob = 0; }
      if (val == '1') {
        basisBob[countBob] = HIGH;
      }
      else {
        basisBob[countBob] = LOW;
      }
      countBob++;
    }
  }

  /* MEDIDA: Quando tiver as bases e bits da Alice 
  + as bases do Bob + a flag habilitada (botão medir) */
  if(countAlice==16 && countBob==8 && flag==1){
    for (i = 0; i < N; i++) {
      if(basisBob[i]==basisAlice[i]){ // Se a base escolhida por Bob for igual a da Alice
        bitsBob[i]=bitsAlice[i]; // Então o bit é medido corretamente
      }
      else{ // Senão
        nrand=random(0,2); // JOGA MOEDA: nrand é aleatóriamente escolhido entre 0 ou 1
        if(nrand==0){ // Se for 0, então a medida de Bob é correta 
          bitsBob[i]=bitsAlice[i];
        }
        else{ // Se for 1, Bob mede errado
          bitsBob[i]=!bitsAlice[i];
        }
      }
      Serial.write(bitsBob[i]); // Envia o valor dos bits medidos para o Processing
    }
    flag=0; // Reinicia a flag
  }

  delay(10);
}
