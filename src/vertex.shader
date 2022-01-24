#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 nigballs;

out vec2 ourCol;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	ourCol = vec2(nigballs.x, nigballs.y);
}

// #version 330 core
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aCol;

// out vec3 ourCol;
// uniform mat4 transform;

// void main()
// {
// 	gl_Position = transform * vec4(aPos, 1.0);
// 	ourCol = aCol;
// }
