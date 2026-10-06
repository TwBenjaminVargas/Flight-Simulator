#pragma once
#include "CameraCommand.h"

struct GLFWwindow;

// Lee el mouse por polling y lo traduce a un CameraCommand
//   Botón izquierdo apretado + mover : rota
//   Botón derecho apretado + mover Y : cambia la distancia
class CameraInput {
public:
    CameraCommand poll(GLFWwindow* window);

private:
    double prev_x_ = 0.0, prev_y_ = 0.0;
    bool   tiene_previa_ = false;  
};
