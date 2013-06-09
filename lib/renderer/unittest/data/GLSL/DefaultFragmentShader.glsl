uniform sampler2D texture; 	
varying vec2 texcoord;	
varying vec4 color;	
void main() 
{
#if ENABLE_FIX_PIPELINE 
	gl_FragColor = texture2D( texture, gl_TexCoord[0].st ); \n"
#else 
	gl_FragColor = texture2D(texture, texcoord);\n"
#endif 
}