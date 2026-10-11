#version 460 core
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
uniform vec4 u_Color;
uniform vec3 u_LightDir;
uniform sampler2D u_Tex = 0;

void main() {
    float intensity = max(dot(vec3(0.0, 0.0, 1.0), u_LightDir), 0.3);
    vec4 texColor = texture(u_Tex, v_TexCoord);
    color = texColor * u_Color * intensity;
}