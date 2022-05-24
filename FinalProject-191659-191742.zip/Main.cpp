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
float golden = ((1+sqrt(5))/2)/2;
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
    
    glm::vec3 v0 = glm::vec3(0.0f,-golden,0.5f);
    glm::vec3 v1 = glm::vec3(-golden,-0.5f,0.0f);
    glm::vec3 v2 = glm::vec3(0.0f,-golden,-0.5f);
    glm::vec3 v3 = glm::vec3(golden,-0.5f,0.0f);
    glm::vec3 v4 = glm::vec3(0.5f,0.0f,golden);
    glm::vec3 v5 = glm::vec3(-0.5f,0.0f,golden);
    glm::vec3 v6 = glm::vec3(0.0f,golden,-0.5f);
    glm::vec3 v7 = glm::vec3(0.5f,0.0f,-golden);
    glm::vec3 v8 = glm::vec3(-0.5f,0.0f,-golden);
    glm::vec3 v9 = glm::vec3(-golden,0.5f, 0.0f);
    glm::vec3 v10 = glm::vec3(0.0f,golden,0.5f);
    glm::vec3 v11 = glm::vec3(golden,0.5f,0.0f);
    
    glm::vec3 vecArray [12] = { v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 };

    glm::vec3 normalt0 = cross((v0-v1),(v1-v2));
    glm::vec3 normalt1 = cross((v0-v2),(v2-v3));
    glm::vec3 normalt2 = cross((v0-v3),(v3-v4));
    glm::vec3 normalt3 = cross((v0-v4),(v4-v5));
    glm::vec3 normalt4 = cross((v0-v5),(v5-v1));
    glm::vec3 normalt5 = cross((v6-v7),(v7-v8));
    glm::vec3 normalt6 = cross((v6-v8),(v8-v9));
    glm::vec3 normalt7 = cross((v6-v9),(v9-v10));
    glm::vec3 normalt8 = cross((v6-v10),(v10-v11));
    glm::vec3 normalt9 = cross((v6-v11),(v11-v7));
    glm::vec3 normalt10 = cross((v1-v8),(v8-v2));
    glm::vec3 normalt11 = cross((v2-v8),(v8-v7));
    glm::vec3 normalt12 = cross((v2-v7),(v7-v3));
    glm::vec3 normalt13 = cross((v3-v7),(v7-v11));
    glm::vec3 normalt14 = cross((v3-v11),(v11-v4));
    glm::vec3 normalt15 = cross((v4-v11),(v11-v10));
    glm::vec3 normalt16 = cross((v4-v10),(v10-v5));
    glm::vec3 normalt17 = cross((v5-v10),(v10-v9));
    glm::vec3 normalt18 = cross((v5-v9),(v9-v1));
    glm::vec3 normalt19 = cross((v1-v9),(v9-v8));
    
    glm::vec3 normalFaces [20] = {
        normalt0, normalt1, normalt2, normalt3, normalt4,
        normalt5, normalt6, normalt7, normalt8, normalt9,
        normalt10, normalt11, normalt12, normalt13, normalt14,
        normalt15, normalt16, normalt17, normalt18, normalt19
    };
    
    glm::vec3 sumv0 = normalt0+normalt1+normalt2+normalt3+normalt4;
    float l_sumv0 = sqrt((sumv0[0]*sumv0[0]) + (sumv0[1]*sumv0[1]) + (sumv0[2]*sumv0[2]));
    glm::vec3 normalv0 = sumv0 / l_sumv0;

    glm::vec3 sumv1 = normalt0+normalt4+normalt10+normalt18+normalt19;
    glm::vec3 normalv1 = sumv1 / l_sumv0;

    glm::vec3 sumv2 = normalt0+normalt1+normalt10+normalt11+normalt12;
    glm::vec3 normalv2 = sumv2 / l_sumv0;

    glm::vec3 sumv3 = normalt1+normalt2+normalt12+normalt13+normalt14;
    glm::vec3 normalv3 = sumv3 / l_sumv0;

    glm::vec3 sumv4 = normalt2+normalt3+normalt14+normalt15+normalt16;
    glm::vec3 normalv4 = sumv4 / l_sumv0;

    glm::vec3 sumv5 = normalt3+normalt4+normalt16+normalt17+normalt18;
    glm::vec3 normalv5 = sumv5 / l_sumv0;

    glm::vec3 sumv6 = normalt5+normalt6+normalt7+normalt8+normalt9;
    glm::vec3 normalv6 = sumv6 / l_sumv0;

    glm::vec3 sumv7 = normalt5+normalt9+normalt11+normalt12+normalt13;
    glm::vec3 normalv7 = sumv7 / l_sumv0;

    glm::vec3 sumv8 = normalt5+normalt6+normalt10+normalt11+normalt19;
    glm::vec3 normalv8 = sumv8 / l_sumv0;

    glm::vec3 sumv9 = normalt6+normalt7+normalt17+normalt18+normalt19;
    glm::vec3 normalv9 = sumv9 / l_sumv0;

    glm::vec3 sumv10 = normalt7+normalt8+normalt15+normalt16+normalt17;
    glm::vec3 normalv10 = sumv10 / l_sumv0;

    glm::vec3 sumv11 = normalt8+normalt9+normalt13+normalt14+normalt15;
    glm::vec3 normalv11 = sumv11 / l_sumv0;
    
