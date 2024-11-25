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

extern "C" {
	void mulMatrixVector4x1(float* matrix4x4Ptr, float* vector4x1Ptr, float* resultVect4x1Ptr);
	void mulMatrix4x4(float* matrixAPtr, float* matrixBPtr, float* resMatrix4x4);
	void translateMatrix(float* matrix4x4Ptr, float* translationVector3x1Ptr, float* resultMatrix4x4Ptr);
	void scaleMatrix(float* matrix4x4Ptr, float* scaleVector3x1Ptr, float* resultMatrix4x4Ptr);
}

void cargarVector(float* vector, size_t vecSize) {
	std::cout << "Ingrese las entradas del vector:\n" << "( ";
	for (size_t index = 0; index < vecSize; ++index) {
		std::cin >> vector[index];
		// std::cout << ((index == vecSize - 1) ? "   )\n" : "  ");
	}
	std::cout << " )";
}

void imprimirVector(float* vect, size_t vectSize) {
	std::cout << std::endl << "Vector resultante: [";
	std::cout << std::fixed;
	std::cout.precision(1);	// precision de 3 decimales
	for (size_t index = 0; index < vectSize; index++) {
		std::cout << vect[index] << ((index == vectSize - 1) ? "]\n" : ", ");
	}
}

// Operarar matriz por vector entre la m. de la transformación y los vertices
void TranformarVertices(float matrizTransform[MATRIZ_SIZE], CubeUI& cubo) {
	// Inicializa vector extendido con w = 1
	float vector[VECTOR_EXTEND] = { (0) };
	vector[VECTOR_SIZE] = 1;
	float newVector[VECTOR_EXTEND] = { (0) };
	// Itera sobre cada vértice del cubo
	for (size_t vertice = 0; vertice < CUBE_VERTICES; ++vertice) {
		// Copia las coordenadas (x, y, z) del vértice actual al vector extendido R4
		for (size_t index = 0; index < VECTOR_SIZE; ++index) {
			vector[index] = cubo.getVertices()[vertice * VECTOR_SIZE + index];
		}
		// Multiplica la matriz de transformacion por el vector extendido
		mulMatrixVector4x1(matrizTransform, vector, newVector);

		// Actualiza las coordenadas del vértice tranformado en el cubo
		for (size_t index = 0; index < VECTOR_SIZE; ++index) {
			cubo.getVertices()[vertice * VECTOR_SIZE + index] = newVector[index];
		}
	}
}

int escalarCubo(CubeUI& cubo, Matriz4x4& matriz) {
	// Input
	std::cout << "Opcion seleccionada: Escala.\n";
	std::cout << "Seleccione una opcion para la escala:\n";
	std::cout << "1. Aumentar\n";
	std::cout << "2. Disminuir\n";
	int scaleChoice;
	std::cin >> scaleChoice;
	// Validar el valor ingresado
	if (std::cin.fail()) {
		std::cerr << "Entrada inválida para el ángulo. Debe ser un número." << std::endl;
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
		std::cout << "Opcion no válida. Intente nuevamente.\n";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return EXIT_FAILURE;
	}

	// Configura la matriz de transformación para escalar los vértices
	float matrizEscala[MATRIZ_SIZE] = {0};
	float scaleFactor[VECTOR_SIZE] = { scale, scale, scale };  // Escala simetrica en todos los ejes
	scaleMatrix(matrizEscala, scaleFactor, matrizEscala);
	
	// Opera los vectos del cubo y la matriz de escala
	TranformarVertices(matrizEscala, cubo);
	return 0;
}

int rotarCubo(CubeUI& cubo, Matriz4x4& matriz) {
	// Input
	double angle = 0.0;
	char axis = ' ';
	// Solicitar el ángulo de rotación
	std::cout << "Ingrese el ángulo de rotacion (en grados): ";
	std::cin >> angle;
	// Validar el valor ingresado
	if (std::cin.fail()) {
		std::cerr << "Entrada inválida para el ángulo. Debe ser un número." << std::endl;
		return EXIT_FAILURE;
	}
	// Solicitar el eje de rotación
	std::cout << "Ingrese el eje de rotacion (x, y, o z): ";
	std::cin >> axis;
	if (tolower(axis) < 'x' || tolower(axis) > 'z') {
		std::cerr << "Eje especificado invalido." << std::endl;
		return EXIT_FAILURE;
	}

	// Configura la matriz de transformación para rotar
	float matrizRotacion[MATRIZ_SIZE] = { 0 };
	matriz.rotation(matrizRotacion, angle, axis);
	// Opera los vectos del cubo y la matriz de rotación
	TranformarVertices(matrizRotacion, cubo);

	return EXIT_SUCCESS;
}

