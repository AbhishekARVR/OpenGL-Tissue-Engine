#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include "shader.h"
#include <glm/glm.hpp>

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
    unsigned int VBO_positions, VBO_colors, VAO, EBO;
    Shader shader;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> colors;

    private:
    int edgeCount;
    int maxEdgeWidth;
    float weight;    

    public:
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Mesh(int edgeCount, int maxEdgeWidth, const char* vertexPath, const char* fragmentPath, float mass=1.0f)
        : shader(vertexPath, fragmentPath), edgeCount(edgeCount), maxEdgeWidth(maxEdgeWidth), weight(1.0f/mass)
    {
        positions = createPositions(edgeCount,maxEdgeWidth); // we have 3 coordinates per vertex and 3 color values
        colors = createColors(edgeCount); // we have 3 coordinates per vertex and 3 color values
        
        std::vector<unsigned int> indices = createMeshIndices(edgeCount,maxEdgeWidth);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &EBO);

        glGenBuffers(1, &VBO_positions);
        glGenBuffers(1, &VBO_colors);

        glBindVertexArray(VAO);

        // positions VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);

        // colors VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO_colors);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(1);


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);
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
        glDeleteBuffers(1, &VBO_positions);
        glDeleteBuffers(1, &VBO_colors);
        glDeleteBuffers(1, &EBO);
        shader.ID = 0;
    }

    void draw(){
        shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, (edgeCount - 1) * (edgeCount - 1) * 6, GL_UNSIGNED_INT, 0);
    }

    void updateVertices(std::vector<float> &vertices){
        // glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
    }

    // utility function for creating mesh vertices and indices.
    // ------------------------------------------------------------------------

    private:
    std::vector<glm::vec3> createPositions(int edgeCount, int maxEdgeWidth) 
    {
        std::vector<glm::vec3> pos(edgeCount * edgeCount);
        for(int i=0; i < edgeCount; ++i){
            for(int j=0; j < edgeCount; ++j){
                pos[i*edgeCount + j] = glm::vec3(
                    -(maxEdgeWidth/2.0f) + j*(maxEdgeWidth/(edgeCount-1.0f)),
                    -(maxEdgeWidth/2.0f) + i*(maxEdgeWidth/(edgeCount-1.0f)),
                    0.0f
                );
            }
        }
        return pos;
    }

    std::vector<glm::vec3> createColors(int edgeCount) 
    {
        std::vector<glm::vec3> cols(edgeCount * edgeCount);
        for(int i=0; i < edgeCount; ++i){
            for(int j=0; j < edgeCount; ++j){
                cols[i*edgeCount + j] = glm::vec3(
                    (float)i / edgeCount,
                    (float)j / edgeCount,
                    0.0f
                );
            }
        }
        return cols;
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