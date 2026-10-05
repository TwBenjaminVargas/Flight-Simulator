#include "camera/CameraSystem.h"

#include <algorithm>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

namespace {
constexpr float kPitchMax = 0.9f * 1.57079632679f;  // evita up colineal con la vista
constexpr float kDistMin  = 0.5f;                   // mayor que 0 y que near
constexpr float kDistMax  = 20.0f;
}

CameraSystem::CameraSystem(int width, int height)
{
    set_viewport(width, height);
}

// Parte 2: proyección 
void CameraSystem::set_viewport(int width, int height)
{
    if (width <= 0 || height <= 0) return;   // ventana minimizada
    const float aspect = static_cast<float>(width) / static_cast<float>(height);
    data_.projection = glm::perspective(fovy_, aspect, near_, far_);
}

// Parte 3: órbita y vista 
void CameraSystem::update(glm::vec3 pos_avion,
                          glm::vec3 /*angulos_avion*/,
                          const CameraCommand& cmd)
{
    yaw_       += cmd.yaw_delta;
    pitch_      = std::clamp(pitch_ + cmd.pitch_delta, -kPitchMax, kPitchMax);
    distancia_  = std::clamp(distancia_ + cmd.dist_delta, kDistMin, kDistMax);

    const glm::vec3 offset(std::cos(pitch_) * std::cos(yaw_),
                           std::cos(pitch_) * std::sin(yaw_),
                           std::sin(pitch_));
    const glm::vec3 eye = pos_avion + distancia_ * offset;

    data_.view = glm::lookAt(eye, pos_avion, glm::vec3(0.0f, 0.0f, 1.0f));
}
