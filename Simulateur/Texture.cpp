#include"Texture.h"

Texture::Texture(const char* image, const char* texType, GLuint slot, GLenum format, GLenum pixelType)
{
	type = texType;

	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true); //fichier image de gauche a droite et de bas en haut contrairement a openGL on doit donc inverser
	// Reads the image from a file and stores it in bytes
	//recupere l'image dans un  tableau de bytes , pointeur
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	//genere un objet texture OpenGL et nous donne son ID dans la variable ID pour pouvoir s'en servir
	glGenTextures(1, &ID);
	//assigne la texture a une texture unit
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	// Configures the type of algorithm that is used to make the image smaller or bigger
	//algo pour rééchantillonner l'image en fonction des dimensions et de la deformation
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// reglage du mode de repetition de la texture ou des bordures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	//assigne l'image à l'objet opengl , cela met l'image dans la memoire de la carte graphique
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	// genere des mip map , des versions plus petites des textures pour accelerer le rendu et ne pas le faire dans notre boucle
	glGenerateMipmap(GL_TEXTURE_2D);

	//libere la memoire que prend l'image  dans la memoire du CPU
	stbi_image_free(bytes);


	//unbind l'objet OPENGL pour éviter de le modifier 
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const char* image, const char* texType, GLuint slot)
{
	type = texType;
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	switch (numColCh)
	{
	case 4:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes); // 4 canaux donc RGBA ( transparence) 
		break;
	case 3:
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,widthImg,heightImg,0,GL_RGB,GL_UNSIGNED_BYTE,bytes); //3 canaux donc RGB
		break;
	case 1:
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,widthImg,heightImg,0,GL_RED,GL_UNSIGNED_BYTE,bytes); // 1 canal channel RED
		break;
	default:
		throw std::invalid_argument("Types de couleurs non implémentées");
		break;
	}	

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	// recupere l'emplacement de l'uniform , c'est une variable global de shader
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	//le shader doit etre actif pour pouvoir modifier une de ses valeurs
	shader.Activate();
	// change la valeur de l'uniform
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}