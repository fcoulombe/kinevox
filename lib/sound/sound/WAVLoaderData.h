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
#include <stdint.h>
namespace GCL
{
struct WavHeader
{
	char riff[4];
	int32_t fileSize;
	char WAVE[4];
	char fmt[4];
	int32_t pad;
	int16_t pad2;//check mmreg.h (i think?) for other
	// possible format tags like ADPCM
	uint16_t channel;//1 mono, 2 stereo
	uint32_t sampleRate;//like 44100, 22050, etc...
	uint32_t byteRate; //probably won't need this
	uint16_t block_align; //probably won't need this
	uint16_t bitsPerSample; //8 bit or 16 bit file?
	uint8_t id[4]; //read in 'data'
	uint32_t data_size; //how many bytes of sound data we have
};
}
