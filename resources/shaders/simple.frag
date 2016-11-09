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
uniform float CelBool;

void main() {
vec3 LightVec = normalize(SunPosition - FragPos);
vec3 HalfwayVec = normalize(LightVec + ViewVec);
// blinn-phong illumiantion model with modified intensities mainly
// for cosmetics
vec3 amb = AmbientVector * 1;
vec3 dif = DiffuseVector * max(dot(LightVec,pass_Normal),0);
vec3 spec = SpecularVector *
            pow(max(dot(pass_Normal,HalfwayVec),0),4*ShiningFloat);
vec3 illumination = (amb + dif + spec)/(abs(PlanetPos.x)*1.5+0.4);
if (CelBool == 0)
    {
        out_Color = vec4(illumination, 1.0);
    } else {
        // drawing the outline
        if (dot(ViewVec,pass_Normal) <= 0.3)
        {
            out_Color = vec4(0.9,0.9,0.9,1.0);
        // several cases for different color intensities
        } else if (dot(LightVec,pass_Normal) <= 0) {
            out_Color = vec4(illumination,1.0) * 0.9;
        } else if (0 < dot(LightVec,pass_Normal) && dot(LightVec,pass_Normal) <= 0.22) {
            out_Color = vec4(illumination,1.0) * 1;
        } else if (0.22 < dot(LightVec,pass_Normal) && dot(LightVec,pass_Normal) <= 0.45) {
            out_Color = vec4(illumination,1.0) * 1.1;
        } else if (0.45 < dot(LightVec,pass_Normal) && dot(LightVec,pass_Normal) <= 0.68) {
            out_Color = vec4(illumination,1.0) * 1.15;
        } else if (0.68 < dot(LightVec,pass_Normal) && dot(LightVec,pass_Normal) <= 0.85) {
            out_Color = vec4(illumination,1.0) * 1.3;
        } else if (0.85 < dot(LightVec,pass_Normal) && dot(LightVec,pass_Normal) <= 0.96) {
            out_Color = vec4(illumination,1.0) * 1.4;
        } else {
            out_Color = vec4(illumination,1.0) * 1.5;
        }
    }
}

