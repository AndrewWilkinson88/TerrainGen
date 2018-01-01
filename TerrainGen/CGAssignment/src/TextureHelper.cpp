#include "TextureHelper.h"


TextureHelper::TextureHelper(void)
{
}


TextureHelper::~TextureHelper(void)
{
}

GLbyte* TextureHelper::gltReadBMPBits(const char *szFileName, int *nWidth, int *nHeight)
{
	FILE*	pFile;
	BMPInfo *pBitmapInfo = NULL;
	unsigned long lInfoSize = 0;
	unsigned long lBitSize = 0;
	GLbyte *pBits = NULL;			// Bitmaps bits
	BMPHeader	bitmapHeader;

    // Attempt to open the file
    fopen_s(&pFile, szFileName, "rb" );
    if(pFile == NULL)
        return NULL;

	// File is Open. Read in bitmap header information
    fread(&bitmapHeader, sizeof(BMPHeader), 1, pFile);

	// Read in bitmap information structure
	lInfoSize = bitmapHeader.offset - sizeof(BMPHeader);
	pBitmapInfo = (BMPInfo *) malloc(sizeof(GLbyte)*lInfoSize);
	if(fread(pBitmapInfo, lInfoSize, 1, pFile) != 1)
		{
		free(pBitmapInfo);
		fclose(pFile);
		return false;
		}

	// Save the size and dimensions of the bitmap
	*nWidth = pBitmapInfo->header.width;
	*nHeight = pBitmapInfo->header.height;
	lBitSize = pBitmapInfo->header.imageSize;

	// If the size isn't specified, calculate it anyway	
	if(pBitmapInfo->header.bits != 24)
		{
		free(pBitmapInfo);
		return false;
		}

	if(lBitSize == 0)
		lBitSize = (*nWidth *
           pBitmapInfo->header.bits + 7) / 8 *
  		  abs(*nHeight);

	// Allocate space for the actual bitmap
	free(pBitmapInfo);
	pBits = (GLbyte*)malloc(sizeof(GLbyte)*lBitSize);

	// Read in the pixmap bits, check for corruption
	if(fread(pBits, lBitSize, 1, pFile) != 1)
		{
		free(pBits);
		pBits = NULL;
		}

	// Close the bitmap file now that we have all the data we need
	fclose(pFile);

	return pBits;
}

bool TextureHelper::LoadBMPTexture( const char *fileName )
{
    GLbyte *pBits;
    GLint iWidth, iHeight;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    pBits = gltReadBMPBits(fileName, &iWidth, &iHeight);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, pBits);

    return true;
}