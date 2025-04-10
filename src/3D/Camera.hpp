#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
    Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch);

    glm::mat4 getViewMatrix() const;

    void processMovement(float deltaTime); // Gère le déplacement sans utiliser GLFW_PRESS
    void keyState(int key, bool isPressed); // Enregistrer l'état de la touche pressée ou relâchée
    void setRadius(float radius);
    void setPitchLimits(float minPitch, float maxPitch);
    float getZoom() const { return m_zoom; };
    float getYaw() const { return m_yaw; }
    void setYaw(float yaw) { m_yaw = yaw; }
    float getPitch() const { return m_pitch; }
    void setPitch(float pitch) { m_pitch = pitch; }

private:
    bool m_mouseEnabled = false; // Contrôler si la souris influence la caméra
    void updatePosition(); // Met à jour la position de la caméra
    void updateCameraVectors(); // Met à jour les vecteurs de la caméra

    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;

    float m_yaw;
    float m_pitch;
    float m_movementSpeed;
    float m_zoom;
    float m_radius;

    float m_minPitch, m_maxPitch;

    bool m_keys[1024]; // Tableau pour stocker l'état des touches
};
