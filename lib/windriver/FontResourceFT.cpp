/*
* Copyright (C) 2011 by Francois Coulombe
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:

* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.

* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/
#ifndef USE_SDL
#include "windriver/FontResource.h"
#include <gcl/Assert.h>
#include <3rdparty/FreeType.h>

using namespace GCL;



namespace GCL
{
    extern FT_Library  library;
    class pFontResource
    {
    public:
        pFontResource(const char *fontName)
        {
            std::string path = Resource::GetResourcePath();
            path += fontName;

            std::string msg("trying to loads " );
            msg += path.c_str();
            msg += "\n";

            FT_Error error = FT_New_Face( library,
                fontName,
                0,
                &face );FTErrorCheck(error);


            // Select the unicode character map
            error = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
            FTErrorCheck(error);

            error = FT_Set_Pixel_Sizes(
                face,   /* handle to face object */
                0,      /* pixel_width           */
                18 );   /* pixel_height          */
            FTErrorCheck(error);

        }

        ~pFontResource()
        {
            FT_Error err = FT_Done_Face(face);
            FTErrorCheck(err);
        }

        const FT_Face *GetFace() const { return &face; }
    private:
        FT_Face face;

    };

    FontResource::FontResource( const char *fontName )
        : Resource()
    {
        mpImpl = new pFontResource(fontName);
    }

    FontResource::~FontResource()
    {
        delete mpImpl;
    }


    const void *FontResource::GetFace() const
    {
        const FT_Face *tempFace = mpImpl->GetFace();
        return tempFace;
    }

}
#endif