//    glm::vec3 normalArray [12] {
//        normalv0, normalv1, normalv2, normalv3, normalv4, normalv5,
//        normalv6, normalv7, normalv8, normalv9, normalv10, normalv11
//    };
    
    // Set up the data for each vertex of the triangle
    Vertex vertices[60];
    
    // t0 - 18 on the die
    // indices 0, 1, 2
    vertices[0].x = vecArray[0][0];
    vertices[0].y = vecArray[0][1];
    vertices[0].z = vecArray[0][2];
    vertices[0].r = 255;
    vertices[0].g = 255;
    vertices[0].b = 255;
    vertices[0].u = 0.0f;
    vertices[0].v = 0.0f;
    vertices[0].nx = 0.0f;
    vertices[0].ny = 0.0f;
    vertices[0].nz = 0.0f;
    
    vertices[0].x = vecArray[0][0];
    vertices[0].y = vecArray[0][1];
    vertices[0].z = vecArray[0][2];
    vertices[0].r = 255;
    vertices[0].g = 255;
    vertices[0].b = 255;
    vertices[0].u = 0.0f;
    vertices[0].v = 0.0f;
    vertices[0].nx = 0.0f;
    vertices[0].ny = 0.0f;
    vertices[0].nz = 0.0f;
    
    for (int i = 0; i < 3; i++) {
        vertices[i].nx = normalFaces[0][0];
        vertices[i].ny = normalFaces[0][1];
        vertices[i].nz = normalFaces[0][2];
    };
    for (int i = 3; i < 6; i++) {
        vertices[i].nx = normalFaces[1][0];
        vertices[i].ny = normalFaces[1][1];
        vertices[i].nz = normalFaces[1][2];
    };
    for (int i = 6; i < 9; i++) {
        vertices[i].nx = normalFaces[2][0];
        vertices[i].ny = normalFaces[2][1];
        vertices[i].nz = normalFaces[2][2];
    };
    for (int i = 9; i < 12; i++) {
        vertices[i].nx = normalFaces[3][0];
        vertices[i].ny = normalFaces[3][1];
        vertices[i].nz = normalFaces[3][2];
    };
    for (int i = 12; i < 15; i++) {
        vertices[i].nx = normalFaces[4][0];
        vertices[i].ny = normalFaces[4][1];
        vertices[i].nz = normalFaces[4][2];
    };
    for (int i = 15; i < 18; i++) {
        vertices[i].nx = normalFaces[5][0];
        vertices[i].ny = normalFaces[5][1];
        vertices[i].nz = normalFaces[5][2];
    };
    for (int i = 18; i < 21; i++) {
        vertices[i].nx = normalFaces[6][0];
        vertices[i].ny = normalFaces[6][1];
        vertices[i].nz = normalFaces[6][2];
    };
    for (int i = 21; i < 24; i++) {
        vertices[i].nx = normalFaces[7][0];
        vertices[i].ny = normalFaces[7][1];
        vertices[i].nz = normalFaces[7][2];
    };
    for (int i = 24; i < 27; i++) {
        vertices[i].nx = normalFaces[8][0];
        vertices[i].ny = normalFaces[8][1];
        vertices[i].nz = normalFaces[8][2];
    };
    for (int i = 27; i < 30; i++) {
        vertices[i].nx = normalFaces[9][0];
        vertices[i].ny = normalFaces[9][1];
        vertices[i].nz = normalFaces[9][2];
    };
    for (int i = 30; i < 33; i++) {
        vertices[i].nx = normalFaces[10][0];
        vertices[i].ny = normalFaces[10][1];
        vertices[i].nz = normalFaces[10][2];
    };
    for (int i = 33; i < 36; i++) {
        vertices[i].nx = normalFaces[11][0];
        vertices[i].ny = normalFaces[11][1];
        vertices[i].nz = normalFaces[11][2];
    };
    for (int i = 36; i < 39; i++) {
        vertices[i].nx = normalFaces[12][0];
        vertices[i].ny = normalFaces[12][1];
        vertices[i].nz = normalFaces[12][2];
    };
    for (int i = 39; i < 42; i++) {
        vertices[i].nx = normalFaces[13][0];
        vertices[i].ny = normalFaces[13][1];
        vertices[i].nz = normalFaces[13][2];
    };
    for (int i = 42; i < 45; i++) {
        vertices[i].nx = normalFaces[14][0];
        vertices[i].ny = normalFaces[14][1];
        vertices[i].nz = normalFaces[14][2];
    };
    for (int i = 45; i < 48; i++) {
        vertices[i].nx = normalFaces[15][0];
        vertices[i].ny = normalFaces[15][1];
        vertices[i].nz = normalFaces[15][2];
    };
    for (int i = 48; i < 51; i++) {
        vertices[i].nx = normalFaces[16][0];
        vertices[i].ny = normalFaces[16][1];
        vertices[i].nz = normalFaces[16][2];
    };
    for (int i = 51; i < 54; i++) {
        vertices[i].nx = normalFaces[17][0];
        vertices[i].ny = normalFaces[17][1];
        vertices[i].nz = normalFaces[17][2];
    };
    for (int i = 54; i < 57; i++) {
        vertices[i].nx = normalFaces[18][0];
        vertices[i].ny = normalFaces[18][1];
        vertices[i].nz = normalFaces[18][2];
    };
    for (int i = 57; i < 60; i++) {
        vertices[i].nx = normalFaces[19][0];
        vertices[i].ny = normalFaces[19][1];
        vertices[i].nz = normalFaces[19][2];
    };
    
    
