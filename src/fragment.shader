// #version 330 core
// in vec3 ourCol;
// out vec4 FragColor;
// void main()
// {
// 	FragColor = vec4(ourCol, 1.0);
// }
#version 330 core
out vec4 FragColor;

in vec2 ourCol;
void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = vec4(ourCol, 1.0, 1.0);
}