#version 330 core
out vec4 FragColor;

in vec3 FragPos;   // Position fragment
in vec3 Normal;    // Normale
in vec2 TexCoord;
in vec3 Kd;
in vec3 Ka;
in vec3 Ks;
in float Ns;
flat in int useTexture;

uniform vec3 lightPos;  // Position de la lumière
uniform vec3 viewPos;   // Position de la caméra
uniform sampler2D texture;

void main() {
    vec3 texColor;
    if (useTexture != 0) {
        texColor = texture(texture, TexCoord).rgb;
    } else {
        texColor = vec3(1.0, 1.0, 1.0); 
    }
    // Lumière simple - Phong
    vec3 ambient = Ka * (vec3(1.0, 1.0, 1.0)*0.5);  // Lumière ambiante (blanc)
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);  // Composant diffuse
    vec3 diffuse = Kd * diff * vec3(1.0, 1.0, 1.0);  // Diffuse (blanc)

    // Calcul de la réflexion (speculaire)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, reflectDir), 0.0), Ns);  // Shininess à 32
    vec3 specular = Ks * spec * vec3(1.0, 1.0, 1.0);  // Spécularité (blanc)

    // Combinaison des composantes
    vec3 result = (ambient + diffuse) * texColor + specular;

    result = pow(result, vec3(1.0/2.2));

    FragColor = vec4(result, 1.0);  // Appliquer la couleur calculée
}
