#version 460 core
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;

uniform vec4 u_Color;
uniform vec3 u_LightDir;
layout(binding = 0) uniform sampler2D u_Tex;

void main() {
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightDir);
    float diff = max(dot(norm, lightDir), 0.0);
    float ambient = 0.25;
    float intensity = diff + ambient;
    vec4 texColor = texture(u_Tex, v_TexCoord);
    color = texColor * u_Color * intensity;
}