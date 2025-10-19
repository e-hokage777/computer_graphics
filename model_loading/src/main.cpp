#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <texture.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cube.h>
#include <camera.h>
#include <light.h>


#define WIDTH 800
#define HEIGHT 600

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininiess;
};

Camera camera;
float deltaTime = 0;


void frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, (int)(((float)HEIGHT / WIDTH) * width)); // maintaining aspect
}

void processInputs(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.move(CameraMovement::Forward, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.move(CameraMovement::Backward, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.move(CameraMovement::Left, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.move(CameraMovement::Right, deltaTime);
    }
}

float lastX = 0;
float lastY = 0;
bool firstMove = true;
void cursorPosCallback(GLFWwindow *window, double xPos, double yPos)
{
    if (firstMove)
    {
        lastX = xPos;
        lastY = yPos;
        firstMove = false;
    }

    float dx = xPos - lastX;
    float dy = lastY - yPos;

    camera.rotate(dx, dy, deltaTime);

    lastX = xPos;
    lastY = yPos;
}

void messageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar *message,
    const void *userParam)
{
    std::cout << "OpenGL Error: " << message << std::endl;
}

int main()
{
    glfwInit();

    // glfw configurations
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

    glfwSwapInterval(1);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Model Loading", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to load window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (gladLoadGL() == 0) // check this part well
    {
        std::cout << "Failed to load glad" << std::endl;
    }

    // gl configs
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(messageCallback, 0);
    glEnable(GL_DEPTH_TEST);
    // vertices, buffers and vertex arrays
    // float vertices[] = CUBE_VERTICES;

    unsigned int vao, vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE_VERTICES), CUBE_VERTICES, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    // creating shader
    Shader shader = Shader("shaders/model_loading.vs", "shaders/model_loading.fs");
    Shader lightShader = Shader("shaders/model_loading_light.vs", "shaders/model_loading_light.fs");
    shader.use();

    // textures
    Texture texture1 = Texture("assets/container2.png");

    // creating a material
    Material material;
    material.ambient = glm::vec3(1.0f);
    material.diffuse = glm::vec3(0.5f);
    material.specular = glm::vec3(1.0f);
    material.shininiess = 5.0f;

    shader.setVec3("material.ambient", glm::value_ptr(material.ambient));
    shader.setVec3("material.diffuse", glm::value_ptr(material.diffuse));
    shader.setVec3("material.specular", glm::value_ptr(material.specular));
    shader.setFloat("material.shininess", material.shininiess);

    // light
    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, -3.0f);
    Light light = Light(lightShader, lightPos, glm::vec3(1.0f, 1.0f, 1.0f));

    // setting light pos
    shader.setVec3("lightPos", glm::value_ptr(lightPos));

    float lastTime = 0;
    while (!glfwWindowShouldClose(window))
    {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        // defining matrices
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.7f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
        model = glm::rotate(model, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        // the view matrix
        glm::mat4 view = camera.getMatrix();

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), WIDTH / HEIGHT * 1.0f, 0.1f, 100.0f);

        // setting matrices
        shader.setMat4("model", glm::value_ptr(model));
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // drawing light
        light.draw(view, projection);

        glfwSwapBuffers(window);
        glfwPollEvents();
        processInputs(window);
    }

    glfwTerminate();
    return 0;
}