//#version 430 core
//
//layout (location = 0) in vec3 VertexPosition;
//layout (location = 1) in vec2 VertexTex;
//layout (location = 2) in vec3 VertexNormal;
// 
//out Data
//{
//	vec3 Position;
//	vec3 Normal;
//	vec2 TexCoord;
//} data;
// 
//uniform mat4 ModelViewMatrix;
//uniform mat4 NormalMatrix;
//uniform mat4 MVP;
// 
//void main()
//{
//	data.Normal = normalize( NormalMatrix * vec4(VertexNormal, 0) ).xyz;
//	data.Position = vec3( ModelViewMatrix * vec4( VertexPosition, 1 ) );
//	data.TexCoord = VertexTex;
// 
//	gl_Position = MVP * vec4( VertexPosition, 1 );
//}
//
#version 430 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTex;
layout (location = 2) in vec3 VertexNormal;
layout (location = 3) in mat4 MVP;                                                  
layout (location = 7) in mat4 ModelViewMatrix; 
layout (location = 11) in mat4 NormalMatrix;
 
out Data
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoord;
} data;

 
void main()
{
	data.Normal = normalize( NormalMatrix * vec4(VertexNormal, 0) ).xyz;
	data.Position = vec3( ModelViewMatrix * vec4( VertexPosition, 1 ) );
	data.TexCoord = VertexTex;
 
	gl_Position = MVP * vec4( VertexPosition, 1 );
	gl_Position = vec4( VertexPosition, 1 );
}
