#pragma once


//this is a define to ease string concatenation at compile time.
//the compiler should be able to figure this out at compile time
//with char pointers but who knows... maybe they didn't feel like it
#define DATA_PATH  "data/"
#define TEXTURE_PATH  DATA_PATH"Texture/"
#define MATERIAL_PATH DATA_PATH"Material/"
