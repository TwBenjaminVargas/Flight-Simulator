#include "camera/CameraInput.h"
#include <GLFW/glfw3.h>

namespace {
constexpr float kGananciaAng  = 0.005f;
constexpr float kGananciaScroll = 0.5f; // Sensibilidad del zoom con ruedita
}

// Implementación del callback estático
void CameraInput::scroll_callback([[maybe_unused]] GLFWwindow* window, 
                                   [[maybe_unused]] double xoffset, 
                                   double yoffset)
{
    // yoffset es positivo hacia arriba (acercar), negativo hacia abajo (alejar)
    scroll_offset_ += static_cast<float>(yoffset);
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
        if (scroll_offset_ != 0.0f) {
            // dist_delta (+) aleja la cámara, por lo que invertimos yoffset:
            cmd.dist_delta = -scroll_offset_ * kGananciaScroll;
            // Consumimos el acumulado para que no siga aplicando el zoom en frames futuros
            scroll_offset_ = 0.0f; 
        }
    }

    prev_x_ = x;
    prev_y_ = y;
    tiene_previa_ = true;

    return cmd;
}