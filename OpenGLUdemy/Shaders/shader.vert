#version 330 
 layout(location = 0) in vec3 pos;
 layout(location = 1) in vec2 textureCoord;
 layout(location = 2) in vec3 vertexNormal;
 
 out vec2 TextureCoordinate;
 out vec3 FragPos;
 out vec3 Normal;
 
 uniform mat4 MVP;
 uniform mat4 model;

 void main()
 {
	gl_Position = MVP * vec4(pos, 1.0);
	TextureCoordinate = textureCoord;
	FragPos = (model * vec4(pos, 1.0f)).xyz;
	Normal = vertexNormal;
 }