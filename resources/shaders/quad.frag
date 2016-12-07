#version 150

in vec2 pass_Texcoord;

uniform sampler2D BufferTex;

uniform bool luminance;
uniform bool horizontal_mirror;
uniform bool vertical_mirror;
uniform bool gaussian_kernel;
uniform bool four_grid;

out vec4 out_Color;

void main() {
  out_Color = texture(BufferTex, pass_Texcoord);
  //press "4" for a fancy 4 grid window
    if (four_grid == true)
    {
        out_Color = texture(BufferTex, pass_Texcoord*2);
        if (pass_Texcoord.x < 0.5 && pass_Texcoord.y > 0.5)
        {
            float lumifloat = 0.2126 * out_Color.r + 0.7152 * out_Color.g + 0.0722 * out_Color.b;
            out_Color = vec4(lumifloat,lumifloat,lumifloat,1.0f);
        }
        if (pass_Texcoord.x > 0.5 && pass_Texcoord.y > 0.5)
        {
            float scale = pass_Texcoord.x + pass_Texcoord.y;
            out_Color.z *= scale*2.5;
        }
        if (pass_Texcoord.x < 0.5 && pass_Texcoord.y < 0.5)
        {
            out_Color = texture(BufferTex, pass_Texcoord*8);
            if (pass_Texcoord.x < 0.25 && pass_Texcoord.y < 0.25)
            {
                out_Color = texture(BufferTex, pass_Texcoord*16);
                if (pass_Texcoord.x < 0.125 && pass_Texcoord.y < 0.125)
                {
                    out_Color = texture(BufferTex, pass_Texcoord*32);
                }
            }
        }
        if (pass_Texcoord.x > 0.5 && pass_Texcoord.y < 0.5)
        {
            out_Color = out_Color*4;
        }
    }

  //for allowing multiple effects, the TexCoordinates have to be global
  float temp_x = pass_Texcoord.x;
  float temp_y = pass_Texcoord.y;

    if (horizontal_mirror == true)
    {
        temp_y = 1.0f - temp_y;

        out_Color = texture(BufferTex, vec2(temp_x,temp_y));
    }

    if (vertical_mirror == true)
    {
        temp_x = 1.0f - temp_x;

        out_Color = texture(BufferTex, vec2(temp_x,temp_y));
    }

    if (gaussian_kernel == true)
    {
        //pass_Texcoord = pixel_size * gl_FragCoord
        vec2 p_s = pass_Texcoord / gl_FragCoord.xy;

        //total 9 texture lookups
        vec4 n1 = texture(
                            BufferTex,
                            (vec2(temp_x - p_s.x,
                                  temp_y + p_s.y))
                         ) * 1/16;

        vec4 n2 = texture(
                            BufferTex,
                            (vec2(temp_x,
                                  temp_y + p_s.y))
                         ) * 1/8;

        vec4 n3 = texture(
                            BufferTex,
                            (vec2(temp_x + p_s.x,
                                  temp_y + p_s.y))
                         ) * 1/16;

        vec4 n4 = texture(
                            BufferTex,
                            (vec2(temp_x - p_s.x,
                                  temp_y))
                         ) * 1/8;

        vec4 n5 = texture(
                            BufferTex,
                            (vec2(temp_x,
                                  temp_y))
                         ) * 1/4;

        vec4 n6 = texture(
                            BufferTex,
                            (vec2(temp_x + p_s.x,
                                  temp_y))
                         ) * 1/8;

        vec4 n7 = texture(
                            BufferTex,
                            (vec2(temp_x - p_s.x,
                                  temp_y - p_s.y))
                         ) * 1/16;

        vec4 n8 = texture(
                            BufferTex,
                            (vec2(temp_x,
                                  temp_y - p_s.y))
                         ) * 1/8;

        vec4 n9 = texture(
                            BufferTex,
                            (vec2(temp_x + p_s.x,
                                  temp_y - p_s.y))
                         ) * 1/16;

        out_Color = n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8 + n9;
    }

    //checking if luminance preserving grayscale is applied
    if (luminance == true)
    {
        float lumifloat = 0.2126 * out_Color.r + 0.7152 * out_Color.g + 0.0722 * out_Color.b;

        out_Color = vec4(lumifloat,lumifloat,lumifloat,1.0f);
    }
}
