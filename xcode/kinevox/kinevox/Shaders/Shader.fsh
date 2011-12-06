//
//  Shader.fsh
//  kinevox
//
//  Created by Francois Coulombe on 12/4/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
