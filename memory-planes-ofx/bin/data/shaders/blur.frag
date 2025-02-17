#version 410

// bloom
// fragment shader

// adapted for ofx from existical's "gaussian blur simple and fast"
// https://www.shadertoy.com/view/Xltfzj

uniform sampler2DRect tex0;
uniform float u_blurMix = 0.0;
uniform float u_blurQuality = 9.0;
uniform float u_blurAngles = 24.0;
uniform float u_blurRadius = 16.0;

in vec2 texCoordVarying;
out vec4 outputColor;

void main() {
    float two_pi = 6.28318530718;
            
    // pixel color
    vec4 color = texture(tex0, texCoordVarying);
    
    // blur calculations
    for( float theta = 0.0; theta < two_pi; theta += two_pi / u_blurAngles) {
        for(float i = 1.0 / u_blurQuality; i <= 1.0; i+= 1.0 / u_blurQuality) {
            color += texture(tex0, texCoordVarying + vec2(cos(theta), sin(theta)) * u_blurRadius * i);
        }
    }
    
    color /= u_blurQuality * u_blurAngles - 15.0;
    outputColor = mix(texture(tex0, texCoordVarying), color, u_blurMix);
}
