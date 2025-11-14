#include "ass.h"

#include <iostream>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "textures.h"

VertInd ProcessMesh(aiMesh* mesh)
{
	std::vector<float> Vertices;
	std::vector<unsigned int> Indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertices.push_back(mesh->mVertices[i].x);
        Vertices.push_back(mesh->mVertices[i].y);
        Vertices.push_back(mesh->mVertices[i].z);

        if (mesh->mTextureCoords[0]) {
            Vertices.push_back(mesh->mTextureCoords[0][i].x);
            Vertices.push_back(mesh->mTextureCoords[0][i].y);
        }
        else {
            Vertices.push_back(0.0f);
            Vertices.push_back(0.0f);
        }
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            Indices.push_back(face.mIndices[j]);
        }
    }

    return { Vertices, Indices };
}


std::vector<VertInd> Ass::Import()
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile("C:\\Users\\lavelle.t\\Projects\\Personal\\TowerRPG\\models\\chest\\wooden_crate_01_4k.gltf", aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || !scene->mRootNode) {
		std::cerr << "Error loading GLTF: " << importer.GetErrorString() << std::endl;
        return std::vector<VertInd>{};
	}

    aiMesh* mesh = scene->mMeshes[0];

    std::vector<VertInd> meshes;

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        meshes.push_back(ProcessMesh(mesh));
    }

    return meshes;
}
