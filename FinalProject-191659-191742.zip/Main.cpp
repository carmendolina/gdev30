// Quick note: GLAD needs to be included first before GLFW.
// Otherwise, GLAD will complain about gl.h being already included.
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// ---------------
// Function declarations
// ---------------

/// <summary>
/// Creates a shader program based on the provided file paths for the vertex and fragment shaders.
/// </summary>
/// <param name="vertexShaderFilePath">Vertex shader file path</param>
/// <param name="fragmentShaderFilePath">Fragment shader file path</param>
/// <returns>OpenGL handle to the created shader program</returns>
GLuint CreateShaderProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

/// <summary>
/// Creates a shader based on the provided shader type and the path to the file containing the shader source.
/// </summary>
/// <param name="shaderType">Shader type</param>
/// <param name="shaderFilePath">Path to the file containing the shader source</param>
/// <returns>OpenGL handle to the created shader</returns>
GLuint CreateShaderFromFile(const GLuint& shaderType, const std::string& shaderFilePath);

/// <summary>
/// Creates a shader based on the provided shader type and the string containing the shader source.
/// </summary>
/// <param name="shaderType">Shader type</param>
/// <param name="shaderSource">Shader source string</param>
/// <returns>OpenGL handle to the created shader</returns>
GLuint CreateShaderFromSource(const GLuint& shaderType, const std::string& shaderSource);

/// <summary>
/// Function for handling the event when the size of the framebuffer changed.
/// </summary>
/// <param name="window">Reference to the window</param>
/// <param name="width">New width</param>
/// <param name="height">New height</param>
void FramebufferSizeChangedCallback(GLFWwindow* window, int width, int height);

/// <summary>
/// Struct containing data about a vertex
/// </summary>
struct Vertex
{
    GLfloat x, y, z;    // Position
    GLubyte r, g, b;    // Color
    GLfloat u, v;        // UV coordinates
    GLfloat nx, ny, nz; // normal vector
};

//float rotvar = sin(glfwGetTime());
//float increment = 0.0f;
int current = 0;
//
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
//    {
//        if (current == 0) {
//            current = 1;
//        } else if (current == 1) {
//            current = 0;
//        }
//    }
//
//    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
//    {
//        if (increment != 1.0f) {
//            increment += 0.2f;
//        }
//    }
//
//    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
//    {
//        if (increment != 0.0f) {
//            increment -= 0.2f;
//        }
//    }
//}



