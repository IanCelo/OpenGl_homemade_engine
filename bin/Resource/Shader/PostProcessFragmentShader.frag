#version 450 core

uniform sampler2D Scene;
uniform sampler2D BloomBlur;
uniform float Exposure;
uniform float Gamma;
uniform bool Bloom;

in vec2 TexCoords;
out vec4 FragColor;


void setSharpKernel(inout float kernel[9])
{
    kernel = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
}

void setEdgeKernel(inout float kernel[9])
{
    kernel = float[](
        1,  1,  1,
        1, -8,  1,
        1,  1,  1
    );
}

void setBlurKernel(inout float kernel[9])
{
    kernel = float[](
        1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0,
        2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0,
        1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0
    );
}

void main(void) 
{
    vec3 hdrColor = texture(Scene, TexCoords).rgb;      

    if(Bloom)
    {
        vec3 bloomColor = texture(BloomBlur, TexCoords).rgb;
        hdrColor += bloomColor; // additive blending
    
        // tone mapping
        vec3 result = vec3(1.0) - exp(-hdrColor * Exposure);

        // also Gamma correct while we're at it       
        result = pow(result, vec3(1.0 / Gamma));
        FragColor = vec4(result, 1.0);
    }
    else
    {
        FragColor = vec4(hdrColor,1.0);
    }
}