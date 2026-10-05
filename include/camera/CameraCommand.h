#pragma once

// Pedido del usuario ya interpretado, en coordenadas esféricas. Se acumula en el cuadro.
struct CameraCommand {
    float yaw_delta   = 0.0f;  // [rad] cambio de longitud
    float pitch_delta = 0.0f;  // [rad] cambio de latitud
    float dist_delta  = 0.0f;  // [unidades de escena] (+) se aleja
};
