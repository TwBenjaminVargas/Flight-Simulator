#include <iostream>
#include <fstream>
#include "ResourceManager.hpp"

// Crea archivos dummy para probar la lectura en disco
void create_dummy_shaders() {
    std::filesystem::create_directories("./assets/shaders");

    std::ofstream vs("./assets/shaders/basic.vert");
    vs << "#version 450 core\nlayout (location = 0) in vec3 aPos;\nvoid main() { gl_Position = vec4(aPos, 1.0); }";
    vs.close();

    std::ofstream fs("./assets/shaders/basic.frag");
    fs << "#version 450 core\nout vec4 FragColor;\nvoid main() { FragColor = vec4(1.0, 0.5, 0.2, 1.0); }";
    fs.close();
}

int main() {
    create_dummy_shaders();

    try {
        ResourceManager manager("./assets/shaders");

        // Declaramos variables con cualquier valor basura inicial (serán ignorados por completo)
        int shader_a = 999; 
        int shader_b = -42;

        std::cout << "--- 1. Carga con Pistas Ignoradas e Incremento Autoincremental ---\n";
        std::cout << "Valor previo de 'shader_a': " << shader_a << "\n";
        
        manager.load_shader_source(shader_a, "basic.vert", "basic.frag");
        std::cout << "Nuevo valor de 'shader_a' pisado por la clase: " << shader_a << "\n\n";

        std::cout << "Valor previo de 'shader_b': " << shader_b << "\n";
        manager.load_shader_source(shader_b, "basic.vert", "basic.frag");
        std::cout << "Nuevo valor de 'shader_b' pisado por la clase: " << shader_b << "\n\n";

        std::cout << "--- 2. Lectura desde Mapa usando las Claves Asignadas ---\n";
        const ShaderSource& source_a = manager.get_shader_source(shader_a);
        std::cout << "Vertex Shader recuperado (ID " << shader_a << "):\n" << source_a.vs << "\n\n";

        std::cout << "--- 3. Probar Limpieza de Memoria y Reinicio de Contador ---\n";
        manager.clear();
        
        int shader_c = 123;
        manager.load_shader_source(shader_c, "basic.vert", "basic.frag");
        std::cout << "Tras el clear(), el nuevo ID asignado a 'shader_c' es: " << shader_c << "\n\n";

        std::cout << "--- 4. Prueba Excepcion Acceso Invalido ---\n";
        [[maybe_unused]] const auto& error = manager.get_shader_source(99);

    } catch (const std::out_of_range& e) {
        std::cerr << "Excepcion atrapada (Out of Range): " << e.what() << "\n\n";
    } catch (const std::exception& e) {
        std::cerr << "Excepcion atrapada: " << e.what() << "\n\n";
    }

    return 0;
}