int trasladarCubo(CubeUI& cubo, Matriz4x4& matriz) {
	float traslationVec[VECTOR_SIZE] = {0};
	float matrix[MATRIZ_SIZE] = { 0 };
	// Input
	matriz.Indentidad(matrix);
	for (int i = 0; i < 3; i++) {
		std::cout << "Traslacion en " << static_cast<char>('x' + i) << ": ";
		std::cin >> traslationVec[i];
	}
	// Configura la matriz de transformación para trasladar
	translateMatrix(matrix, traslationVec, matrix);
	// Opera los vectos del cubo y la matriz de traslación
	TranformarVertices(matrix, cubo);

	return 0;
}

int componerTransf(float* resultMatrix) {
	float compositionM[MATRIZ_SIZE] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1 };
	float matriz[MATRIZ_SIZE] = { 0 };
	// Input (pedir las composiciones que se desea hacer, entiendase con 1,2)
	std::cout << "Ingrese las composiciones que desea hacer (1,2,3): ";
	int comp1, comp2 = 0;
	std::cin >> comp1;
	std::cin >> comp2;
	if (comp1 == 1 || comp2 == 1) {
		// Input
		std::cout << "Opcion seleccionada: Escala.\n";
		std::cout << "Seleccione una opcion para la escala:\n";
		std::cout << "1. Aumentar\n";
		std::cout << "2. Disminuir\n";
		int scaleChoice;
		std::cin >> scaleChoice;
		// Validar el valor ingresado
		if (std::cin.fail()) {
			std::cerr << "Entrada inválida para el ángulo. Debe ser un número." << std::endl;
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
			std::cout << "Opcion no válida. Intente nuevamente.\n";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return EXIT_FAILURE;
		}

		// Configura la matriz de transformación para escalar los vértices
		float matrizEscala[MATRIZ_SIZE] = { 0 };
		float scaleFactor[VECTOR_SIZE] = { scale, scale, scale };  // Escala simetrica en todos los ejes
		scaleMatrix(matrizEscala, scaleFactor, matriz);
		mulMatrix4x4(matriz, compositionM, compositionM);
	}
	if (comp1 == 2 || comp2 == 2) {
		// Input
		Matriz4x4 rotationM;
		double angle = 0.0;
		char axis = ' ';
		// Solicitar el ángulo de rotación
		std::cout << "Ingrese el ángulo de rotacion (en grados): ";
		std::cin >> angle;
		// Validar el valor ingresado
		if (std::cin.fail()) {
			std::cerr << "Entrada inválida para el ángulo. Debe ser un número." << std::endl;
			return EXIT_FAILURE;
		}
		// Solicitar el eje de rotación
		std::cout << "Ingrese el eje de rotacion (x, y, o z): ";
		std::cin >> axis;
		if (tolower(axis) < 'x' || tolower(axis) > 'z') {
			std::cerr << "Eje especificado invalido." << std::endl;
			return EXIT_FAILURE;
		}

		// Configura la matriz de transformación para rotar
		float matrizRotacion[MATRIZ_SIZE] = { 0 };
		rotationM.rotation(matrizRotacion, angle, axis);
		mulMatrix4x4(matrizRotacion, compositionM, compositionM);
	}
	if (comp1 == 3 || comp2 == 3) {
		float traslationVec[VECTOR_SIZE] = { 0 };
		float matrix[MATRIZ_SIZE] = { 0 };
		// Input
		Matriz4x4 traslationM;
		traslationM.Indentidad(matrix);
		for (int i = 0; i < 3; i++) {
			std::cout << "Traslacion en " << static_cast<char>('x' + i) << ": ";
			std::cin >> traslationVec[i];
		}
		// Configura la matriz de transformación para trasladar
		translateMatrix(matrix, traslationVec, matrix);
		mulMatrix4x4(matrix, compositionM, compositionM);
	}
	// Copiar la matriz resultante a la matriz de composición
	for (size_t index = 0; index < MATRIZ_SIZE; ++index) {
		resultMatrix[index] = compositionM[index];
	}

	// Imprimir la matriz resultante
	std::cout << "Matriz resultante: \n";
	for (size_t index = 0; index < MATRIZ_SIZE; ++index) {
		std::cout << compositionM[index] << ((index % 4 == 3) ? "\n" : " ");
	}
	return 0;
}

int menu(CubeUI& cubo, Matriz4x4& matriz) {
	std::cout << "\n//////////////////////////////////////\n"
	<< "Seleccione una opcion:\n"
	<< "1. Traslacion\n"
	<< "2. Rotacion\n"
	<< "3. Escala\n"
	<< "4. Composicion\n";


	int choice;
	float identidad[MATRIZ_SIZE] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1 };
	std::cin >> choice;

	switch (choice) {
	case 1:
		trasladarCubo(cubo,matriz);
		break;
	case 2:
		rotarCubo(cubo, matriz);
		break;
	case 3:
		escalarCubo(cubo, matriz);
		break;
	case 4:
		componerTransf(identidad);
		break;
	default:
		std::cout << "Opcion no válida. Intente nuevamente.\n";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return EXIT_FAILURE;
	}
	std::cout << "//////////////////////////////////////\n";
	return 0;
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
