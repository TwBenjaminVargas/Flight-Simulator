#include "camera/CameraInput.h"
#include <GLFW/glfw3.h>

namespace {
constexpr float kGananciaAng  = 0.005f;
constexpr float kGananciaDist = 0.01f;  
}

CameraCommand CameraInput::poll(GLFWwindow* window)
{
    double x = 0.0, y = 0.0;
    glfwGetCursorPos(window, &x, &y); 

    CameraCommand cmd;

    if (tiene_previa_) {
        const float dx = static_cast<float>(x - prev_x_);
        const float dy = static_cast<float>(y - prev_y_);   // en pantalla, Y crece hacia abajo

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            cmd.yaw_delta   = -dx * kGananciaAng;   // arrastrar a la derecha: la escena gira
            cmd.pitch_delta =  dy * kGananciaAng;   // arrastrar hacia abajo: la cámara sube
        }
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            cmd.dist_delta = dy * kGananciaDist;    // hacia abajo: se aleja
        }
    }

    prev_x_ = x;
    prev_y_ = y;
    tiene_previa_ = true;

    return cmd;
}