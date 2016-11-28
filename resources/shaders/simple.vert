#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_Texcoord;
layout(location = 3) in vec3 in_Tangent;

//Matrix Uniforms as specified with glUniformMatrix4fv
uniform float CelBool;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;

out vec2 pass_Texcoord;
out vec3 pass_Tangent;
out vec3 pass_Normal;
out vec3 ViewVec;
out vec3 FragPos;

void main(void)
{
    pass_Texcoord = in_Texcoord;
    vec4 border = vec4(in_Position + in_Normal * 0,1.0);
    pass_Tangent = in_Tangent * 2 - 1;
    // vec3 pass_Tangent = (mat3(NormalMatrix) * in_Tangent);

    if (CelBool == 1)
    {
        // used to prevent the planets from "shrinking" when adding the outline
        // but this causes a small circle to appear on our planets, because
        // the model of the spheres seems to have that texture
        // by multiplying with 0 this doesn’t happen though
        border = vec4(in_Position + in_Normal * 0.1,1.0);
    }
    gl_Position = (ProjectionMatrix * ViewMatrix * ModelMatrix) * border;

    // pass_Normal = normalize((NormalMatrix * vec4(in_Normal, 0.0)).xyz);
    // changed the pass_Normal variable because the old one caused our shadows
    // to go all nuts when changing the camera position
    pass_Normal = normalize(mat3(transpose(inverse(ModelMatrix))) * in_Normal);

    vec3 pass_Tangent = (pass_Normal * in_Tangent);
    mat4 CameraMatrix = inverse(ViewMatrix);
    vec3 ViewPos = CameraMatrix[3].xyz;
    FragPos = vec3(ModelMatrix * vec4(in_Position,1.0));
    ViewVec = normalize(ViewPos - FragPos);
}
