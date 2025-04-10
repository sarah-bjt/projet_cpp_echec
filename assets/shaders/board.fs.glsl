#version 330 core
out vec4 FragColor;

in vec2 TexCoord; // Reçoit la coordonnée de texture du vertex shader

uniform sampler2D texture1; // La texture que tu veux utiliser

void main() {
    FragColor = texture(texture1, TexCoord); // Applique la texture à l'objet
}
