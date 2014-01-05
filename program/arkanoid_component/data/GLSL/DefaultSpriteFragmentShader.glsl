
uniform sampler2D texture; 	
varying vec2 texcoord;	

void main() 
{
	vec4 col  = texture2D(texture, texcoord);
	gl_FragColor = col;
	//gl_FragColor= vec4(texcoord.x, 0, texcoord.y, 1);
}