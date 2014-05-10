uniform sampler2D texture; 	
varying vec2 texcoord;	
varying vec4 color;	
void main() 
{
	gl_FragColor = texture2D(texture, texcoord)+(color-color);
}