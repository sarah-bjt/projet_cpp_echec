#version 330 core
out vec4 FragColor;

in vec3 FragPos;   // Position fragment
in vec3 Normal;    // Normale

uniform vec3 lightPos;  // Position de la lumière
uniform vec3 viewPos;   // Position de la caméra

void main() {
    // Lumière simple - Phong
    vec3 ambient = 0.1 * vec3(1.0, 1.0, 1.0);  // Lumière ambiante (blanc)
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);  // Composant diffuse
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);  // Diffuse (blanc)

    // Calcul de la réflexion (speculaire)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);  // Shininess à 32
    vec3 specular = spec * vec3(1.0, 1.0, 1.0);  // Spécularité (blanc)

    // Combinaison des composantes
    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);  // Appliquer la couleur calculée
}
