#version 330 core
uniform mat4 u_MVPMatrix;
uniform mat4 u_MVMatrix;
uniform vec4 u_DiffuseColour; 

attribute vec3 position;
attribute vec2 textureCoords;
attribute vec3 normal;
// // attribute float testtt;

uniform sampler2D textureSampler;
varying vec4 v_Position;
varying vec3 v_Normal;
varying vec2 v_TextureCoords;

void main() {
   v_Position = vec4(u_MVMatrix * vec4(position,1));
   v_Normal = vec3(u_MVMatrix * vec4(normal, 0.0));
   // v_Normal = vec3(1,1,1);

   v_TextureCoords = textureCoords;

   gl_Position = u_MVPMatrix * vec4(position,1);
}
