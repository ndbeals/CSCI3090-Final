#version 330
uniform vec3 u_EyePosition;
uniform vec3 u_LightPos;
uniform int u_hasTexture;

uniform mat4 u_MVPMatrix;
uniform mat4 u_MVMatrix;

// uniform vec4 u_AmbientColour;
uniform vec4 u_DiffuseColour;
// // uniform vec4 u_SpecularColour;

uniform float u_Shininess;
uniform sampler2D textureSampler;

varying vec4 v_Position;
varying vec3 v_Normal;
varying vec2 v_TextureCoords;

void main() {
   vec3 positionViewSpace = vec3(u_MVMatrix*v_Position); 
   vec3 lightViewSpace = vec3(u_MVMatrix*vec4(u_LightPos,1));
   vec3 eyeViewSpace = vec3(u_MVMatrix*vec4(u_EyePosition,1));
   

   float distance = length(lightViewSpace - v_Position.xyz);

   vec3 lightVector_viewspace = normalize(lightViewSpace - vec3(v_Position));
   vec3 normal = normalize(v_Normal);

   vec4 baseColour;
   if (u_hasTexture>-1)
   {
      baseColour = vec4(texture(textureSampler, v_TextureCoords).rgb, 1.0);
   }
   else
   {
      baseColour = u_DiffuseColour*0.8;
   }
      // baseColour = vec4(texture(textureSampler, v_TextureCoords).rgb, 1.0);
   // ambient
   vec4 ambientColour = u_DiffuseColour*0.1f;//u_AmbientColour*0.1f;//vec4(0.1, 0.1, 0.1, 1.0) * u_DiffuseColour;

   // diffuse
   float diffuse = clamp(dot(normal, lightVector_viewspace), 0, 1);

   // attenuate
   //diffuse = diffuse * (1.0 / (1.0 + (0.00025 * distance * distance)));

   // specular
   vec3 incidenceVector = -lightVector_viewspace;
   vec3 reflectionVector = reflect(incidenceVector, normal);
   vec3 eyeVector = normalize(eyeViewSpace - v_Position.xyz);
   float cosAngle = clamp(dot(eyeVector, reflectionVector), 0, 1);
   float specular = clamp(pow(cosAngle, u_Shininess),0.0f,200.0f);

   gl_FragColor = baseColour * specular +
                  baseColour * diffuse +
                  ambientColour; //+ vec4(v_TextureCoords.x,v_TextureCoords.y,0.1,1);
   // gl_FragColor = vec4(0.8,0.8,0.8,1);

   // gl_FragColor = baseColour;
   // gl_FragColor = vec4(v_TextureCoords.x,v_TextureCoords.y,0.5,1);


   // gl_FragColor = vec4(0.5, 0.9, 0.0, 1.0);
}
