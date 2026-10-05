#pragma once
#include <glm/glm.hpp>
#include "CameraData.h"
#include "CameraCommand.h"

// Administra el estado de la cámara orbital y calcula las matrices de vista y proyección.
// No lee el mouse, no dibuja y no sabe cómo está compuesta la escena.
class CameraSystem {
public:
    CameraSystem(int width, int height);                 // arma la proyección inicial

    void update(glm::vec3 pos_avion,                     // punto al que mira la cámara
                glm::vec3 angulos_avion,                 // no se usa por ahora
                const CameraCommand& cmd);               // cada cuadro

    void set_viewport(int width, int height);            // al redimensionar la ventana
    const CameraData& data() const { return data_; }

private:
    // Estado de la órbita (vista inicial: 3/4 delantero)
    float yaw_       = 2.35f;   // [rad] ~135°
    float pitch_     = 0.30f;   // [rad]
    float distancia_ = 3.0f;    // [unidades de escena]

    // Parámetros de la proyección
    float fovy_ = 0.7854f;      // [rad] 45°
    float near_ = 0.1f;
    float far_  = 100.0f;

    CameraData data_ {};
};