/// <summary>
/// Main function.
/// </summary>
/// <returns>An integer indicating whether the program ended successfully or not.
/// A value of 0 indicates the program ended succesfully, while a non-zero value indicates
/// something wrong happened during execution.</returns>
int main()
{
    // Initialize GLFW
    int glfwInitStatus = glfwInit();
    if (glfwInitStatus == GLFW_FALSE)
    {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return 1;
    }

    // Tell GLFW that we prefer to use OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Tell GLFW that we prefer to use the modern OpenGL
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Tell GLFW to create a window
    int windowWidth = 800;
    int windowHeight = 800;
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Lighting", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Tell GLFW to use the OpenGL context that was assigned to the window that we just created
    glfwMakeContextCurrent(window);

    // Register the callback function that handles when the framebuffer size has changed
    glfwSetFramebufferSizeCallback(window, FramebufferSizeChangedCallback);
    
    // Register the callback function that handles keyboard input
//    glfwSetKeyCallback(window, key_callback);

    // Tell GLAD to load the OpenGL function pointers
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return 1;
    }

    // --- Vertex specification ---
    
    // Set up the data for each vertex of the triangle
    Vertex vertices[36];
    
    vertices[0].x = -0.5f;    vertices[0].y = 0.5f;    vertices[0].z = -0.5f;
    vertices[0].r = 255;        vertices[0].g = 255;        vertices[0].b = 255;
    vertices[0].u = 0.25f;    vertices[0].v = 0.666f;
    vertices[0].nx = 0.0f;       vertices[0].ny = 0.0f;       vertices[0].nz = 1.0f;

    vertices[1].x = -0.5f;    vertices[1].y = -0.5f;    vertices[1].z = -0.5f;
    vertices[1].r = 255;    vertices[1].g = 255;        vertices[1].b = 255;
    vertices[1].u = 0.25f;    vertices[1].v = 0.333f;
    vertices[1].nx = 0.0f;       vertices[1].ny = 0.0f;       vertices[1].nz = 1.0f;

    vertices[2].x = 0.5f;    vertices[2].y = -0.5f;    vertices[2].z = -0.5f;
    vertices[2].r = 255;        vertices[2].g = 255;        vertices[2].b = 255;
    vertices[2].u = 0.5f;    vertices[2].v = 0.333f;
    vertices[2].nx = 0.0f;       vertices[2].ny = 0.0f;       vertices[2].nz = 1.0f;
    
    vertices[3].x = -0.5f;    vertices[3].y = 0.5f;    vertices[3].z = -0.5f;
    vertices[3].r = 255;        vertices[3].g = 255;        vertices[3].b = 255;
    vertices[3].u = 0.25f;    vertices[3].v = 0.666f;
    vertices[3].nx = 0.0f;       vertices[3].ny = 0.0f;       vertices[3].nz = 1.0f;
    
    vertices[4].x = 0.5f;    vertices[4].y = -0.5f;    vertices[4].z = -0.5f;
    vertices[4].r = 255;        vertices[4].g = 255;        vertices[4].b = 255;
    vertices[4].u = 0.5f;    vertices[4].v = 0.333f;
    vertices[4].nx = 0.0f;       vertices[4].ny = 0.0f;       vertices[4].nz = 1.0f;
    
    vertices[5].x = 0.5f;    vertices[5].y = 0.5f;    vertices[5].z = -0.5f;
    vertices[5].r = 255;        vertices[5].g = 255;        vertices[5].b = 255;
    vertices[5].u = 0.5;    vertices[5].v = 0.666f;
    vertices[5].nx = 0.0f;       vertices[5].ny = 0.0f;       vertices[5].nz = 1.0f;
    
    vertices[6].x = -0.5f;    vertices[6].y = 0.5f;    vertices[6].z = 0.5f;
    vertices[6].r = 255;        vertices[6].g = 255;        vertices[6].b = 255;
    vertices[6].u = 0.75f;    vertices[6].v = 0.666f;
    vertices[6].nx = 0.0f;       vertices[6].ny = 0.0f;       vertices[6].nz = 1.0f;

    vertices[7].x = -0.5f;    vertices[7].y = -0.5f;    vertices[7].z = 0.5f;
    vertices[7].r = 255;    vertices[7].g = 255;        vertices[7].b = 255;
    vertices[7].u = 0.75f;    vertices[7].v = 0.333f;
    vertices[7].nx = 0.0f;       vertices[7].ny = 0.0f;       vertices[7].nz = 1.0f;

    vertices[8].x = 0.5f;    vertices[8].y = -0.5f;    vertices[8].z = 0.5f;
    vertices[8].r = 255;        vertices[8].g = 255;        vertices[8].b = 255;
    vertices[8].u = 1.0f;    vertices[8].v = 0.333f;
    vertices[8].nx = 0.0f;       vertices[8].ny = 0.0f;       vertices[8].nz = 1.0f;

    vertices[9].x = -0.5f;    vertices[9].y = 0.5f;    vertices[9].z = 0.5f;
    vertices[9].r = 255;        vertices[9].g = 255;        vertices[9].b = 255;
    vertices[9].u = 0.75f;    vertices[9].v = 0.666f;
    vertices[9].nx = 0.0f;       vertices[9].ny = 0.0f;       vertices[9].nz = 1.0f;

    vertices[10].x = 0.5f;    vertices[10].y = -0.5f;    vertices[10].z = 0.5f;
    vertices[10].r = 255;        vertices[10].g = 255;        vertices[10].b = 255;
    vertices[10].u = 1.0f;    vertices[10].v = 0.333f;
    vertices[10].nx = 0.0f;       vertices[10].ny = 0.0f;       vertices[10].nz = 1.0f;

    vertices[11].x = 0.5f;    vertices[11].y = 0.5f;    vertices[11].z = 0.5f;
    vertices[11].r = 255;        vertices[11].g = 255;        vertices[11].b = 255;
    vertices[11].u = 1.0f;    vertices[11].v = 0.666f;
    vertices[11].nx = 0.0f;       vertices[11].ny = 0.0f;       vertices[11].nz = 1.0f;
    
    vertices[12].x = 0.5f;    vertices[12].y = 0.5f;    vertices[12].z = -0.5f;
    vertices[12].r = 255;        vertices[12].g = 255;        vertices[12].b = 255;
    vertices[12].u = 0.5f;    vertices[12].v = 0.666f;
    vertices[12].nx = -1.0f;       vertices[12].ny = 0.0f;       vertices[12].nz = 0.0f;

    vertices[13].x = 0.5f;    vertices[13].y = -0.5f;    vertices[13].z = -0.5f;
    vertices[13].r = 255;        vertices[13].g = 255;        vertices[13].b = 255;
    vertices[13].u = 0.5f;    vertices[13].v = 0.333f;
    vertices[13].nx = -1.0f;       vertices[13].ny = 0.0f;       vertices[13].nz = 0.0f;

    vertices[14].x = 0.5f;    vertices[14].y = -0.5f;    vertices[14].z = 0.5f;
    vertices[14].r = 255;        vertices[14].g = 255;        vertices[14].b = 255;
    vertices[14].u = 0.75f;    vertices[14].v = 0.333f;
    vertices[14].nx = -1.0f;       vertices[14].ny = 0.0f;       vertices[14].nz = 0.0f;

    vertices[15].x = 0.5f;    vertices[15].y = 0.5f;    vertices[15].z = -0.5f;
    vertices[15].r = 255;        vertices[15].g = 255;        vertices[15].b = 255;
    vertices[15].u = 0.5f;    vertices[15].v = 0.666f;
    vertices[15].nx = -1.0f;       vertices[15].ny = 0.0f;       vertices[15].nz = 0.0f;

    vertices[16].x = 0.5f;    vertices[16].y = -0.5f;    vertices[16].z = 0.5f;
    vertices[16].r = 255;        vertices[16].g = 255;        vertices[16].b = 255;
    vertices[16].u = 0.75f;    vertices[16].v = 0.333f;
    vertices[16].nx = -1.0f;       vertices[16].ny = 0.0f;       vertices[16].nz = 0.0f;

    vertices[17].x = 0.5f;    vertices[17].y = 0.5f;    vertices[17].z = 0.5f;
    vertices[17].r = 255;        vertices[17].g = 255;        vertices[17].b = 255;
    vertices[17].u = 0.75f;    vertices[17].v = 0.666f;
    vertices[17].nx = -1.0f;       vertices[17].ny = 0.0f;       vertices[17].nz = 0.0f;
    
    vertices[18].x = -0.5f;    vertices[18].y = 0.5f;    vertices[18].z = -0.5f;
    vertices[18].r = 255;        vertices[18].g = 255;        vertices[18].b = 255;
    vertices[18].u = 0.0f;    vertices[18].v = 0.666f;
    vertices[18].nx = -1.0f;       vertices[18].ny = 0.0f;       vertices[18].nz = 0.0f;

    vertices[19].x = -0.5f;    vertices[19].y = -0.5f;    vertices[19].z = -0.5f;
    vertices[19].r = 255;        vertices[19].g = 255;        vertices[19].b = 255;
    vertices[19].u = 0.0f;    vertices[19].v = 0.333f;
    vertices[19].nx = -1.0f;       vertices[19].ny = 0.0f;       vertices[19].nz = 0.0f;

    vertices[20].x = -0.5f;    vertices[20].y = -0.5f;    vertices[20].z = 0.5f;
    vertices[20].r = 255;        vertices[20].g = 255;        vertices[20].b = 255;
    vertices[20].u = 0.25f;    vertices[20].v = 0.333f;
    vertices[20].nx = -1.0f;       vertices[20].ny = 0.0f;       vertices[20].nz = 0.0f;

    vertices[21].x = -0.5f;    vertices[21].y = 0.5f;    vertices[21].z = -0.5f;
    vertices[21].r = 255;        vertices[21].g = 255;        vertices[21].b = 255;
    vertices[21].u = 0.0f;    vertices[21].v = 0.666f;
    vertices[21].nx = -1.0f;       vertices[21].ny = 0.0f;       vertices[21].nz = 0.0f;

    vertices[22].x = -0.5f;    vertices[22].y = -0.5f;    vertices[22].z = 0.5f;
    vertices[22].r = 255;        vertices[22].g = 255;        vertices[22].b = 255;
    vertices[22].u = 0.25f;    vertices[22].v = 0.333f;
    vertices[22].nx = -1.0f;       vertices[22].ny = 0.0f;       vertices[22].nz = 0.0f;

    vertices[23].x = -0.5f;    vertices[23].y = 0.5f;    vertices[23].z = 0.5f;
    vertices[23].r = 255;        vertices[23].g = 255;        vertices[23].b = 255;
    vertices[23].u = 0.25f;    vertices[23].v = 0.666f;
    vertices[23].nx = -1.0f;       vertices[23].ny = 0.0f;       vertices[23].nz = 0.0f;
    
    vertices[24].x = -0.5f;    vertices[24].y = -0.5f;    vertices[24].z = 0.5f;
    vertices[24].r = 255;        vertices[24].g = 255;        vertices[24].b = 255;
    vertices[24].u = 0.25f;    vertices[24].v = 0.333f;
    vertices[24].nx = 0.0f;       vertices[24].ny = -1.0f;       vertices[24].nz = 0.0f;
    
    vertices[25].x = -0.5f;    vertices[25].y = -0.5f;    vertices[25].z = -0.5f;
    vertices[25].r = 255;        vertices[25].g = 255;        vertices[25].b = 255;
    vertices[25].u = 0.25f;    vertices[25].v = 0.0f;
    vertices[25].nx = 0.0f;       vertices[25].ny = -1.0f;       vertices[25].nz = 0.0f;
    
    vertices[26].x = 0.5f;    vertices[26].y = -0.5f;    vertices[26].z = -0.5f;
    vertices[26].r = 255;        vertices[26].g = 255;        vertices[26].b = 255;
    vertices[26].u = 0.5f;    vertices[26].v = 0.0f;
    vertices[26].nx = 0.0f;       vertices[26].ny = -1.0f;       vertices[26].nz = 0.0f;
    
    vertices[27].x = -0.5f;    vertices[27].y = -0.5f;    vertices[27].z = 0.5f;
    vertices[27].r = 255;        vertices[27].g = 255;        vertices[27].b = 255;
    vertices[27].u = 0.25f;    vertices[27].v = 0.333f;
    vertices[27].nx = 0.0f;       vertices[27].ny = -1.0f;       vertices[27].nz = 0.0f;
    
    vertices[28].x = 0.5f;    vertices[28].y = -0.5f;    vertices[28].z = -0.5f;
    vertices[28].r = 255;        vertices[28].g = 255;        vertices[28].b = 255;
    vertices[28].u = 0.5f;    vertices[28].v = 0.0f;
    vertices[28].nx = 0.0f;       vertices[28].ny = -1.0f;       vertices[28].nz = 0.0f;
    
    vertices[29].x = 0.5f;    vertices[29].y = -0.5f;    vertices[29].z = 0.5f;
    vertices[29].r = 255;        vertices[29].g = 255;        vertices[29].b = 255;
    vertices[29].u = 0.5f;    vertices[29].v = 0.333f;
    vertices[29].nx = 0.0f;       vertices[29].ny = -1.0f;       vertices[29].nz = 0.0f;
    
    vertices[30].x = -0.5f;    vertices[30].y = 0.5f;    vertices[30].z = 0.5f;
    vertices[30].r = 255;        vertices[30].g = 255;        vertices[30].b = 255;
    vertices[30].u = 0.25f;    vertices[30].v = 1.0f;
    vertices[30].nx = 0.0f;       vertices[30].ny = -1.0f;       vertices[30].nz = 0.0f;
    
    vertices[31].x = -0.5f;    vertices[31].y = 0.5f;    vertices[31].z = -0.5f;
    vertices[31].r = 255;        vertices[31].g = 255;        vertices[31].b = 255;
    vertices[31].u = 0.25f;    vertices[31].v = 0.666f;
    vertices[31].nx = 0.0f;       vertices[31].ny = -1.0f;       vertices[31].nz = 0.0f;
    
    vertices[32].x = 0.5f;    vertices[32].y = 0.5f;    vertices[32].z = -0.5f;
    vertices[32].r = 255;        vertices[32].g = 255;        vertices[32].b = 255;
    vertices[32].u = 0.5f;    vertices[32].v = 0.666f;
    vertices[32].nx = 0.0f;       vertices[32].ny = -1.0f;       vertices[32].nz = 0.0f;
    
    vertices[33].x = -0.5f;    vertices[33].y = 0.5f;    vertices[33].z = 0.5f;
    vertices[33].r = 255;        vertices[33].g = 255;        vertices[33].b = 255;
    vertices[33].u = 0.25f;    vertices[33].v = 1.0f;
    vertices[33].nx = 0.0f;       vertices[33].ny = -1.0f;       vertices[33].nz = 0.0f;
    
    vertices[34].x = 0.5f;    vertices[34].y = 0.5f;    vertices[34].z = -0.5f;
    vertices[34].r = 255;        vertices[34].g = 255;        vertices[34].b = 255;
    vertices[34].u = 0.5f;    vertices[34].v = 0.666f;
    vertices[34].nx = 0.0f;       vertices[34].ny = -1.0f;       vertices[34].nz = 0.0f;
    
    vertices[35].x = 0.5f;    vertices[35].y = 0.5f;    vertices[35].z = 0.5f;
    vertices[35].r = 255;        vertices[35].g = 255;        vertices[35].b = 255;
    vertices[35].u = 0.5f;    vertices[35].v = 1.0f;
    vertices[35].nx = 0.0f;       vertices[35].ny = -1.0f;       vertices[35].nz = 0.0f;
    

