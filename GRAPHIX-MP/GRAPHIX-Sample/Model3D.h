#pragma once
#include <string>
#include <iostream>


class Model3D {
public:

    std::vector<GLfloat> fullVertexData;

    GLuint newTexture;


	Model3D() {


	}

    /* Loads the Model's fullVectorData */
    void loadModel(std::string objectPath) {

        std::string path = objectPath;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warning, error;

        tinyobj::attrib_t attributes;

        bool success = tinyobj::LoadObj(
            &attributes,
            &shapes,
            &materials,
            &warning,
            &error,
            path.c_str()
        );

        std::vector<GLuint> mesh_indices1;
        for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
            mesh_indices1.push_back(shapes[0].mesh.indices[i].vertex_index);
        }

        std::vector<glm::vec3> tangents;
        std::vector<glm::vec3> bitangents;

        for (int i = 0; i < shapes[0].mesh.indices.size(); i += 3) {
            tinyobj::index_t vData1 = shapes[0].mesh.indices[i];
            tinyobj::index_t vData2 = shapes[0].mesh.indices[i + 1];
            tinyobj::index_t vData3 = shapes[0].mesh.indices[i + 2];

            glm::vec3 v1 = glm::vec3(
                attributes.vertices[vData1.vertex_index * 3],
                attributes.vertices[vData1.vertex_index * 3 + 1],
                attributes.vertices[vData1.vertex_index * 3 + 2]
            );

            glm::vec3 v2 = glm::vec3(
                attributes.vertices[vData2.vertex_index * 3],
                attributes.vertices[vData2.vertex_index * 3 + 1],
                attributes.vertices[vData2.vertex_index * 3 + 2]
            );

            glm::vec3 v3 = glm::vec3(
                attributes.vertices[vData3.vertex_index * 3],
                attributes.vertices[vData3.vertex_index * 3 + 1],
                attributes.vertices[vData3.vertex_index * 3 + 2]
            );

            glm::vec2 uv1 = glm::vec2(
                attributes.texcoords[vData1.texcoord_index * 2],
                attributes.texcoords[vData1.texcoord_index * 2 + 1]
            );

            glm::vec2 uv2 = glm::vec2(
                attributes.texcoords[vData2.texcoord_index * 2],
                attributes.texcoords[vData2.texcoord_index * 2 + 1]
            );

            glm::vec2 uv3 = glm::vec2(
                attributes.texcoords[vData3.texcoord_index * 2],
                attributes.texcoords[vData3.texcoord_index * 2 + 1]
            );

            glm::vec3 deltaPos1 = v2 - v1;
            glm::vec3 deltaPos2 = v3 - v1;

            glm::vec2 deltaUV1 = uv2 - uv1;
            glm::vec2 deltaUV2 = uv3 - uv1;

            float r = 1.f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));

            glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
            glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

            tangents.push_back(tangent);
            tangents.push_back(tangent);
            tangents.push_back(tangent);

            bitangents.push_back(bitangent);
            bitangents.push_back(bitangent);
            bitangents.push_back(bitangent);

        }

        for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
            tinyobj::index_t vData = shapes[0].mesh.indices[i];

            int vertexIndex = vData.vertex_index * 3;
            int uvIndex = vData.texcoord_index * 2;
            int normalIndex = vData.normal_index * 3;

            // X
            this->fullVertexData.push_back(
                attributes.vertices[vertexIndex]
            );

            // Y
            this->fullVertexData.push_back(
                attributes.vertices[vertexIndex + 1]
            );

            // Z
            this->fullVertexData.push_back(
                attributes.vertices[vertexIndex + 2]
            );

            // X
            this->fullVertexData.push_back(
                attributes.normals[normalIndex]
            );

            // Y
            this->fullVertexData.push_back(
                attributes.normals[normalIndex + 1]
            );

            // Z
            this->fullVertexData.push_back(
                attributes.normals[normalIndex + 2]
            );

            // U
            this->fullVertexData.push_back(
                attributes.texcoords[uvIndex]
            );

            // V
            this->fullVertexData.push_back(
                attributes.texcoords[uvIndex + 1]
            );

            this->fullVertexData.push_back(
                tangents[i].x
            );

            this->fullVertexData.push_back(
                tangents[i].y
            );

            this->fullVertexData.push_back(
                tangents[i].z
            );

            this->fullVertexData.push_back(
                bitangents[i].x
            );

            this->fullVertexData.push_back(
                bitangents[i].y
            );

            this->fullVertexData.push_back(
                bitangents[i].z
            );

        }

    }

    /* Loads Textures to the Model that has only RGB*/
    void loadTexturesRGB(const char* texturePath) {

        int img_width, img_height, color_channels;

        unsigned char* tex_bytes = stbi_load(texturePath, &img_width, &img_height, &color_channels, 0);

        glGenTextures(1, &newTexture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->newTexture);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB, // GL_RGB = jpegs /pngs w/o a GL_RGBA = png / images w/ a
            img_width,
            img_height,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            tex_bytes
        );

        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(tex_bytes);

        glEnable(GL_DEPTH_TEST);

    }


    /* Loads Textures to the Model that has only RGBA */
    void loadTexturesRGBA(const char* texturePath) {

        int img_width, img_height, color_channels;

        unsigned char* tex_bytes = stbi_load(texturePath, &img_width, &img_height, &color_channels, 0);

        glGenTextures(1, &newTexture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->newTexture);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA, // GL_RGB = jpegs /pngs w/o a GL_RGBA = png / images w/ a
            img_width,
            img_height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            tex_bytes
        );

        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(tex_bytes);

        glEnable(GL_DEPTH_TEST);

    }


};