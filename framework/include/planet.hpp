#ifndef PLANET_HPP
#define PLANET_HPP

#include <glbinding/gl/types.h>

#include <vector>

struct Planet
{
    glm::fvec3 size;
    float rotat_sp;
    glm::fvec3 distance;
    glm::fvec3 ambient;
    glm::fvec3 diffuse;
    glm::fvec3 specular;
    float gloss;

};
// negative distance only used for asthetics
Planet sun = { {0.25f, 0.25f, 0.25f}, 1.0f, {0.0f, 0.0f, 0.0f},
{1.0f, 0.2f, 0.0f}, {0.95f, 0.2f, 0.0f}, {0.4f, 0.4f, 0.4f}, 1.0f };

Planet mercury = { {0.04f, 0.04f, 0.04f}, 1.6f, {0.4f, 0.0f, 0.0f},
{0.45f, 0.4f, 0.4f}, {0.15f, 0.1f, 0.1f}, {0.2f, 0.2f, 0.2f}, 0.2f };

Planet venus = { {0.095f, 0.095f, 0.095f}, 1.17f, {-0.6f, 0.0f, 0.0f},
{1.0f, 1.0f, 0.6f}, {0.4f, 0.4f, 0.1f}, {0.4f, 0.4f, 0.4f}, 0.5f };

Planet earth = { {0.1f, 0.1f, 0.1f}, 1.0f, {0.9f, 0.0f, 0.0f},
{0.0f, 0.0f, 0.75f}, {0.0f, 0.0f, 0.65f}, {0.4f, 0.4f, 0.4f}, 0.65f };

// not really a planet:
Planet moon = { {0.02f, 0.02f, 0.02f}, 0.5f, {0.16f, 0.0f, 0.0f},
{0.3f, 0.3f, 0.3f}, {0.15f, 0.15f, 0.15f}, {0.2f, 0.2f, 0.2f}, 0.2f };

Planet mars = { {0.05f, 0.05f, 0.05f}, 0.8f, {1.2f, 0.0f, 0.0f},
{0.6f, 0.4f, 0.0f}, {0.5f, 0.3f, 0.0f}, {0.4f, 0.4f, 0.4f}, 0.5f };

Planet jupiter = { {0.2f, 0.2f, 0.2f}, 0.43f, {-1.52f, 0.0f, 0.0f},
{0.9f, 0.6f, 0.3f}, {0.9f, 0.6f, 0.3f}, {0.4f, 0.4f, 0.4f}, 0.75f };

Planet saturn = { {0.19f, 0.19f, 0.19f}, 0.32f, {1.96f, 0.0f, 0.0f},
{0.9f, 0.7f, 0.5f}, {0.9f, 0.7f, 0.5f}, {0.4f, 0.4f, 0.4f}, 0.75f };

Planet uranus = { {0.145f, 0.145f, 0.145f}, 0.23f, {-2.35f, 0.0f, 0.0f},
{0.2f, 0.2f, 1.0f}, {0.2f, 0.2f, 1.0f}, {0.4f, 0.4f, 0.4f}, 0.75f };

Planet neptune = { {0.14f, 0.14f, 0.14f}, 0.18f, {2.69f, 0.0f, 0.0f},
{0.2f, 0.2f, 1.0f}, {0.2f, 0.2f, 1.0f}, {0.4f, 0.4f, 0.4f}, 0.75f };

// not really a planet:
Planet pluto = { {0.02f, 0.02f, 0.02f}, 0.16f, {-2.9f, 0.0f, 0.0f},
{1.0f, 0.8f, 0.4f}, {0.5f, 0.4f, 0.2f}, {0.2f, 0.2f, 0.2f}, 0.2f };

#endif
