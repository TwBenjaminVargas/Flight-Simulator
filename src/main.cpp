#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Primitives.h"
#include "Mesh.h"
#include "Shader.h"
#include "ResourceManager.hpp"
#include "Aircraft.h"
#include "RenderItem.h"

static const char* kWindowTitle     = "Practico 03 - Primitivas y Matriz de Modelo";
static constexpr int kWindowWidth   = 800;
static constexpr int kWindowHeight  = 600;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow *window);

int main()
{
    if (!glfwInit()) return EXIT_FAILURE;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(kWindowWidth, kWindowHeight, kWindowTitle, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGL(glfwGetProcAddress)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);

    glEnable(GL_DEPTH_TEST);

    // =========================================================================
    // CARGA DE RECURSOS (Mallas cargadas UNA SOLA VEZ en la VRAM)
    // =========================================================================
    Aircraft avion;
    Shader shader;

    try {
        avion.init();

        ResourceManager resourceManager("./assets/shaders"); 
        int shaderKey = -1;
        const ShaderSource& source = resourceManager.load_shader_source(
            shaderKey, "vertex.vert", "fragment.frag"
        );

        shader.compile_from_source(source.vs, source.fs);

    } catch (const std::exception& e) {
        std::cerr << "[ERROR RECURSOS]: " << e.what() << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // =========================================================================
    // MATRIZ DE AJUSTE (Dada en clase)
    // =========================================================================
    const float ancho = static_cast<float>(kWindowWidth);
    const float alto  = static_cast<float>(kWindowHeight);
    
    // Ajusta la relación de aspecto y niega el eje Z
    const glm::mat4 ajuste = glm::scale(glm::mat4(1.0f), glm::vec3(alto / ancho, 1.0f, -1.0f));

    // =========================================================================
    // BUCLE DE RENDERIZADO
    // =========================================================================
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.15f, 0.15f, 0.18f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        // Se envía la matriz de ajuste fija
        shader.set_uniform("uAjuste", ajuste);

        float tiempo = static_cast<float>(glfwGetTime());

        // Cámara provisoria: lo gira un poco para ver en 3D y lo achica para que entre.
        const glm::mat4 vista =
            glm::scale(glm::mat4(1.0f), glm::vec3(2.5f, 2.5f, 1.0f)) *
            glm::rotate(glm::mat4(1.0f), 0.5f, glm::vec3(0, 1, 0)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));

        // Solo cabeceo (eje Y del modelo, paralelo al ala), alrededor del centro de gravedad
        avion.update(glm::vec3(0.0f), glm::vec3(std::sin(tiempo) * 0.6f, std::sin(tiempo) * 0.6f, std::sin(tiempo) * 0.6f));

        std::vector<RenderItem> items;
        avion.collect(items);
        for (const auto& item : items) {
            shader.set_uniform("uModel", vista * item.model);
            shader.set_uniform("uColor", item.color);
            glBindVertexArray(item.mesh->vao());
            glDrawElements(GL_TRIANGLES, item.mesh->count(), GL_UNSIGNED_INT, nullptr);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}