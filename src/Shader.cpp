#include "Shader.h"

#include <glad/gl.h>
#include <iostream>

/**
 * @brief Verifica si la última llamada a glProgramUniform* provocó un choque de tipos.
 * 
 * Captura específicamente GL_INVALID_OPERATION, el cual es generado por el driver
 * cuando se intenta enviar un tipo de dato desde C++ que no coincide con la firma
 * declarada en GLSL (ej. enviar glm::mat4 a un vec3).
 * 
 * @param nombre Nombre del uniform involucrado para reporte de errores.
 * @throws std::runtime_error Si OpenGL reporta GL_INVALID_OPERATION.
 */
static void check_uniform_type_error(const std::string& nombre)
{
    GLenum err = glGetError();
    if (err == GL_INVALID_OPERATION) {
        throw std::runtime_error(
            "ERROR::SHADER::UNIFORM_TYPE_MISMATCH\n"
            "El tipo de dato enviado al uniform '" + nombre + 
            "' no coincide con el tipo declarado en el shader GLSL."
        );
    }
}

Shader::~Shader()
{
    clear();
}

Shader::Shader(Shader&& other) noexcept
    : id_(other.id_), location_cache_(std::move(other.location_cache_))
{
    other.id_ = 0U;
    other.location_cache_.clear();
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other) {
        clear();
        id_ = other.id_;
        location_cache_ = std::move(other.location_cache_);
        
        other.id_ = 0U;
        other.location_cache_.clear();
    }
    return *this;
}

void Shader::clear()
{
    glDeleteProgram(id_); // legal con 0: no hace nada
    id_ = 0U;
    location_cache_.clear(); // Limpia el mapa de locations almacenados
}

void Shader::use() const
{
    if (id_ == 0U) {
        throw std::runtime_error(
            "ERROR::SHADER::USE_CALLED_ON_INVALID_PROGRAM\n"
            "Se intentó llamar a use() en un Shader no inicializado, que falló al compilar o que fue movido."
        );
    }
    glUseProgram(id_);
}

static void compile_stage_or_throw(unsigned int shader, const std::string& source, const std::string& stage_name)
{
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        int log_len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
        
        std::string log;
        if (log_len > 0) {
            log.resize(static_cast<size_t>(log_len));
            glGetShaderInfoLog(shader, log_len, nullptr, log.data());
        }

        throw std::runtime_error("ERROR::SHADER::" + stage_name + "::COMPILATION_FAILED\n" + log);
    }
}

void Shader::compile_from_source(const std::string& vs, const std::string& fs)
{
    clear(); // Libera el programa anterior y limpia el mapa de caché

    unsigned int vert = glCreateShader(GL_VERTEX_SHADER);
    unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);

    try {
        // compilar cada etapa y lanzar excepción si falla
        compile_stage_or_throw(vert, vs, "VERTEX");
        compile_stage_or_throw(frag, fs, "FRAGMENT");

        // linkear el programa
        id_ = glCreateProgram();
        glAttachShader(id_, vert);
        glAttachShader(id_, frag);
        glLinkProgram(id_);

        // verificar el linkeo y lanzar excepción si falla
        int success = 0;
        glGetProgramiv(id_, GL_LINK_STATUS, &success);
        if (!success) {
            int log_len = 0;
            glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &log_len);

            std::string log;
            if (log_len > 0) {
                log.resize(static_cast<size_t>(log_len));
                glGetProgramInfoLog(id_, log_len, nullptr, log.data());
            }
            // limpieza ante error: liberar shaders y programa
            clear();
            throw std::runtime_error("ERROR::SHADER::LINKING_FAILED\n" + log);
        }
        // liberar shaders después de linkear, ya no son necesarios
        glDeleteShader(vert);
        glDeleteShader(frag);

    } catch (...) {
        // liberar los shaders y el programa si hubo un error
        glDeleteShader(vert);
        glDeleteShader(frag);
        throw;
    }
}

int Shader::get_uniform_location(const std::string& nombre) const
{
    if (id_ == 0U) {
        throw std::runtime_error(
            "ERROR::SHADER::UNIFORM_SET_ON_INVALID_PROGRAM\n"
            "Se intentó establecer un uniform en un Shader no inicializado o destruido."
        );
    }

    // Verificación previa: Buscar en la caché interna
    auto it = location_cache_.find(nombre);
    if (it != location_cache_.end()) {
        return it->second;
    }

    // Si no está en caché, consultar a OpenGL
    int location = glGetUniformLocation(id_, nombre.c_str());
    if (location == -1) {
        throw std::runtime_error(
            "ERROR::SHADER::UNIFORM_NOT_FOUND\n"
            "El uniform '" + nombre + "' no existe dentro del shader o fue descartado por optimización del compilador GLSL."
        );
    }

    //Almacenar la localización válida en el mapa para futuras llamadas
    location_cache_[nombre] = location;
    return location;
}

void Shader::set_uniform(const std::string& nombre, const glm::mat4& m) const
{
    int loc = get_uniform_location(nombre);
    glProgramUniformMatrix4fv(id_, loc, 1, GL_FALSE, glm::value_ptr(m));
    check_uniform_type_error(nombre);
}

void Shader::set_uniform(const std::string& nombre, const glm::vec3& v) const
{
    int loc = get_uniform_location(nombre);
    glProgramUniform3fv(id_, loc, 1, glm::value_ptr(v));
    check_uniform_type_error(nombre);
}

void Shader::set_uniform(const std::string& nombre, float valor) const
{
    int loc = get_uniform_location(nombre);
    glProgramUniform1f(id_, loc, valor);
    check_uniform_type_error(nombre);
}