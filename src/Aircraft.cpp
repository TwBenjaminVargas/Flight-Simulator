#include "Aircraft.h"
#include "Primitives.h"

#include <glm/gtc/matrix_transform.hpp>

namespace {
// Unidad del modelo: Lf = 1 
constexpr float kLf = 1.0f;

// Punto de referencia: centro de gravedad, a 0.438*Lf de la nariz, sobre el eje del fuselaje
const glm::vec3 kRef(0.438f * kLf, 0.0f, 0.0f);

constexpr float kPi2 = 1.57079632679f;

// Fuselaje en tres tramos que suman exactamente Lf 
constexpr float kRadio    = 0.065f;
constexpr float kLargoNar = 0.22f;                              // cono de nariz
constexpr float kLargoCol = 0.26f;                              // cono de cola
constexpr float kLargoFus = kLf - kLargoNar - kLargoCol;        // cilindro central = 0.52
}

void Aircraft::init()
{
    // Mallas
    fuselaje_.load(primitives::cylinder(kRadio, kLargoFus, 32));
    cono_nariz_.load(primitives::cone_from_height(kRadio, kLargoNar, 32));
    cono_cola_.load(primitives::cone_from_height(kRadio, kLargoCol, 32));
    cubo_.load(primitives::cube(1.0f, 1.0f, 1.0f));

    locales_.clear();
    meshes_.clear();
    colores_.clear();

    const glm::mat4 I(1.0f);
    const glm::vec3 ejeZ(0.0f, 0.0f, 1.0f);

    // función anónima (lambda) que guarda al mismo tiempo la malla, la matriz local y el color de una pieza en los tres vectores correspondientes.
    auto agregar = [&](const Mesh& m, const glm::mat4& local, const glm::vec4& color) {
        meshes_.push_back(&m);
        locales_.push_back(local);
        colores_.push_back(color);
    };

    const glm::vec4 gris   (0.70f, 0.70f, 0.75f, 1.0f);
    const glm::vec4 rojo   (0.80f, 0.30f, 0.30f, 1.0f);
    const glm::vec4 azul   (0.50f, 0.60f, 0.80f, 1.0f);
    const glm::vec4 naranja(0.80f, 0.50f, 0.20f, 1.0f);


    // Nariz: ápice en x = 0 (la punta), base en x = kLargoNar
    agregar(cono_nariz_,
            glm::translate(I, glm::vec3(kLargoNar / 2.0f, 0, 0)) * glm::rotate(I, kPi2, ejeZ), rojo);

    // Fuselaje: entre x = kLargoNar y x = kLargoNar + kLargoFus
    agregar(fuselaje_,
            glm::translate(I, glm::vec3(kLargoNar + kLargoFus / 2.0f, 0, 0)) * glm::rotate(I, -kPi2, ejeZ), gris);

    // Cono de cola: base en x = kLf - kLargoCol, ápice en x = kLf
    agregar(cono_cola_,
            glm::translate(I, glm::vec3(kLf - kLargoCol / 2.0f, 0, 0)) * glm::rotate(I, -kPi2, ejeZ), gris);

    // Alas: una malla, dos matrices 
    // Placa: cuerda 0.15 (X), envergadura 0.39 (Y), espesor 0.013 (Z)
    // El borde interno queda en y = 0.065 (el radio del fuselaje)
    const glm::mat4 escalaAla = glm::scale(I, glm::vec3(0.15f, 0.39f, 0.013f));
    agregar(cubo_, glm::translate(I, glm::vec3(0.46f,  0.26f, -0.02f)) * escalaAla, azul);
    agregar(cubo_, glm::translate(I, glm::vec3(0.46f, -0.26f, -0.02f)) * escalaAla, azul);

    // Empenaje horizontal (dos placas) 
    const glm::mat4 escalaEH = glm::scale(I, glm::vec3(0.09f, 0.15f, 0.009f));
    agregar(cubo_, glm::translate(I, glm::vec3(0.85f,  0.085f, 0.0f)) * escalaEH, azul);
    agregar(cubo_, glm::translate(I, glm::vec3(0.85f, -0.085f, 0.0f)) * escalaEH, azul);

    // Empenaje vertical
    const glm::mat4 escalaEV = glm::scale(I, glm::vec3(0.11f, 0.009f, 0.13f));
    agregar(cubo_, glm::translate(I, glm::vec3(0.85f, 0.0f, 0.085f)) * escalaEV, naranja);
}

// angulos (radianes): x = alabeo, y = cabeceo, z = guiñada
// Cabeceo positivo = nariz hacia arriba.
void Aircraft::update(glm::vec3 pos, glm::vec3 ang)
{
    glm::mat4 R(1.0f);
    R = glm::rotate(R, ang.z, glm::vec3(0, 0, 1));
    R = glm::rotate(R, ang.y, glm::vec3(0, 1, 0));
    R = glm::rotate(R, ang.x, glm::vec3(1, 0, 0));

    pose_ = glm::translate(glm::mat4(1.0f), pos) * R * glm::translate(glm::mat4(1.0f), -kRef);
}

void Aircraft::collect(std::vector<RenderItem>& items) const
{
    for (size_t i = 0; i < locales_.size(); ++i) {
        RenderItem it;
        it.mesh  = meshes_[i];
        it.model = pose_ * locales_[i];
        it.color = colores_[i];
        items.push_back(it);
    }
}
