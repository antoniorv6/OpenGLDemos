#version 330 
 layout(location = 0) in vec3 pos;
 layout(location = 1) in vec2 textureCoord;
 
 out vec2 TextureCoordinate;
 
 uniform mat4 MVP;
 void main()
 {
	gl_Position = MVP * vec4(pos, 1.0);
	TextureCoordinate = textureCoord;
 }