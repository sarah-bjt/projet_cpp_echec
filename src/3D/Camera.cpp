#include "Camera.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

Camera::Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch)
    : m_position(position), m_worldUp(up), m_yaw(yaw), m_pitch(pitch),
      m_movementSpeed(2.5f), m_mouseSensitivity(0.1f), m_zoom(45.0f),
      m_firstMouse(true), m_lastX(0.0f), m_lastY(0.0f) // Initialisation des variables
{
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::processKeyboard(GLFWwindow* window, float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        m_position += m_front * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        m_position -= m_front * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        m_position -= m_right * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        m_position += m_right * velocity;
    }

    // Debugging position
    std::cout << "Camera Position: " << m_position.x << ", " 
              << m_position.y << ", " << m_position.z << std::endl;
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
    if (m_firstMouse) {
        m_lastX = xOffset;
        m_lastY = yOffset;
        m_firstMouse = false;
    }

    xOffset *= m_mouseSensitivity;
    yOffset *= m_mouseSensitivity;

    float xOffsetDelta = xOffset - m_lastX;
    float yOffsetDelta = m_lastY - yOffset; // Inversé

    m_lastX = xOffset;
    m_lastY = yOffset;

    m_yaw += xOffsetDelta;
    m_pitch += yOffsetDelta;

    if (constrainPitch) {
        if (m_pitch > 89.0f) m_pitch = 89.0f;
        if (m_pitch < -89.0f) m_pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::processMouseScroll(double yOffset)
{
    if (m_zoom >= 1.0f && m_zoom <= 45.0f) {
        m_zoom -= yOffset; // Ajuste le zoom en fonction du défilement
    }
    if (m_zoom < 1.0f) {
        m_zoom = 1.0f;  // Limite inférieure
    }
    if (m_zoom > 45.0f) {
        m_zoom = 45.0f;  // Limite supérieure
    }
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
