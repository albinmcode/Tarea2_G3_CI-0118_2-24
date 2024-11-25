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

int escala(float* matrizEscala) {
	// Input
	std::cout << "Opcion seleccionada: Escala.\n";
	std::cout << "Seleccione una opcion para la escala:\n";
	std::cout << "1. Aumentar\n";
	std::cout << "2. Disminuir\n";
	int scaleChoice;
	std::cin >> scaleChoice;
	// Validar el valor ingresado
	if (std::cin.fail()) {
		std::cerr << "Entrada invalida para el angulo. Debe ser un número." << std::endl;
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
		std::cout << "Opcion no valida. Intente nuevamente.\n";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return EXIT_FAILURE;
	}

	// Configura la matriz de transformación para escalar los vértices
	float scaleFactor[VECTOR_SIZE] = { scale, scale, scale };  // Escala simetrica en todos los ejes
	scaleMatrix(matrizEscala, scaleFactor, matrizEscala);
	
	return EXIT_SUCCESS;
}

int rotacion(float* matrizRotacion, Matriz4x4& generatriz) {
	// Input
	double angle = 0.0;
	char axis = ' ';
	// Solicitar el ángulo de rotación
	std::cout << "Ingrese el angulo de rotacion (en grados): ";
	std::cin >> angle;
	// Validar el valor ingresado
	if (std::cin.fail()) {
		std::cerr << "Entrada invalida para el angulo. Debe ser un número." << std::endl;
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
	generatriz.rotation(matrizRotacion, angle, axis);

	return EXIT_SUCCESS;
}

int traslacion(float* matrizTraslacion) {
	float traslationVec[VECTOR_SIZE] = {0};
	// Input
	for (int i = 0; i < 3; i++) {
		std::cout << "Traslacion en " << static_cast<char>('x' + i) << ": ";
		std::cin >> traslationVec[i];
	}
	// Configura la matriz de transformación para trasladar
	translateMatrix(matrizTraslacion, traslationVec, matrizTraslacion);

	return EXIT_SUCCESS;
}

int componerTransf(float* matrizComp, Matriz4x4& generatriz) {
	generatriz.Indentidad(matrizComp);
	// Input (pedir las composiciones que se desea hacer, entiendase con 1,2)
	std::cout << "Ingrese las 2 composiciones que desea hacer (1,2,3): ";
	int comp1, comp2 = 0;
	std::cin >> comp1;
	std::cin >> comp2;

	// Escala
	if (comp1 == 1 || comp2 == 1) {
		// Obtener matriz
		float matrizEscala[MATRIZ_SIZE] = { 0 };
		if (escala(matrizEscala) == EXIT_FAILURE) return EXIT_FAILURE;
		// Multiplicacion de la composición y escala
		mulMatrix4x4(matrizComp, matrizEscala, matrizComp);
	}
	// Rotación
	if (comp1 == 2 || comp2 == 2) {
		// Obtener matriz
		float matrizRotacion[MATRIZ_SIZE] = { 0 };
		if (rotacion(matrizRotacion, generatriz) == EXIT_FAILURE) return EXIT_FAILURE;
		// Multiplicacion de la composición y rotacion
		mulMatrix4x4(matrizComp, matrizRotacion, matrizComp);
	}
	// Traslación
	if (comp1 == 3 || comp2 == 3) {
		// Obtener matriz
		float matrizTraslacion[MATRIZ_SIZE] = { 0 };
		generatriz.Indentidad(matrizTraslacion);
		if (traslacion(matrizTraslacion) == EXIT_FAILURE) return EXIT_FAILURE;
		// Multiplicacion de la composición y traslacion
		mulMatrix4x4(matrizComp, matrizTraslacion, matrizComp);
	}

	return EXIT_SUCCESS;
}

int menu(CubeUI& cubo, Matriz4x4& generatriz) {
	std::cout << "\n//////////////////////////////////////\n"
		<< "Seleccione una opcion:\n"
		<< "1. Traslacion\n"
		<< "2. Rotacion\n"
		<< "3. Escala\n"
		<< "4. Composicion\n";

	int choice;
	float matrizT[MATRIZ_SIZE] = { 0 };  // matriz de la transformación
	generatriz.Indentidad(matrizT);
	std::cin >> choice;

	switch (choice) {
	case 1:
		if (traslacion(matrizT) == EXIT_FAILURE) return EXIT_FAILURE;
		break;
	case 2:
		if (rotacion(matrizT, generatriz) == EXIT_FAILURE) return EXIT_FAILURE;
		break;
	case 3:
		if (escala(matrizT) == EXIT_FAILURE) return EXIT_FAILURE;
		break;
	case 4:
		if (componerTransf(matrizT, generatriz) == EXIT_FAILURE) return EXIT_FAILURE;
		break;
	default:
		std::cout << "Opcion no valida. Intente nuevamente.\n";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return EXIT_FAILURE;
	}
	std::cout << "//////////////////////////////////////\n";

	// Aplicar transformación al cubo
	TranformarVertices(matrizT, cubo);

	return EXIT_SUCCESS;
}

int main() {
	Matriz4x4 generatriz;
	CubeUI cube;

	while (cube.deviceRun()) {
		cube.render();
		menu(cube, generatriz);
		std::cin.clear();
	}
	
	return 0;
}
