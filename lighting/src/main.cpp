#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <camera.h>

#define WIDTH 800
#define HEIGHT 600

void frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "lighting", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "ERROR::Failed to init window" << std::endl;
        return -1;
    }

    // glfw configs
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwMakeContextCurrent(window);

    if (gladLoadGL(glfwGetProcAddress) == 0)
    {
        std::cout << "Failed to initalize glad" << std::endl;
        glfwTerminate();
    }

    // creating vertices
    float vertices[] = {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f};

    // creating buffer
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // creating vertex array
    unsigned int vao;
    glGenVertexArrays(1, &vao);

    // creating binding for vertex array
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);

    // creating shader
    Shader objectShader = Shader("shaders/vertex_shader.vs", "shaders/fragment_shader.fs");

    // creating camera

    // defining object parameters
    glm::vec3 objectPosition = glm::vec3(0.0f);

    Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f), objectPosition);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        objectShader.use();

        // defining transformation matrices
        glm::mat4 model = glm::mat4(1.0f);

        // setting transformation matrices
        glm::mat4 projection = glm::perspective(45.0f, (WIDTH * 1.0f / HEIGHT), 0.1f, 100.0f);
        glm::mat4 view = camera.getMatrix();
        model = glm::translate(model, objectPosition);
        model = glm::scale(model, glm::vec3(0.8));

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        processInput(window);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
