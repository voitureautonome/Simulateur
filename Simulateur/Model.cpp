#include "Model.h"
#include <glm/gtx/string_cast.hpp>

Model::Model(const char* file)
{
	std::string text = get_file_contents(file);
	JSON = json::parse(text);

	Model::file = file;
	data = getData();
	Model::model = glm::mat4(1.0f);
	traverseNode(0);
}

void Model::Draw(Shader& shader, Camera& camera)
{
	//std::cout << "Tableau d'indices" << std::endl;
	//for (int i = 0; i < meshes[0].indices.size(); i++)
	//{
	//	std::cout << meshes[0].indices[i];
	//	std::cout << std::endl;

	//}

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		//std::cout << "Mesh numero " << i << std::endl;
		//for (int j = 0; j < meshes[i].vertices.size(); j++)
		//{
		//	std::cout << "Vertex numero " << j << std::endl;
		//	std::cout << glm::to_string(meshes[i].vertices[j].position) << std::endl;
		//}

		meshes[i].Mesh::Draw(shader, camera, glm::translate(matricesMeshes[i], position) * glm::toMat4(rotation));

		//std::cout << glm::to_string(matricesMeshes[i]) << std::endl;
		//std::cout << "Mesh numero " << i << std::endl;
		//for (int j = 0; j < meshes[i].vertices.size(); j++)
		//{
		//	std::cout << "Vertex numero " << j << std::endl;
		//	std::cout << glm::to_string(meshes[i].vertices[j].position) << std::endl;
		//}
	}
}

void Model::loadMesh(unsigned int indMesh)
{
	unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
	unsigned int normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
	unsigned int texAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];

	std::vector<float> posVec = getFloats(JSON["accessors"][posAccInd]);
	std::vector<glm::vec3> positions = groupFloatsVec3(posVec);
	std::vector<float> normalVec = getFloats(JSON["accessors"][normalAccInd]);
	std::vector<glm::vec3> normals = groupFloatsVec3(normalVec);
	std::vector<float> texVec = getFloats(JSON["accessors"][texAccInd]);
	std::vector<glm::vec2> texUVs = groupFloatsVec2(texVec);

	std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
	std::vector<GLuint> indices = getIndices(JSON["accessors"][indAccInd]);
	std::vector<Texture> textures = getTextures();

	meshes.push_back(Mesh(vertices, indices, textures));
}

void Model::traverseNode(unsigned int nextNode, glm::mat4 matrix)
{
	json node = JSON["nodes"][nextNode];

	//decale le modele si une donn�e de translation existe
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	if (node.find("translation") != node.end())
	{
		float transValues[3];
		for (unsigned int i = 0; i < node["translation"].size(); i++)
			transValues[i] = (node["translation"][i]);
		translation = glm::make_vec3(transValues);
	}
	//recupere l'orientation par des quaternions si present
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (node.find("rotation") != node.end())
	{
		float rotValues[4] =
		{
			node["rotation"][3],
			node["rotation"][0],
			node["rotation"][1],
			node["rotation"][2]
		};
		rotation = glm::make_quat(rotValues);
	}
	//recupere l'echelle si elle existe
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	if (node.find("scale") != node.end())
	{
		float scaleValues[3];
		for (unsigned int i = 0; i < node["scale"].size(); i++)
			scaleValues[i] = (node["scale"][i]);
		scale = glm::make_vec3(scaleValues);
	}
	//recupere la matrice si elle existe
	glm::mat4 matNode = glm::mat4(1.0f);
	if (node.find("matrix") != node.end())
	{
		float matValues[16];
		for (unsigned int i = 0; i < node["matrix"].size(); i++)
			matValues[i] = (node["matrix"][i]);
		matNode = glm::make_mat4(matValues);
	}

	// initialisation en matrice identit�
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	//modifie la matrice par les donn�es de translation rotation et echelle pr�c�dentes
	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	//multiplie les matrices ensemble ( ordre important AB!=BA )
	glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

	//test si le noeud contient un maillage et le charge
	if (node.find("mesh") != node.end())
	{
		translationsMeshes.push_back(translation);
		rotationsMeshes.push_back(rotation);
		scalesMeshes.push_back(scale);
		matricesMeshes.push_back(matNextNode);

		loadMesh(node["mesh"]);
	}

	//test si le noeud a des enfants si oui applique la fonction sur ce noeud ( recursif )
	if (node.find("children") != node.end())
	{
		for (unsigned int i = 0; i < node["children"].size(); i++)
			traverseNode(node["children"][i], matNextNode);
	}
}

std::vector<unsigned char> Model::getData()
{
	std::string bytesText;
	std::string uri = JSON["buffers"][0]["uri"];

	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of("/") + 1);
	bytesText = get_file_contents((fileDirectory + uri).c_str());
	std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
	return data;
}

