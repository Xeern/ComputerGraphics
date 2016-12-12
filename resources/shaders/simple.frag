#version 150
#extension GL_ARB_explicit_attrib_location : require

in vec3 pass_Normal;
in vec3 ViewVec;
in vec3 FragPos;
in vec2 pass_Texcoord;
in vec3 pass_Tangent;

uniform vec3 PlanetPos;
uniform vec3 SunPosition;
uniform vec3 AmbientVector;
uniform vec3 DiffuseVector;
uniform vec3 SpecularVector;
uniform float ShiningFloat;

uniform float CelBool;

uniform sampler2D ColorTex;
uniform sampler2D NormalTex;
uniform sampler2D SkyTex;

layout(location = 0) out vec4 out_Color;

void main() {
//general illumination
vec3 LightVec = normalize(SunPosition - FragPos);
vec3 HalfwayVec = normalize(LightVec + ViewVec);

//color of the skysphere
vec4 skyColor = texture(SkyTex,pass_Texcoord);
//normal texture color
vec4 texColor = texture(ColorTex,pass_Texcoord);

//calculations for normal mapping
vec3 biTangent = cross(pass_Normal,pass_Tangent);
mat3 tMat = transpose(mat3(pass_Tangent,biTangent,pass_Normal));
vec3 texSpaceNorm = texture(NormalTex,pass_Texcoord).rgb;

//texture space transformation
float temp_x = texSpaceNorm.x * 2.0f - 1.0f;
float temp_y = texSpaceNorm.y * 2.0f - 1.0f;
vec3 tanSpaceNorm = vec3(temp_x, temp_y, texSpaceNorm.z);
vec3 textureNormal = tanSpaceNorm * tMat;


//blinn-phong illumination model with modified intensities mainly
// for cosmetics (general illumination)
vec3 amb = AmbientVector/*vec3(0.9f, 0.9f, 0.9f)*/ * 1 * vec3(texColor);
vec3 dif = DiffuseVector/*vec3(0.7f, 0.7f, 0.7f)*/ * max(dot(LightVec,pass_Normal),0) *
        vec3(texColor);
vec3 spec = SpecularVector *
            pow(max(dot(pass_Normal,HalfwayVec),0),4*ShiningFloat);
vec3 illumination = (amb + dif + spec)/(abs(PlanetPos.x)*1.5+0.4);
//illumination if normal texture is available
if (texSpaceNorm.x != 0.0 )
{
amb = AmbientVector * 1 * vec3(texColor);
dif = DiffuseVector * max(dot(LightVec,textureNormal),0) * vec3(texColor);
spec = SpecularVector *
            pow(max(dot(pass_Normal,HalfwayVec),0),4*ShiningFloat);
illumination = (amb + dif + spec)/(abs(PlanetPos.x)*1.5+0.4);
}

if (CelBool == 0)
    //normal illumination
    {
        out_Color = vec4(illumination, 1.0);
        if (skyColor.x != 0.0)
        {
            out_Color = skyColor;
        }
    } else if (CelBool == 1) {
        //cel-shading
        dif = DiffuseVector * max(dot(LightVec,pass_Normal),0) * vec3(texColor);
        spec = SpecularVector *
            pow(max(dot(pass_Normal,HalfwayVec),0),4*ShiningFloat);
        illumination = (amb + dif + spec)/(abs(PlanetPos.x)*1.5+0.4);
        // drawing the outline
        if (dot(ViewVec,pass_Normal) <= 0.3)
        {
            //small fix for the outline, yet to solve properly
            // out_Color = vec4(0.7,0.7,0.7,1.0);
            out_Color = skyColor;
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
        if (skyColor.x != 0.0)
        {
            out_Color = skyColor;
        }
    } else if (CelBool == 2) {
        //mostly for cheking several things
        //currently textures only without illumination
        out_Color = vec4(textureNormal,1.0);
        if (skyColor.x != 0.0)
        {
            out_Color = skyColor;
        }
    }
}

