// GLFW-based subviewer implementation

#include "pickviewer.hpp"
#include "GLFW/glfw3.h"

#include <iostream>
#include <vector>

// Global variables for GLFW windows
static std::vector<GLFWwindow*> windows;
static std::vector<Viewer*> viewers;

// Callback functions for GLFW
static void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    
    // Find which viewer this window belongs to
    for (size_t i = 0; i < windows.size(); ++i) {
        if (windows[i] == window && i < viewers.size()) {
            // Convert GLFW key to ASCII (simplified)
            unsigned char key_char = (key >= 32 && key <= 126) ? (char)key : 0;
            if (key_char != 0) {
                double xpos, ypos;
                glfwGetCursorPos(window, &xpos, &ypos);
                viewers[i]->key(key_char, (int)xpos, (int)ypos);
            }
            break;
        }
    }
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    // Find which viewer this window belongs to
    for (size_t i = 0; i < windows.size(); ++i) {
        if (windows[i] == window && i < viewers.size()) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            int state = (action == GLFW_PRESS) ? 0 : 1; // GLUT_DOWN = 0, GLUT_UP = 1
            viewers[i]->mouse(button, state, (int)xpos, (int)ypos);
            break;
        }
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    // Find which viewer this window belongs to
    for (size_t i = 0; i < windows.size(); ++i) {
        if (windows[i] == window && i < viewers.size()) {
            viewers[i]->motion((int)xpos, (int)ypos);
            break;
        }
    }
}

static void window_size_callback(GLFWwindow* window, int width, int height) {
    // Find which viewer this window belongs to
    for (size_t i = 0; i < windows.size(); ++i) {
        if (windows[i] == window && i < viewers.size()) {
            viewers[i]->reshape(width, height);
            break;
        }
    }
}

// Main function
int main(int argc, char** argv) {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    // Create a viewer for the triangle-based structure
    PickViewer triViewer(const_cast<char*>("triViewer"));
    triViewer.setSize(512, 512);
    
    // Create a viewer for the quad-based structure
    PickViewer quadViewer(const_cast<char*>("quadViewer"));
    quadViewer.setSize(512, 512);

    // Store viewers for callbacks
    viewers.push_back(&triViewer);
    viewers.push_back(&quadViewer);

    // Create windows for each viewer
    for (size_t i = 0; i < viewers.size(); ++i) {
        GLFWwindow* window = glfwCreateWindow(
            viewers[i]->getWidth(), 
            viewers[i]->getHeight(), 
            viewers[i] == &triViewer ? "Triangle Viewer" : "Quad Viewer", 
            NULL, NULL);
        
        if (!window) {
            glfwTerminate();
            std::cerr << "Failed to create GLFW window" << std::endl;
            return -1;
        }

        glfwMakeContextCurrent(window);
        glfwSetKeyCallback(window, key_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
        glfwSetWindowSizeCallback(window, window_size_callback);
        
        windows.push_back(window);
    }

    // Position windows
    glfwSetWindowPos(windows[0], 100, 100);
    glfwSetWindowPos(windows[1], 650, 100);

    // Main loop
    while (!windows.empty()) {
        // Process each window
        for (size_t i = 0; i < windows.size(); ) {
            GLFWwindow* window = windows[i];
            
            if (glfwWindowShouldClose(window)) {
                // Remove this window
                glfwDestroyWindow(window);
                windows.erase(windows.begin() + i);
                if (i < viewers.size()) {
                    viewers.erase(viewers.begin() + i);
                }
                continue;
            }
            
            // Make the window's context current
            glfwMakeContextCurrent(window);
            
            // Get window size
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            
            // Set viewport
            glViewport(0, 0, width, height);
            
            // Clear the buffer
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            // Render the scene
            if (i < viewers.size()) {
                viewers[i]->display();
            }
            
            // Swap front and back buffers
            glfwSwapBuffers(window);
            
            i++;
        }
        
        // Poll for and process events
        glfwPollEvents();
        
        // Break if all windows are closed
        if (windows.empty()) {
            break;
        }
    }

    glfwTerminate();
    return 0;
}
