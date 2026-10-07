#pragma once

/**
 * @brief Estructura de transferencia de datos con los desplazamientos del usuario para la cámara.
 * 
 * Contiene las variaciones relativas (deltas) generadas en el cuadro actual.
 * Permite desacoplar los eventos de entrada (mouse, teclado, scroll) de la lógica interna 
 * de la cámara orbital en `CameraSystem`[cite: 2].
 */
struct CameraCommand {
    /**
     * @brief Desplazamiento angular horizontal (azimut / cambio de longitud) en radianes[cite: 2].
     * 
     * Un valor positivo gira la cámara alrededor del eje vertical[cite: 2].
     */
    float yaw_delta = 0.0f;  

    /**
     * @brief Desplazamiento angular vertical (elevación / cambio de latitud) en radianes[cite: 2].
     * 
     * Un valor positivo eleva la posición de la cámara sobre el objetivo[cite: 2].
     */
    float pitch_delta = 0.0f; 

    /**
     * @brief Variación de la distancia hacia el objetivo en unidades de la escena[cite: 2].
     * 
     * Un valor positivo (+) aleja la cámara del objeto y un valor negativo (-) la acerca[cite: 2].
     */
    float dist_delta = 0.0f;  
};