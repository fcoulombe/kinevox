uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

uniform int CurrentFrame;
uniform int FrameCount;
uniform ivec2 Dimension;
uniform ivec2 TextureSize;

attribute vec4 InPosition;
attribute vec2 InTexCoord;    

varying vec2 texcoord;  	

void main()          
{                    
	int width = TextureSize.x;
	int height = TextureSize.y;
	int framePerWidth = width/Dimension.x;
	int rowPerTexture = height/Dimension.y;

	int whatRow = CurrentFrame/framePerWidth;
	int whatCol = int(mod(CurrentFrame,framePerWidth));
	int whatTexture = whatRow/rowPerTexture;
	
	whatRow = whatRow - (whatTexture*rowPerTexture);

	float widthRatio = float(Dimension.x)/float(width);
	float heightRatio = float(Dimension.y)/float(height);
	
	texcoord.x = (whatCol*widthRatio) + (widthRatio*InTexCoord.x);
	texcoord.y = (1.0-whatRow*heightRatio) - (heightRatio*InTexCoord.y);
	
    gl_Position = ProjectionMatrix * ModelViewMatrix * InPosition; 
}                           