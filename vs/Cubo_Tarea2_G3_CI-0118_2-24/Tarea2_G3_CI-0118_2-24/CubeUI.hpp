#pragma once

#include <irrlicht.h>
#include <stdexcept>

#include "common.hpp"

using namespace irr;

class CubeUI {
 public:
	//default constructor and destructor
	CubeUI();
	DISABLE_COPY(CubeUI);
	~CubeUI();
	// Render cicle
	inline bool deviceRun() { return this->device->run(); }
	void render();
	// Puntero a los vertices del cubo como modificables
	float* getVertices();
	void setVertices(float newVertices[CUBE_VERTICES_ARRAY]);

 private:
	 void refreshBuffer();

 private:
	 // componentes de la libreria
	 IrrlichtDevice* device = nullptr;
	 video::IVideoDriver* driver = nullptr;
	 scene::ISceneManager* smgr = nullptr;
	 scene::SMeshBuffer* buffer = nullptr;
	 scene::SMesh* mesh = nullptr;
	 scene::ISceneNode* node = nullptr;
	 // Vertices array
	 float vertices[CUBE_VERTICES_ARRAY];
	 // Colores de las caras
		video::SColor colores[6] = {
		video::SColor(255, 255, 0, 0),    // Rojo
		video::SColor(255, 255, 255, 0),  // Amarillo
		video::SColor(255, 0, 255, 0),    // Verde
		video::SColor(255, 0, 0, 255),    // Azul
		video::SColor(255, 255, 255, 255),// Blanco
		video::SColor(255, 255, 165, 0)   // Naranja
	 };
};
