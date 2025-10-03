#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "mesh.h"
#include <vector>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

float lastTime = 0.0f;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const unsigned int EDGE_COUNT = 40;
const unsigned int MAX_EDGE_WIDTH = 1;
const unsigned int ITERATIONS = 20;

bool mousePressed = false;
glm::vec2 lastMousePos(0.0f, 0.0f);

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);


    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    std::cout << "GLAD initialized successfully" << std::endl;
    
    Mesh mesh(EDGE_COUNT, MAX_EDGE_WIDTH, "../shaders/VertexShader.vert", "../shaders/FragmentShader.frag");
    std::cout << "Mesh created successfully" << std::endl;
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        if(mousePressed)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            glm::vec2 mousePos = glm::vec2((xpos / SCR_WIDTH) * 2.0f - 1.0f, 1.0f - (ypos / SCR_HEIGHT) * 2.0f);
            if(lastMousePos == glm::vec2(0.0f, 0.0f)) lastMousePos = mousePos;

            glm::vec2 deltaMouse = mousePos - lastMousePos;
            lastMousePos = mousePos;

            mesh.positions[EDGE_COUNT- 1] += glm::vec3(deltaMouse, 0.0f);
            mesh.velocities[EDGE_COUNT - 1] = glm::vec3(deltaMouse / deltaTime, 0.0f);
            mesh.isVerticesFixed[EDGE_COUNT - 1] = 1;
        }
        else
        {
            lastMousePos = glm::vec2(0.0f, 0.0f);
            mesh.isVerticesFixed[EDGE_COUNT - 1] = 0;
        }
        
        mesh.addGravity(deltaTime, glm::vec3(0.0f, -0.5f, 0.0f));
        mesh.applyDamping(0.01f);
        mesh.updateEstimatedPositions(deltaTime);
        for(int i=0; i<ITERATIONS; i++) mesh.SolveAllStretchConstraints();
        mesh.updateVelocitiesAndPositions(deltaTime);
        mesh.updatePositions();

        // render the triangle
        mesh.draw();
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    mesh.deleteArraysAndBuffers();
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        mousePressed = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        mousePressed = false;
    }
}