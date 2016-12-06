#version 150
#extension GL_ARB_explicit_attrib_location : require

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

    if (horizontal_mirror == true)
    {
        float h1 = pass_Texcoord.x;
        float h2 = 1.0f - pass_Texcoord.y;

        out_Color = texture(BufferTex, vec2(h1,h2));
    }

    if (vertical_mirror == true)
    {
        float v1 = 1.0f - pass_Texcoord.x;
        float v2 = pass_Texcoord.y;

        out_Color = texture(BufferTex, vec2(v1,v2));
    }

    if (gaussian_kernel == true)
    {
        //pass_Texcoord = pixel_size * gl_FragCoord
        vec2 p_s = pass_Texcoord / gl_FragCoord.xy;

        //total 9 texture lookups
        vec4 n1 = texture(
                            BufferTex,
                            (vec2(pass_Texcoord.x - p_s.x,
                                  pass_Texcoord.y + p_s.y))
                         ) * 1/16;

        vec4 n2 = texture(
                            BufferTex,
                            (vec2(pass_Texcoord.x,
                                  pass_Texcoord.y + p_s.y))
                         ) * 1/8;

        vec4 n3 = texture(
                            BufferTex,
                            (vec2(pass_Texcoord.x + p_s.x,
                                  pass_Texcoord.y + p_s.y))
                         ) * 1/16;

        vec4 n4 = texture(
                            BufferTex,
                            (vec2(pass_Texcoord.x - p_s.x,
                                  pass_Texcoord.y))
                         ) * 1/8;

        vec4 n5 = texture(
                            BufferTex,
                            (vec2(pass_Texcoord.x,
                                  pass_Texcoord.y))
                         ) * 1/4;

        vec4 n6 = texture(
                            BufferTex,
                            (vec2(pass_Texcoord.x + p_s.x,
                                  pass_Texcoord.y))
                         ) * 1/8;

        vec4 n7 = texture(
                            BufferTex,
                            (vec2(pass_Texcoord.x - p_s.x,
                                  pass_Texcoord.y - p_s.y))
                         ) * 1/16;

        vec4 n8 = texture(
                            BufferTex,
                            (vec2(pass_Texcoord.x,
                                  pass_Texcoord.y - p_s.y))
                         ) * 1/8;

        vec4 n9 = texture(
                            BufferTex,
                            (vec2(pass_Texcoord.x + p_s.x,
                                  pass_Texcoord.y - p_s.y))
                         ) * 1/16;

        out_Color = n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8 + n9;
    }
}
