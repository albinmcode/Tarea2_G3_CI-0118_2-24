#define matrixRowSize 4
#define matrixColSize 4
#define vectorRowSize 4
#define vectorColSize 1
#define VECTOR_SIZE 3
#define VECTOR_EXTEND (VECTOR_SIZE + 1)

#include <iostream>
#include <stdexcept>
#include <cmath>

/**
* | 1 0 0 0 |   |x|
* | 0 1 0 0 | * |y|
* | 0 0 1 0 |   |z|
* | 0 0 0 1 |   |w|
* 
*/

extern "C" float* mulMatrixVector4x1(float* matrixPtr, float* vectorPtr, float* resultVectPtr);
extern "C" void translateMatrix(float* matrixPtr, float* translationVectorPtr, float* resultMatrixPtr);

void cargarVector(float* vector, size_t vecSize) {
	std::cout << "Ingrese las entradas del vector:\n" << "( ";
	for (size_t index = 0; index < vecSize; ++index) {
		std::cin >> vector[index];
		std::cout << ((index == vecSize - 1) ? ")\n" : ", ");
	}
}

void imprimirVector(float* vect) {
	std::cout << std::endl << "Vector resultante: [";
	std::cout << std::fixed;
	std::cout.precision(1);	// precision de 3 decimales
	for (int index = 0; index < VECTOR_EXTEND; index++) {
		std::cout << vect[index] << ((index == VECTOR_EXTEND - 1) ? "]\n" : ", ");
	}
}

int scale(float vect[VECTOR_SIZE], float vectRes[VECTOR_EXTEND],const float vectorScale[VECTOR_SIZE]) {
	float matriz[16] = {0};
	// diagonal de la matriz identidad = escala
	matriz[0] = vectorScale[0];
	matriz[1 * VECTOR_EXTEND + 1] = vectorScale[1];
	matriz[2 * VECTOR_EXTEND + 2] = vectorScale[2];
	mulMatrixVector4x1(matriz, vect, vectRes);
	return 0;
}

void rotation(float* vect, float* vectRes, double angle, char axis) {
	double PI = 3.14159265358979323846;
	angle = angle * PI / 180;
    switch (axis) {
		// Matrices de rotacion
    case 'x': {
        float rotationX[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, cos((float)angle), -sin((float)angle), 0.0f,
            0.0f, sin((float)angle), cos((float)angle), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
		mulMatrixVector4x1(rotationX, vect, vectRes);
		break;
    }
    case 'y': {
        float rotationY[16] = {
            cos((float)angle), 0.0f, sin((float)angle), 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -sin((float)angle), 0.0f, cos((float)angle), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
		mulMatrixVector4x1(rotationY, vect, vectRes);
		break;
    }
    case 'z': {
        float rotationZ[16] = {
            cos((float)angle), -sin((float)angle), 0.0f, 0.0f,
            sin((float)angle), cos((float)angle), 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
		mulMatrixVector4x1(rotationZ, vect, vectRes);
		break;
    }
	default: {
		std::cout << "Error: Eje de rotacion no valido" << std::endl;
		break;
	}
    }
}


int main() {
	float vectorExtended[4] = { 0,0,0,0 };
	float vecResultado[4] = { 0 };

	// Input vector
	cargarVector(vectorExtended, VECTOR_SIZE);
	// esto era un test de rotacion
	rotation(vectorExtended, vecResultado, 90, 'x');
	//Imprimir
	imprimirVector(vecResultado);

	//Metodo de traslacion

	float matrix[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	float resultMatrix[16] = { 0 };
	translateMatrix(matrix, vectorExtended,resultMatrix);

	std::cout << "Matriz despues de la traslacion:" << std::endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << resultMatrix[i * 4 + j] << " ";
		}
		std::cout << std::endl;
	}
	

	// esto era un test de escala
	std::cout << "\nEscala:\n";
	float scaleVector[VECTOR_SIZE] = { 2, 2, 2 };
	scale(vectorExtended, vecResultado, scaleVector);
	//Imprimir
	imprimirVector(vecResultado);
	return 0;
}
