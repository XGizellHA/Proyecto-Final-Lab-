#pragma once
#include<glew.h>

class Texture
{
public:
	Texture();
	Texture(const char* FileLoc);
	bool LoadTexture(); //Se llama cuando no tiene canal alpha
	bool LoadTextureA(); //Se llama cuando la textura tiene canal alpha
	void UseTexture();
	void ClearTexture();
	~Texture();
private: 
	GLuint textureID;
	int width, height, bitDepth;
	const char *fileLocation;

};

