#pragma once

#include <string>

/**
 * @brief Administra el ciclo de vida de un programa de shader de OpenGL.
 * 
 * Es el único dueño del programa linkeado. No se puede copiar,
 * solo mover. El programa se libera automáticamente en el destructor.
 */
class Shader {
public:
    Shader() = default;
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    /**
     * @brief Compila y linkea los shaders a partir de su código fuente.
     * @param vs Código fuente del vertex shader.
     * @param fs Código fuente del fragment shader.
     * @return true si compiló y linkeó correctamente, false en caso contrario.
     * @note Si falla, imprime el log del driver y el objeto queda vacío.
     */
    bool compile_from_source(const std::string& vs, const std::string& fs);

    /**
     * @brief Activa este programa de shader para el draw call siguiente.
     */
    void use() const;

    /**
     * @brief Libera el programa de OpenGL. Se puede llamar más de una vez sin problema.
     */
    void clear();

    unsigned int id() const { return id_; }

private:
    unsigned int id_ {0U};
};
