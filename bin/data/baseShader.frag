#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

// define our rectangular texture sampler
uniform sampler2DRect tex0;
uniform float tex_w;
uniform float tex_h;

void main( void )
{

    vec2 st = gl_TexCoord[0].st;
    vec4 colorBase = texture2DRect(tex0, st);

    gl_FragColor = colorBase;
}

