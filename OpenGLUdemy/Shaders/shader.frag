 #version 330 
 in vec4 VertexColor; 
 in vec2 TextureCoordinate;

 out vec4 colour;

 uniform sampler2D textureSampler;

 void main()
 {
     colour = texture(textureSampler,TextureCoordinate);
 };