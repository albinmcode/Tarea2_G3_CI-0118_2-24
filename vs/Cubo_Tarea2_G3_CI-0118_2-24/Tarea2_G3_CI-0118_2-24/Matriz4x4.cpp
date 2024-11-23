#include "Matriz4x4.hpp"
#include <iostream>
#include <stdexcept>
#include <cmath>

Matriz4x4::Matriz4x4() {}
int Matriz4x4::Indentidad(float matriz[MATRIZ_SIZE]) {
    if (matriz == nullptr) return EXIT_FAILURE;
	for (size_t index = 0; index < MATRIZ_SIZE; ++index) {
		matriz[index] = this->matrizIdentidad[index];
	}
    return 0;
}

int Matriz4x4::escale(float matriz[MATRIZ_SIZE], const float vectorScale[VECTOR_SIZE]) {
	this->Indentidad(matriz);
	// diagonal de la matriz identidad = escala
	matriz[0] = vectorScale[0];
	matriz[1 * VECTOR_EXTEND + 1] = vectorScale[1];
	matriz[2 * VECTOR_EXTEND + 2] = vectorScale[2];
    return 0;
}

int Matriz4x4::rotation(float matriz[MATRIZ_SIZE], double angle, const char axis) {
    const double PI = 3.14159265358979323846;
    angle = angle * PI / 180;
    float* rotationMatrix = nullptr;
    switch (tolower(axis)) {
        // Matrices de rotacion
        case 'x': {
            float rotationX[MATRIZ_SIZE] = {
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, cos((float)angle), -sin((float)angle), 0.0f,
                0.0f, sin((float)angle), cos((float)angle), 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };
            rotationMatrix = rotationX;
            break;
        }
        case 'y': {
            float rotationY[MATRIZ_SIZE] = {
                cos((float)angle), 0.0f, sin((float)angle), 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                -sin((float)angle), 0.0f, cos((float)angle), 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };
            rotationMatrix = rotationY;
            break;
        }
        case 'z': {
            float rotationZ[16] = {
                cos((float)angle), -sin((float)angle), 0.0f, 0.0f,
                sin((float)angle), cos((float)angle), 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };
            rotationMatrix = rotationZ;
            break;
        }
        default: {
            std::cout << "Error: Eje de rotacion no valido" << std::endl;
            return EXIT_FAILURE;
        }      
    }
    for (size_t index = 0; index < MATRIZ_SIZE; ++index) {
        matriz[index] = rotationMatrix[index];
    }
    return 0;
}