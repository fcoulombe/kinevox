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

#pragma once
#include <cstdlib>
#include <cstring>
#include <stdint.h>
#include <map>
#include <vector>
#include <gcl/Rect.h>
#include <gcl/PixelBuffer.h>
#include <gcl/Point2.h>
#include <gcl/Point4.h>

namespace GCL
{
    struct Row
    {
        Row(uint32_t rowTop, uint32_t rowHeight)
            : width(0), top(rowTop), height(rowHeight) {}

        uint32_t width;  ///< Current width of the row
        uint32_t top;    ///< Y position of the row into the texture
        uint32_t height; ///< Height of the row
    };
    class Glyph
    {
    public :
        Glyph() : advance(0) {}
        int     advance;     ///< Offset to move horizontically to the next character
        Rect<int> bounds;      ///< Bounding rectangle of the glyph, in coordinates relative to the baseline
        Rect<int> textureRect; ///< Texture coordinates of the glyph inside the font's texture
    };
    typedef std::map<uint32_t, Glyph> GlyphTable; ///< Table mapping a codepoint to its glyph

    struct Page
    {
        Page();

        GlyphTable       glyphs;  ///< Table mapping code points to their corresponding glyph
        PixelBuffer      texture; ///< Texture containing the pixels of the glyphs
        uint32_t     nextRow; ///< Y position of the next new row in the texture
        std::vector<Row> rows;    ///< List containing the position of all the existing rows
    };

    typedef std::map<size_t, Page> PageTable; ///< Table mapping a character size to its page (texture)

class FontResource;
class PixelBuffer;
class TTFFont
{
public:
	TTFFont(const char *fontName);
	~TTFFont();
	void BlitText(PixelBuffer &buffer, const char * text, size_t fontSize, size_t x, size_t y) ;
    Glyph LoadGlyph(char glyph, size_t characterSize, bool bold) ;
    const Glyph& GetGlyph(uint32_t codePoint, size_t characterSize, bool bold=false) ;
    int GetKerning(uint32_t first, uint32_t second) const;
    Rect<int> FindGlyphRect(Page& page, unsigned int width, unsigned int height) const;
    void SetFontSize(size_t fontSize);
    int GetLineSpacing() const;

    PageTable          m_pages;       ///< Table containing the glyphs pages by character size

private:
    Point2<size_t> GetBufferSize(const char *text, size_t fontSize) ;
	const FontResource *mFontResource;
   // mutable std::vector<Uint8> m_pixelBuffer; ///< Pixel buffer holding a glyph's pixels before being written to the texture
};
}
