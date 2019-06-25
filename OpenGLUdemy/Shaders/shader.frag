 #version 330 
 in vec4 VertexColor; 
 in vec2 TextureCoordinate;
 in vec3 FragPos;
 in vec3 Normal;

 out vec4 colour;

 //Luz puntual
 uniform vec3 lightColor;
 uniform float ambientIntensity;
 uniform vec3 lightPosition;
 uniform float diffuseIntensity;

 uniform vec3 eyePosition;

 uniform sampler2D textureSampler;

 float shininess = 83.2;
 float specularIntensity = 0.79;

 vec4 CalculateLight()
 {
     vec4 ambientColor = vec4(lightColor, 1.0f) * ambientIntensity;

     //Calcular la dirección de mi luz puntual
     vec3 direction = FragPos - lightPosition;
     float distance = length(direction);
     direction = normalize(direction);

     float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
     vec4 diffuseColor = vec4(lightColor, 1.0f) * diffuseIntensity * diffuseFactor;

     //Calcular la especular
     vec4 specularColor = vec4(0,0,0,0);
     if(diffuseFactor > 0.0f)
     {
          vec3 fragToEye = normalize(eyePosition - FragPos);
          vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));
          float specularFactor = dot(fragToEye, reflectedVertex);
          if(specularFactor > 0.0f)
          {
             specularFactor = pow(specularFactor, shininess);
             specularColor = vec4(lightColor * specularIntensity * specularFactor, 1.0f);
          }
     }

     return (ambientColor + diffuseColor + specularColor);
 }

 void main()
 {
     vec4 light = CalculateLight();
     colour = texture(textureSampler,TextureCoordinate) * light;
 };