//    glm::mat4 mat = glm::mat4(1.0f);
//    mat = glm::rotate(mat, (float)glfwGetTime(), glm::vec3(.0f, 1.0f, 0.0f));

    // Create a vertex buffer object (VBO), and upload our vertices data to the VBO
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Create a vertex array object that contains data on how to map vertex attributes
    // (e.g., position, color) to vertex shader properties.
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    // Vertex attribute 0 - Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

    // Vertex attribute 1 - Color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)(offsetof(Vertex, r)));

    // Vertex attribute 2 - UV coordinate
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, u)));
    
    // Vertex attribute 3 - normal
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, nx)));
    
    glEnableVertexAttribArray(0);

    // Create a shader program
    // for windows:
//     GLuint program = CreateShaderProgram("main.vsh", "main.fsh");
    
    // for mac:
     GLuint program = CreateShaderProgram("/Users/carmen/Downloads/OpenGL/Projects/testing/testing/main.vs", "/Users/carmen/Downloads/OpenGL/Projects/testing/testing/main.fs");

    // Tell OpenGL the dimensions of the region where stuff will be drawn.
    // For now, tell OpenGL to use the whole screen
    glViewport(0, 0, windowWidth, windowHeight);

    // Create a variable that will contain the ID for our texture,
    // and use glGenTextures() to generate the texture itself
    GLuint tex0;
    glGenTextures(1, &tex0);

    // --- Load our image using stb_image ---

    // Im image-space (pixels), (0, 0) is the upper-left corner of the image
    // However, in u-v coordinates, (0, 0) is the lower-left corner of the image
    // This means that the image will appear upside-down when we use the image data as is
    // This function tells stbi to flip the image vertically so that it is not upside-down when we use it
    stbi_set_flip_vertically_on_load(true);

    // 'imageWidth' and imageHeight will contain the width and height of the loaded image respectively
    int imageWidth, imageHeight, numChannels;

    // Read the image data and store it in an unsigned char array
    
    // for windows:
