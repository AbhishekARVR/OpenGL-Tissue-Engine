#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaders/shader.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
std::vector<float> createMeshVertices(int edgeCount, int maxEdgeWidth);
std::vector<unsigned int> createMeshIndices(int edgeCount, int maxEdgeWidth);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const unsigned int EDGE_COUNT = 2;
const unsigned int MAX_EDGE_WIDTH = 1;

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

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("../shaders/VertexShader.vert", "../shaders/FragmentShader.frag"); // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    std::vector<float> vertices = createMeshVertices(EDGE_COUNT,MAX_EDGE_WIDTH); // we have 3 coordinates per vertex and 3 color values
    std::vector<unsigned int> indices = createMeshIndices(EDGE_COUNT,MAX_EDGE_WIDTH); // we have 3 coordinates per vertex and 3 color values

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);


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

        // render the triangle
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    ourShader.ID = 0;
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

std::vector<float> createMeshVertices(int edgeCount, int maxEdgeWidth){
    std::vector<float> vertices(edgeCount * edgeCount * 6); // each vertex: 3 pos + 3 color
    
    for(int i=0; i<edgeCount; i++){
        for(int j=0; j<edgeCount; j++)
        {
            vertices[(i*edgeCount+j)*6+0] = -(maxEdgeWidth/2.0f) + j/(edgeCount-1.0f); 
            vertices[(i*edgeCount+j)*6+1] = -(maxEdgeWidth/2.0f) + i/(edgeCount-1.0f);
            vertices[(i*edgeCount+j)*6+2] = 0.0f;

            vertices[(i*edgeCount+j)*6+3] = (float)i/edgeCount;
            vertices[(i*edgeCount+j)*6+4] = (float)j/edgeCount;
            vertices[(i*edgeCount+j)*6+5] = 0.0f;
        }
    }

    return vertices;
}

std::vector<unsigned int> createMeshIndices(int edgeCount, int maxEdgeWidth){
    std::vector<unsigned int> indices((edgeCount-1)*(edgeCount-1)*6); // we have 6 indices per quad
    for(int i=0; i<edgeCount-1; i++){
        for(int j=0; j<edgeCount-1; j++)
        {
            indices[(i*(edgeCount-1)+j)*6+0] = i*edgeCount + j;         // top left
            indices[(i*(edgeCount-1)+j)*6+1] = i*edgeCount + (j+1);     // top right
            indices[(i*(edgeCount-1)+j)*6+2] = (i+1)*edgeCount + (j+1); // bottom right
            indices[(i*(edgeCount-1)+j)*6+3] = i*edgeCount + j;         // top left
            indices[(i*(edgeCount-1)+j)*6+4] = (i+1)*edgeCount + (j+1); // bottom right
            indices[(i*(edgeCount-1)+j)*6+5] = (i+1)*edgeCount + j;     // bottom left
        }
    }

    return indices;
}