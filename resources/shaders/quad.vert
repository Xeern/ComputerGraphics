#version 150
#extension GL_ARB_explicit_attrib_location : require
// glVertexAttribPointer mapped positions to first
layout(location = 0) in vec3 in_Position;

out vec2 pass_Texcoord;

void main() {
	gl_Position = /*(ProjectionMatrix * ViewMatrix * ModelMatrix) * */vec4(in_Position, 1.0);
    if (in_Position == vec3(-1.0f,-1.0f,0.0f))
        pass_Texcoord = vec2(0.0f,0.0f);
    else if (in_Position == vec3(1.0f,-1.0f,0.0f))
        pass_Texcoord = vec2(1.0f,0.0f);
    else if (in_Position == vec3(1.0f,1.0f,0.0f))
        pass_Texcoord = vec2(1.0f,1.0f);
    else if (in_Position == vec3(-1.0f,1.0f,0.0f))
        pass_Texcoord = vec2(0.0f,1.0f);
}