//     unsigned char* imageData = stbi_load("dice.jpeg", &imageWidth, &imageHeight, &numChannels, 0);
    
    // for mac:
     unsigned char* imageData = stbi_load("/Users/carmen/Downloads/OpenGL/Projects/testing/testing/dice.jpeg", &imageWidth, &imageHeight, &numChannels, 0);
    

    // Make sure that we actually loaded the image before uploading the data to the GPU
    if (imageData != nullptr)
    {
        // Our texture is 2D, so we bind our texture to the GL_TEXTURE_2D target
        glBindTexture(GL_TEXTURE_2D, tex0);

        // Set the filtering methods for magnification and minification
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // Set the wrapping method for the s-axis (x-axis) and t-axis (y-axis)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

        // Upload the image data to GPU memory
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

        // If we set minification to use mipmaps, we can tell OpenGL to generate the mipmaps for us
        //glGenerateMipmap(GL_TEXTURE_2D);

        // Once we have copied the data over to the GPU, we can delete
        // the data on the CPU side, since we won't be using it anymore
        stbi_image_free(imageData);
        imageData = nullptr;
    }
    else
    {
        std::cerr << "Failed to load image" << std::endl;
    }
    
    
    glEnable(GL_DEPTH_TEST);
    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        
        // Clear the colors in our off-screen framebuffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the shader program that we created
        glUseProgram(program);

        // Use the vertex array object that we created
        glBindVertexArray(vao);
        
        // Bind tex0 to texture unit 0, and set our tex0 uniform to texture unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex0);
        glUniform1i(glGetUniformLocation(program, "tex0"), current);
        
        // cube 1
        
        
