#pragma once

#include <string>
#include <stdexcept>

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
     * @throws std::runtime_error si la compilación o el linkeo fallan, incluyendo el log de OpenGL.
     */
    void compile_from_source(const std::string& vs, const std::string& fs);

    /**
     * @brief Activa este programa de shader para el draw call siguiente.
     * @throws std::runtime_error si el shader no ha sido compilado previamente 
     *         o si fue movido (id_ es 0).
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