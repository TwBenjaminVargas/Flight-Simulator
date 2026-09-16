#include "Shader.h"

#include <glad/gl.h>
#include <vector>

Shader::~Shader()
{
    clear();
}

Shader::Shader(Shader&& other) noexcept
    : id_(other.id_)
{
    other.id_ = 0U;
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other) {
        clear();
        id_ = other.id_;
        other.id_ = 0U;
    }
    return *this;
}

void Shader::clear()
{
    glDeleteProgram(id_); // legal con 0: no hace nada
    id_ = 0U;
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
    clear(); // Si había un programa anterior, lo libera

    unsigned int vert = glCreateShader(GL_VERTEX_SHADER);
    unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);

    try {
        // Compilamos ambas etapas; si alguna falla, el catch captura y limpia
        compile_stage_or_throw(vert, vs, "VERTEX");
        compile_stage_or_throw(frag, fs, "FRAGMENT");

        // Crear y vincular el programa
        id_ = glCreateProgram();
        glAttachShader(id_, vert);
        glAttachShader(id_, frag);
        glLinkProgram(id_);

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

            clear(); // Libera id_ asignado arriba
            throw std::runtime_error("ERROR::SHADER::LINKING_FAILED\n" + log);
        }

        // Éxito: eliminamos los objetos intermedios
        glDeleteShader(vert);
        glDeleteShader(frag);

    } catch (...) {
        // Garantiza liberar los shaders de etapa antes de propagar la excepción
        glDeleteShader(vert);
        glDeleteShader(frag);
        throw;
    }
}