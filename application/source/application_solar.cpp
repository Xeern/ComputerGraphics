#include "application_solar.hpp"
#include "launcher.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"

#include "planet.hpp"
#include "structs.hpp"

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding
using namespace gl;

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <time.h>

// vector of planet structs
std::vector<Planet> planets{ sun, mercury, venus, earth,
    moon, mars, jupiter, saturn, uranus, neptune, pluto };

std::vector<float> stars;
std::vector<float> orbits;
model_object star_object;
model_object orbit_object;
// float that works as boolean
float Cel = 0.0;

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
{
  initializeGeometry();
  initializeShaderPrograms();
}

void ApplicationSolar::render() const {

glUseProgram(m_shaders.at("star").handle);
glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ModelMatrix"),
    1, GL_FALSE, glm::value_ptr(glm::fmat4{}));
glBindVertexArray(star_object.vertex_AO);
glDrawArrays(GL_POINTS, 0, stars.size()/6);
  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("planet").handle);
    // vector that holds the models which will be drawn
    std::vector<glm::fmat4> models;

    // for loop over the planets vector
    for (int i = 0; i < planets.size(); ++i)
        {
            glm::fmat4 model_matrix = glm::fmat4{};
            // distance for different intensity of illumination
            glUniform3fv(m_shaders.at("planet").u_locs.at("PlanetPos"),1,
                glm::value_ptr(planets[i].distance));
            // if-cause for the moon, since it rotates around the earth
            if (i == 4)
            {
                // extra rotate and translate for the rotation around the moon while also
                // rotating alongside the earth
                model_matrix = glm::rotate(model_matrix,
                    float(glfwGetTime()) * planets[3].rotat_sp, { 0.0f, 0.0f, 0.1f });
                model_matrix = glm::translate(model_matrix, planets[3].distance);
                glUniform3fv(m_shaders.at("planet").u_locs.at("PlanetPos"),1,
                    glm::value_ptr(planets[3].distance));
            }
            glm::fmat4 orbit_matrix = model_matrix;
            orbit_matrix = glm::scale(orbit_matrix,glm::fvec3(planets[i].distance.x));
            glUseProgram(m_shaders.at("orbit").handle);
            glUniformMatrix4fv(m_shaders.at("orbit").u_locs.at("ModelMatrix"),
                1, GL_FALSE, glm::value_ptr(orbit_matrix));
            glBindVertexArray(orbit_object.vertex_AO);
            glDrawArrays(GL_LINE_LOOP, 0, orbits.size()/3);
            glUseProgram(m_shaders.at("planet").handle);
            model_matrix = glm::rotate(model_matrix,
                float(glfwGetTime()) * planets[i].rotat_sp, {0.0f, 0.0f, 0.1f});
            model_matrix = glm::translate(model_matrix, planets[i].distance);
            model_matrix = glm::scale(model_matrix, planets[i].size);
            models.push_back(model_matrix);

            glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                1, GL_FALSE, glm::value_ptr(models[i]));

            // extra matrix for normal transformation to keep them orthogonal to surface
            glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) *
                model_matrix);
            glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                1, GL_FALSE, glm::value_ptr(normal_matrix));

            // bind the VAO to draw
            glBindVertexArray(planet_object.vertex_AO);
            glUseProgram(m_shaders.at("planet").handle);
            // uniform that shows if cel-shading needs to be applied or not
            glUniform1f(m_shaders.at("planet").u_locs.at("CelBool"),Cel);
            glUniform3fv(m_shaders.at("planet").u_locs.at("SunPosition"),1,
                glm::value_ptr(planets[0].distance));
            glUniform3fv(m_shaders.at("planet").u_locs.at("AmbientVector"),1,
                glm::value_ptr(planets[i].ambient));
            glUniform3fv(m_shaders.at("planet").u_locs.at("DiffuseVector"),1,
                glm::value_ptr(planets[i].diffuse));
            glUniform3fv(m_shaders.at("planet").u_locs.at("SpecularVector"),1,
                glm::value_ptr(planets[i].specular));
            glUniform1f(m_shaders.at("planet").u_locs.at("ShiningFloat"),planets[i].gloss);
            // draw bound vertex array using bound shader
            glDrawElements(planet_object.draw_mode, planet_object.num_elements,
                model::INDEX.type, NULL);
        }
// planet for-loop ends



}

void ApplicationSolar::updateView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
  // upload matrix to gpu
  glUseProgram(m_shaders.at("star").handle);
  glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));

  glUseProgram(m_shaders.at("orbit").handle);
  glUniformMatrix4fv(m_shaders.at("orbit").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));

  glUseProgram(m_shaders.at("planet").handle);
}

void ApplicationSolar::updateProjection() {
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));

  glUseProgram(m_shaders.at("star").handle);
  glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));

  glUseProgram(m_shaders.at("orbit").handle);
  glUniformMatrix4fv(m_shaders.at("orbit").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));

  glUseProgram(m_shaders.at("planet").handle);
}

// update uniform locations
void ApplicationSolar::uploadUniforms() {
  updateUniformLocations();

  // bind new shader
  glUseProgram(m_shaders.at("planet").handle);

  updateView();
  updateProjection();
}