//    for (int i = 0; i < 12; i++) {
//        vertices[i].x = vecArray[i][0];
//        vertices[i].y = vecArray[i][1];
//        vertices[i].z = vecArray[i][2];
//        vertices[i].r = 255;
//        vertices[i].g = 255;
//        vertices[i].b = 255;
//        vertices[i].u = vertices[i].u;
//        vertices[i].v = vertices[i].v;
//        vertices[i].nx = normalArray[i][0];
//        vertices[i].ny = normalArray[i][1];
//        vertices[i].nz = normalArray[i][2];
//        std::cout << i << " " << vertices[i].nx << " " << vertices[i].ny << " " << vertices[i].nz << std::endl;
//    };

    
    GLuint indexOrder[] = {
        // store the index order of drawing
        // indeces use up A LOT LESS memory than duplicates
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 1,
        6, 7, 8,
        6, 8, 9,
        6, 9, 10,
        6, 10, 11,
        6, 11, 7,
        1, 8, 2,
        2, 8, 7,
        2, 7, 3,
        3, 7, 11,
        3, 11, 4,
        4, 11, 10,
        4, 10, 5,
        5, 10, 9,
        5, 9, 1,
        1, 9, 8
    };
    
    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(indexOrder) * sizeof(unsigned int),
                 &indexOrder[0], GL_STATIC_DRAW
    );

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
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexOrder), indexOrder, GL_STATIC_DRAW);
        
        // Bind tex0 to texture unit 0, and set our tex0 uniform to texture unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex0);
        glUniform1i(glGetUniformLocation(program, "tex0"), current);
        
        // cube 1
        
        
        glm::vec3 lightPos = glm::vec3(2.0f, 0.0f, -20.0f);