//        glm::vec3 lightPos = glm::vec3(8.0f, -7.0f, -17.0f);
        glm::vec3 lightPos = glm::vec3(20.0f, -10.0f, -30.0f);
        glUniform3fv(glGetUniformLocation(program, "lightPos"), 1, glm::value_ptr(lightPos));
        
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(glGetUniformLocation(program, "lightColor"), 1, glm::value_ptr(lightColor));
        
        glm::vec3 ambientLight = 0.1f * lightColor;
        glUniform3fv(glGetUniformLocation(program, "ambientLight"), 1, glm::value_ptr(ambientLight));
        
        glm::vec3 matlAmbient = glm::vec3(0.3f, 0.3f, 0.3f);
        glm::vec3 matlDiffuse = glm::vec3(0.2f, 0.2f, 0.2f);
        glm::vec3 matlSpecular = glm::vec3(2.0f, 2.0f, 2.0f);
        float matlShiny = 1.5f;
        
        glUniform3fv(glGetUniformLocation(program, "matlAmbient"), 1, glm::value_ptr(matlAmbient));
        glUniform3fv(glGetUniformLocation(program, "matlDiffuse"), 1, glm::value_ptr(matlDiffuse));
        glUniform3fv(glGetUniformLocation(program, "matlSpecular"), 1, glm::value_ptr(matlSpecular));
        glUniform1f(glGetUniformLocation(program, "matlShiny"), matlShiny);
        
        glm::mat4 mat = glm::mat4(1.0f);
        mat = glm::translate(mat, glm::vec3(-0.5f, 0.0f, -0.0f));
        mat = glm::rotate(mat, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 1.0f));
        mat = glm::scale(mat, glm::vec3(0.5f, 0.5f, 0.5f));
        
        glm::mat4 view; // position, target, up
        view = glm::lookAt(glm::vec3(0.5f, 0.0f, 1.25f),
                     glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3(0.0f, 1.0f, 0.0f));
        
        glm::mat4 persp = glm::perspective(90.0f, 1.0f, 0.1f, 100.0f);
        
        mat = persp * view * mat;
        
        glUniformMatrix4fv(glGetUniformLocation(program, "persp"), 1, GL_FALSE, glm::value_ptr(persp));
        glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        
        GLint uniformLocation = glGetUniformLocation(program, "mat");
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat));
        
        GLint nmuniformLocation = glGetUniformLocation(program, "nmat");
        glm::mat4 nmat = transpose(inverse(mat));
        glUniformMatrix4fv(nmuniformLocation, 1, GL_FALSE, glm::value_ptr(nmat));
        
        glBindVertexArray(0);
        glBindVertexArray(vao);

        // Draw the 3 vertices using triangle primitives
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        // step 6 - cube 2
        
        glBindVertexArray(0);
        glBindVertexArray(vao);
        
        glm::mat4 mat1 = glm::mat4(1.0f);
        mat1 = glm::translate(mat1, glm::vec3(0.3f, -0.4f, -0.5f));
        mat1 = glm::rotate(mat1, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
        mat1 = glm::scale(mat1, glm::vec3(0.3f, 0.3f, 0.3f));
        
        mat1 = persp * view * mat1;
        
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat1));
        
        glm::mat4 nmat1 = transpose(inverse(mat1));
        glUniformMatrix4fv(nmuniformLocation, 1, GL_FALSE, glm::value_ptr(nmat1));

        // Draw the 3 vertices using triangle primitives
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        // step 6 - cube 3
        
        glBindVertexArray(0);
        glBindVertexArray(vao);
        
        glm::mat4 mat2 = glm::mat4(1.0f);
        mat2 = glm::translate(mat2, glm::vec3(1.0f, 0.4f, -1.0f));
        mat2 = glm::rotate(mat2, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        mat2 = glm::scale(mat2, glm::vec3(0.3f, 0.3f, 0.3f));
        
        mat2 = persp * view * mat2;
        
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat2));
        
        glm::mat4 nmat2 = transpose(inverse(mat2));
        glUniformMatrix4fv(nmuniformLocation, 1, GL_FALSE, glm::value_ptr(nmat2));

        // Draw the 3 vertices using triangle primitives
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        // revolving cube around origin
        
        glBindVertexArray(0);
        glBindVertexArray(vao);
        
        float radius = 0.8f;
        float lookX = sin(glfwGetTime()) * radius;
        float lookZ = cos(glfwGetTime()) * radius;
        
        glm::mat4 mat3 = glm::mat4(1.0f);
        mat3 = glm::translate(mat3, glm::vec3(lookX, 0.0f, lookZ));
        mat3 = glm::rotate(mat3, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
        mat3 = glm::scale(mat3, glm::vec3(0.1f, 0.1f, 0.1f));
        
        mat3 = persp * view * mat3;
        
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat3));
        
        glm::mat4 nmat3 = transpose(inverse(mat3));
        glUniformMatrix4fv(nmuniformLocation, 1, GL_FALSE, glm::value_ptr(nmat3));

        // Draw the 3 vertices using triangle primitives
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        // revolving cube around another cube
        
        glBindVertexArray(0);
        glBindVertexArray(vao);
        
        radius = 0.4f;
        // values gotten from translation matrix of step 6 - cube 3
        // add x, y, z components to lookX, y, and lookZ
        lookX = sin(glfwGetTime()) * radius + 1.0f;
        lookZ = cos(glfwGetTime()) * radius - 1.0f;
        
        glm::mat4 mat4 = glm::mat4(1.0f);
        mat4 = glm::translate(mat4, glm::vec3(lookX, 0.4f, lookZ));
        mat4 = glm::rotate(mat4, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
        mat4 = glm::scale(mat4, glm::vec3(0.1f, 0.1f, 0.1f));
        
        mat4 = persp * view * mat4;
        
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat4));
        
        glm::mat4 nmat4 = transpose(inverse(mat4));
        glUniformMatrix4fv(nmuniformLocation, 1, GL_FALSE, glm::value_ptr(nmat4));

        // Draw the 3 vertices using triangle primitives
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // "Unuse" the vertex array object
        glBindVertexArray(0);

        // Tell GLFW to swap the screen buffer with the offscreen buffer
        glfwSwapBuffers(window);

        // Tell GLFW to process window events (e.g., input events, window closed events, etc.)
        glfwPollEvents();
    }

    // --- Cleanup ---

    // Make sure to delete the shader program
    glDeleteProgram(program);

    // Delete the VBO that contains our vertices
    glDeleteBuffers(1, &vbo);

    // Delete the vertex array object
    glDeleteVertexArrays(1, &vao);

    // Remember to tell GLFW to clean itself up before exiting the application
    glfwTerminate();

    return 0;
}

