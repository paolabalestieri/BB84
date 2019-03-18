//////////////////////////////////////////////////////////
/* AliceArduino.ino                                     */
/* Código que recebe as bases e bits enviados pela      */
/* interface e comunica através do protocolo i2c para   */
/* outro arduino (Bob).                                 */
/*                                                      */
/* Projeto de extensão: Protocolo BB84                  */
/* Aluna: Paola dos Santos Balestieri                   */
/* Orientador: Bruno Gouvea Taketani                    */
//////////////////////////////////////////////////////////

#include "Wire.h"

#define slaveAdress 0x08

const int N = 8; // Número de qbits
int i, count = 0;
boolean basis[N], bits[N];
char val;

void setup() {
	Wire.begin(); // Inicia a comunicação i2c (entre arduinos)
	Serial.begin(9600); // Inicia a comunicação serial (entre computador e arduino)
}

void loop() {
	if (Serial.available()){ // Se o computador mandar alguma coisa
		val = Serial.read(); // Pega a mensagem
		
		if(val == 'R'){ // Processing envia 'R' para reiniciar
			for(i=0; i<N; i++){ // Zera tudo
				basis[i] = 0;
				bits[i] = 0;
			}
			count = 0; // E reinicia o contador

      Wire.beginTransmission(0x08);
			Wire.write('R'); // Envia 'R' para o outro arduino 
      Wire.endTransmission();
		}

    /* Cada bit/base é enviado individualmente por isso precisa do contador */
		else{ // Se não for 'R' então é um valor de base ou de bit
			if(count<8){ // As primeiras 8 serão bases (como programado no Processing)
				if(val == '1'){
					basis[count] = HIGH;
				}
				else{
					basis[count] = LOW;
				}
			}
			else{ // Depois, de 8 a 16 serão bits
				if(val == '1'){
					bits[count-8] = HIGH;
				}
				else{
					bits[count-8] = LOW;
				}
			}
			count++; 
     
			if(count>=16){ // Quando o contador chegar em 16, todas as bases e bits são conhecidas
				for(i=0; i<N; i++){ 
					Wire.beginTransmission(slaveAdress);
					Wire.write(basis[i]); // envia as bases para o outro arduino
					Wire.endTransmission();
				}
				for(i=0; i<N; i++){
					Wire.beginTransmission(slaveAdress);
					Wire.write(bits[i]); // envia os bits para o outro arduino
					Wire.endTransmission();
				}
				count = 0; // e zera o contador para recomeçar
			}
		}
	}
 
	delay(10);
}
