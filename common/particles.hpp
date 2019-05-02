#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <stdlib.h>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>
#include "objloader.hpp"

class Particle {
    public:
        Particle(GLfloat dt, GLuint numVertices, GLuint vertexbuffer, GLuint uvbuffer, GLuint normalbuffer);
        void respawn(glm::vec3 position, glm::vec3 velocity);
        bool isAlive();
        void update();
        void draw(GLuint shaders);
        void scale(GLfloat scaleFactor);
        void useTexture(GLuint textureID);

    private:
        glm::vec3 position;
        glm::vec3 velocity;
        GLfloat life;
        GLfloat dt;
        GLfloat scaleFactor;

        GLuint vertexbuffer;
        GLuint uvbuffer;
        GLuint normalbuffer;

        GLuint numVertices;
        GLuint textureID;
};

class ParticleGenerator {
    public:
        ParticleGenerator(GLuint nr_particles, GLfloat dt, const char * objpath);
        int firstUnusedParticle();
        void respawnParticle(Particle* particle);
        void update();
        void draw(GLuint shaders);
        void setInitialPosition(glm::vec3 position, glm::vec3 randomPositionRadius);
        void setInitialVelocity(glm::vec3 velocity, glm::vec3 randomVelocityRadius);
        void useTexture(GLuint textureID);
        void scale(GLfloat scaleFactor);

    private:
        GLuint nr_particles;
        GLuint lastUsedParticle;
        std::vector<Particle*>* particles;

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;

        GLuint vertexbuffer;
        GLuint uvbuffer;
        GLuint normalbuffer;

        glm::vec3 initialPosition;
        glm::vec3 randomPositionRadius;

        glm::vec3 initialVelocity;
        glm::vec3 randomVelocityRadius;
};

#endif