/// <summary>
/// Creates a shader program based on the provided file paths for the vertex and fragment shaders.
/// </summary>
/// <param name="vertexShaderFilePath">Vertex shader file path</param>
/// <param name="fragmentShaderFilePath">Fragment shader file path</param>
/// <returns>OpenGL handle to the created shader program</returns>
GLuint CreateShaderProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
    GLuint vertexShader = CreateShaderFromFile(GL_VERTEX_SHADER, vertexShaderFilePath);
    GLuint fragmentShader = CreateShaderFromFile(GL_FRAGMENT_SHADER, fragmentShaderFilePath);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    // Check shader program link status
    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        char infoLog[512];
        GLsizei infoLogLen = sizeof(infoLog);
        glGetProgramInfoLog(program, infoLogLen, &infoLogLen, infoLog);
        std::cerr << "program link error: " << infoLog << std::endl;
    }

    return program;
}

/// <summary>
/// Creates a shader based on the provided shader type and the path to the file containing the shader source.
/// </summary>
/// <param name="shaderType">Shader type</param>
/// <param name="shaderFilePath">Path to the file containing the shader source</param>
/// <returns>OpenGL handle to the created shader</returns>
GLuint CreateShaderFromFile(const GLuint& shaderType, const std::string& shaderFilePath)
{
    std::ifstream shaderFile(shaderFilePath);
    if (shaderFile.fail())
    {
        std::cerr << "Unable to open shader file: " << shaderFilePath << std::endl;
        return 0;
    }

    std::string shaderSource;
    std::string temp;
    while (std::getline(shaderFile, temp))
    {
        shaderSource += temp + "\n";
    }
    shaderFile.close();

    return CreateShaderFromSource(shaderType, shaderSource);
}

