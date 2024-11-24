#pragma once

#include "common.hpp"

class Matriz4x4 {
 public:
	 // Constructor
	 Matriz4x4();
	 // Rellenar matriz con la matriz identidad
	 int Indentidad(float matriz[MATRIZ_SIZE]);
	 // Cargar matrices de transformaci�n
	 int rotation(float matriz[MATRIZ_SIZE], double angle, const char axis);

 private:
	 float matrizIdentidad[MATRIZ_SIZE] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	 };
};