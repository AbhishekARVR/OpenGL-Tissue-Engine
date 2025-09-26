#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
/*  
    Mesh
        Vertices
            pos
            est pos
            weight
            velocity
            color
            UPDATE VERTICES VELOCITY
            UPDATE VERTICES POSITIONS
        Indices
*/
class Mesh
{
    public: 
    unsigned int VBO, VAO, EBO;
    Shader shader;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Mesh(int edgeCount, int maxEdgeWidth, const char* vertexPath, const char* fragmentPath)
        : shader(vertexPath, fragmentPath)
    {
        std::vector<float> vertices = createMeshVertices(edgeCount,maxEdgeWidth); // we have 3 coordinates per vertex and 3 color values
        std::vector<unsigned int> indices = createMeshIndices(edgeCount,maxEdgeWidth); // we have 3 coordinates per vertex and 3 color values

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

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
    }

    void unbind(){
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void bind(){
        glBindVertexArray(VAO);
    }

    void deleteArraysAndBuffers()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        shader.ID = 0;
    }

    void draw(){
        shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        std::cout << "Drawing mesh " << VAO << std::endl;
    }

    // utility function for creating mesh vertices and indices.
    // ------------------------------------------------------------------------

    private:
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

    std::vector<unsigned int> createMeshIndices(int edgeCount, int maxEdgeWidth)
    {
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
};
    
#endif