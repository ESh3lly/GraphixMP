#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float x = 0.0f, y = 0.0f, z = 0.0f;

float scale_x = 5.f, scale_y = 5.f, scale_z = 5.f;

float rot_x = 1, rot_y = 0, rot_z = 0;

float theta = 0.0f;


void Key_Callback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods
) {
    // Movement && Scaling

    switch (key) {
    case GLFW_KEY_W:
        y += 0.2f;
        break;
    case GLFW_KEY_A:
        x -= 0.2f;
        break;
    case GLFW_KEY_S:
        y -= 0.2f;
        break;
    case GLFW_KEY_D:
        x += 0.2f;
        break;
    case GLFW_KEY_Q:
        scale_x += 0.5f;
        scale_y += 0.5f;
        scale_z += 0.5f;
        break;
    case GLFW_KEY_E:
        scale_x -= 0.5f;
        scale_y -= 0.5f;
        scale_z -= 0.5f;
        break;
    }

    // Rotation
    switch (key) {
    case GLFW_KEY_RIGHT:
        theta += 7.0f;
        rot_y = 1;
        rot_x = 0;
        break;

    case GLFW_KEY_LEFT:
        theta -= 7.0f;
        rot_y = 1;
        rot_x = 0;
        break;

    case GLFW_KEY_UP:
        theta -= 7.0f;
        rot_y = 0;
        rot_x = 1;
        break;

    case GLFW_KEY_DOWN:
        theta += 7.0f;
        rot_y = 0;
        rot_x = 1;
        break;

    }

}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    float screenWidth = 800.0f;
    float screenHeight = 800.0f;

    window = glfwCreateWindow(screenWidth, screenHeight, "Jerickson Lee", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, Key_Callback);

    glfwMakeContextCurrent(window);

    gladLoadGL();

    stbi_set_flip_vertically_on_load(true);

    glViewport(0, 0, screenWidth, screenHeight);

    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &v, NULL);
    glCompileShader(vertShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);

    glLinkProgram(shaderProgram);

    /////////////////

    std::fstream skybox_vertSrc("Shaders/skybox.vert");
    std::stringstream skybox_vertBuff;
    skybox_vertBuff << skybox_vertSrc.rdbuf();
    std::string skybox_vertS = skybox_vertBuff.str();
    const char* skybox_v = skybox_vertS.c_str();

    std::fstream skybox_fragSrc("Shaders/skybox.frag");
    std::stringstream skybox_fragBuff;
    skybox_fragBuff << skybox_fragSrc.rdbuf();
    std::string skybox_fragS = skybox_fragBuff.str();
    const char* skybox_f = skybox_fragS.c_str();

    GLuint skybox_vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(skybox_vertShader, 1, &skybox_v, NULL);
    glCompileShader(skybox_vertShader);

    GLuint skybox_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(skybox_fragShader, 1, &skybox_f, NULL);
    glCompileShader(skybox_fragShader);

    GLuint skybox_shaderProgram = glCreateProgram();
    glAttachShader(skybox_shaderProgram, skybox_vertShader);
    glAttachShader(skybox_shaderProgram, skybox_fragShader);

    glLinkProgram(skybox_shaderProgram);

    float skyboxVertices[]{
        -1.f, -1.f, 1.f, //0
        1.f, -1.f, 1.f,  //1
        1.f, -1.f, -1.f, //2
        -1.f, -1.f, -1.f,//3
        -1.f, 1.f, 1.f,  //4
        1.f, 1.f, 1.f,   //5
        1.f, 1.f, -1.f,  //6
        -1.f, 1.f, -1.f  //7
    };

    //Skybox Indices
    unsigned int skyboxIndices[]{
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    };

    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_INT) * 36, &skyboxIndices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    std::string facesSkybox[]{
        "Skybox/rainbow_rt.png",
        "Skybox/rainbow_lf.png",
        "Skybox/rainbow_up.png",
        "Skybox/rainbow_dn.png",
        "Skybox/rainbow_ft.png",
        "Skybox/rainbow_bk.png",
    };

    unsigned int skyboxTex;

    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++) {

        int w, h, skyCChannel;

        stbi_set_flip_vertically_on_load(false);

        unsigned char* data = stbi_load(facesSkybox[i].c_str(), &w, &h, &skyCChannel, 0);

        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }

        stbi_image_free(data);
    }


    stbi_set_flip_vertically_on_load(true);


    std::string path = "3D/plane.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    GLfloat UV[]{
        0.f, 1.f,
        0.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        0.f, 1.f,
        0.f, 0.f
    };

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &materials,
        &warning,
        &error,
        path.c_str()
    );

    int img_width, img_height, color_channels;

    unsigned char* tex_bytes = stbi_load("3D/brickwall.jpg", &img_width, &img_height, &color_channels, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(
        GL_TEXTURE_2D,
        0, // Texture 0
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

    /////////////

    unsigned char* norm_bytes = stbi_load("3D/brickwall_normal.jpg", &img_width, &img_height, &color_channels, 0);

    GLuint norm_tex;
    glGenTextures(1, &norm_tex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, norm_tex);

    glTexImage2D(
        GL_TEXTURE_2D,
        0, // Texture 0
        GL_RGB, // GL_RGB = jpegs /pngs w/o a GL_RGBA = png / images w/ a
        img_width,
        img_height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        norm_bytes
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(norm_bytes);


    glEnable(GL_DEPTH_TEST);


    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );
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



    std::vector<GLfloat> fullVertexData;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];

        int vertexIndex = vData.vertex_index * 3;
        int uvIndex = vData.texcoord_index * 2;
        int normalIndex = vData.normal_index * 3;

        // X
        fullVertexData.push_back(
            attributes.vertices[vertexIndex]
        );

        // Y
        fullVertexData.push_back(
            attributes.vertices[vertexIndex + 1]
        );

        // Z
        fullVertexData.push_back(
            attributes.vertices[vertexIndex + 2]
        );

        // X
        fullVertexData.push_back(
            attributes.normals[normalIndex]
        );

        // Y
        fullVertexData.push_back(
            attributes.normals[normalIndex + 1]
        );

        // Z
        fullVertexData.push_back(
            attributes.normals[normalIndex + 2]
        );

        // U
        fullVertexData.push_back(
            attributes.texcoords[uvIndex]
        );

        // V
        fullVertexData.push_back(
            attributes.texcoords[uvIndex + 1]
        );

        fullVertexData.push_back(
            tangents[i].x
        );

        fullVertexData.push_back(
            tangents[i].y
        );

        fullVertexData.push_back(
            tangents[i].z
        );

        fullVertexData.push_back(
            bitangents[i].x
        );

        fullVertexData.push_back(
            bitangents[i].y
        );

        fullVertexData.push_back(
            bitangents[i].z
        );

    }

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * fullVertexData.size(), fullVertexData.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)0);

    GLintptr normPtr = 3 * sizeof(GL_FLOAT);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void*)normPtr
    );

    GLintptr uvPtr = 6 * sizeof(GL_FLOAT);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)uvPtr);

    GLintptr tangentPtr = 8 * sizeof(GL_FLOAT);
    GLintptr bitangentPtr = 11 * sizeof(GL_FLOAT);

    glVertexAttribPointer(
        3,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void*)tangentPtr
    );

    glVertexAttribPointer(
        4,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void*)bitangentPtr
    );

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm::mat4 projection_matrix = glm::perspective(
        glm::radians(60.0f),
        screenHeight / screenWidth,
        0.1f,
        100.0f
    );

    glm::vec3 lightPos = glm::vec3(-1, 5, 1);
    glm::vec3 lightColor = glm::vec3(1, 1, 1);

    float ambientStr = 0.35f;
    glm::vec3 ambientColor = lightColor;

    float specStr = 0.5f; // 0.5
    float specPhong = 16.0f; // 16

    float constant = 1.0f;
    float linear = 0.1f;
    float quadratic = 0.04f;


    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);


        glm::vec3 cameraPos = glm::vec3(0, 0, 10.f);
        glm::mat4 cameraPosMatrix = glm::translate(glm::mat4(1.0f), cameraPos * -1.f);

        glm::vec3 WorldUp = glm::normalize(glm::vec3(0, 1.f, 0));
        glm::vec3 CameraCenter = glm::vec3(0, 0.f, 0.f);

        glm::vec3 F = CameraCenter - cameraPos;
        F = glm::normalize(F);

        glm::vec3 R = glm::cross(F, WorldUp);
        glm::vec3 U = glm::cross(R, F);

        glm::mat4 cameraOrientationMatrix = glm::mat4(1.0f);

        glm::mat4 transformation_matrix = glm::mat4(1.0f);

        transformation_matrix = glm::translate(transformation_matrix,
            glm::vec3(x, y, z)
        );

        transformation_matrix = glm::scale(transformation_matrix,
            glm::vec3(scale_x, scale_y, scale_z)
        );

        transformation_matrix = glm::rotate(transformation_matrix,
            glm::radians(theta),
            glm::normalize(glm::vec3(rot_x, rot_y, rot_z))
        );

        transformation_matrix = glm::rotate(transformation_matrix,
            glm::radians(-90.f),
            glm::normalize(glm::vec3(0, 0, 1))
        );


        unsigned int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

        unsigned int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
        glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

        unsigned int ambientStrLoc = glGetUniformLocation(shaderProgram, "ambientStr");
        glUniform1f(ambientStrLoc, ambientStr);

        unsigned int ambientColorLoc = glGetUniformLocation(shaderProgram, "ambientColor");
        glUniform3fv(ambientColorLoc, 1, glm::value_ptr(ambientColor));

        unsigned int cameraPosLoc = glGetUniformLocation(shaderProgram, "cameraPos");
        glUniform3fv(cameraPosLoc, 1, glm::value_ptr(cameraPos));

        unsigned int specStrLoc = glGetUniformLocation(shaderProgram, "specStr");
        glUniform1f(specStrLoc, specStr);

        unsigned int specPhongLoc = glGetUniformLocation(shaderProgram, "specPhog");
        glUniform1f(specPhongLoc, specPhong);

        unsigned int constantLoc = glGetUniformLocation(shaderProgram, "constant");
        glUniform1f(constantLoc, constant);

        unsigned int linearLoc = glGetUniformLocation(shaderProgram, "linear");
        glUniform1f(linearLoc, linear);

        unsigned int quadraticLoc = glGetUniformLocation(shaderProgram, "quadratic");
        glUniform1f(quadraticLoc, quadratic);

        glActiveTexture(GL_TEXTURE0);
        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(tex0Address, 0);

        glActiveTexture(GL_TEXTURE1);
        GLuint tex1Address = glGetUniformLocation(shaderProgram, "norm_tex");
        glBindTexture(GL_TEXTURE_2D, norm_tex);
        glUniform1i(tex1Address, 1);

        glm::mat4 viewMatrix = glm::lookAt(cameraPos, CameraCenter, WorldUp);
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glUseProgram(skybox_shaderProgram);

        glm::mat4 sky_view = glm::mat4(1.f);
        sky_view = glm::mat4(glm::mat3(viewMatrix));

        unsigned int sky_projectionLoc = glGetUniformLocation(skybox_shaderProgram, "projection");
        glUniformMatrix4fv(sky_projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        unsigned int sky_viewLoc = glGetUniformLocation(skybox_shaderProgram, "view");
        glUniformMatrix4fv(sky_viewLoc, 1, GL_FALSE, glm::value_ptr(sky_view));

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        glUseProgram(shaderProgram);

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        unsigned int transformationLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 14);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
