#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "RenderItem.h"

/**
 * @brief Clase gestora del modelo 3D del avión y sus jerarquías de transformación.
 * 
 * Actúa como administradora de los recursos de geometría (mallas en VRAM) y
 * calcula tanto las transformaciones locales de las subpiezas como la pose global
 * de la aeronave en el mundo.
 */
class Aircraft {
public:
    /**
     * @brief Inicializa las mallas en GPU y construye la jerarquía de piezas.
     * 
     * Carga las geometrías primitivas en memoria de VRAM utilizando llamadas Direct State Access (DSA)
     * a través de la clase Mesh, calcula las matrices de transformación locales fijas y asigna
     * los colores uniformes para cada componente del avión. Debe ejecutarse una sola vez en la fase de setup.
     */
    void init();

    /**
     * @brief Recalcula la pose global del avión en el espacio de mundo (World Space).
     * 
     * Actualiza la matriz de transformación global (`pose_`) aplicando la traslación de posición,
     * la rotación dada por los ángulos de Euler (alabeo, cabeceo y guiñada) y el ajuste respecto
     * al centro de gravedad (punto de referencia). Debe llamarse en cada frame antes del renderizado.
     * 
     * @param pos Posición actual del centro de masa en el mundo (x, y, z).
     * @param angulos Ángulos de orientación en radianes (x = alabeo, y = cabeceo, z = guiñada).
     */
    void update(glm::vec3 pos, glm::vec3 angulos);

    /**
     * @brief Empaqueta y emite la lista de elementos listos para ser renderizados.
     * 
     * Combina la pose global del avión con las transformaciones locales de cada pieza
     * (\f$M_{model} = M_{pose} \cdot M_{local}\f$) y llena el contenedor de `RenderItem`
     * con referencias ligeras a las mallas y sus respectivos colores uniformes.
     * 
     * @param items Referencia al vector donde se insertarán los elementos a dibujar en el frame actual
     */
    void collect(std::vector<RenderItem>& items) const;

private:
    // =========================================================================
    // Recursos de Geometría en VRAM 
    // =========================================================================
    
    /**
     * Instancias propietarias de los buffers de OpenGL (VAO/VBO/EBO).
     * Al destruirse la clase `Aircraft`, el destructor de `Mesh` liberará automáticamente la memoria GPU.
     * La malla `cubo_` se instancia una sola vez y se reutiliza mediante referencias para múltiples piezas (alas y empenajes).
     */
    Mesh fuselaje_;   ///< Malla cilíndrica para la sección central del cuerpo.
    Mesh cono_nariz_; ///< Malla cónica para el frente del avión.
    Mesh cono_cola_;  ///< Malla cónica para la sección trasera.
    Mesh cubo_;       ///< Malla de cubo unitario reutilizada paramétricamente mediante escalas.

    // =========================================================================
    // Jerarquía y Estructura de Subpiezas
    // =========================================================================
    
    std::vector<glm::mat4>   locales_;  ///< Matrices de transformación fija relativas al origen del avión.
    std::vector<const Mesh*> meshes_;   ///< Punteros no propietarios a las mallas en VRAM para evitar duplicaciones.
    std::vector<glm::vec4>   colores_;  ///< Colores base (RGBA) asociados a cada subpieza.

    /**
     * @brief Matriz de traslación y orientación global del avión en el mundo (\f$M_{pose}\f$).
     */
    glm::mat4 pose_ {1.0f};
};