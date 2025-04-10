#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
    Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch, const glm::vec3& center);

    glm::mat4 getViewMatrix() const;

    void processMovement(float deltaTime); 
    void keyState(int key, bool isPressed);
    void setRadius(float radius);
    void setPitchLimits(float minPitch, float maxPitch);
    float getZoom() const { return m_zoom; };
    float getYaw() const { return m_yaw; }
    void setYaw(float yaw) { m_yaw = yaw; }
    float getPitch() const { return m_pitch; }
    void setPitch(float pitch) { m_pitch = pitch; }
    glm::vec3 getPosition() const { return m_position; }

private:
    void updatePosition();
    void updateCameraVectors();

    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;
    glm::vec3 m_center;

    float m_yaw;
    float m_pitch;
    float m_movementSpeed;
    float m_zoom;
    float m_radius;
    float m_minPitch, m_maxPitch;
    float m_minHeight;

    bool m_keys[1024];
};
