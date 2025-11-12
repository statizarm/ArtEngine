#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in mat4 instanceMatrix;

void main() {
    gl_Position = instanceMatrix * vec4(position, 1);
}

// SHADER DELIMITER

#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
