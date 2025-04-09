// Camera.hpp
#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <GLFW/glfw3.h>  // Assurez-vous que GLFW est bien inclus

// Camera.hpp

class Camera {
    public:
        Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch);
        
        glm::mat4 getViewMatrix() const;
        void processKeyboard(GLFWwindow* window, float deltaTime);
        void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
        void processMouseScroll(double yOffset);
    
        float getZoom() const { return m_zoom; }
    
    private:
        void updateCameraVectors();
    
        glm::vec3 m_position;
        glm::vec3 m_front;
        glm::vec3 m_up;
        glm::vec3 m_right;
        glm::vec3 m_worldUp;
    
        float m_yaw;
        float m_pitch;
        float m_movementSpeed;
        float m_mouseSensitivity;
        float m_zoom;
    
        // Variables pour le suivi des mouvements de la souris
        float m_lastX;
        float m_lastY;
        bool m_firstMouse;
    };
    