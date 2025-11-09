#version 330 core
layout (location = 0) in vec3 position;
uniform mat4 mvp;
void main() {
    gl_Position = mvp * vec4(position, 1);
}

// SHADER DELIMITER

#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
