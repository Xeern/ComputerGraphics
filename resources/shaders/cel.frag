#version 150

in vec3 pass_Normal;
in vec3 ViewVec;
in vec3 FragPos;
out vec4 out_Color;

// uniform vec3 SunPosition;
// uniform vec3 AmbientVector;
// uniform vec3 DiffuseVector;
// uniform vec3 SpecularVector;
// uniform float ShiningFloat;

void main() {
  out_Color = vec4(0.0,0.0,1.0,1.0);
}

