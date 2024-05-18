#include <glad/glad.h>
#include <../GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <filesystem>


std::string readFile(std::filesystem::path path)
{
    std::ifstream in(path, std::ios::binary);
    const auto size = std::filesystem::file_size(path);
    std::string result(size, '\0');
    in.read(result.data(), size);

    return result;
}

unsigned int compileShader(unsigned int shaderType, std::string& src)
{
    unsigned id = glCreateShader(shaderType);
    const char* raw = src.c_str();

    glShaderSource(id, 1, &raw, nullptr);
    glCompileShader(id);
    return id;
}

unsigned int createShader(std::string& vertex, std::string& fragment)
{
    unsigned vs = compileShader(GL_VERTEX_SHADER, vertex);
    unsigned fs = compileShader(GL_FRAGMENT_SHADER, fragment);

    unsigned program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //INIT GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init OPENGL context" << std::endl;
        return -1;
    }

    float vertices[] = {
        -1.0, -1.0,
         1.0, -1.0,
         1.0, 1.0,
    };
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    glEnableVertexAttribArray(0);

    std::string vertex = readFile("vertex.shader");
    std::string fragment = readFile("fragment.shader");

    unsigned int shader = createShader(vertex, fragment);
    glUseProgram(shader);


    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}