#include"shaderClass.h"

// lit un fichier texte et convertit son contenu en string
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

//compile le vertex et fragment shader
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	//lit leurs fichiers
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// converti les string en tableaux de char
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// cree un objet opengl de vertex Shader et on garde son identifiant/reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// lie le code source a l'objet
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile le vertex shader en code machine
	glCompileShader(vertexShader);
	erreurCompilation(vertexShader, "VERTEX");

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	erreurCompilation(fragmentShader, "FRAGMENT");
	erreurCompilation(ID, "PROGRAM");

	// cree un objet shader
	ID = glCreateProgram();
	// ajoute le vertex et fragment shader a cette objet
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	//lie les programmes
	glLinkProgram(ID);

	//supprime les objets qui sont maintenant inutiles
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

// active le shader
void Shader::Activate()
{
	glUseProgram(ID);
}

// Supprime le shader
void Shader::Delete()
{
	glDeleteProgram(ID);
}

void Shader::erreurCompilation(unsigned int shader, const char* type) {
	GLint hasCompiled=0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
	char infoLog[1024];
	if (hasCompiled == GL_TRUE) return;
	if (type != "PROGAM") {
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::cout << "ERREUR COMPILATION SHADER : " << type << std::endl;
	}
	else {
		glad_glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERREUR DE LIEN" << type << std::endl;
		}
	}
}