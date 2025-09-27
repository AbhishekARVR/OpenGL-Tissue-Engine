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
    std::vector<glm::vec3> estimatedPositions;
    std::vector<glm::vec3> velocities;
    std::vector<glm::vec3> colors;
    std::vector<int> isVerticesFixed;

    private:
    int edgeCount;
    int maxEdgeWidth;
    float weight;    
    float dampingFactor;
    std::vector<glm::vec4> stretchConstraints;
    std::vector<float> stretchConstraintsRestLength;

    public:
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Mesh(int edgeCount, int maxEdgeWidth, const char* vertexPath, const char* fragmentPath, float mass=0.001f)
        : shader(vertexPath, fragmentPath), edgeCount(edgeCount), maxEdgeWidth(maxEdgeWidth), weight(1.0f/mass)
    {
        dampingFactor = 0.75f;


        positions = createPositions(edgeCount,maxEdgeWidth);
        colors = createColors(edgeCount); // we have 3 coordinates per vertex and 3 color values
        
        for (int i = 0; i < edgeCount*edgeCount; i++)
        {
            velocities.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        }
        

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
    
        createStretchConstraints();
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

    void updateEstimatedPositions(float deltaTime)
    {
        if(estimatedPositions.size() == 0) 
        {
            for(int i=0; i<positions.size(); i++){
                if(isVerticesFixed[i] == 0) estimatedPositions.push_back(positions[i] + velocities[i] * deltaTime);
                else estimatedPositions.push_back(positions[i]);
            }
        } 
        else 
        {
            for(int i=0; i<positions.size(); i++){
                if(isVerticesFixed[i] == 0) estimatedPositions[i] = positions[i] + velocities[i] * deltaTime;
                else estimatedPositions[i] = positions[i];
            }
        }
    }

    void updateVelocitiesAndPositions(float deltaTime)
    {
        for(int i=0; i<velocities.size(); i++){
            if(isVerticesFixed[i] == 0) 
            {
                velocities[i] = (estimatedPositions[i] - positions[i]) / deltaTime;
                positions[i] = estimatedPositions[i];
            }
            else velocities[i] = glm::vec3(0.0f);   
        }
    }

    void updatePositions()
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
        glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(glm::vec3), positions.data());
    }

    void addGravity(float deltaTime, glm::vec3 gravity)
    {
        for(int i=0; i<velocities.size(); i++){
            if(isVerticesFixed[i] == 0) velocities[i] += gravity * deltaTime;
        }
    }

    void applyDamping(float dampingFactor)
    {
        glm::vec3 centerOfMassPosition = glm::vec3(0.0f);
        glm::vec3 centerOfMassVelocity = glm::vec3(0.0f);
        
        float totalMass = 1/weight * edgeCount * edgeCount;
        float mass = 1/weight;

        for(int i=0; i<velocities.size(); i++)
        {
            if(isVerticesFixed[i] == 1) continue;

            centerOfMassPosition += positions[i] * mass;
            centerOfMassVelocity += velocities[i] * mass;
    
        }
  
        centerOfMassPosition /= totalMass;
        centerOfMassVelocity /= totalMass;
        
        std::vector<glm::vec3> relativePositions(velocities.size(), glm::vec3(0.0f));
        glm::vec3 angularMomentum = glm::vec3(0.0f);
        glm::mat3 inertiaTensor = glm::mat3(0.0f);

        for(int i=0; i<velocities.size(); i++)
        {
            if(isVerticesFixed[i] == 1) continue;

            relativePositions[i] = positions[i] - centerOfMassPosition;
            glm::vec3 relativeVelocity = velocities[i] - centerOfMassVelocity;

            angularMomentum += glm::cross(relativePositions[i], relativeVelocity * mass);

            glm::mat3 ri_tilde = skewSymmetric(relativePositions[i]);
            inertiaTensor += mass * (ri_tilde * glm::transpose(ri_tilde));
        }

        glm::mat3 inertiaTensorInv = glm::inverse(inertiaTensor);
        glm::vec3 angularVelocity = inertiaTensorInv * angularMomentum;

        for(int i=0; i<velocities.size(); i++)
        {
            if(isVerticesFixed[i] == 1) continue;
            glm::vec3 deltaVelocity = centerOfMassVelocity + glm::cross(angularVelocity, relativePositions[i]) - velocities[i];

            velocities[i] -= deltaVelocity * dampingFactor;
        }
    }

    void SolveAllStretchConstraints()
    {
        for(int i=0; i<stretchConstraints.size(); i++)
        {
            SolveStretchConstraint(stretchConstraints[i], stretchConstraintsRestLength[i]);
        }
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

                if(i == edgeCount - 1 && (j == 0 || j == edgeCount - 1)){
                    isVerticesFixed.push_back(1);
                } else {
                    isVerticesFixed.push_back(0);
                }
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

    // Create the skew-symmetric cross-product matrix of vector r
    glm::mat3 skewSymmetric(const glm::vec3& r) {
        return glm::mat3(
            0,      -r.z,    r.y,
            r.z,    0,      -r.x,
        -r.y,    r.x,    0
        );
    }

    void createStretchConstraints()
    {
        for(int i=0; i<edgeCount; i++){
            for(int j=0; j<edgeCount; j++)
            {
                glm::vec4 stretchConstrainstIndex;
                if(j < edgeCount - 1 && i < edgeCount - 1)
                {
                    stretchConstrainstIndex = glm::vec4(i , j, i, j+1);
                    stretchConstraints.push_back(stretchConstrainstIndex);
                    stretchConstraintsRestLength.push_back(glm::length(positions[i*edgeCount + j] - positions[i*edgeCount + (j+1)]));
                    stretchConstrainstIndex = glm::vec4(i , j, i+1, j);
                    stretchConstraints.push_back(stretchConstrainstIndex);
                    stretchConstraintsRestLength.push_back(glm::length(positions[i*edgeCount + j] - positions[(i+1)*edgeCount + j]));
                    stretchConstrainstIndex = glm::vec4(i , j, i+1, j+1);
                    stretchConstraints.push_back(stretchConstrainstIndex);
                    stretchConstraintsRestLength.push_back(glm::length(positions[i*edgeCount + j] - positions[(i+1)*edgeCount + (j+1)]));
                    if(j > 0)
                    {
                        stretchConstrainstIndex = glm::vec4(i , j, i+1, j-1);
                        stretchConstraints.push_back(stretchConstrainstIndex);
                        stretchConstraintsRestLength.push_back(glm::length(positions[i*edgeCount + j] - positions[(i+1)*edgeCount + (j-1)]));
                    }
                }
                else if(j == edgeCount - 1 && i < edgeCount - 1)
                {
                    stretchConstrainstIndex = glm::vec4(i , j, i+1, j);
                    stretchConstraints.push_back(stretchConstrainstIndex);
                    stretchConstraintsRestLength.push_back(glm::length(positions[i*edgeCount + j] - positions[(i+1)*edgeCount + j]));
                
                    stretchConstrainstIndex = glm::vec4(i , j, i+1, j-1);
                    stretchConstraints.push_back(stretchConstrainstIndex);
                    stretchConstraintsRestLength.push_back(glm::length(positions[i*edgeCount + j] - positions[(i+1)*edgeCount + (j-1)]));
                }
                else if(j < edgeCount - 1 && i == edgeCount - 1)
                {
                    stretchConstrainstIndex = glm::vec4(i , j, i, j+1);
                    stretchConstraints.push_back(stretchConstrainstIndex);
                    stretchConstraintsRestLength.push_back(glm::length(positions[i*edgeCount + j] - positions[i*edgeCount + (j+1)]));
                }
            }
        }
    }

    void SolveStretchConstraint(const glm::vec4& constraint, float restLength)
    {
        int i1 = constraint.x * edgeCount + constraint.y;
        int i2 = constraint.z * edgeCount + constraint.w;

        glm::vec3 p1 = estimatedPositions[i1];
        glm::vec3 p2 = estimatedPositions[i2];

        float currentLength = glm::length(p1 - p2);
        if(currentLength == 0.0f) return;

        glm::vec3 deltaPNormalized = (p1 - p2)/currentLength; 
        float deltaLength = currentLength - restLength;

        float w1 = (isVerticesFixed[i1] == 1) ? 0.0f : weight;
        float w2 = (isVerticesFixed[i2] == 1) ? 0.0f : weight;
        float wSum = w1 + w2;
        if(wSum == 0.0f) return;        

        if(isVerticesFixed[i1] == 0) estimatedPositions[i1] -= (w1/wSum) * deltaLength * deltaPNormalized * 0.25f;// * 0.01 is STIFFNESS;
        if(isVerticesFixed[i2] == 0) estimatedPositions[i2] += (w2/wSum) * deltaLength * deltaPNormalized * 0.25f;// * STIFFNESS;
    }
};
    
#endif