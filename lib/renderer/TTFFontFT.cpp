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

#include "renderer/TTFFont.h"

#include <sstream>
#include <3rdparty/FreeType.h>
#include <gcl/Assert.h>
#include <gcl/PixelBuffer.h>
#include "renderer/FontResource.h"
#include "renderer/FontResourceManager.h"


using namespace GCL;
namespace GCL
{
        extern FT_Library  library;
}



TTFFont::TTFFont(const char *fontName)
{
    const Resource *tempResource = FontResourceManager::Instance().LoadResource(fontName);
    mFontResource = static_cast<const FontResource*>(tempResource);
}

TTFFont::~TTFFont()
{
    FontResourceManager::Instance().ReleaseResource(mFontResource);
}

Rect<int> TTFFont::FindGlyphRect(Page& page, unsigned int width, unsigned int height) const
{
    // Find the line that fits well the glyph
    Row* row = NULL;
    float bestRatio = 0;
    for (std::vector<Row>::iterator it = page.rows.begin(); it != page.rows.end() && !row; ++it)
    {
        float ratio = static_cast<float>(height) / it->height;

        // Ignore rows that are either too small or too high
        if ((ratio < 0.7f) || (ratio > 1.f))
            continue;

        // Check if there's enough horizontal space left in the row
        if (width > page.texture.mWidth - it->width)
            continue;

        // Make sure that this new row is the best found so far
        if (ratio < bestRatio)
            continue;

        // The current row passed all the tests: we can select it
        row = &*it;
        bestRatio = ratio;
    }

    // If we didn't find a matching row, create a new one (10% taller than the glyph)
    if (!row)
    {
        int rowHeight = height + height / 10;
        while (page.nextRow + rowHeight >= page.texture.mHeight)
        {
            // Not enough space: resize the texture if possible
            size_t textureWidth  = page.texture.mWidth;
            size_t textureHeight = page.texture.mHeight;

            // Make the texture 2 times bigger
            PixelBuffer newImage(textureWidth * 2, textureHeight * 2, 4);
            newImage.Blit(page.texture, 0,0);
            delete [] page.texture.mPixels;
            page.texture = newImage;

        }

        // We can now create the new row
        page.rows.push_back(Row(page.nextRow, rowHeight));
        page.nextRow += rowHeight;
        row = &page.rows.back();
    }

    // Find the glyph's rectangle on the selected row
    Rect<int> rect(row->width, row->top, width, height);

    // Update the row informations
    row->width += width;

    return rect;
}

void TTFFont::SetFontSize(size_t fontSize)
{
    // FT_Set_Pixel_Sizes is an expensive function, so we must call it
    // only when necessary to avoid killing performances
    const FT_Face &face = *(const FT_Face *)(mFontResource->GetFace());
    FT_UShort currentSize = face->size->metrics.x_ppem;

    if (currentSize != fontSize)
    {
        FT_Error err = FT_Set_Pixel_Sizes(face, 0, (FT_UInt)fontSize);
        FTErrorCheck(err);
    }
}

