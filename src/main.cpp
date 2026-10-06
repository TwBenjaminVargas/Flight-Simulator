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
#include "camera/CameraSystem.h"
#include "camera/CameraInput.h"

static const char* kWindowTitle     = "Practico 03 - Primitivas y Matriz de Modelo";
static constexpr int kWindowWidth   = 800;
static constexpr int kWindowHeight  = 600;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow *window);

int main()
{
    if (!glfwInit()) return EXIT_FAILURE;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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

    // Tamaño real del framebuffer (puede diferir del de la ventana en pantallas HiDPI)
    int fbw = 0, fbh = 0;
    glfwGetFramebufferSize(window, &fbw, &fbh);

    CameraSystem camara(fbw, fbh);

    // El callback llega al objeto por el puntero de la ventana
    glfwSetWindowUserPointer(window, &camara);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSwapInterval(1);

    glEnable(GL_DEPTH_TEST);

    // =========================================================================
    // CARGA DE RECURSOS (Mallas cargadas UNA SOLA VEZ en la VRAM)
    // =========================================================================
    //Mesh cubeMesh, cylinderMesh, coneMesh;
    Aircraft avion;
    CameraInput entrada;
    Shader shader;

    try {
        //cubeMesh.load(primitives::cube(1.0f, 1.0f, 1.0f));
        //cylinderMesh.load(primitives::cylinder(0.6f, 1.5f, 32));
        //coneMesh.load(primitives::cone(0.7f, 45.0f, 32));
        avion.init();

        ResourceManager resourceManager("./assets/shaders"); 
        int shaderKey = -1;
        const ShaderSource& source = resourceManager.load_shader_source(
            shaderKey, "normal_color.vert", "normal_color.frag"
        );

        shader.compile_from_source(source.vs, source.fs);

    } catch (const std::exception& e) {
        std::cerr << "[ERROR RECURSOS]: " << e.what() << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    /*
    // =========================================================================
    // MATRIZ DE AJUSTE (Dada en clase)
    // =========================================================================
    const float ancho = static_cast<float>(kWindowWidth);
    const float alto  = static_cast<float>(kWindowHeight);
    
    // Ajusta la relación de aspecto y niega el eje Z
    const glm::mat4 ajuste = glm::scale(glm::mat4(1.0f), glm::vec3(alto / ancho, 1.0f, -1.0f));
    */

    // =========================================================================
    // BUCLE DE RENDERIZADO
    // =========================================================================
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.15f, 0.15f, 0.18f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        // Se envía la matriz de ajuste fija
        //shader.set_uniform("uAjuste", ajuste);

        float tiempo = static_cast<float>(glfwGetTime());

        /*
        // ---------------------------------------------------------------------
        // CUBO (Izquierda, tamaño reducido)
        // ---------------------------------------------------------------------
        glm::mat4 uModelCubo = glm::translate(glm::mat4(1.0f), glm::vec3(-0.6f, 0.0f, 0.0f));
        uModelCubo = glm::rotate(uModelCubo, tiempo, glm::vec3(0.5f, 1.0f, 0.0f));
        uModelCubo = glm::scale(uModelCubo, glm::vec3(0.4f)); // Cambio de tamaño vía uniform
        
        shader.set_uniform("uModel", uModelCubo);
        glBindVertexArray(cubeMesh.vao());
        glDrawElements(GL_TRIANGLES, cubeMesh.count(), GL_UNSIGNED_INT, nullptr);

        // ---------------------------------------------------------------------
        // CILINDRO (Centro)
        // ---------------------------------------------------------------------
        glm::mat4 uModelCilindro = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        uModelCilindro = glm::rotate(uModelCilindro, tiempo * 0.7f, glm::vec3(1.0f, 0.0f, 0.0f));
        uModelCilindro = glm::scale(uModelCilindro, glm::vec3(0.35f)); // Cambio de tamaño
        
        shader.set_uniform("uModel", uModelCilindro);
        glBindVertexArray(cylinderMesh.vao());
        glDrawElements(GL_TRIANGLES, cylinderMesh.count(), GL_UNSIGNED_INT, nullptr);

        // ---------------------------------------------------------------------
        // CONO (Derecha)
        // ---------------------------------------------------------------------
        glm::mat4 uModelCono = glm::translate(glm::mat4(1.0f), glm::vec3(0.6f, 0.0f, 0.0f));
        uModelCono = glm::rotate(uModelCono, tiempo * 1.2f, glm::vec3(0.0f, 1.0f, 0.5f));
        uModelCono = glm::scale(uModelCono, glm::vec3(0.4f)); // Cambio de tamaño
        
        shader.set_uniform("uModel", uModelCono);
        glBindVertexArray(coneMesh.vao());
        glDrawElements(GL_TRIANGLES, coneMesh.count(), GL_UNSIGNED_INT, nullptr);
        */

        const glm::vec3 posAvion(0.0f);
        const glm::vec3 angAvion(0.0f, std::sin(tiempo) * 0.0f, 0.0f);  // cabeceo

        avion.update(posAvion, angAvion);
        const CameraCommand cmd = entrada.poll(window);
        camara.update(posAvion, angAvion, cmd);

        shader.set_uniform("uView",       camara.data().view);
        shader.set_uniform("uProjection", camara.data().projection);

        std::vector<RenderItem> items;
        avion.collect(items);
        for (const auto& item : items) {
            shader.set_uniform("uModel", item.model);
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);   

    auto* camara = static_cast<CameraSystem*>(glfwGetWindowUserPointer(window));
    if (camara) camara->set_viewport(width, height); 
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}