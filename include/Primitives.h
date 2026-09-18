#pragma once

#include "MeshData.h"

namespace primitives {

/**
 * @brief Genera un cubo centrado en el origen (0, 0, 0).
 * 
 * - **Geometría:** Cubo indexado con caras alineadas a los ejes X, Y, Z.
 * - **Atributos:** 24 vértices únicos (4 por cara para permitir normales ortogonales) y 36 índices.
 * - **Culling:** Triángulos en orden antihorario (CCW) mirando las caras desde afuera.
 * 
 * @param scale_x Ancho total del cubo en el eje X.
 * @param scale_y Alto total del cubo en el eje Y.
 * @param scale_z Largo total del cubo en el eje Z.
 */
MeshData cube(float scale_x = 1.0f, float scale_y = 1.0f, float scale_z = 1.0f);

/**
 * @brief Genera la superficie lateral de un cilindro paramétrico (sin tapas).
 * 
 * - **Alineación:** Centrado en (0, 0, 0), extendiéndose sobre el eje Y desde -largo/2 hasta +largo/2.
 * - **Normales:** Radiales puras en el plano XZ: n = (cos θ, 0, sin θ).
 * - **Costura UV:** Genera (gajos + 1) vértices por anillo para cerrar la coordenada U (0 a 1).
 * 
 * @param radio   Radio del cilindro en el plano XZ.
 * @param largo   Altura total del cilindro a lo largo del eje Y.
 * @param gajos   Discretización angular (divisiones radiales alrededor del eje Y).
 * @param anillos Discretización axial (divisiones a lo largo de la altura en Y). Por defecto = 1.
 */
MeshData cylinder(float radio, float largo, unsigned gajos, unsigned anillos = 1U);

/**
 * @brief Genera la superficie lateral de un cono paramétrico (sin tapa).
 * 
 * - **Alineación:** Centrado en (0, 0, 0). Base en Y = -altura/2 y ápice en Y = +altura/2.
 * - **Dimensiones:** La altura total se calcula mediante trigonometría: h = radio / tan(conicidad / 2).
 * - **Normales:** Inclinadas por el semiángulo: n = (cos θ · cos(α/2), sin(α/2), sin θ · cos(α/2)).
 * - **Ápice:** Los vértices del último anillo colapsan en la posición 3D del ápice, pero se mantienen 
 *   duplicados por gajo para preservar la normal inclinada individual.
 * 
 * @param radio     Radio de la base en el plano XZ.
 * @param conicidad Ángulo de apertura total (α) del cono en GRADOS SEXAGESIMALES (0° a 180°).
 * @param gajos     Discretización angular (divisiones radiales alrededor del eje Y).
 * @param anillos   Discretización axial (divisiones desde la base hasta el ápice). Por defecto = 1.
 */
MeshData cone(float radio, float conicidad, unsigned gajos, unsigned anillos = 1U);

/**
 * @brief Genera la superficie lateral de un cono especificando su altura en lugar de la conicidad.
 * 
 * - **Reutilización:** Convierte internamente la altura 'h' al ángulo de conicidad equivalente
 *   y delega la generación a la función cone(radio, conicidad, gajos, anillos).
 * 
 * @param radio   Radio de la base en el plano XZ.
 * @param altura  Altura total del cono a lo largo del eje Y.
 * @param gajos   Discretización angular (divisiones radiales).
 * @param anillos Discretización axial. Por defecto = 1.
 */
MeshData cone_from_height(float radio, float altura, unsigned gajos, unsigned anillos = 1U);

} // namespace primitives