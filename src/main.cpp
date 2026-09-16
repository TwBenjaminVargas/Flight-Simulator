#include <cstdlib>          // EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>
#include <string>
#include <exception>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

// Tus cabeceras del proyecto
#include "Primitives.h"
#include "Mesh.h"
#include "Shader.h"
#include "ResourceManager.hpp"

static const char* kWindowTitle     = "OpenGL Cube - Sin GLM / Uniforms";
static constexpr int kWindowWidth   = 800;
static constexpr int kWindowHeight  = 600;
static constexpr int kGLVerMajor    = 4;
static constexpr int kGLVerMinor    = 5;

static int glfw_error_code{};
static std::string glfw_error_str{};

static void error_callback(int error, const char *description);
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow *window);
static void print_gl_version(void);

int main()
{
    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit()) {
        std::cout << "GLFW initialization failed! - " << glfw_error_str << std::endl;
        return EXIT_FAILURE;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, kGLVerMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, kGLVerMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(kWindowWidth, kWindowHeight, kWindowTitle, nullptr, nullptr);
    
    if (window == nullptr){
        glfwTerminate();
        std::cout << "GLFW window creation failed! - " << glfw_error_str << std::endl;
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGL(glfwGetProcAddress)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        std::cout << "GLAD initialization failed!" << std::endl;
        return EXIT_FAILURE;
    }

    print_gl_version();
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);

    // =========================================================================
    // CONFIGURACIÓN DE ESTADO DE OPENGL
    // =========================================================================
    // Habilitar Z-Buffer para ordenamiento de profundidad
    glEnable(GL_DEPTH_TEST);

    // =========================================================================
    // CARGA DE RECURSOS (Mesh y Shaders)
    // =========================================================================
    Mesh cubeMesh;
    Shader cubeShader;

    try {
        //Generar los datos del cubo en CPU y cargarlos en la GPU
        MeshData cubeData = primitives::cube(1.0f, 1.0f, 1.0f);
        cubeMesh.load(cubeData);

        // Cargar fuentes del Shader desde disco
        ResourceManager resourceManager("./assets/shaders"); 
        int shaderKey = -1;
        
        const ShaderSource& source = resourceManager.load_shader_source(
            shaderKey, "cube.vert", "cube.frag"
        );

        // Compilar y vincular el Shader
        cubeShader.compile_from_source(source.vs, source.fs);

    } catch (const std::exception& e) {
        std::cerr << "\n[EXCEPCIÓN EN LA CARGA DE RECURSOS]:\n" << e.what() << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // =========================================================================
    // BUCLE DE RENDERIZADO
    // =========================================================================
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // Limpiar el buffer de color y el buffer de profundidad
        glClearColor(51.0f / 255.0f, 55.0f / 255.0f, 76.0f / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activar el Shader
        cubeShader.use();

        // Dibujar el cubo usando su VAO y la cantidad de índices
        glBindVertexArray(cubeMesh.vao());
        glDrawElements(GL_TRIANGLES, cubeMesh.count(), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    
    return EXIT_SUCCESS;
}

void error_callback(int error, const char *description) {
    glfw_error_code = error;
    glfw_error_str = std::string(description);
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void print_gl_version(void) {
    std::cout << " OpenGL Vendor: "   << glGetString(GL_VENDOR)                   << std::endl;
    std::cout << " OpenGL Renderer: " << glGetString(GL_RENDERER)                 << std::endl;
    std::cout << " OpenGL Version: "  << glGetString(GL_VERSION)                  << std::endl;
    std::cout << " GLSL Version: "    << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}