int TTFFont::GetKerning(uint32_t first, uint32_t second) const
{
    // Special case where first or second is 0 (null character)
    if (first == 0 || second == 0)
        return 0;

    const FT_Face &face = *(const FT_Face *)(mFontResource->GetFace());

    if (face && FT_HAS_KERNING(face) )
    {
        // Convert the characters to indices
        FT_UInt index1 = FT_Get_Char_Index(face, first);
        FT_UInt index2 = FT_Get_Char_Index(face, second);

        // Get the kerning vector
        FT_Vector kerning;
        FT_Get_Kerning(face, index1, index2, FT_KERNING_DEFAULT, &kerning);

        // Return the X advance
        return kerning.x >> 6;
    }
    else
    {
        // Invalid font, or no kerning
        return 0;
    }
}
const Glyph& TTFFont::GetGlyph(uint32_t codePoint, size_t characterSize, bool bold) 
{
    // Get the page corresponding to the character size
    GlyphTable& glyphs = m_pages[characterSize].glyphs;

    // Build the key by combining the code point and the bold flag
    uint32_t key = ((bold ? 1 : 0) << 31) | codePoint;

    // Search the glyph into the cache
    GlyphTable::const_iterator it = glyphs.find(key);
    if (it != glyphs.end())
    {
        // Found: just return it
        return it->second;
    }
    else
    {
        // Not found: we have to load it
        Glyph glyph = LoadGlyph(codePoint, characterSize, bold);
        return glyphs.insert(std::make_pair(key, glyph)).first->second;
    }
}
Glyph TTFFont::LoadGlyph(uint32_t codepoint, size_t characterSize, bool bold)
{
#if 0
    const FT_Face &face = *(const FT_Face *)(mFontResource->GetFace());
    FT_GlyphSlot  slot = face->glyph;
    FT_UInt glyphIndex = FT_Get_Char_Index( face, glyph );
    FT_Error error;

    error = FT_Load_Glyph( face, glyphIndex, FT_LOAD_RENDER );
    FTErrorCheck(error);

    size_t cellW = slot->metrics.horiAdvance/64;
    size_t cellH = slot->metrics.vertAdvance/64;


    //PixelBuffer *buffer = new PixelBuffer(new PixelRGB[slot->bitmap.rows*slot->bitmap.width], slot->bitmap.width, slot->bitmap.rows);
    PixelBuffer *buffer = new PixelBuffer(new PixelRGB[cellW*cellH], cellW, cellH);
    memset(buffer->mPixels, 0, cellW*cellH*buffer->mBytesPerPixel);

    ftBlitGlyph(slot, *buffer);
    return buffer;
#else
    // The glyph to return
    Glyph glyph;

    // First, transform our ugly void* to a FT_Face
    const FT_Face &face = *(const FT_Face *)(mFontResource->GetFace());
    GCLAssert(face);

    // Set the character size
    SetFontSize(characterSize);

    // Load the glyph corresponding to the code point
    FT_Error err = FT_Load_Char(face, codepoint, FT_LOAD_TARGET_NORMAL);
    FTErrorCheck(err);

    // Retrieve the glyph
    FT_Glyph glyphDesc;
    err = FT_Get_Glyph(face->glyph, &glyphDesc) ;
    FTErrorCheck(err);

    // Apply bold if necessary -- first technique using outline (highest quality)
    FT_Pos weight = 1 << 6;
    bool outline = (glyphDesc->format == FT_GLYPH_FORMAT_OUTLINE);
    if (bold && outline)
    {
        FT_OutlineGlyph outlineGlyph = (FT_OutlineGlyph)glyphDesc;
        FT_Outline_Embolden(&outlineGlyph->outline, weight);
    }

    // Convert the glyph to a bitmap (i.e. rasterize it)
    FT_Glyph_To_Bitmap(&glyphDesc, FT_RENDER_MODE_NORMAL, 0, 1);
    FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyphDesc;
    FT_Bitmap& bitmap = bitmapGlyph->bitmap;

    // Apply bold if necessary -- fallback technique using bitmap (lower quality)
    if (bold && !outline)
    {
        FT_Bitmap_Embolden(static_cast<FT_Library>(library), &bitmap, weight, weight);
    }

    // Compute the glyph's advance offset
    glyph.advance = glyphDesc->advance.x >> 16;
    if (bold)
        glyph.advance += weight >> 6;

    int width  = bitmap.width;
    int height = bitmap.rows;
    if ((width > 0) && (height > 0))
    {
        // Leave a small padding around characters, so that filtering doesn't
        // pollute them with pixels from neighbours
        const unsigned int padding = 1;

        // Get the glyphs page corresponding to the character size
        Page& page = m_pages[characterSize];

        // Find a good position for the new glyph into the texture
        glyph.textureRect = FindGlyphRect(page, width + 2 * padding, height + 2 * padding);

        // Compute the glyph's bounding box
#if 0
        glyph.bounds.x   = bitmapGlyph->left - padding;
        glyph.bounds.y    = -bitmapGlyph->top - padding;
        glyph.bounds.width  = width + 2 * padding;
        glyph.bounds.height = height + 2 * padding;

#else
        glyph.bounds.x   = bitmapGlyph->left;
        glyph.bounds.y    = height-bitmapGlyph->top;
        glyph.bounds.width  = width + 2 * padding;
        glyph.bounds.height = height + 2 * padding;
#endif
        // Extract the glyph's pixels from the bitmap
        PixelBuffer b(width, height, 4);
        uint8_t *m_pixelBuffer = b.mPixels;
        const uint8_t* pixels = bitmap.buffer;
        if (bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
        {
            // Pixels are 1 bit monochrome values
            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    // The color channels remain white, just fill the alpha channel
                    std::size_t index = (x + y * width) * 4 + 3;
                    m_pixelBuffer[index] = ((pixels[x / 8]) & (1 << (7 - (x % 8)))) ? 255 : 0;
                }
                pixels += bitmap.pitch;
            }
        }
        else
        {
            // Pixels are 8 bits gray levels
            for (int y = height-1; y >= 0; --y)
            {
                for (int x = 0; x < width; ++x)
                {
                    // The color channels remain white, just fill the alpha channel
                    std::size_t index = (x + y * width) * 4 ;//+ 3;
                    m_pixelBuffer[index] = pixels[x];
                    m_pixelBuffer[index+1] = pixels[x];
                    m_pixelBuffer[index+2] = pixels[x];
                    m_pixelBuffer[index+3] = 255;
                }
                pixels += bitmap.pitch;
            }
        }

        // Write the pixels to the texture
        unsigned int x = glyph.textureRect.x + padding;
        unsigned int y = glyph.textureRect.y + padding;
        //unsigned int w = glyph.textureRect.width - 2 * padding;
        //unsigned int h = glyph.textureRect.height - 2 * padding;
        page.texture.Blit(b, x, y);
    }

    // Delete the FT glyph
    FT_Done_Glyph(glyphDesc);

    // Done :)
    return glyph;
