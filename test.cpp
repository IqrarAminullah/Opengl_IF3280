#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.hpp"

//g++ -o main.exe test.cpp glad.c -lglfw3 -lopengl32 -lgdi32


const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 720;


glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
float deltaTime = 0.0f;
float lastFrame  = 0.0f;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        cameraPos += cameraUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        cameraPos -= cameraUp * cameraSpeed;

}


int main()
{

    GLFWwindow* window;
    GLuint VAO, VBO;


    float stripVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, -0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, -0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, -0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, -0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, -1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, -1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, -1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, -0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, -0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, -0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, -0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, -1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, -0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, -0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, -1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, -0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, -0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, -0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, -1.0f
    };

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Grafkom", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    //glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);

    // Bind VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //Bind VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(stripVertices), stripVertices, GL_STATIC_DRAW);


    Shader ourShader("shaders/vertexShader.vs", "shaders/fragmentShader.fs");

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //// Specify the location of the vertex data
//    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
//    //    8*sizeof(float), 0);
//    //glEnableVertexAttribArray(0);
//
//    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
//    // 8*sizeof(float), (void*) (3*sizeof(float)));
//    //glEnableVertexAttribArray(1);
//
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
    // 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);

     unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load("textures/koi.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);

    while (!glfwWindowShouldClose(window))
    {

        // Clear the screen to black
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setMat4("view", view);

         // create transformations
        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 projection    = glm::mat4(1.0f);
        //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", projection);

        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        ourShader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        // Draw a triangle from the 3 vertices
        //int size = (sizeof(stripVertices)/sizeof(float))/3;
        //glDrawArrays(GL_TRIANGLES, 0, size);

        // Swap buffer
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);

    glDeleteVertexArrays(1, &VAO);

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}