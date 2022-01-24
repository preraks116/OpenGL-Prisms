#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "camera.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

// Globals
glm::mat4 model, view, projection;
glm::vec3 objectcoord;
float angleR = 0.0f;
bool boolT = false;
bool boolR = false;

glm::vec3 x = glm::vec3(1, 0, 0);
glm::vec3 y = glm::vec3(0, 1, 0);
glm::vec3 z = glm::vec3(0, 0, 1);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(int argc, char **argv)
{
    int n = argv[1] ? atoi(argv[1]) : 3;
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

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("../src/vertex.shader", "../src/fragment.shader");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float vertices[n*4][15];
    for (int i = 0; i < n; i++) 
    {
        // first vertex is always origin
        for(int j = 0; j < 4; j++)
        {
            vertices[i][j] = 0.0f;
        }
        vertices[i][2] = 1.0f;
        vertices[i][4] = 1.0f;
        // second vertex of the triangle
        vertices[i][5] = 0.5 * cos(((i+1) % n) * 2 * M_PI / n);
        vertices[i][6] = 0.5 * sin(((i+1) % n) * 2 * M_PI / n);
        vertices[i][7] = 1.0f;
        vertices[i][8] = 0.0f;
        vertices[i][9] = 1.0f;

        // third vertex of the triangle
        vertices[i][10] = 0.5 * cos(((i+2) % n) * 2 * M_PI / n);
        vertices[i][11] = 0.5 * sin(((i+2) % n) * 2 * M_PI / n);
        vertices[i][12] = 1.0f;
        vertices[i][13] = 0.0f;
        vertices[i][14] = 1.0f;
    }
    for(int i = n; i < 2*n; i++)
    {
        // copying evertrhing from the first face
        for(int j = 0; j < 15; j++)
        {
            vertices[i][j] = vertices[i-n][j];
        }

        // pushing it back
        vertices[i][2] = -1.0f;
        vertices[i][7] = -1.0f;
        vertices[i][12] = -1.0f;

        // changing the color
        vertices[i][3] = 0.7f;
        vertices[i][8] = 0.7f;
        vertices[i][13] = 0.7f;
    }
    // making sure the colors in both loops is same so that each face has only 1 color
    for(int i = 2*n; i < 3*n; i++)
    {
        // 2 vertices from the first face 
        for(int j = 0; j < 3; j++)
        {
            vertices[i][j] = vertices[i-2*n][j+5];
        }
        //color
        vertices[i][3] = (float)1.0/(i - 2*n + 1);
        vertices[i][4] = (float)1.0/(i - 2*n + 1);

        for(int j = 5; j < 8; j++)
        {
            vertices[i][j] = vertices[i-2*n][j+5];
        }
        //color
        vertices[i][8] = (float)1.0/(i - 2*n + 1);
        vertices[i][9] = (float)1.0/(i - 2*n + 1);

        // one vertice from the second face
        for(int j = 10; j < 13; j++)
        {
            vertices[i][j] = vertices[i-n][j-5];
        }
        //color
        vertices[i][13] = (float)1.0/(i - 2*n + 1);
        vertices[i][14] = (float)1.0/(i - 2*n + 1);
    }
    for(int i = 3*n; i < 4*n; i++)
    {
        // 2 vertices from the second face
        for(int j = 0; j < 3; j++)
        {
            vertices[i][j] = vertices[i-2*n][j+5];
        }
        //color
        vertices[i][3] = (float)1.0/(i - 3*n + 1);
        vertices[i][4] = (float)1.0/(i - 3*n + 1);

        for(int j = 5; j < 8; j++)
        {
            vertices[i][j] = vertices[i-2*n][j+5];
        }
        //color
        vertices[i][8] = (float)1.0/(i - 3*n + 1);
        vertices[i][9] = (float)1.0/(i - 3*n + 1);

        // 1 vertice from the first face
        for(int j = 10; j < 13; j++)
        {
            vertices[i][j] = vertices[i-3*n][j];
        }
        //color
        vertices[i][13] = (float)1.0/(i - 3*n + 1);
        vertices[i][14] = (float)1.0/(i - 3*n + 1);
    }

    view = camera.GetViewMatrix();
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();

    glfwSetKeyCallback(window, key_callback);

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        ourShader.use();

        model = glm::mat4(1.0f);
        model = glm::translate(model, objectcoord);

        if(boolR){angleR++;}
        model = glm::rotate(model, glm::radians(angleR), x);
        // Perspective and view

        projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
        // projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);

        ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        // render box
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 12*n);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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

    // Flying camera
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        view=glm::lookAt(camera.Position,objectcoord,y);
        camera.ProcessKeyboard(FORWARD, 0.05f);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        view=glm::lookAt(camera.Position,objectcoord,y);
        camera.ProcessKeyboard(BACKWARD, 0.05f);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        view=glm::lookAt(camera.Position,objectcoord,y);
        camera.ProcessKeyboard(LEFT, 0.05f);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        view=glm::lookAt(camera.Position,objectcoord,y);
        camera.ProcessKeyboard(RIGHT, 0.05f);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        view=glm::lookAt(camera.Position,objectcoord,y);
        camera.ProcessKeyboard(UP, 0.05f);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        view=glm::lookAt(camera.Position,objectcoord,y);
        camera.ProcessKeyboard(DOWN, 0.05f);
    }

    // Prism Fashion Show
    if (glfwGetKey(window,GLFW_KEY_1) == GLFW_PRESS)
    {
        camera.Position = glm::vec3(0, 2, 2);
        view=glm::lookAt(camera.Position,objectcoord,y);
    }
    if (glfwGetKey(window,GLFW_KEY_2) == GLFW_PRESS)
    {
        camera.Position = glm::vec3(4, 2, -2);
        view=glm::lookAt(camera.Position,objectcoord,y);
    }

    // Object Translation
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        objectcoord += x * 0.1f;
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        objectcoord -= x * 0.1f;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        objectcoord -= y * 0.1f;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        objectcoord += y * 0.1f;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        objectcoord += z * 0.1f;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        objectcoord -= z * 0.1f;

    // Turntables
    if(boolT)
    {
        view=glm::lookAt(camera.Position,objectcoord,y);
        glm::mat4 rotatemat = glm::mat3(1.0f);
        rotatemat = glm::rotate(rotatemat, glm::radians(1.0f), glm::vec3(0, 1, 0));
        camera.Position = glm::vec3(rotatemat * glm::vec4(camera.Position,1.0f));
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
        boolT = !boolT;
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        boolR = !boolR;
}