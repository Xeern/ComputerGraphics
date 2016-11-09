#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;

//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;
uniform float CelBool;

out vec3 pass_Normal;
out vec3 ViewVec;
out vec3 FragPos;

void main(void)
{
    vec4 border = vec4(in_Position + in_Normal * 0,1.0);
    if (CelBool == 1)
    {
        border = vec4(in_Position + in_Normal * 0.05,1.0);
    }
    gl_Position = (ProjectionMatrix * ViewMatrix * ModelMatrix) * border;
    // pass_Normal = normalize((NormalMatrix * vec4(in_Normal, 0.0)).xyz);

    // changed the pass_Normal variable because the old one caused our shadows to go
    // all nuts when changing the camera position
    pass_Normal = mat3(transpose(inverse(ModelMatrix))) * in_Normal;

    mat4 CameraMatrix = inverse(ViewMatrix);
    vec3 ViewPos = CameraMatrix[3].xyz;
    FragPos = vec3(ModelMatrix * vec4(in_Position,1.0));
    ViewVec = normalize(ViewPos - FragPos);
}