std::vector<float> Model::getFloats(json accessor)
{
	std::vector<float> floatVec;
	unsigned int buffViewInd = accessor.value("bufferView", 1); // pas obligatoire donc on utilise value
	unsigned int count = accessor["count"]; //obligatoire
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	std::string type = accessor["type"]; // obligatoire dans un fichier gltf

	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];
	unsigned int numPerVert;

	if (type == "SCALAR") numPerVert = 1; // en C++ on ne peut pas utiliser un switch sur un string sans enumeration ou fonction de hash pour convertir la chaine :/
	else if (type == "VEC2") numPerVert = 2;
	else if (type == "VEC3") numPerVert = 3;
	else if (type == "VEC4") numPerVert = 4;
	else throw std::invalid_argument("Type invalide type autoris� : ( SCALAR, VEC2, VEC3, or VEC4)");

	unsigned int beginningOfData = byteOffset + accByteOffset;
	unsigned int lengthOfData = count * 4 * numPerVert;
	for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i+=4)
	{
		unsigned char bytes[] = { data[i], data[i+1], data[i+2], data[i+3] };
		float value;
		std::memcpy(&value, bytes, sizeof(float));
		floatVec.push_back(value);
	}

	return floatVec;
}

std::vector<GLuint> Model::getIndices(json accessor)
{
	std::vector<GLuint> indices;

	unsigned int buffViewInd = accessor.value("bufferView", 0);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	unsigned int componentType = accessor["componentType"];

	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];


	//recupere les indices en tenant compte de la longueur du type , aide -> GitHub khronos-gltf , opengl-tutorial
	unsigned int beginningOfData = byteOffset + accByteOffset;
	switch (componentType)
	{
	case 5125:
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i+=4)
		{
			unsigned char bytes[] = { data[i], data[i+1], data[i+2], data[i+3] };
			unsigned int value;
			std::memcpy(&value, bytes, sizeof(unsigned int));
			indices.push_back((GLuint)value);
		}
		break;
	case 5123:
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i+=2)
		{
			unsigned char bytes[] = { data[i], data[i+1] };
			unsigned short value;
			std::memcpy(&value, bytes, sizeof(unsigned short));
			indices.push_back((GLuint)value);
		}
		break;
	case 5122:
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i+=2)
		{
			unsigned char bytes[] = { data[i], data[i+1] };
			short value;
			std::memcpy(&value, bytes, sizeof(short));
			indices.push_back((GLuint)value);
		}
		break;
	default:
		break;
	}
	return indices;
}

std::vector<Texture> Model::getTextures()
{
	std::vector<Texture> textures;

	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	//parcours de toutes les images
	for (unsigned int i = 0; i < JSON["images"].size(); i++)
	{
		//path de l'image courante
		std::string texPath = JSON["images"][i]["uri"];

		//test si la texture a d�ja �t� charg�  dans le cas ou une texture est utilis� plusieurs fois dans le model
		bool skip = false;
		for (unsigned int j = 0; j < loadedTexName.size(); j++)
		{
			if (loadedTexName[j] == texPath)
			{
				textures.push_back(loadedTex[j]);
				skip = true;
				break;
			}
		}

		//si la texture a d�ja �t� charg�e on passe 
		if (!skip)
		{
			//charge la texture diffuse , couleur de base
			if (texPath.find("baseColor") != std::string::npos || texPath.find("diffuse") != std::string::npos)
			{
				Texture diffuse = Texture((fileDirectory + texPath).c_str(), "diffuse", (GLuint)loadedTex.size());
				textures.push_back(diffuse);
				loadedTex.push_back(diffuse);
				loadedTexName.push_back(texPath);
			}
			//charge la texture specular , reflection de la lumiere
			else if (texPath.find("metallicRoughness") != std::string::npos || texPath.find("specular") != std::string::npos)
			{
				Texture specular = Texture((fileDirectory + texPath).c_str(), "specular", (GLuint)loadedTex.size());
				textures.push_back(specular);
				loadedTex.push_back(specular);
				loadedTexName.push_back(texPath);
			}
		}
	}

	return textures;
}

std::vector<Vertex> Model::assembleVertices(std::vector<glm::vec3> positions,std::vector<glm::vec3> normals,std::vector<glm::vec2> texUVs)
{
	std::vector<Vertex> vertices;
	for (int i = 0; i < positions.size(); i++)
	{
		vertices.push_back
		(
			Vertex
			{
				positions[i],
				normals[i],
				glm::vec3(1.0f, 1.0f, 1.0f),
				texUVs[i]
			}
		);
	}
	return vertices;
}


std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> floatVec)
{
	std::vector<glm::vec2> vectors;
	for (int i = 0; i < floatVec.size(); i+=2)
	{
		vectors.push_back(glm::vec2(floatVec[i], floatVec[i+1]));
	}
	return vectors;
}
std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec)
{
	std::vector<glm::vec3> vectors;
	for (int i = 0; i < floatVec.size(); i+=3)
	{
		vectors.push_back(glm::vec3(floatVec[i], floatVec[i+1], floatVec[i+2]));
	}
	return vectors;
}
std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec)
{
	std::vector<glm::vec4> vectors;
	for (int i = 0; i < floatVec.size(); i+=4)
	{
		vectors.push_back(glm::vec4(floatVec[i], floatVec[i+1], floatVec[i+2], floatVec[i+3]));
	}
	return vectors;
}