uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
attribute vec4 InPosition;   
attribute vec4 InNormal;   
attribute vec2 InTexCoord; 
varying vec2 texcoord;  
varying vec4 color;  
void main()          
{                    
    gl_Position = ProjectionMatrix * ModelViewMatrix * InPosition; 
	texcoord = InTexCoord.xy;
	color = InNormal; //vec4(InTexCoord.x,InTexCoord.y, 0.0,1.0); 
}                           