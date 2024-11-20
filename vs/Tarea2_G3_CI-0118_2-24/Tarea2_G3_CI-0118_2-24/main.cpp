#define matrixRowSize 4
#define matrixColSize 4
#define vectorRowSize 4
#define vectorColSize 1
#define VECTOR_SIZE 3
#define VECTOR_EXTEND (VECTOR_SIZE + 1)

#include <iostream>

/**
* | 1 0 0 0 |   |x|
* | 0 1 0 0 | * |y|
* | 0 0 1 0 |   |z|
* | 0 0 0 1 |   |w|
* 
*/

extern "C" float* mulMatrixVector4x1(float* matrixPtr, float* vectorPtr, float* resultVectPtr);

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

int main() {
	// Operandos
	float matrizIdentidad[16] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	float vectorExtended[4] = { 0 };
	float vecResultado[8] = { 0 };

	// Input vector
	cargarVector(vectorExtended, VECTOR_SIZE);
	// PROC punto flotante
	mulMatrixVector4x1(matrizIdentidad, vectorExtended, vecResultado);
	//Imprimir
	imprimirVector(vecResultado);
	
	return 0;
}