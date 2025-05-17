#version 410

// degauss
// fragment shader

uniform sampler2DRect tex0;
uniform float u_time = 0.0;
uniform float u_degauss = 0.0;
uniform vec2 u_resolution = vec2(1280, 1024);

const float PI = 3.14159265359;

in vec2 texCoordVarying;
out vec4 outputColor;

void main() {
    vec2 st = texCoordVarying / u_resolution;
    vec2 cPos = -1.0 + 2.0 * st;
    float cLength = length(cPos);
    
    vec2 add = (cPos / cLength) * cos(cLength * 25.0 - u_time * 4.0) * 0.015;
    add *= pow(u_degauss, 3);
    
    vec3 color = texture(tex0, texCoordVarying + add * 1000.0).rgb;
    outputColor = vec4(color, 1.0);
}
