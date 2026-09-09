#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <vector>

#include "Mesh.h"
#include "Primitives.h"

// Shaders mínimos para probar el VAO y los colores pasados por atributo
const char* vertexShaderSource = R"(
    #version 450 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;

    out vec3 vertexColor;

    void main() {
        gl_Position = vec4(aPos, 1.0);
        vertexColor = aColor;
    }
)";

const char* fragmentShaderSource = R"(
    #version 450 core
    in vec3 vertexColor;
    out vec4 FragColor;

    void main() {
        FragColor = vec4(vertexColor, 1.0);
    }
)";

// Función auxiliar para compilar un shader básico de prueba
GLuint createTestProgram() {
    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertexShaderSource, nullptr);
    glCompileShader(vert);

    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragmentShaderSource, nullptr);
    glCompileShader(frag);

    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    glDeleteShader(vert);
    glDeleteShader(frag);

    return program;
}

int main() {
    // 1. Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "Error al inicializar GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Test Mesh & Primitives - Flight Sim", nullptr, nullptr);
    if (!window) {
        std::cerr << "Error al crear la ventana de GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // 2. Cargar punteros de OpenGL mediante GLAD
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        std::cerr << "Error al inicializar GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL cargado exitosamente. Version: " 
              << GLAD_VERSION_MAJOR(version) << "." 
              << GLAD_VERSION_MINOR(version) << std::endl;

    // Habilitar prueba de profundidad
    glEnable(GL_DEPTH_TEST);

    // 3. Crear Shader de prueba
    GLuint shaderProgram = createTestProgram();

    // 4. TEST 1: Captura de excepción con MeshData vacío
    std::cout << "\n--- TEST 1: Excepcion con MeshData vacio ---" << std::endl;
    try {
        MeshData emptyData;
        Mesh emptyMesh;
        emptyMesh.load(emptyData); // Debe lanzar std::invalid_argument
        std::cout << "[FAIL] No se lanzo la excepcion esperada." << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "[OK] Excepcion capturada correctamente: " << e.what() << std::endl;
    }

    // 5. TEST 2: Carga normal de primitives::cube()
    std::cout << "\n--- TEST 2: Carga de primitives::cube() ---" << std::endl;
    Mesh cubeMesh;
    try {
        MeshData cubeData = primitives::cube(0.8f, 0.8f, 0.8f);
        cubeMesh.load(cubeData);
        std::cout << "[OK] Cubo cargado en GPU. VAO: " << cubeMesh.vao() 
                  << " | Elementos/Indices: " << cubeMesh.count() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[FAIL] Error al cargar el cubo: " << e.what() << std::endl;
        return -1;
    }

    // 6. TEST 3: Semántica de movimiento (Move Semantics)
    std::cout << "\n--- TEST 3: Semantica de Movimiento ---" << std::endl;
    Mesh movedMesh = std::move(cubeMesh);
    std::cout << "[OK] Malla movida. Nuevo VAO: " << movedMesh.vao() 
              << " | Antiguo VAO (debe ser 0): " << cubeMesh.vao() << std::endl;

    // 7. Bucle Principal de Renderizado
    std::cout << "\nIniciando bucle de renderizado. Cierra la ventana para terminar..." << std::endl;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Renderizado de la malla movida
        glBindVertexArray(movedMesh.vao());

        // Consultar directamente a OpenGL si el VAO tiene un Element Buffer (EBO) enlazado
        GLint eboBinding = 0;
        glGetVertexArrayiv(movedMesh.vao(), GL_ELEMENT_ARRAY_BUFFER_BINDING, &eboBinding);

        if (eboBinding != 0) {
            // Malla indexada
            glDrawElements(GL_TRIANGLES, movedMesh.count(), GL_UNSIGNED_INT, nullptr);
        } else {
            // Malla no indexada
            glDrawArrays(GL_TRIANGLES, 0, movedMesh.count());
        }

        glfwSwapBuffers(window);
    }

    // Limpieza de OpenGL
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "Programa finalizado correctamente." << std::endl;
    return 0;
}