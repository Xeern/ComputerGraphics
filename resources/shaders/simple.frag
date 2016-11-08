#version 150

in vec3 pass_Normal;
in vec3 ViewVec;
in vec3 FragPos;
out vec4 out_Color;

uniform vec3 SunPosition;
uniform vec3 AmbientVector;
uniform vec3 DiffuseVector;
uniform vec3 SpecularVector;
uniform float ShiningFloat;
uniform vec3 PlanetPos;

void main() {
  vec3 LightVec = normalize(SunPosition - FragPos);
  vec3 HalfwayVec = normalize(LightVec + ViewVec);
  // blinn-phong illumiantion model with modified intensities mainly
  // for cosmetics
  vec3 amb = AmbientVector * 1;
  vec3 dif = DiffuseVector * 0.2 * max(dot(LightVec,pass_Normal),0);
  vec3 spec = SpecularVector * 0.0025 * pow(max(dot(pass_Normal,HalfwayVec),0),4*ShiningFloat);
  vec3 illumination = (amb + dif + spec)/(abs(PlanetPos.x)*1.5+0.4);
  out_Color = vec4(illumination, 1.0);
}

