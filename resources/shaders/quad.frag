#version 150

in vec2 pass_Texcoord;

uniform sampler2D BufferTex;

out vec4 out_Color;

void main() {
  out_Color = vec4(pass_Texcoord, 1.0, 1.0);
}
