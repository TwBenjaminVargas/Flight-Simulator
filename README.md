# Flight-Simulator
Proyecto final materia Computacion Grafica y Ambientes Virtuales
---
## 📁 Estructura del Proyecto

```text
simulador_vuelo/
├── assets/                 # Recursos estáticos del proyecto (modelos, texturas, audios, etc.)
│   └── shaders/            # Archivos de código fuente de los Shaders (.vert, .frag, .geom)
├── include/                # Archivos de cabeceras públicas de C++ (.hpp / .h)
│   └── ResourceManager.hpp # Interfaz y declaración de la clase gestora de recursos
├── src/                    # Implementación del código fuente (.cpp)
│   └── ResourceManager.cpp # Lógica de negocio e implementación de métodos de ResourceManager
├── tests/                  # Pruebas unitarias y programas de prueba
│   └── test_resource_manager.cpp # Código de prueba para validar el funcionamiento de ResourceManager
├── build/                  # Archivos binarios ejecutables e intermediarios de compilación (ignorado en Git)
    └── rmtest              # Ejecutable compilado del test de pruebas
```

---

## 🛠️ Compilación

### 1. Compilación de Tests

Para compilar el ejecutable de pruebas (`test_resource_manager.cpp`) utilizando el estándar **C++17**, las banderas de advertencia recomendadas y la ruta de cabeceras `include/`, ejecuta el siguiente comando desde la raíz del proyecto:

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/ResourceManager.cpp tests/test_resource_manager.cpp -o build/rmtest

```

#### Ejecución del Test (Ejemplo):

```bash
./build/rmtest

```

---

### 2. Compilación del Programa Principal (`main.cpp`)

Si en el futuro agregas un archivo principal en la carpeta de código fuente (`src/main.cpp`), puedes compilar el ejecutable de la aplicación con el siguiente comando:

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/ResourceManager.cpp src/main.cpp -o build/simulador

```

#### Ejecución de la Aplicación:

```bash
./build/simulador

```

---

### 💡 Explicación de Banderas del Compilador

| Bandera | Descripción |
| --- | --- |
| `-std=c++17` | Utiliza el estándar de C++17 para la compilación. |
| `-Wall -Wextra` | Activa la mayoría de las advertencias recomendadas del compilador. |
| `-Iinclude` | Agrega el directorio `include/` a las rutas de búsqueda para las cabeceras (`#include "..."`). |
| `-o build/...` | Define la ubicación y el nombre del archivo ejecutable generado. |

```