#endif
}

int TTFFont::GetLineSpacing() const
{
    const FT_Face &face = *(const FT_Face *)(mFontResource->GetFace());
    GCLAssert(face);

    if (face )
    {
        return (face->size->metrics.height >> 6);
    }
    else
    {
        return 0;
    }
}

Point2<size_t> TTFFont::GetBufferSize(const char *text, size_t fontSize) 
{
    bool bold = false;
    //bool underlined = false;
    // Real italic             = (m_style & Italic) ? 0.208f : 0.f; // 12 degrees
    //Real underlineOffset    = fontSize * 0.1f;
    //Real underlineThickness = fontSize * (bold ? 0.1f : 0.07f);
    int hspace = GetGlyph(L' ', fontSize, bold).advance;
    int vspace = GetLineSpacing();
    int x      = 0;
    int y      = (int)fontSize;
    uint32_t prev = 0;
    for (size_t i = 0; i < strlen(text); ++i)
    {
        uint32_t curChar = text[i];

        // Apply the kerning offset
        x += GetKerning(prev, curChar);
        prev = curChar;
        // Handle special characters
        switch (curChar)
        {
        case L' ' :  x += hspace;        continue;
        case L'\t' : x += hspace * 4;    continue;
        case L'\n' : y += vspace; x = 0; continue;
        case L'\v' : y += vspace * 4;    continue;
        }
        // Extract the current glyph's description
        const Glyph& glyph = GetGlyph(curChar, fontSize, bold);

        //int left   = glyph.bounds.x;
        //int top    = glyph.bounds.y;
        //int right  = glyph.bounds.x + glyph.bounds.width;
        //int bottom = glyph.bounds.y  + glyph.bounds.height;
        // Advance to the next character
        x += glyph.advance;
    }
    return Point2<size_t>(x,y);
}
void TTFFont::BlitText(PixelBuffer &buffer, const char * text, size_t fontSize, size_t /*x*/, size_t /*y*/)
{
    Point2<size_t> bufferSize = GetBufferSize(text, fontSize);
    buffer = PixelBuffer(bufferSize.x, bufferSize.y, 4);
    for (size_t x = 0; x < bufferSize.x; ++x)
        for (size_t y = 0; y < bufferSize.y; ++y)
            buffer.SetPixel(x, y, Point4<uint8_t>(0, 0, 0, 255));
   
    bool bold = false;
    //bool underlined = false;
   // Real italic             = (m_style & Italic) ? 0.208f : 0.f; // 12 degrees
    //Real underlineOffset    = fontSize * 0.1f;
    //Real underlineThickness = fontSize * (bold ? 0.1f : 0.07f);
    int hspace = GetGlyph(L' ', fontSize, bold).advance;
    int vspace = GetLineSpacing();
    int x      = 0;
    int y      = 0;

    uint32_t prev = 0;
    for (size_t i = 0; i < strlen(text); ++i)
    {
        uint32_t curChar = text[i];

        // Apply the kerning offset
        x += GetKerning(prev, curChar);
        prev = curChar;

        // If we're using the underlined style and there's a new line, draw a line
#if 0
        if (underlined && (curChar == L'\n'))
        {
            float top = y + underlineOffset;
            float bottom = top + underlineThickness;

            m_vertices.append(Vertex(Vector2f(0, top),    m_color, Vector2f(1, 1)));
            m_vertices.append(Vertex(Vector2f(x, top),    m_color, Vector2f(1, 1)));
            m_vertices.append(Vertex(Vector2f(x, bottom), m_color, Vector2f(1, 1)));
            m_vertices.append(Vertex(Vector2f(0, bottom), m_color, Vector2f(1, 1)));
        }
#endif

        // Handle special characters
        switch (curChar)
        {
        case L' ' :  x += hspace;        continue;
        case L'\t' : x += hspace * 4;    continue;
        case L'\n' : y += vspace; x = 0; continue;
        case L'\v' : y += vspace * 4;    continue;
        }

        // Extract the current glyph's description
        const Glyph& glyph = GetGlyph(curChar, fontSize, bold);
#if 0
        int left   = glyph.bounds.x;
        int top    = glyph.bounds.y;
        int right  = glyph.bounds.x + glyph.bounds.width;
        int bottom = glyph.bounds.y  + glyph.bounds.height;

        float u1 = static_cast<float>(glyph.textureRect.left);
        float v1 = static_cast<float>(glyph.textureRect.top);
        float u2 = static_cast<float>(glyph.textureRect.left + glyph.textureRect.width);
        float v2 = static_cast<float>(glyph.textureRect.top  + glyph.textureRect.height);

        // Add a quad for the current character
        m_vertices.append(Vertex(Vector2f(x + left  - italic * top,    y + top),    m_color, Vector2f(u1, v1)));
        m_vertices.append(Vertex(Vector2f(x + right - italic * top,    y + top),    m_color, Vector2f(u2, v1)));
        m_vertices.append(Vertex(Vector2f(x + right - italic * bottom, y + bottom), m_color, Vector2f(u2, v2)));
        m_vertices.append(Vertex(Vector2f(x + left  - italic * bottom, y + bottom), m_color, Vector2f(u1, v2)));
#else
        Rect<int> clip;
        clip.x = glyph.textureRect.x;
        clip.y = glyph.textureRect.y;
        clip.width = glyph.textureRect.width;
        clip.height = glyph.textureRect.height;


        buffer.Blit(m_pages[fontSize].texture, x, y , clip);
#endif
        // Advance to the next character
        x += glyph.advance;
    }
}

Page::Page() :
    nextRow(2)
{
    // Make sure that the texture is initialized by default
    PixelBuffer image(128, 128, 4);

    // Reserve a 2x2 white square for texturing underlines
    for (int x = 0; x < 2; ++x)
        for (int y = 0; y < 2; ++y)
            image.SetPixel(x, y, Point4<uint8_t>(255, 255, 255, 255));

    // Create the texture
    texture = image;
}
