#ifndef GLTEXTURE_H
#define GLTEXTURE_H

class GLTexture  
{
public:
	char *texturename;								// The textures name
	GLuint texture[1];						// OpenGL's number for the texture
	int width;										// Texture's width
	int height;										// Texture's height
	void Use();										// Binds the texture for use
	void BuildColorTexture(unsigned char r, unsigned char g, unsigned char b);	// Sometimes we want a texture of uniform color
	void LoadTGAResource(char *name);				// Load a targa from the resources
	void LoadBMPResource(char *name);				// Load a bitmap from the resources
	void LoadFromResource(char *name);				// Load the texture from a resource
	void LoadTGA(char *name);						// Loads a targa file
	void LoadBMP(char *name);						// Loads a bitmap file
	GLuint LoadTexImage( char* theFileName);
	void Load(char *name);							// Load the texture
	GLTexture();									// Constructor
	virtual ~GLTexture();							// Destructor

};

#endif GLTEXTURE_H
