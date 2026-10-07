#pragma once

#include <glm/glm.hpp>
#include "CameraData.h"
#include "CameraCommand.h"

/**
 * @brief Administrador del estado de la cámara orbital y generador de matrices de visualización.
 * 
 * Gestiona la posición de la cámara en coordenadas esféricas alrededor de un objetivo,
 * calcula la matriz de vista (View Matrix) y reconstruye la matriz de proyección (Projection Matrix)
 * al cambiar las dimensiones del viewport.
 * 
 * @note Este módulo está totalmente desacoplado de la API gráfica (no realiza llamadas directas a OpenGL),
 * no captura periféricos directamente ni conoce la geometría interna de la escena.
 */
class CameraSystem {
public:
    /**
     * @brief Construye el sistema de cámara e inicializa el frustum de proyección.
     * 
     * @param width Ancho inicial del framebuffer en píxeles.
     * @param height Alto inicial del framebuffer en píxeles.
     */
    CameraSystem(int width, int height);

    /**
     * @brief Actualiza la pose orbital de la cámara y recalcula la matriz de vista (View Matrix).
     * 
     * Acumula las órdenes del usuario (deltas angulares y de distancia), restringe los parámetros
     * dentro de límites seguros de estabilidad numérica y convierte las coordenadas esféricas
     * a una posición cartesiana (eye) para generar la matriz mediante glm::lookAt.
     * 
     * @param pos_avion Posición central del objetivo a enfocar en el espacio de mundo (World Space).
     * @param angulos_avion Orientación del objetivo (reservado para futuras vistas de cabina o seguimiento).
     * @param cmd Estructura con las deltas angulares y de distancia acumuladas en el cuadro actual.
     */
    void update(glm::vec3 pos_avion,
                glm::vec3 angulos_avion,
                const CameraCommand& cmd);

    /**
     * @brief Recalcula la matriz de proyección perspectiva al redimensionar la ventana.
     * 
     * @param width Nuevo ancho de la ventana en píxeles.
     * @param height Nuevo alto de la ventana en píxeles. Ignora la actualización si las dimensiones son <= 0.
     */
    void set_viewport(int width, int height);

    /**
     * @brief Obtiene el contenedor de matrices calculado para el frame actual.
     * 
     * @return Referencia constante a CameraData con las matrices de vista y proyección listas para los shaders.
     */
    const CameraData& data() const { return data_; }

private:
    // =========================================================================
    // Estado de la Órbita (Coordenadas Esféricas)
    // =========================================================================
    
    float yaw_       = 2.35f;   ///< Ángulo de azimut/longitud en radianes (~135°, vista 3/4 frontal).
    float pitch_     = 0.30f;   ///< Ángulo de elevación/latitud en radianes.
    float distancia_ = 2.0f;    ///< Distancia radial respecto al objetivo (unidades de escena).

    // =========================================================================
    // Parámetros del Frustum de Proyección Perspectiva
    // =========================================================================
    
    float fovy_ = 0.7854f;      ///< Campo de visión vertical (Field of View) en radianes (45°).
    float near_ = 0.1f;         ///< Distancia al plano de recorte cercano (Near Clipping Plane).
    float far_  = 100.0f;       ///< Distancia al plano de recorte lejano (Far Clipping Plane).

    CameraData data_ {};        ///< Caché interna con el par de matrices calculadas.
};