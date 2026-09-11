#include "Shader.h"

#include <glad/gl.h>
#include <iostream>


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
    glUseProgram(id_);
}


static bool compile_stage(unsigned int shader, const std::string& source, const std::string& stage_name)
{
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        int log_len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
        std::string log(log_len, '\0');
        glGetShaderInfoLog(shader, log_len, nullptr, log.data());
        std::cerr << "ERROR::SHADER::" << stage_name << "::COMPILATION\n" << log << std::endl;
        return false;
    }
    return true;
}

bool Shader::compile_from_source(const std::string& vs, const std::string& fs)
{
    clear(); // si había un programa anterior, lo libera

    unsigned int vert = glCreateShader(GL_VERTEX_SHADER);
    unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);

    bool ok = compile_stage(vert, vs, "VERTEX") &&
              compile_stage(frag, fs, "FRAGMENT");

    if (ok) {
        id_ = glCreateProgram();
        glAttachShader(id_, vert);
        glAttachShader(id_, frag);
        glLinkProgram(id_);

        int success;
        glGetProgramiv(id_, GL_LINK_STATUS, &success);
        if (!success) {
            int log_len = 0;
            glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &log_len);
            std::string log(log_len, '\0');
            glGetProgramInfoLog(id_, log_len, nullptr, log.data());
            std::cerr << "ERROR::SHADER::LINKING\n" << log << std::endl;
            glDeleteProgram(id_);
            id_ = 0U;
            ok = false;
        }
    }

    // Los objetos intermedios ya no se necesitan
    glDeleteShader(vert);
    glDeleteShader(frag);

    return ok;
}