/// <summary>
/// Creates a shader based on the provided shader type and the string containing the shader source.
/// </summary>
/// <param name="shaderType">Shader type</param>
/// <param name="shaderSource">Shader source string</param>
/// <returns>OpenGL handle to the created shader</returns>
GLuint CreateShaderFromSource(const GLuint& shaderType, const std::string& shaderSource)
{
    GLuint shader = glCreateShader(shaderType);

    const char* shaderSourceCStr = shaderSource.c_str();
    GLint shaderSourceLen = static_cast<GLint>(shaderSource.length());
    glShaderSource(shader, 1, &shaderSourceCStr, &shaderSourceLen);
    glCompileShader(shader);

    // Check compilation status
    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE)
    {
        char infoLog[512];
        GLsizei infoLogLen = sizeof(infoLog);
        glGetShaderInfoLog(shader, infoLogLen, &infoLogLen, infoLog);
        std::cerr << "shader compilation error: " << infoLog << std::endl;
    }

    return shader;
}

/// <summary>
/// Function for handling the event when the size of the framebuffer changed.
/// </summary>
/// <param name="window">Reference to the window</param>
/// <param name="width">New width</param>
/// <param name="height">New height</param>
void FramebufferSizeChangedCallback(GLFWwindow* window, int width, int height)
{
    // Whenever the size of the framebuffer changed (due to window resizing, etc.),
    // update the dimensions of the region to the new size
    glViewport(0, 0, width, height);
}
