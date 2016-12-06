#version 150

in vec2 pass_Texcoord;

uniform sampler2D BufferTex;

uniform bool luminance;
uniform bool horizontal_mirror;
uniform bool vertical_mirror;
uniform bool gaussian_kernel;

out vec4 out_Color;

void main() {
  // out_Color = vec4(pass_Texcoord, 1.0, 1.0);
  out_Color = texture(BufferTex, pass_Texcoord);
  out_Color = out_Color;
    if (luminance == true)
    {
        float lumifloat = 0.2126 * out_Color.r + 0.7152 * out_Color.g + 0.0722 * out_Color.b;
        out_Color = vec4(lumifloat,lumifloat,lumifloat,1.0f);
    }
}
