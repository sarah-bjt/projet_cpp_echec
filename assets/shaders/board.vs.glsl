#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aKd;
layout (location = 4) in vec3 aKs;
layout (location = 5) in vec3 aKa;
layout (location = 6) in float aNs;
layout (location = 7) in int aUseTexture;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out vec3 Kd;
out vec3 Ks;
out vec3 Ka;
out float Ns;
flat out int useTexture;

void main() {
    TexCoord = aTexCoord;
    FragPos = vec3(uModel * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(uModel))) * aNormal;
    Kd = aKd;
    Ks = aKs;
    Ka = aKa;
    Ns = aNs;
    useTexture = aUseTexture;
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}
