#version 330

// Vertex position
layout(location = 0) in vec3 vertexPosition;

// Vertex color
layout(location = 1) in vec3 vertexColor;

// Vertex UV coordinate
layout(location = 2) in vec2 vertexUV;

// Vertex normals
layout(location = 3) in vec3 vertexNormal;

// UV coordinate (will be passed to the fragment shader)
out vec2 outUV;

// Color (will be passed to the fragment shader)
out vec3 outColor;

// Normal
out vec3 outNormal;

out vec3 outPos;

uniform mat4 mat;
uniform mat4 persp;
uniform mat4 view;

void main()
{
    gl_Position = mat * vec4(vertexPosition, 1.0);
    outUV = vertexUV;
    outColor = vertexColor;
    outNormal = mat3(transpose(inverse(mat))) * vertexNormal;
    outPos = vec3(mat * vec4(vertexPosition, 1.0));
}
