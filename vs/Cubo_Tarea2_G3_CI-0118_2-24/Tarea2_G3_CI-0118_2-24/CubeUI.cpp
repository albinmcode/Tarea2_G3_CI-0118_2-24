#include "CubeUI.hpp"

CubeUI::CubeUI()
    : device(createDevice(video::EDT_OPENGL, core::dimension2d<u32>(800, 600), 16, false, false, false, 0))
{
    using namespace core;
    using namespace scene;
    using namespace video;
    using namespace io;
    using namespace gui;

    if (!device) throw;
    device->setWindowCaption(L"Cubo patrio");
    this->driver = device->getVideoDriver();
    this->smgr = device->getSceneManager();

    // Coordenadas por defecto
    // Coordenadas de los vértices (24 vértices, 3 valores por vértice: x, y, z)
    float defaultVertices[] = {
        // Cara trasera
        -4.0f, -4.0f, -4.0f,   4.0f, -4.0f, -4.0f,   4.0f,  4.0f, -4.0f,  -4.0f,  4.0f, -4.0f,
        // Cara delantera
        -4.0f, -4.0f,  4.0f,   4.0f, -4.0f,  4.0f,   4.0f,  4.0f,  4.0f,  -4.0f,  4.0f,  4.0f,
        // Cara izquierda
        -4.0f, -4.0f, -4.0f,  -4.0f, -4.0f,  4.0f,  -4.0f,  4.0f,  4.0f,  -4.0f,  4.0f, -4.0f,
        // Cara derecha
         4.0f, -4.0f, -4.0f,   4.0f, -4.0f,  4.0f,   4.0f,  4.0f,  4.0f,   4.0f,  4.0f, -4.0f,
         // Cara superior
         -4.0f,  4.0f, -4.0f,   4.0f,  4.0f, -4.0f,   4.0f,  4.0f,  4.0f,  -4.0f,  4.0f,  4.0f,
         // Cara inferior
         -4.0f, -4.0f, -4.0f,   4.0f, -4.0f, -4.0f,   4.0f, -4.0f,  4.0f,  -4.0f, -4.0f,  4.0f
    };
    this->setVertices(defaultVertices);

    // Crear el buffer de malla
    this->buffer = new SMeshBuffer();
    this->buffer->Vertices.set_used(24);
    this->refreshBuffer();

    // Definir los índices de los triángulos
    this->buffer->Indices.set_used(36);

    u16 indices[] = {
        0, 1, 2,  0, 2, 3,  // Cara trasera
        4, 5, 6,  4, 6, 7,  // Cara delantera
        8, 9, 10, 8, 10, 11, // Cara izquierda
        12, 13, 14, 12, 14, 15, // Cara derecha
        16, 17, 18, 16, 18, 19, // Cara superior
        20, 21, 22, 20, 22, 23  // Cara inferior
    };

    for (u32 i = 0; i < 36; ++i)
        this->buffer->Indices[i] = indices[i];

    // Configurar el buffer
    this->buffer->recalculateBoundingBox();

    // Crear la malla
    this->mesh = new SMesh();
    this->mesh->addMeshBuffer(buffer);
    this->buffer->drop();

    // Crear el nodo de la malla
    this->node = smgr->addMeshSceneNode(mesh);
    this->mesh->drop();

    if (this->node) {
        this->node->setMaterialFlag(EMF_LIGHTING, false); // Desactivar iluminación
        this->node->setMaterialFlag(EMF_BACK_FACE_CULLING, false); // Mostrar ambas caras

        // Rotar el cubo 45 grados en los ejes X, Y y Z
        this->node->setRotation(vector3df(0, 45, 0));
    }

    // Agregar una cámara
    this->smgr->addCameraSceneNode(0, vector3df(0, 30, -50), vector3df(0, 0, 0));
} // end constructor

CubeUI::~CubeUI() {
    // Liberar el nodo de escena (si fue creado)
    if (node) {
        node->remove();
        node = nullptr;
    }
    // Liberar la malla (si fue creada)
    if (mesh) {
        mesh->drop();
        mesh = nullptr;
    }
    // Liberar el buffer (si lo administraste directamente)
    if (buffer) {
        buffer->drop();
        buffer = nullptr;
    }
    // Cerrar y liberar el dispositivo de Irrlicht
    if (device) {
        device->drop();
        device = nullptr;
    }
}

float* CubeUI::getVertices() {
    return this->vertices;
}

void CubeUI::setVertices(float newVertices[CUBE_VERTICES_ARRAY]) {
    if (!newVertices) {
        throw std::runtime_error("Null pointer exception");
    }
    for (size_t index = 0; index < CUBE_VERTICES_ARRAY; ++index) {
        this->vertices[index] = newVertices[index];
    }
    
}

void CubeUI::refreshBuffer() {
    // Asignar vértices y colores desde el arreglo
    for (u32 i = 0; i < CUBE_VERTICES; ++i) {
        this->buffer->Vertices[i] = video::S3DVertex(
            this->vertices[i * 3 + 0], // x
            this->vertices[i * 3 + 1], // y
            this->vertices[i * 3 + 2], // z
            0, 0, 1,              // Normal (por ahora fija)
            this->colores[i / 4], // Color según la cara
            0, 0                  // Coordenadas de textura (opcional)
        );
    }
}

void CubeUI::render() {
    // actualizar coordenadas
    this->refreshBuffer();
    // Render
    driver->beginScene(true, true, video::SColor(255, 100, 101, 140));
    smgr->drawAll();
    driver->endScene();
}