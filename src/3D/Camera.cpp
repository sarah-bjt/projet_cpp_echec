#include "Camera.hpp"
#include <iostream>
#include <cmath>
#include <GLFW/glfw3.h>

Camera::Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch, const glm::vec3& center)
    : m_position(position), m_worldUp(up), m_yaw(yaw), m_pitch(pitch),
      m_movementSpeed(0.5f), m_zoom(45.0f), m_radius(10.0f), m_minPitch(-89.0f), m_maxPitch(89.0f),
      m_center(center), m_minHeight(0.0f)
{
    std::fill(std::begin(m_keys), std::end(m_keys), false);
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::keyState(int key, bool isPressed)
{
    m_keys[key] = isPressed;
}

void Camera::processMovement(float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;

    if (m_keys[GLFW_KEY_W]) {
        m_pitch += m_movementSpeed;
    }
    if (m_keys[GLFW_KEY_S]) {
        m_pitch -= m_movementSpeed;
    }
    if (m_keys[GLFW_KEY_A]) {
        m_yaw -= m_movementSpeed;
    }
    if (m_keys[GLFW_KEY_D]) {
        m_yaw += m_movementSpeed;
    }

    if (m_keys[GLFW_KEY_Q]) {
        m_radius -= m_movementSpeed;
    }
    if (m_keys[GLFW_KEY_E]) {
        m_radius += m_movementSpeed;
    }

    if (m_radius < 5.0f) m_radius = 5.0f;
    if (m_radius > 50.0f) m_radius = 50.0f;

    if (m_pitch > m_maxPitch) m_pitch = m_maxPitch;
    if (m_pitch < m_minPitch) m_pitch = m_minPitch;

    updatePosition();
}



void Camera::setRadius(float radius)
{
    m_radius = radius;
    updatePosition();
}

void Camera::setPitchLimits(float minPitch, float maxPitch)
{
    m_minPitch = minPitch;
    m_maxPitch = maxPitch;
}

void Camera::updatePosition()
{
    m_position.x = m_center.x + m_radius * cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
    m_position.y = m_center.y + m_radius * sin(glm::radians(m_pitch));

    if (m_position.y < m_minHeight) {
        m_position.y = m_minHeight; 
    }

    m_position.z = m_center.z + m_radius * cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));

    m_front = glm::normalize(m_center - m_position);

    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up    = glm::normalize(glm::cross(m_right, m_front));
}



void Camera::updateCameraVectors()
{
    m_front = glm::normalize(m_position - glm::vec3(0.0f));
    
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up    = glm::normalize(glm::cross(m_right, m_front));
}
