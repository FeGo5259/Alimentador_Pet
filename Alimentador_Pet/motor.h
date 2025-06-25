#ifndef MOTOR_H
#define MOTOR_H

// Função para inicializar o motor e seus pinos.
void inicializarMotor();

// Sinaliza para a task do motor começar a girar.
// A velocidade será calculada com base na porção definida.
void ligarMotor();

// Sinaliza para a task do motor parar de girar e desliga os pinos.
void desligarMotor();

// Define o tamanho da porção em gramas a ser liberada.
void setPorcao(int gramas);

#endif
