#pragma once

#include <string>
#include <stdexcept>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * @brief Administra el ciclo de vida de un programa de shader de OpenGL (RAII).
 * 
 * Es el único dueño del programa linkeado. Deshabilita operaciones de copia
 * pero permite la transferencia de propiedad por movimiento (Move-Only).
 * Mantiene una caché interna (lazy) de las ubicaciones (locations) de uniforms.
 */
class Shader {
public:
    Shader() = default;
    ~Shader();

    // Deshabilitar copia
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    // Permitir movimiento
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    /**
     * @brief Compila y linkea los shaders a partir de su código fuente.
     * @param vs Código fuente del vertex shader.
     * @param fs Código fuente del fragment shader.
     * @throws std::runtime_error Si la compilación o el linkeo fallan, incluyendo el log de OpenGL.
     */
    void compile_from_source(const std::string& vs, const std::string& fs);

    /**
     * @brief Activa este programa de shader para el draw call siguiente (glUseProgram).
     * @throws std::runtime_error Si el shader no ha sido compilado previamente o fue movido.
     */
    void use() const;

    /**
     * @brief Libera el programa de OpenGL y limpia la caché de uniforms.
     */
    void clear();

    /**
     * @brief Envía una matriz 4x4 al uniform especificado.
     * @param nombre Nombre exacto de la variable uniform en GLSL.
     * @param m Matriz glm::mat4 a transferir.
     * @throws std::runtime_error Si el uniform no existe, o si hay un desacople de tipo
     *         entre C++ y GLSL (GL_INVALID_OPERATION).
     */
    void set_uniform(const std::string& nombre, const glm::mat4& m) const;

    /**
     * @brief Envía un vector de 3 componentes al uniform especificado.
     * @param nombre Nombre exacto de la variable uniform en GLSL.
     * @param v Vector glm::vec3 a transferir.
     * @throws std::runtime_error Si el uniform no existe, o si hay un desacople de tipo
     *         entre C++ y GLSL (GL_INVALID_OPERATION).
     */
    void set_uniform(const std::string& nombre, const glm::vec3& v) const;

    /**
     * @brief Envía un valor de punto flotante al uniform especificado.
     * @param nombre Nombre exacto de la variable uniform en GLSL.
     * @param valor Valor float a transferir.
     * @throws std::runtime_error Si el uniform no existe, o si hay un desacople de tipo
     *         entre C++ y GLSL (GL_INVALID_OPERATION).
     */
    void set_uniform(const std::string& nombre, float valor) const;

    unsigned int id() const { return id_; }

private:
    /**
     * @brief Consulta la ubicación (location) de un uniform de forma optimizada.
     * Usa un mapa interno mutable para consultar a OpenGL solo la primera vez.
     * @param nombre Nombre de la variable uniform.
     * @return Identificador entero de la ubicación en el shader.
     * @throws std::runtime_error Si el shader es inválido o el uniform no existe.
     */
    int get_uniform_location(const std::string& nombre) const;

    unsigned int id_{0U};
    mutable std::unordered_map<std::string, int> location_cache_;
};