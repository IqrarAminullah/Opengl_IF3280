#include "particles.hpp"

Particle::Particle(GLfloat dt, GLuint numVertices, GLuint vertexbuffer, GLuint uvbuffer, GLuint normalbuffer)
{
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    this->life = 0.0f;
    this->dt = dt;

    this->vertexbuffer = vertexbuffer;
    this->uvbuffer = uvbuffer;
    this->normalbuffer = normalbuffer;

    this->numVertices = numVertices;
}

void Particle::respawn(glm::vec3 position, glm::vec3 velocity)
{
    this->position = position;
    this->life = 1.0f;
    this->velocity = velocity;
}

bool Particle::isAlive()
{
    return this->life >= 0;
}

void Particle::update()
{
    this->life -= this->dt;
    if (this->isAlive())
    {	// particle is alive, thus update
        this->position -= this->velocity * dt;
        if (this->position.y <= 0) {
            this->position.y = -this->position.y;
            this->velocity.y = -this->velocity.y * 0.2f;
        }
    }
}

void Particle::draw(GLuint shaders)
{
    if (this->isAlive())
    {
        GLuint offsetID = glGetUniformLocation(shaders, "offset");
        GLuint scaleID = glGetUniformLocation(shaders, "scale");
        GLuint alphaID = glGetUniformLocation(shaders, "alpha");

        glUniform3f(offsetID, this->position.x, this->position.y, this->position.z);
        glUniform1f(scaleID, 0.01f);
        glUniform1f(alphaID, this->life);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
        glVertexAttribPointer(
            0,                  // attribute
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, this->uvbuffer);
        glVertexAttribPointer(
            1,                                // attribute
            2,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

        // 3rd attribute buffer : normals
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, this->normalbuffer);
        glVertexAttribPointer(
            2,                                // attribute
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

        // Draw the triangles !
        glDrawArrays(GL_TRIANGLES, 0, this->numVertices);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    }
}

ParticleGenerator::ParticleGenerator(GLuint nr_particles, GLfloat dt, const char * objpath)
{
    this->nr_particles = nr_particles;
    this->lastUsedParticle = 0;
    this->particles = new std::vector<Particle*>();

	// Read .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool res = loadOBJ(objpath, vertices, uvs, normals);

	// Load it into a VBO
	glGenBuffers(1, &this->vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	for (GLuint i = 0; i < nr_particles; ++i)
		this->particles->push_back(
            new Particle(
                dt,
                vertices.size(),
                this->vertexbuffer,
                this->uvbuffer,
                this->normalbuffer
            )
        );

    this->initialPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    this->randomPositionRadius = glm::vec3(0.0f, 0.0f, 0.0f);

    this->initialVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
    this->randomVelocityRadius = glm::vec3(0.0f, 0.0f, 0.0f);
}

int ParticleGenerator::firstUnusedParticle()
{
    // Search from last used particle, this will usually return almost instantly
    for (GLuint i = this->lastUsedParticle; i < this->nr_particles; ++i){
        if (!particles->at(i)->isAlive()){
            this->lastUsedParticle = i;
            return i;
        }
    }
    // Otherwise, do a linear search
    for (GLuint i = 0; i < this->lastUsedParticle; ++i){
        if (!this->particles->at(i)->isAlive()){
            this->lastUsedParticle = i;
            return i;
        }
    }
    // Override first particle if all others are alive
    this->lastUsedParticle = 0;
    return -1;
}

void ParticleGenerator::respawnParticle(Particle* particle)
{
    glm::vec3 randomPosition = glm::vec3(
        ((rand() % int(this->randomPositionRadius.x * 200)) / 100.0f - this->randomPositionRadius.x),
        ((rand() % int(this->randomPositionRadius.y * 200)) / 100.0f - this->randomPositionRadius.y),
        ((rand() % int(this->randomPositionRadius.z * 200)) / 100.0f - this->randomPositionRadius.z)
    );
    glm::vec3 randomVelocity = glm::vec3(
        ((rand() % int(this->randomVelocityRadius.x * 200)) / 100.0f - this->randomVelocityRadius.x),
        ((rand() % int(this->randomVelocityRadius.y * 200)) / 100.0f - this->randomVelocityRadius.y),
        ((rand() % int(this->randomVelocityRadius.z * 200)) / 100.0f - this->randomVelocityRadius.z)
    );

    glm::vec3 newPosition = randomPosition + this->initialPosition;
    glm::vec3 newVelocity = randomVelocity + this->initialVelocity;
    particle->respawn(newPosition, newVelocity);
}

void ParticleGenerator::update()
{
    // Uupdate all particles
    for (GLuint i = 0; i < this->nr_particles; ++i)
    {
        Particle* p = particles->at(i);
        p->update();
    }

    GLuint nr_new_particles = 2;
    // Add new particles
    for (GLuint i = 0; i < nr_new_particles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        if (unusedParticle != -1) {
            this->respawnParticle(this->particles->at(unusedParticle));
        }
    }
}

void ParticleGenerator::draw(GLuint shaders)
{
    for (Particle* particle : *(this->particles))
    {
        particle->draw(shaders);
    }
}

void ParticleGenerator::setInitialPosition(glm::vec3 position, glm::vec3 randomPositionRadius)
{
    this->initialPosition = position;
    this->randomPositionRadius = randomPositionRadius;
}

void ParticleGenerator::setInitialVelocity(glm::vec3 velocity, glm::vec3 randomVelocityRadius)
{
    this->initialVelocity = velocity;
    this->randomVelocityRadius = randomVelocityRadius;
}