//        glm::vec3 lightPos = glm::vec3(-30.0f, -10.0f, -30.0f);
        glUniform3fv(glGetUniformLocation(program, "lightPos"), 1, glm::value_ptr(lightPos));
        
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(glGetUniformLocation(program, "lightColor"), 1, glm::value_ptr(lightColor));
        
        glm::vec3 ambientLight = 0.1f * lightColor;
        glUniform3fv(glGetUniformLocation(program, "ambientLight"), 1, glm::value_ptr(ambientLight));
        
        glm::vec3 matlAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
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
        mat = glm::scale(mat, glm::vec3(0.8f, 0.8f, 0.8f));
        
        glm::mat4 view; // position, target, up
        view = glm::lookAt(glm::vec3(0.5f, 0.0f, 1.25f),
                     glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3(0.0f, 1.0f, 0.0f));
//
        glm::mat4 persp = glm::mat4(1.0f);
        persp = glm::perspective(90.0f, 1.0f, 0.1f, 100.0f);
        
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
//        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
//        glDrawArrays(GL_TRIANGLE_FAN, 6, 6);
        glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);
        
        // step 6 - cube 2
        
//        glBindVertexArray(0);
//        glBindVertexArray(vao);
//
//        glm::mat4 mat1 = glm::mat4(1.0f);
//        mat1 = glm::translate(mat1, glm::vec3(0.3f, -0.4f, -0.5f));
//        mat1 = glm::rotate(mat1, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
//        mat1 = glm::scale(mat1, glm::vec3(0.3f, 0.3f, 0.3f));
//
//        mat1 = persp * view * mat1;
//
//        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat1));
//
//        glm::mat4 nmat1 = transpose(inverse(mat1));
//        glUniformMatrix4fv(nmuniformLocation, 1, GL_FALSE, glm::value_ptr(nmat1));
//
//        // Draw the 3 vertices using triangle primitives
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//
//        // step 6 - cube 3
//
//        glBindVertexArray(0);
//        glBindVertexArray(vao);
//
//        glm::mat4 mat2 = glm::mat4(1.0f);
//        mat2 = glm::translate(mat2, glm::vec3(1.0f, 0.4f, -1.0f));
//        mat2 = glm::rotate(mat2, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
//        mat2 = glm::scale(mat2, glm::vec3(0.3f, 0.3f, 0.3f));
//
//        mat2 = persp * view * mat2;
//
//        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat2));
//
//        glm::mat4 nmat2 = transpose(inverse(mat2));
//        glUniformMatrix4fv(nmuniformLocation, 1, GL_FALSE, glm::value_ptr(nmat2));
//
//        // Draw the 3 vertices using triangle primitives
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//
//        // revolving cube around origin
//
//        glBindVertexArray(0);
//        glBindVertexArray(vao);
//
//        float radius = 0.8f;
//        float lookX = sin(glfwGetTime()) * radius;
//        float lookZ = cos(glfwGetTime()) * radius;
//
//        glm::mat4 mat3 = glm::mat4(1.0f);
//        mat3 = glm::translate(mat3, glm::vec3(lookX, 0.0f, lookZ));
//        mat3 = glm::rotate(mat3, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
//        mat3 = glm::scale(mat3, glm::vec3(0.1f, 0.1f, 0.1f));
//
//        mat3 = persp * view * mat3;
//
//        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat3));
//
//        glm::mat4 nmat3 = transpose(inverse(mat3));
//        glUniformMatrix4fv(nmuniformLocation, 1, GL_FALSE, glm::value_ptr(nmat3));
//
//        // Draw the 3 vertices using triangle primitives
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//
//        // revolving cube around another cube
//
//        glBindVertexArray(0);
//        glBindVertexArray(vao);
//
//        radius = 0.4f;
//        // values gotten from translation matrix of step 6 - cube 3
//        // add x, y, z components to lookX, y, and lookZ
//        lookX = sin(glfwGetTime()) * radius + 1.0f;
//        lookZ = cos(glfwGetTime()) * radius - 1.0f;
//
//        glm::mat4 mat4 = glm::mat4(1.0f);
//        mat4 = glm::translate(mat4, glm::vec3(lookX, 0.4f, lookZ));
//        mat4 = glm::rotate(mat4, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
//        mat4 = glm::scale(mat4, glm::vec3(0.1f, 0.1f, 0.1f));
//
//        mat4 = persp * view * mat4;
//
//        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat4));
//
//        glm::mat4 nmat4 = transpose(inverse(mat4));
//        glUniformMatrix4fv(nmuniformLocation, 1, GL_FALSE, glm::value_ptr(nmat4));
//
//        // Draw the 3 vertices using triangle primitives
//        glDrawArrays(GL_TRIANGLES, 0, 36);

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