// handle key input
void ApplicationSolar::keyCallback(int key, int scancode, int action, int mods) {
  if ((key == GLFW_KEY_W || key == GLFW_KEY_UP) &&
    (action == GLFW_REPEAT || action == GLFW_PRESS)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.025f, 0.0f});
    updateView();
  }
  else if ((key == GLFW_KEY_A || key == GLFW_KEY_LEFT) &&
    (action == GLFW_REPEAT || action == GLFW_PRESS)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{-0.025f, 0.0f, 0.0f});
    updateView();
  }
  else if ((key == GLFW_KEY_S || key == GLFW_KEY_DOWN) &&
    (action == GLFW_REPEAT || action == GLFW_PRESS)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, -0.025f, 0.0f});
    updateView();
  }
  else if ((key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) &&
    (action == GLFW_REPEAT || action == GLFW_PRESS)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.025f, 0.0f, 0.0f});
    updateView();
  }
  else if ((key == GLFW_KEY_E || key == GLFW_KEY_Y) && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{ 0.0f, 0.0f, 0.05f });
    updateView();
  }
  else if ((key == GLFW_KEY_T || key == GLFW_KEY_X) && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{ 0.0f, 0.0f, -0.05f });
    updateView();
  }
  if (key == GLFW_KEY_1)
  {
      Cel = 0.0;
  }
  if (key == GLFW_KEY_2)
  {
      Cel = 1.0;
  }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  // mouse handling
  m_view_transform = glm::rotate(m_view_transform, (float)(pos_x/1000),
    glm::fvec3(0.0f,1.0f,0.0f));
  m_view_transform = glm::rotate(m_view_transform, (float)(pos_y/1000),
    glm::fvec3(1.0f,0.0f,0.0f));
  updateView();
}

// load shader programs
void ApplicationSolar::initializeShaderPrograms() {
  // store shader program objects in container
  m_shaders.emplace("planet", shader_program{m_resource_path + "shaders/simple.vert",
                                           m_resource_path + "shaders/simple.frag"});
  // request uniform locations for shader program
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
  // initialization of the new uniforms
  m_shaders.at("planet").u_locs["CelBool"] = -1;
  m_shaders.at("planet").u_locs["SunPosition"] = -1;
  m_shaders.at("planet").u_locs["AmbientVector"] = -1;
  m_shaders.at("planet").u_locs["DiffuseVector"] = -1;
  m_shaders.at("planet").u_locs["SpecularVector"] = -1;
  m_shaders.at("planet").u_locs["PlanetPos"] = -1;
  m_shaders.at("planet").u_locs["ShiningFloat"] = -1;

  // attempt to create a shader
  m_shaders.emplace("star", shader_program{m_resource_path + "shaders/star.vert",
                                           m_resource_path + "shaders/star.frag"});

  m_shaders.at("star").u_locs["ModelMatrix"] = -1;
  m_shaders.at("star").u_locs["ViewMatrix"] = -1;
  m_shaders.at("star").u_locs["ProjectionMatrix"] = -1;

  m_shaders.emplace("orbit", shader_program{m_resource_path + "shaders/orbit.vert",
                                            m_resource_path + "shaders/orbit.frag"});

  m_shaders.at("orbit").u_locs["ModelMatrix"] = -1;
  m_shaders.at("orbit").u_locs["ViewMatrix"] = -1;
  m_shaders.at("orbit").u_locs["ProjectionMatrix"] = -1;
}

// load models
void ApplicationSolar::initializeGeometry() {
  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);

  // glEnable(GL_PROGRAM_POINT_SIZE);
  // generate vertex array object
  glGenVertexArrays(1, &planet_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(planet_object.vertex_AO);


  // generate generic buffer
  glGenBuffers(1, &planet_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(), planet_model.data.data(), GL_STATIC_DRAW);
  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::NORMAL]);

   // generate generic buffer
  glGenBuffers(1, &planet_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  planet_object.draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object
  planet_object.num_elements = GLsizei(planet_model.indices.size());

    //assigment 2 buffer:
    glGenBuffers(1, &star_object.vertex_BO);
    glBindBuffer(GL_ARRAY_BUFFER, star_object.vertex_BO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * stars.size(), stars.data(), GL_STATIC_DRAW);
    // assigment 2 Vertex Array:
    glGenVertexArrays(1, &star_object.vertex_AO);
    glBindVertexArray(star_object.vertex_AO);
        glBindBuffer(GL_ARRAY_BUFFER, star_object.vertex_BO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)(sizeof(float)*3));
    // assigment 2 index buffer object
    glGenBuffers(1, &star_object.element_BO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, star_object.element_BO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * stars.size(),
        stars.data(), GL_STATIC_DRAW);

    //orbit buffer:
    glGenBuffers(1, &orbit_object.vertex_BO);
    glBindBuffer(GL_ARRAY_BUFFER, orbit_object.vertex_BO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*orbits.size(), orbits.data(), GL_STATIC_DRAW);
    // orbit Vertex Array:
    glGenVertexArrays(1, &orbit_object.vertex_AO);
    glBindVertexArray(orbit_object.vertex_AO);
        glBindBuffer(GL_ARRAY_BUFFER, orbit_object.vertex_BO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);
    // orbit index buffer object
    glGenBuffers(1, &orbit_object.element_BO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, orbit_object.element_BO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * orbits.size(),
        orbits.data(), GL_STATIC_DRAW);
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}

// exe entry point
int main(int argc, char* argv[]) {
for (int j = 0; j <= 179; j++) {
    double angle = 3.1415 * j / 90;
    float x = 1.0f * cos(angle);
    float y = 1.0f * sin(angle);
    orbits.push_back(x);
    orbits.push_back(y);
    orbits.push_back(0.0f);
}

    srand(time(NULL));

    for (int i = 0; i < 1000; ++i) {
       float p1 = -3 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (6)));
       float p2 = -3 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (6)));
       float p3 = -3 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (6)));

       float c1 = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
       float c2 = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
       float c3 = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

       stars.push_back(p1);
       stars.push_back(p2);
       stars.push_back(p3);
       stars.push_back(c1);
       stars.push_back(c2);
       stars.push_back(c3);
    }
  Launcher::run<ApplicationSolar>(argc, argv);
}
