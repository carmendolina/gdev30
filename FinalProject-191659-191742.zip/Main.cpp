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
int n = 1;
float rotateVar = 0.0f;
float golden = ((1+sqrt(5))/2)/2;
//
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
//        glfwSetTime(5.0f);
//        while (glfwGetTime() < 7.0f) {
//            rotateVar = (float)glfwGetTime();
//        }
//        glfwSetTime(0.0f);
        if (rotateVar == (float)glfwGetTime()) {
            rotateVar = 0.0f;
        } else {
            rotateVar = (float)glfwGetTime();
        }
    }

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
}



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
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Final Project", nullptr, nullptr);
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
    glfwSetKeyCallback(window, key_callback);

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
    
//    glm::vec3 sumv0 = normalt0+normalt1+normalt2+normalt3+normalt4;
//    float l_sumv0 = sqrt((sumv0[0]*sumv0[0]) + (sumv0[1]*sumv0[1]) + (sumv0[2]*sumv0[2]));
//    glm::vec3 normalv0 = sumv0 / l_sumv0;
//
//    glm::vec3 sumv1 = normalt0+normalt4+normalt10+normalt18+normalt19;
//    glm::vec3 normalv1 = sumv1 / l_sumv0;
//
//    glm::vec3 sumv2 = normalt0+normalt1+normalt10+normalt11+normalt12;
//    glm::vec3 normalv2 = sumv2 / l_sumv0;
//
//    glm::vec3 sumv3 = normalt1+normalt2+normalt12+normalt13+normalt14;
//    glm::vec3 normalv3 = sumv3 / l_sumv0;
//
//    glm::vec3 sumv4 = normalt2+normalt3+normalt14+normalt15+normalt16;
//    glm::vec3 normalv4 = sumv4 / l_sumv0;
//
//    glm::vec3 sumv5 = normalt3+normalt4+normalt16+normalt17+normalt18;
//    glm::vec3 normalv5 = sumv5 / l_sumv0;
//
//    glm::vec3 sumv6 = normalt5+normalt6+normalt7+normalt8+normalt9;
//    glm::vec3 normalv6 = sumv6 / l_sumv0;
//
//    glm::vec3 sumv7 = normalt5+normalt9+normalt11+normalt12+normalt13;
//    glm::vec3 normalv7 = sumv7 / l_sumv0;
//
//    glm::vec3 sumv8 = normalt5+normalt6+normalt10+normalt11+normalt19;
//    glm::vec3 normalv8 = sumv8 / l_sumv0;
//
//    glm::vec3 sumv9 = normalt6+normalt7+normalt17+normalt18+normalt19;
//    glm::vec3 normalv9 = sumv9 / l_sumv0;
//
//    glm::vec3 sumv10 = normalt7+normalt8+normalt15+normalt16+normalt17;
//    glm::vec3 normalv10 = sumv10 / l_sumv0;
//
//    glm::vec3 sumv11 = normalt8+normalt9+normalt13+normalt14+normalt15;
//    glm::vec3 normalv11 = sumv11 / l_sumv0;
    
