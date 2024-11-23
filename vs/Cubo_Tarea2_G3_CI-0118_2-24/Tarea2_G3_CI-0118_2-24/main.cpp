#include <iostream>

#include "common.hpp"
#include "Matriz4x4.hpp"
#include "CubeUI.hpp"

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
		// std::cout << ((index == vecSize - 1) ? "   )\n" : "  ");
	}
	std::cout << " )";
}
// Operarar matriz por vector entre la m. de la transformaci�n y los vertices
void TranformarVertices(float matrizTransform[MATRIZ_SIZE], CubeUI& cubo) {
	// Inicializa vector extendido con w = 1
	float vector[VECTOR_EXTEND] = { (0) };
	vector[VECTOR_SIZE] = 1;
	float newVector[VECTOR_EXTEND] = { (0) };
	// Itera sobre cada v�rtice del cubo
	for (size_t vertice = 0; vertice < CUBE_VERTICES; ++vertice) {
		// Copia las coordenadas (x, y, z) del v�rtice actual al vector extendido R4
		for (size_t index = 0; index < VECTOR_SIZE; ++index) {
			vector[index] = cubo.getVertices()[vertice * VECTOR_SIZE + index];
		}
		// Multiplica la matriz de transformacion por el vector extendido
		mulMatrixVector4x1(matrizTransform, vector, newVector);

		// Actualiza las coordenadas del v�rtice tranformado en el cubo
		for (size_t index = 0; index < VECTOR_SIZE; ++index) {
			cubo.getVertices()[vertice * VECTOR_SIZE + index] = newVector[index];
		}
	}
}

int escalarCubo(CubeUI& cubo, Matriz4x4& matriz) {
	// Input
	std::cout << "Opci�n seleccionada: Escala.\n";
	std::cout << "Seleccione una opci�n para la escala:\n";
	std::cout << "1. Aumentar\n";
	std::cout << "2. Disminuir\n";
	int scaleChoice;
	std::cin >> scaleChoice;
	// Validar el valor ingresado
	if (std::cin.fail()) {
		std::cerr << "Entrada inv�lida para el �ngulo. Debe ser un n�mero." << std::endl;
		return EXIT_FAILURE;
	}

	float scale = 1.0f;
	switch (scaleChoice) {
	case 1:
		scale = 2.0f;
		break;
	case 2:
		scale = 0.5f;
		break;
	default:
		std::cout << "Opci�n no v�lida. Intente nuevamente.\n";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return EXIT_FAILURE;
	}

	// Configura la matriz de transformaci�n para escalar los v�rtices
	float matrizEscala[MATRIZ_SIZE] = { 0 };
	float scaleFactor[VECTOR_SIZE] = { scale, scale, scale };  // Escala simetrica en todos los ejes
	matriz.escale(matrizEscala, scaleFactor);
	// Opera los vectos del cubo y la matriz de escala
	TranformarVertices(matrizEscala, cubo);
	return 0;
}

int rotarCubo(CubeUI& cubo, Matriz4x4& matriz) {
	// Input
	double angle = 0.0;
	char axis = ' ';
	// Solicitar el �ngulo de rotaci�n
	std::cout << "Ingrese el �ngulo de rotaci�n (en grados): ";
	std::cin >> angle;
	// Validar el valor ingresado
	if (std::cin.fail()) {
		std::cerr << "Entrada inv�lida para el �ngulo. Debe ser un n�mero." << std::endl;
		return EXIT_FAILURE;
	}
	// Solicitar el eje de rotaci�n
	std::cout << "Ingrese el eje de rotaci�n (x, y, o z): ";
	std::cin >> axis;
	if (tolower(axis) < 'x' || tolower(axis) > 'z') {
		std::cerr << "Eje especificado invalido." << std::endl;
		return EXIT_FAILURE;
	}

	// Configura la matriz de transformaci�n para rotar
	float matrizRotacion[MATRIZ_SIZE] = { 0 };
	matriz.rotation(matrizRotacion, angle, axis);
	// Opera los vectos del cubo y la matriz de rotaci�n
	TranformarVertices(matrizRotacion, cubo);

	return EXIT_SUCCESS;
}

int TrasladarCubo(CubeUI& cubo, Matriz4x4& matriz) {
	return 0;
}

int menu(CubeUI& cubo, Matriz4x4& matriz) {
	std::cout << "\n//////////////////////////////////////\n";
	std::cout << "Seleccione una opci�n:\n";
	std::cout << "1. Traslaci�n\n";
	std::cout << "2. Rotaci�n\n";
	std::cout << "3. Escala\n";

	int choice;
	std::cin >> choice;

	switch (choice) {
	case 1:
		//handleTranslation();
		break;
	case 2:
		rotarCubo(cubo, matriz);
		break;
	case 3:
		escalarCubo(cubo, matriz);
		break;
	default:
		std::cout << "Opci�n no v�lida. Intente nuevamente.\n";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return EXIT_FAILURE;
	}
	return 0;
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
	Matriz4x4 matriz;
	CubeUI cube;

	while (cube.deviceRun()) {
		cube.render();
		menu(cube, matriz);
		std::cin.clear();
	}
	
	return 0;
}