//    glm::vec3 normalArray [12] {
//        normalv0, normalv1, normalv2, normalv3, normalv4, normalv5,
//        normalv6, normalv7, normalv8, normalv9, normalv10, normalv11
//    };
    
    // Set up the data for each vertex of the triangle
    Vertex vertices[60];
    
    // t0 - 18 on the die
    // indices 0, 1, 2 (change vecArray[i][constant])
    // 0 = pix 600, 95
    // u, v = 0.0f, 0.667f
    // 1 = pix 545.5, 0
    // u, v = 0.909f, 1.0f
    // 2 = pix 491, 95
    // u, v = 0.818f, 0.667f

    vertices[0].x = vecArray[0][0];
    vertices[0].y = vecArray[0][1];
    vertices[0].z = vecArray[0][2];
    vertices[0].r = 255;
    vertices[0].g = 255;
    vertices[0].b = 255;
    vertices[0].u = 0.0f;
    vertices[0].v = 0.337f;
    vertices[0].nx = 0.0f;
    vertices[0].ny = 0.0f;
    vertices[0].nz = 0.0f;
    
    vertices[1].x = vecArray[1][0];
    vertices[1].y = vecArray[1][1];
    vertices[1].z = vecArray[1][2];
    vertices[1].r = 255;
    vertices[1].g = 255;
    vertices[1].b = 255;
    vertices[1].u = 0.909f;
    vertices[1].v = 1.0f;
    vertices[1].nx = 0.0f;
    vertices[1].ny = 0.0f;
    vertices[1].nz = 0.0f;

    vertices[2].x = vecArray[2][0];
    vertices[2].y = vecArray[2][1];
    vertices[2].z = vecArray[2][2];
    vertices[2].r = 255;
    vertices[2].g = 255;
    vertices[2].b = 255;
    vertices[2].u = 0.8183f;
    vertices[2].v = 0.667f;
    vertices[2].nx = 0.0f;
    vertices[2].ny = 0.0f;
    vertices[2].nz = 0.0f;

    // t1 - 4 on the die
    // indices 0, 2, 3 (change vecArray[i][constant])
    // 0 = pix 600, 95
    // u, v = 0.0f, 0.667f
    // 2 = pix 491, 95
    // u, v = 0.818f, 0.667f
    // 3 = pix 545.5, 190
    // u, v = 0.909f, 0.333f
    
    vertices[3].x = vecArray[0][0];
    vertices[3].y = vecArray[0][1];
    vertices[3].z = vecArray[0][2];
    vertices[3].r = 255;
    vertices[3].g = 255;
    vertices[3].b = 255;
    vertices[3].u = 0.0f;
    vertices[3].v = 0.667f;
    vertices[3].nx = 0.0f;
    vertices[3].ny = 0.0f;
    vertices[3].nz = 0.0f;
    
    vertices[4].x = vecArray[2][0];
    vertices[4].y = vecArray[2][1];
    vertices[4].z = vecArray[2][2];
    vertices[4].r = 255;
    vertices[4].g = 255;
    vertices[4].b = 255;
    vertices[4].u = 0.818f;
    vertices[4].v = 0.667f;
    vertices[4].nx = 0.0f;
    vertices[4].ny = 0.0f;
    vertices[4].nz = 0.0f;

    vertices[5].x = vecArray[3][0];
    vertices[5].y = vecArray[3][1];
    vertices[5].z = vecArray[3][2];
    vertices[5].r = 255;
    vertices[5].g = 255;
    vertices[5].b = 255;
    vertices[5].u = 0.909f;
    vertices[5].v = 0.333f;
    vertices[5].nx = 0.0f;
    vertices[5].ny = 0.0f;
    vertices[5].nz = 0.0f;

    // t2 - 11 on the die
    // indices 0, 3, 4 (change vecArray[i][constant])
    // 0 = pix 55, 95
    // u, v = 0.0917f, 0.667f
    // 3 = pix 0.5, 190
    // u, v = 0.0008f, 0.333f
    // 4 = pix 109.5, 190
    // u, v = 0.1825f, 0.333f
    
    
    vertices[6].x = vecArray[0][0];
    vertices[6].y = vecArray[0][1];
    vertices[6].z = vecArray[0][2];
    vertices[6].r = 255;
    vertices[6].g = 255;
    vertices[6].b = 255;
    vertices[6].u = 0.0917f;
    vertices[6].v = 0.667f;
    vertices[6].nx = 0.0f;
    vertices[6].ny = 0.0f;
    vertices[6].nz = 0.0f;
    
    vertices[7].x = vecArray[3][0];
    vertices[7].y = vecArray[3][1];
    vertices[7].z = vecArray[3][2];
    vertices[7].r = 255;
    vertices[7].g = 255;
    vertices[7].b = 255;
    vertices[7].u = 0.0008f;
    vertices[7].v = 0.333f;
    vertices[7].nx = 0.0f;
    vertices[7].ny = 0.0f;
    vertices[7].nz = 0.0f;

    vertices[8].x = vecArray[4][0];
    vertices[8].y = vecArray[4][1];
    vertices[8].z = vecArray[4][2];
    vertices[8].r = 255;
    vertices[8].g = 255;
    vertices[8].b = 255;
    vertices[8].u = 0.1825f;
    vertices[8].v = 0.333f;
    vertices[8].nx = 0.0f;
    vertices[8].ny = 0.0f;
    vertices[8].nz = 0.0f;

    // t3 - 13 on the die
    // indices 0, 4, 5 (change vecArray[i][constant])
    // 0 = pix 55, 95
    // u, v = 0.0917f, 0.667f
    // 4 = pix 109.5, 190
    // u, v = 0.1825f, 0.333f
    // 5 = pix 164, 95
    // u, v = 0.273f, 0.667f
    
    vertices[9].x = vecArray[0][0];
    vertices[9].y = vecArray[0][1];
    vertices[9].z = vecArray[0][2];
    vertices[9].r = 255;
    vertices[9].g = 255;
    vertices[9].b = 255;
    vertices[9].u = 0.0917f;
    vertices[9].v = 0.667f;
    vertices[9].nx = 0.0f;
    vertices[9].ny = 0.0f;
    vertices[9].nz = 0.0f;
    
    vertices[10].x = vecArray[4][0];
    vertices[10].y = vecArray[4][1];
    vertices[10].z = vecArray[4][2];
    vertices[10].r = 255;
    vertices[10].g = 255;
    vertices[10].b = 255;
    vertices[10].u = 0.1825f;
    vertices[10].v = 0.333f;
    vertices[10].nx = 0.0f;
    vertices[10].ny = 0.0f;
    vertices[10].nz = 0.0f;

    vertices[11].x = vecArray[5][0];
    vertices[11].y = vecArray[5][1];
    vertices[11].z = vecArray[5][2];
    vertices[11].r = 255;
    vertices[11].g = 255;
    vertices[11].b = 255;
    vertices[11].u = 0.273f;
    vertices[11].v = 0.667f;
    vertices[11].nx = 0.0f;
    vertices[11].ny = 0.0f;
    vertices[11].nz = 0.0f;

    // t4 - 5 on the die
    // indices 0, 5, 1
    // 0 = pix 55, 95
    // u, v = 0.0917f, 0.667f
    // 5 = pix 164, 95
    // u, v = 0.273f, 0.667f
    // 1 = pix 109.5, 0
    // u, v = 0.1825f, 1.0f
    
    vertices[12].x = vecArray[0][0];
    vertices[12].y = vecArray[0][1];
    vertices[12].z = vecArray[0][2];
    vertices[12].r = 255;
    vertices[12].g = 255;
    vertices[12].b = 255;
    vertices[12].u = 0.0917f;
    vertices[12].v = 0.667f;
    vertices[12].nx = 0.0f;
    vertices[12].ny = 0.0f;
    vertices[12].nz = 0.0f;
    
    vertices[13].x = vecArray[5][0];
    vertices[13].y = vecArray[5][1];
    vertices[13].z = vecArray[5][2];
    vertices[13].r = 255;
    vertices[13].g = 255;
    vertices[13].b = 255;
    vertices[13].u = 0.273f;
    vertices[13].v = 0.667f;
    vertices[13].nx = 0.0f;
    vertices[13].ny = 0.0f;
    vertices[13].nz = 0.0f;

    vertices[14].x = vecArray[1][0];
    vertices[14].y = vecArray[1][1];
    vertices[14].z = vecArray[1][2];
    vertices[14].r = 255;
    vertices[14].g = 255;
    vertices[14].b = 255;
    vertices[14].u = 0.1825f;
    vertices[14].v = 1.0f;
    vertices[14].nx = 0.0f;
    vertices[14].ny = 0.0f;
    vertices[14].nz = 0.0f;

    // t5 - 8 on the die
    // indices 6, 7, 8
    // 6 = pix 327.5, 190
    // u, v = 0.546f, 0.333f
    // 7 = pix 436.5, 190
    // u, v = 0.7275f, 0.333f
    // 8 = pix 382, 95
    // u, v = 0.637f, 0.667f

    vertices[15].x = vecArray[6][0];
    vertices[15].y = vecArray[6][1];
    vertices[15].z = vecArray[6][2];
    vertices[15].r = 255;
    vertices[15].g = 255;
    vertices[15].b = 255;
    vertices[15].u = 0.546f;
    vertices[15].v = 0.333f;
    vertices[15].nx = 0.0f;
    vertices[15].ny = 0.0f;
    vertices[15].nz = 0.0f;
    
    vertices[16].x = vecArray[7][0];
    vertices[16].y = vecArray[7][1];
    vertices[16].z = vecArray[7][2];
    vertices[16].r = 255;
    vertices[16].g = 255;
    vertices[16].b = 255;
    vertices[16].u =  0.7275f;
    vertices[16].v = 0.333f;
    vertices[16].nx = 0.0f;
    vertices[16].ny = 0.0f;
    vertices[16].nz = 0.0f;

    vertices[17].x = vecArray[8][0];
    vertices[17].y = vecArray[8][1];
    vertices[17].z = vecArray[8][2];
    vertices[17].r = 255;
    vertices[17].g = 255;
    vertices[17].b = 255;
    vertices[17].u = 0.637f;
    vertices[17].v = 0.667f;
    vertices[17].nx = 0.0f;
    vertices[17].ny = 0.0f;
    vertices[17].nz = 0.0f;

    // t6 - 10 on the die
    // indices 6, 8, 9
    // 6 = pix 327.5, 190
    // u, v = 0.546f, 0.333f
    // 8 = pix 382, 95
    // u, v = 0.637f, 0.667f
    // 9 = pix 273, 95
    // u, v = 0.455f, 0.667f

    vertices[18].x = vecArray[6][0];
    vertices[18].y = vecArray[6][1];
    vertices[18].z = vecArray[6][2];
    vertices[18].r = 255;
    vertices[18].g = 255;
    vertices[18].b = 255;
    vertices[18].u = 0.546f;
    vertices[18].v = 0.333f;
    vertices[18].nx = 0.0f;
    vertices[18].ny = 0.0f;
    vertices[18].nz = 0.0f;
    
    vertices[19].x = vecArray[8][0];
    vertices[19].y = vecArray[8][1];
    vertices[19].z = vecArray[8][2];
    vertices[19].r = 255;
    vertices[19].g = 255;
    vertices[19].b = 255;
    vertices[19].u = 0.637f;
    vertices[19].v = 0.667f;
    vertices[19].nx = 0.0f;
    vertices[19].ny = 0.0f;
    vertices[19].nz = 0.0f;

    vertices[20].x = vecArray[9][0];
    vertices[20].y = vecArray[9][1];
    vertices[20].z = vecArray[9][2];
    vertices[20].r = 255;
    vertices[20].g = 255;
    vertices[20].b = 255;
    vertices[20].u = 0.455f;
    vertices[20].v = 0.667f;
    vertices[20].nx = 0.0f;
    vertices[20].ny = 0.0f;
    vertices[20].nz = 0.0f;

    // t7 - 17 on the die
    // indices 6, 9, 10
    // 6 = pix 327.5, 190
    // u, v = 0.546f, 0.333f
    // 9 = pix 273, 95
    // u, v = 0.455f, 0.667f
    // 10 = pix 218.5, 190
    // u, v = 0.364f, 0.333f

    vertices[21].x = vecArray[6][0];
    vertices[21].y = vecArray[6][1];
    vertices[21].z = vecArray[6][2];
    vertices[21].r = 255;
    vertices[21].g = 255;
    vertices[21].b = 255;
    vertices[21].u = 0.546f;
    vertices[21].v = 0.333f;
    vertices[21].nx = 0.0f;
    vertices[21].ny = 0.0f;
    vertices[21].nz = 0.0f;
    
    vertices[22].x = vecArray[9][0];
    vertices[22].y = vecArray[9][1];
    vertices[22].z = vecArray[9][2];
    vertices[22].r = 255;
    vertices[22].g = 255;
    vertices[22].b = 255;
    vertices[22].u = 0.455f;
    vertices[22].v =  0.667f;
    vertices[22].nx = 0.0f;
    vertices[22].ny = 0.0f;
    vertices[22].nz = 0.0f;

    vertices[23].x = vecArray[10][0];
    vertices[23].y = vecArray[10][1];
    vertices[23].z = vecArray[10][2];
    vertices[23].r = 255;
    vertices[23].g = 255;
    vertices[23].b = 255;
    vertices[23].u = 0.364f;
    vertices[23].v = 0.333f;
    vertices[23].nx = 0.0f;
    vertices[23].ny = 0.0f;
    vertices[23].nz = 0.0f;

    // t8 - 3 on the die
    // indices 6, 10, 11
    // 6 = pix 327.5, 190
    // u, v = 0.546f, 0.333f
    // 10 = pix 218.5, 190
    // u, v = 0.364f, 0.333f
    // 11 = pix 272.5, 285
    // u, v = 0.454f, 0.0f

    vertices[24].x = vecArray[6][0];
    vertices[24].y = vecArray[6][1];
    vertices[24].z = vecArray[6][2];
    vertices[24].r = 255;
    vertices[24].g = 255;
    vertices[24].b = 255;
    vertices[24].u = 0.546f;
    vertices[24].v = 0.333f;
    vertices[24].nx = 0.0f;
    vertices[24].ny = 0.0f;
    vertices[24].nz = 0.0f;
    
    vertices[25].x = vecArray[10][0];
    vertices[25].y = vecArray[10][1];
    vertices[25].z = vecArray[10][2];
    vertices[25].r = 255;
    vertices[25].g = 255;
    vertices[25].b = 255;
    vertices[25].u = 0.364f;
    vertices[25].v = 0.333f;
    vertices[25].nx = 0.0f;
    vertices[25].ny = 0.0f;
    vertices[25].nz = 0.0f;

    vertices[26].x = vecArray[11][0];
    vertices[26].y = vecArray[11][1];
    vertices[26].z = vecArray[11][2];
    vertices[26].r = 255;
    vertices[26].g = 255;
    vertices[26].b = 255;
    vertices[26].u = 0.454f;
    vertices[26].v = 0.0f;
    vertices[26].nx = 0.0f;
    vertices[26].ny = 0.0f;
    vertices[26].nz = 0.0f;

    // t9 - 16 on the die
    // indices 6, 11, 7
    // 6 = pix 327.5, 190
    // u, v = 0.546f, 0.333f
    // 11 = pix 382, 285
    // u, v = 0.637f, 0.0f
    // 7 = pix 436.5, 190
    // u, v = 0.7275f, 0.333f

    vertices[27].x = vecArray[6][0];
    vertices[27].y = vecArray[6][1];
    vertices[27].z = vecArray[6][2];
    vertices[27].r = 255;
    vertices[27].g = 255;
    vertices[27].b = 255;
    vertices[27].u = 0.546f;
    vertices[27].v = 0.333f;
    vertices[27].nx = 0.0f;
    vertices[27].ny = 0.0f;
    vertices[27].nz = 0.0f;
    
    vertices[28].x = vecArray[11][0];
    vertices[28].y = vecArray[11][1];
    vertices[28].z = vecArray[11][2];
    vertices[28].r = 255;
    vertices[28].g = 255;
    vertices[28].b = 255;
    vertices[28].u = 0.637f;
    vertices[28].v = 0.0f;
    vertices[28].nx = 0.0f;
    vertices[28].ny = 0.0f;
    vertices[28].nz = 0.0f;

    vertices[29].x = vecArray[7][0];
    vertices[29].y = vecArray[7][1];
    vertices[29].z = vecArray[7][2];
    vertices[29].r = 255;
    vertices[29].g = 255;
    vertices[29].b = 255;
    vertices[29].u = 0.7275f;
    vertices[29].v = 0.333f;
    vertices[29].nx = 0.0f;
    vertices[29].ny = 0.0f;
    vertices[29].nz = 0.0f;

    // t10 - 2 on the die
    // indices 1, 8, 2

    vertices[30].x = vecArray[1][0];
    vertices[30].y = vecArray[1][1];
    vertices[30].z = vecArray[1][2];
    vertices[30].r = 255;
    vertices[30].g = 255;
    vertices[30].b = 255;
    vertices[30].u = 0.0f;
    vertices[30].v = 0.0f;
    vertices[30].nx = 0.0f;
    vertices[30].ny = 0.0f;
    vertices[30].nz = 0.0f;
    
    vertices[31].x = vecArray[8][0];
    vertices[31].y = vecArray[8][1];
    vertices[31].z = vecArray[8][2];
    vertices[31].r = 255;
    vertices[31].g = 255;
    vertices[31].b = 255;
    vertices[31].u = 0.0f;
    vertices[31].v = 0.0f;
    vertices[31].nx = 0.0f;
    vertices[31].ny = 0.0f;
    vertices[31].nz = 0.0f;

    vertices[32].x = vecArray[2][0];
    vertices[32].y = vecArray[2][1];
    vertices[32].z = vecArray[2][2];
    vertices[32].r = 255;
    vertices[32].g = 255;
    vertices[32].b = 255;
    vertices[32].u = 0.0f;
    vertices[32].v = 0.0f;
    vertices[32].nx = 0.0f;
    vertices[32].ny = 0.0f;
    vertices[32].nz = 0.0f;

    // t11 - 20 on the die
    // indices 2, 8, 7

    vertices[33].x = vecArray[2][0];
    vertices[33].y = vecArray[2][1];
    vertices[33].z = vecArray[2][2];
    vertices[33].r = 255;
    vertices[33].g = 255;
    vertices[33].b = 255;
    vertices[33].u = 0.0f;
    vertices[33].v = 0.0f;
    vertices[33].nx = 0.0f;
    vertices[33].ny = 0.0f;
    vertices[33].nz = 0.0f;
    
    vertices[34].x = vecArray[8][0];
    vertices[34].y = vecArray[8][1];
    vertices[34].z = vecArray[8][2];
    vertices[34].r = 255;
    vertices[34].g = 255;
    vertices[34].b = 255;
    vertices[34].u = 0.0f;
    vertices[34].v = 0.0f;
    vertices[34].nx = 0.0f;
    vertices[34].ny = 0.0f;
    vertices[34].nz = 0.0f;

    vertices[35].x = vecArray[7][0];
    vertices[35].y = vecArray[7][1];
    vertices[35].z = vecArray[7][2];
    vertices[35].r = 255;
    vertices[35].g = 255;
    vertices[35].b = 255;
    vertices[35].u = 0.0f;
    vertices[35].v = 0.0f;
    vertices[35].nx = 0.0f;
    vertices[35].ny = 0.0f;
    vertices[35].nz = 0.0f;

    // t12 - 14 on the die
    // indices 2, 7, 3

    vertices[36].x = vecArray[2][0];
    vertices[36].y = vecArray[2][1];
    vertices[36].z = vecArray[2][2];
    vertices[36].r = 255;
    vertices[36].g = 255;
    vertices[36].b = 255;
    vertices[36].u = 0.0f;
    vertices[36].v = 0.0f;
    vertices[36].nx = 0.0f;
    vertices[36].ny = 0.0f;
    vertices[36].nz = 0.0f;
    
    vertices[37].x = vecArray[7][0];
    vertices[37].y = vecArray[7][1];
    vertices[37].z = vecArray[7][2];
    vertices[37].r = 255;
    vertices[37].g = 255;
    vertices[37].b = 255;
    vertices[37].u = 0.0f;
    vertices[37].v = 0.0f;
    vertices[37].nx = 0.0f;
    vertices[37].ny = 0.0f;
    vertices[37].nz = 0.0f;

    vertices[38].x = vecArray[3][0];
    vertices[38].y = vecArray[3][1];
    vertices[38].z = vecArray[3][2];
    vertices[38].r = 255;
    vertices[38].g = 255;
    vertices[38].b = 255;
    vertices[38].u = 0.0f;
    vertices[38].v = 0.0f;
    vertices[38].nx = 0.0f;
    vertices[38].ny = 0.0f;
    vertices[38].nz = 0.0f;

    // t13 - 6 on the die
    // indices 3, 7, 11

    vertices[39].x = vecArray[3][0];
    vertices[39].y = vecArray[3][1];
    vertices[39].z = vecArray[3][2];
    vertices[39].r = 255;
    vertices[39].g = 255;
    vertices[39].b = 255;
    vertices[39].u = 0.0f;
    vertices[39].v = 0.0f;
    vertices[39].nx = 0.0f;
    vertices[39].ny = 0.0f;
    vertices[39].nz = 0.0f;
    
    vertices[40].x = vecArray[7][0];
    vertices[40].y = vecArray[7][1];
    vertices[40].z = vecArray[7][2];
    vertices[40].r = 255;
    vertices[40].g = 255;
    vertices[40].b = 255;
    vertices[40].u = 0.0f;
    vertices[40].v = 0.0f;
    vertices[40].nx = 0.0f;
    vertices[40].ny = 0.0f;
    vertices[40].nz = 0.0f;

    vertices[41].x = vecArray[11][0];
    vertices[41].y = vecArray[11][1];
    vertices[41].z = vecArray[11][2];
    vertices[41].r = 255;
    vertices[41].g = 255;
    vertices[41].b = 255;
    vertices[41].u = 0.0f;
    vertices[41].v = 0.0f;
    vertices[41].nx = 0.0f;
    vertices[41].ny = 0.0f;
    vertices[41].nz = 0.0f;

    // t14 - 9 on the die
    // indices 3, 11, 4

    vertices[42].x = vecArray[3][0];
    vertices[42].y = vecArray[3][1];
    vertices[42].z = vecArray[3][2];
    vertices[42].r = 255;
    vertices[42].g = 255;
    vertices[42].b = 255;
    vertices[42].u = 0.0f;
    vertices[42].v = 0.0f;
    vertices[42].nx = 0.0f;
    vertices[42].ny = 0.0f;
    vertices[42].nz = 0.0f;
    
    vertices[43].x = vecArray[11][0];
    vertices[43].y = vecArray[11][1];
    vertices[43].z = vecArray[11][2];
    vertices[43].r = 255;
    vertices[43].g = 255;
    vertices[43].b = 255;
    vertices[43].u = 0.0f;
    vertices[43].v = 0.0f;
    vertices[43].nx = 0.0f;
    vertices[43].ny = 0.0f;
    vertices[43].nz = 0.0f;

    vertices[44].x = vecArray[4][0];
    vertices[44].y = vecArray[4][1];
    vertices[44].z = vecArray[4][2];
    vertices[44].r = 255;
    vertices[44].g = 255;
    vertices[44].b = 255;
    vertices[44].u = 0.0f;
    vertices[44].v = 0.0f;
    vertices[44].nx = 0.0f;
    vertices[44].ny = 0.0f;
    vertices[44].nz = 0.0f;

    // t15 - 19 on the die
    // indices 4, 11, 10

    vertices[45].x = vecArray[4][0];
    vertices[45].y = vecArray[4][1];
    vertices[45].z = vecArray[4][2];
    vertices[45].r = 255;
    vertices[45].g = 255;
    vertices[45].b = 255;
    vertices[45].u = 0.0f;
    vertices[45].v = 0.0f;
    vertices[45].nx = 0.0f;
    vertices[45].ny = 0.0f;
    vertices[45].nz = 0.0f;
    
    vertices[46].x = vecArray[11][0];
    vertices[46].y = vecArray[11][1];
    vertices[46].z = vecArray[11][2];
    vertices[46].r = 255;
    vertices[46].g = 255;
    vertices[46].b = 255;
    vertices[46].u = 0.0f;
    vertices[46].v = 0.0f;
    vertices[46].nx = 0.0f;
    vertices[46].ny = 0.0f;
    vertices[46].nz = 0.0f;

    vertices[47].x = vecArray[10][0];
    vertices[47].y = vecArray[10][1];
    vertices[47].z = vecArray[10][2];
    vertices[47].r = 255;
    vertices[47].g = 255;
    vertices[47].b = 255;
    vertices[47].u = 0.0f;
    vertices[47].v = 0.0f;
    vertices[47].nx = 0.0f;
    vertices[47].ny = 0.0f;
    vertices[47].nz = 0.0f;

    // t16 - 1 on the die
    // indices 4, 10, 5

    vertices[48].x = vecArray[4][0];
    vertices[48].y = vecArray[4][1];
    vertices[48].z = vecArray[4][2];
    vertices[48].r = 255;
    vertices[48].g = 255;
    vertices[48].b = 255;
    vertices[48].u = 0.0f;
    vertices[48].v = 0.0f;
    vertices[48].nx = 0.0f;
    vertices[48].ny = 0.0f;
    vertices[48].nz = 0.0f;
    
    vertices[49].x = vecArray[10][0];
    vertices[49].y = vecArray[10][1];
    vertices[49].z = vecArray[10][2];
    vertices[49].r = 255;
    vertices[49].g = 255;
    vertices[49].b = 255;
    vertices[49].u = 0.0f;
    vertices[49].v = 0.0f;
    vertices[49].nx = 0.0f;
    vertices[49].ny = 0.0f;
    vertices[49].nz = 0.0f;

    vertices[50].x = vecArray[5][0];
    vertices[50].y = vecArray[5][1];
    vertices[50].z = vecArray[5][2];
    vertices[50].r = 255;
    vertices[50].g = 255;
    vertices[50].b = 255;
    vertices[50].u = 0.0f;
    vertices[50].v = 0.0f;
    vertices[50].nx = 0.0f;
    vertices[50].ny = 0.0f;
    vertices[50].nz = 0.0f;

    // t17 - 7 on the die
    // indices 5, 10, 9

    vertices[51].x = vecArray[5][0];
    vertices[51].y = vecArray[5][1];
    vertices[51].z = vecArray[5][2];
    vertices[51].r = 255;
    vertices[51].g = 255;
    vertices[51].b = 255;
    vertices[51].u = 0.0f;
    vertices[51].v = 0.0f;
    vertices[51].nx = 0.0f;
    vertices[51].ny = 0.0f;
    vertices[51].nz = 0.0f;
    
    vertices[52].x = vecArray[10][0];
    vertices[52].y = vecArray[10][1];
    vertices[52].z = vecArray[10][2];
    vertices[52].r = 255;
    vertices[52].g = 255;
    vertices[52].b = 255;
    vertices[52].u = 0.0f;
    vertices[52].v = 0.0f;
    vertices[52].nx = 0.0f;
    vertices[52].ny = 0.0f;
    vertices[52].nz = 0.0f;

    vertices[53].x = vecArray[9][0];
    vertices[53].y = vecArray[9][1];
    vertices[53].z = vecArray[9][2];
    vertices[53].r = 255;
    vertices[53].g = 255;
    vertices[53].b = 255;
    vertices[53].u = 0.0f;
    vertices[53].v = 0.0f;
    vertices[53].nx = 0.0f;
    vertices[53].ny = 0.0f;
    vertices[53].nz = 0.0f;

    // t18 - 15 on the die
    // indices 5, 9, 1

    vertices[54].x = vecArray[5][0];
    vertices[54].y = vecArray[5][1];
    vertices[54].z = vecArray[5][2];
    vertices[54].r = 255;
    vertices[54].g = 255;
    vertices[54].b = 255;
    vertices[54].u = 0.0f;
    vertices[54].v = 0.0f;
    vertices[54].nx = 0.0f;
    vertices[54].ny = 0.0f;
    vertices[54].nz = 0.0f;
    
    vertices[55].x = vecArray[9][0];
    vertices[55].y = vecArray[9][1];
    vertices[55].z = vecArray[9][2];
    vertices[55].r = 255;
    vertices[55].g = 255;
    vertices[55].b = 255;
    vertices[55].u = 0.0f;
    vertices[55].v = 0.0f;
    vertices[55].nx = 0.0f;
    vertices[55].ny = 0.0f;
    vertices[55].nz = 0.0f;

    vertices[56].x = vecArray[1][0];
    vertices[56].y = vecArray[1][1];
    vertices[56].z = vecArray[1][2];
    vertices[56].r = 255;
    vertices[56].g = 255;
    vertices[56].b = 255;
    vertices[56].u = 0.0f;
    vertices[56].v = 0.0f;
    vertices[56].nx = 0.0f;
    vertices[56].ny = 0.0f;
    vertices[56].nz = 0.0f;

    // t19 - 12 on the die
    // indices 1, 9, 8

    vertices[57].x = vecArray[1][0];
    vertices[57].y = vecArray[1][1];
    vertices[57].z = vecArray[1][2];
    vertices[57].r = 255;
    vertices[57].g = 255;
    vertices[57].b = 255;
    vertices[57].u = 0.0f;
    vertices[57].v = 0.0f;
    vertices[57].nx = 0.0f;
    vertices[57].ny = 0.0f;
    vertices[57].nz = 0.0f;
    
    vertices[58].x = vecArray[9][0];
    vertices[58].y = vecArray[9][1];
    vertices[58].z = vecArray[9][2];
    vertices[58].r = 255;
    vertices[58].g = 255;
    vertices[58].b = 255;
    vertices[58].u = 0.0f;
    vertices[58].v = 0.0f;
    vertices[58].nx = 0.0f;
    vertices[58].ny = 0.0f;
    vertices[58].nz = 0.0f;

    vertices[59].x = vecArray[8][0];
    vertices[59].y = vecArray[8][1];
    vertices[59].z = vecArray[8][2];
    vertices[59].r = 255;
    vertices[59].g = 255;
    vertices[59].b = 255;
    vertices[59].u = 0.0f;
    vertices[59].v = 0.0f;
    vertices[59].nx = 0.0f;
    vertices[59].ny = 0.0f;
    vertices[59].nz = 0.0f;

    
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
//     unsigned char* imageData = stbi_load("d20.png", &imageWidth, &imageHeight, &numChannels, 0);
    
    // for mac:
     unsigned char* imageData = stbi_load("/Users/carmen/Downloads/OpenGL/Projects/testing/testing/d20.png", &imageWidth, &imageHeight, &numChannels, 0);
    

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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

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
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        
        // Clear the colors in our off-screen framebuffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the shader program that we created
        glUseProgram(program);

        // Use the vertex array object that we created
        glBindVertexArray(vao);
        
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexOrder), indexOrder, GL_STATIC_DRAW);
        
        // Bind tex0 to texture unit 0, and set our tex0 uniform to texture unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex0);
        glUniform1i(glGetUniformLocation(program, "tex0"), current);
        
        // cube 1
        
        
        glm::vec3 lightPos = glm::vec3(2.0f, 0.0f, -20.0f);
//        glm::vec3 lightPos = glm::vec3(-30.0f, -10.0f, -30.0f);
        glUniform3fv(glGetUniformLocation(program, "lightPos"), 1, glm::value_ptr(lightPos));
        
        glm::vec3 lightColor = glm::vec3(1.0f, 0.8f, 0.9f);
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
        
        rotateVar = 0.0f;
        
        glm::mat4 mat = glm::mat4(1.0f);
        mat = glm::translate(mat, glm::vec3(-0.5f, 0.0f, -0.0f));
//        mat = glm::rotate(mat, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));
        mat = glm::rotate(mat, (float)glfwGetTime(), glm::vec3(-1.0f, 1.0f, 1.0f));
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
        glDrawArrays(GL_TRIANGLES, 0, 60);
//        glDrawArrays(GL_TRIANGLE_FAN, 6, 6);
//        glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);
        
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
