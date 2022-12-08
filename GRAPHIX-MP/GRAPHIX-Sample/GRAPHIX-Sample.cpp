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


bool firstTimePressed = true;
double currentTimePressed, lastTimePressed;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float yaw = -90.0f;
float pitch = 0.0f;

bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0F / 2.0;

int camMode = 1; //1 for perspective, 2 for orthographic
int pov = 1; //1 for 3pv, 2 for fpv

glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 3.f);
glm::vec3 CameraCenter = glm::vec3(0.f, 0.f, -1.f);
glm::vec3 WorldUp = glm::vec3(0.f, 1.f, 0.f);

float subPos_z = 0.0f;
float subPos_x = 0.0f;
float subPos_y = -3.0f;

float planePos_z = -12.f;

std::vector<GLfloat> fullVertexDataShip1, fullVertexDataShip2, fullVertexDataFighterShip, fullVertexDataRacingBoat, fullVertexDataWhaleShark, fullVertexDataSubmarine, fullVertexDataSquid, fullVertexDataPlane;
GLuint ship1Texture, ship2Texture, fighterShipTexture, racingBoatTexture, whaleSharkTexture, submarineTexture, squidTexture, planeTexture;

GLuint VAO_ship1, VBO_ship1, VAO_ship2, VBO_ship2, VAO_fighterShip, VBO_fighterShip, VAO_racingBoat, VBO_racingBoat, VAO_whaleShark, VBO_whaleShark, VAO_submarine, VBO_submarine, VAO_squid, VBO_squid, VAO_plane, VBO_plane; //EBO, VBO_UV;



#include "MyCamera.h"
#include "PerspectiveCamera1.h"
#include "PerspectiveCamera2.h"
#include "OrthoCamera.h"

#include "PointLight.h"
#include "DirectionLight.h"





int lightIntensity = 1;
PointLight pointlight = PointLight(subPos_x, subPos_y, subPos_z);
DirectionLight directionlight = DirectionLight(5.0f, 12.0f, 0.0f);

void keyInput(GLFWwindow* window);
void mouseInput(GLFWwindow* window, double xPos, double yPos);
void Key_Callback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods) {


    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        if (lightIntensity == 1) {
            lightIntensity = 2;
            pointlight.ambientStr = 0.5f;
            printf("%d", lightIntensity);
        }

        else if (lightIntensity == 2) {
            lightIntensity = 3;
            pointlight.ambientStr = 2.0f;
            printf("%d", lightIntensity);
        }

        else if (lightIntensity == 3) {
            lightIntensity = 1;
            pointlight.ambientStr = 4.0f;
            printf("%d", lightIntensity);

        }

    }
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        if (pov == 1) {
            camMode = 1;
            pov = 2;
        }
        else if (pov == 2) {
            camMode = 1;
            pov = 1;
        }
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        camMode = 2;
    }
    
}

std::vector<GLfloat> loadModel(std::string objectPath, std::vector<GLfloat> fullVertexData) {

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

    return fullVertexData;
}


GLuint loadTexturesRGB(const char* texturePath, GLuint newTexture) {

    int img_width, img_height, color_channels;

    unsigned char* tex_bytes = stbi_load(texturePath, &img_width, &img_height, &color_channels, 0);

    glGenTextures(1, &newTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, newTexture);

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

    return newTexture;
}

GLuint loadTexturesRGBA(const char* texturePath, GLuint newTexture) {

    int img_width, img_height, color_channels;

    unsigned char* tex_bytes = stbi_load(texturePath, &img_width, &img_height, &color_channels, 0);

    glGenTextures(1, &newTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, newTexture);

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

    return newTexture;
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    float screenWidth = 1000.0f;
    float screenHeight = 1000.0f;

    window = glfwCreateWindow(screenWidth, screenHeight, "Aguilar, Earl Angelo | Aquino, Karl Matthew | Lee, Jerickson", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(0, 0, screenWidth, screenHeight);

    glfwSetCursorPosCallback(window, mouseInput);

    // Remove This if you want mouse back when application is running
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, Key_Callback);
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

    //**********************************************************************************************

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
        -1.f, -1.f, 1.f, 
        1.f, -1.f, 1.f,  
        1.f, -1.f, -1.f, 
        -1.f, -1.f, -1.f,
        -1.f, 1.f, 1.f,  
        1.f, 1.f, 1.f,   
        1.f, 1.f, -1.f,  
        -1.f, 1.f, -1.f  
    };

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
        "Skybox/OceanRight.png",
        "Skybox/OceanLeft.png",
        "Skybox/OceanUp.png",
        "Skybox/OceanDown.png",
        "Skybox/OceanForward.png",
        "Skybox/OceanBack.png"
    };

    unsigned int skyboxTex;
    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++)
    {
        int w, h, skyCChannel;
        stbi_set_flip_vertically_on_load(false);

        unsigned char* data = stbi_load(facesSkybox[i].c_str(),
            &w,
            &h,
            &skyCChannel,
            0);
        if (data)
        {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGB,
                w,
                h,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data
            );
        }
    }
    stbi_set_flip_vertically_on_load(true);

    //**********************************************************************************************

    /* Ship 1 */

    fullVertexDataShip1 = loadModel("3D/Durham Boat V1/ship1.obj", fullVertexDataShip1);
    ship1Texture = loadTexturesRGB("3D/Textures/wood.jpg", ship1Texture);

    glGenVertexArrays(1, &VAO_ship1);
    glGenBuffers(1, &VBO_ship1);

    glBindVertexArray(VAO_ship1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ship1);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)* fullVertexDataShip1.size(), fullVertexDataShip1.data(), GL_STATIC_DRAW);

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

    /* Ship 2 */

    fullVertexDataShip2 = loadModel("3D/Fireboat V1/ship2.obj", fullVertexDataShip2);
    ship2Texture = loadTexturesRGB("3D/Textures/futuristic.jpg", ship2Texture);

    glGenVertexArrays(1, &VAO_ship2);
    glGenBuffers(1, &VBO_ship2);

    glBindVertexArray(VAO_ship2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ship2);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * fullVertexDataShip2.size(), fullVertexDataShip2.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)0);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void*)normPtr
    );

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)uvPtr);

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

    /* fighterShip */

    fullVertexDataFighterShip = loadModel("3D/FighterShip/fightership.obj", fullVertexDataFighterShip);
    fighterShipTexture = loadTexturesRGB("3D/FighterShip/Fighter_Ship_body_diffuse.jpg", fighterShipTexture);

    glGenVertexArrays(1, &VAO_fighterShip);
    glGenBuffers(1, &VBO_fighterShip);

    glBindVertexArray(VAO_fighterShip);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_fighterShip);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)* fullVertexDataFighterShip.size(), fullVertexDataFighterShip.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)0);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void*)normPtr
    );


    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)uvPtr);

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

    /* Racing Boat */

    fullVertexDataRacingBoat = loadModel("3D/RacingBoat/racingboat.obj", fullVertexDataRacingBoat);
    racingBoatTexture = loadTexturesRGB("3D/RacingBoat/racingBoatTexture.jpg", racingBoatTexture);

    glGenVertexArrays(1, &VAO_racingBoat);
    glGenBuffers(1, &VBO_racingBoat);

    glBindVertexArray(VAO_racingBoat);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_racingBoat);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)* fullVertexDataRacingBoat.size(), fullVertexDataRacingBoat.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)0);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void*)normPtr
    );

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)uvPtr);

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

    /* Whale Shark */

    fullVertexDataWhaleShark = loadModel("3D/whaleshark.obj", fullVertexDataWhaleShark);
    whaleSharkTexture = loadTexturesRGB("3D/Textures/rust.jpg", whaleSharkTexture);

    glGenVertexArrays(1, &VAO_whaleShark);
    glGenBuffers(1, &VBO_whaleShark);

    glBindVertexArray(VAO_whaleShark);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_whaleShark);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)* fullVertexDataWhaleShark.size(), fullVertexDataWhaleShark.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)0);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void*)normPtr
    );

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)uvPtr);

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

    /* Submarine */

    fullVertexDataSubmarine = loadModel("3D/submarine.obj", fullVertexDataSubmarine);
    submarineTexture = loadTexturesRGB("3D/Textures/camo.jpg", submarineTexture);

    glGenVertexArrays(1, &VAO_submarine);
    glGenBuffers(1, &VBO_submarine);

    glBindVertexArray(VAO_submarine);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_submarine);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)* fullVertexDataSubmarine.size(), fullVertexDataSubmarine.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)0);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void*)normPtr
    );

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)uvPtr);

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

    /* Squid */

    fullVertexDataSquid = loadModel("3D/squid.obj", fullVertexDataSquid);
    squidTexture = loadTexturesRGB("3D/Textures/floral.jpg", squidTexture);

    glGenVertexArrays(1, &VAO_squid);
    glGenBuffers(1, &VBO_squid);

    glBindVertexArray(VAO_squid);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_squid);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)* fullVertexDataSquid.size(), fullVertexDataSquid.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)0);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void*)normPtr
    );

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)uvPtr);

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

    /* Plane */

    fullVertexDataPlane = loadModel("3D/plane.obj", fullVertexDataPlane);
    planeTexture = loadTexturesRGBA("3D/Textures/gradient.png", planeTexture);

    glGenVertexArrays(1, &VAO_plane);
    glGenBuffers(1, &VBO_plane);

    glBindVertexArray(VAO_plane);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_plane);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)* fullVertexDataPlane.size(), fullVertexDataPlane.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)0);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void*)normPtr
    );

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)uvPtr);

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

    glm::mat4 identity_matrix = glm::mat4(1.0f);

    glm::mat4 projection_matrix = glm::perspective(
        glm::radians(60.0f),
        screenHeight / screenWidth,
        0.1f,
        100.0f
    );
    float theta = -90.0f;

    glm::vec3 lightPos = glm::vec3(1, 4, 1);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    //Ambient
    float ambientStr = 0.5f;
    glm::vec3 ambientColor = lightColor;

    //Spec
    float specStr = 1.0f;
    float specPhong = 10.0f;

    PerspectiveCamera1 FPVCam(2);   //perspective camera
    PerspectiveCamera2 TPVCam(2);
    OrthoCamera oCam(-50.0f, 50.0f, -50.0f, 50.0f, -50.f, 50.0f);    //orthographic projection
    glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraPos + CameraCenter, WorldUp);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        pointlight.setPointLight(shaderProgram);
        directionlight.setDirectionLight(shaderProgram);
        currentTimePressed = glfwGetTime();


        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        pointlight.lightPosition = glm::vec3(subPos_x, subPos_y, subPos_z - 5.0f);

        keyInput(window);

        glm::vec3 cameraPos = glm::vec3(0, 0, subPos_z + 10.0f);

        glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraPos + CameraCenter, WorldUp);
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_EQUAL);

        glUseProgram(skybox_shaderProgram);

        glm::mat4 sky_view = glm::mat4(1.f);
        sky_view = glm::mat4(glm::mat3(viewMatrix));

        unsigned int sky_projectionLoc = glGetUniformLocation(skybox_shaderProgram, "projection");
        glUniformMatrix4fv(sky_projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        unsigned int sky_viewLoc = glGetUniformLocation(skybox_shaderProgram, "view");
        glUniformMatrix4fv(sky_viewLoc, 1, GL_FALSE, glm::value_ptr(sky_view));

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        glUseProgram(shaderProgram);

        theta += 0.1f;

        //SHIP1
        glm::mat4 transformation_matrix = glm::mat4(1.0f);
        transformation_matrix = glm::translate(transformation_matrix, glm::vec3(6.0f, -3.0f, -25.0f));
        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(0.005f, 0.005f, 0.005f));
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(-90.0f), glm::normalize(glm::vec3(1, 0, 0)));
        unsigned int transformationLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));


        /* Ship 1 */

        glActiveTexture(GL_TEXTURE0);
        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        if (pov == 1) {
            glBindTexture(GL_TEXTURE_2D, ship1Texture);
            glUniform1i(tex0Address, 0);
        }

        if (camMode == 1) {
            if (pov == 1)
                FPVCam.getPCamera(shaderProgram, viewMatrix);
            else if (pov == 2)
                TPVCam.getPCamera2(shaderProgram, viewMatrix, tex0Address);
        }
        else if (camMode == 2)
            oCam.getOCamera(shaderProgram);

        /*unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        */


        //unsigned int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
        //glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

        //unsigned int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
        //glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

        ////Ambient
        //unsigned int ambientStrLoc = glGetUniformLocation(shaderProgram, "ambientStr");
        //glUniform1f(ambientStrLoc, ambientStr);

        //unsigned int ambientColorLoc = glGetUniformLocation(shaderProgram, "ambientColor");
        //glUniform3fv(ambientColorLoc, 1, glm::value_ptr(ambientColor));

        ////Spec
        //unsigned int cameraPosLoc = glGetUniformLocation(shaderProgram, "cameraPos");
        //glUniform3fv(cameraPosLoc, 1, glm::value_ptr(cameraPos));
        //unsigned int specStrLoc = glGetUniformLocation(shaderProgram, "specStr");
        //glUniform1f(specStrLoc, specStr);
        //unsigned int specPhongLoc = glGetUniformLocation(shaderProgram, "specPhog");
        //glUniform1f(specPhongLoc, specPhong);

        //glUniform1f(glGetUniformLocation(shaderProgram, "constant"), 1.0f);
        //glUniform1f(glGetUniformLocation(shaderProgram, "lin"), 0.1f);
        //glUniform1f(glGetUniformLocation(shaderProgram, "quad"), 0.04f);

        glUseProgram(shaderProgram); // Declare in loop once
        glBindVertexArray(VAO_ship1); // Declare in loop once
        glDrawArrays(GL_TRIANGLES, 0, fullVertexDataShip1.size() / 14);

        //SHIP2
        glBindVertexArray(VAO_ship2);

        if ((pov == 1)||(camMode==2)) {
            glBindTexture(GL_TEXTURE_2D, ship2Texture);
            glUniform1i(tex0Address, 0);
        }

        glm::mat4 transformation_matrix2 = glm::mat4(1.0f);
        transformation_matrix2 = glm::translate(transformation_matrix2, glm::vec3(-4.0f, -3.0f, -40.0f));
        transformation_matrix2 = glm::scale(transformation_matrix2, glm::vec3(0.005f, 0.005f, 0.005f));
        transformation_matrix2 = glm::rotate(transformation_matrix2, glm::radians(-90.0f), glm::normalize(glm::vec3(1, 0, 0)));
        unsigned int transformationLoc2 = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc2, 1, GL_FALSE, glm::value_ptr(transformation_matrix2));
        glDrawArrays(GL_TRIANGLES, 0, fullVertexDataShip2.size() / 14);

        //FIGHTERSHIP
        glBindVertexArray(VAO_fighterShip);

        if ((pov == 1) || (camMode == 2)) {
            glBindTexture(GL_TEXTURE_2D, fighterShipTexture);
            glUniform1i(tex0Address, 0);
        }

        glm::mat4 transformation_matrix3 = glm::mat4(1.0f);
        transformation_matrix3 = glm::translate(transformation_matrix3, glm::vec3(-4.0f, -3.0f, -80.0f));
        transformation_matrix3 = glm::scale(transformation_matrix3, glm::vec3(0.003f, 0.003f, 0.003f));
        transformation_matrix3 = glm::rotate(transformation_matrix3, glm::radians(-90.0f), glm::normalize(glm::vec3(1, 0, 0)));
        transformation_matrix3 = glm::rotate(transformation_matrix3, glm::radians(180.0f), glm::normalize(glm::vec3(0, 1, 0)));
        unsigned int transformationLoc3 = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc3, 1, GL_FALSE, glm::value_ptr(transformation_matrix3));
        glDrawArrays(GL_TRIANGLES, 0, fullVertexDataFighterShip.size() / 14);

        //RACINGBOAT
        glBindVertexArray(VAO_racingBoat);

        if ((pov == 1) || (camMode == 2)) {
            glBindTexture(GL_TEXTURE_2D, racingBoatTexture);
            glUniform1i(tex0Address, 0);
        }

        glm::mat4 transformation_matrix4 = glm::mat4(1.0f);
        transformation_matrix4 = glm::translate(transformation_matrix4, glm::vec3(4.5f, -3.0f, -60.0f));
        transformation_matrix4 = glm::scale(transformation_matrix4, glm::vec3(0.01f, 0.01f, 0.01f));
        transformation_matrix4 = glm::rotate(transformation_matrix4, glm::radians(-90.0f), glm::normalize(glm::vec3(1, 0, 0)));
        unsigned int transformationLoc4 = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc4, 1, GL_FALSE, glm::value_ptr(transformation_matrix4));
        glDrawArrays(GL_TRIANGLES, 0, fullVertexDataRacingBoat.size() / 14);

        //SQUID
        glBindVertexArray(VAO_squid);

        if ((pov == 1) || (camMode == 2)) {
            glBindTexture(GL_TEXTURE_2D, squidTexture);
            glUniform1i(tex0Address, 0);
        }

        glm::mat4 transformation_matrix5 = glm::mat4(1.0f);
        transformation_matrix5 = glm::translate(transformation_matrix5, glm::vec3(-6.0f, -3.0f, -120.0f));
        transformation_matrix5 = glm::scale(transformation_matrix5, glm::vec3(0.1f, 0.1f, 0.1f));
        transformation_matrix5 = glm::rotate(transformation_matrix5, glm::radians(0.0f), glm::normalize(glm::vec3(1, 0, 0)));
        unsigned int transformationLoc5 = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc5, 1, GL_FALSE, glm::value_ptr(transformation_matrix5));
        glDrawArrays(GL_TRIANGLES, 0, fullVertexDataSquid.size() / 14);

        //WHALESHARK
        glBindVertexArray(VAO_whaleShark);

        if ((pov == 1) || (camMode == 2)) {
            glBindTexture(GL_TEXTURE_2D, whaleSharkTexture);
            glUniform1i(tex0Address, 0);
        }

        glm::mat4 transformation_matrix6 = glm::mat4(1.0f);
        transformation_matrix6 = glm::translate(transformation_matrix6, glm::vec3(6.0f, -3.5f, -100.0f));
        transformation_matrix6 = glm::scale(transformation_matrix6, glm::vec3(0.008f, 0.008f, 0.008f));
        transformation_matrix6 = glm::rotate(transformation_matrix6, glm::radians(-90.0f), glm::normalize(glm::vec3(1, 0, 0)));
        unsigned int transformationLoc6 = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc6, 1, GL_FALSE, glm::value_ptr(transformation_matrix6));
        glDrawArrays(GL_TRIANGLES, 0, fullVertexDataWhaleShark.size() / 14);

        //SUBMARINE
        glBindVertexArray(VAO_submarine);

        if ((pov == 1) || (camMode == 2)) {
            glBindTexture(GL_TEXTURE_2D, submarineTexture);
            glUniform1i(tex0Address, 0);
        }
        
        glm::mat4 transformation_matrix7 = glm::mat4(1.0f);
        transformation_matrix7 = glm::translate(transformation_matrix7, glm::vec3(0.0f, -3.0f, subPos_z));
        transformation_matrix7 = glm::scale(transformation_matrix7, glm::vec3(0.0007f, 0.0007f, 0.0007f));
        transformation_matrix7 = glm::rotate(transformation_matrix7, glm::radians(90.0f), glm::normalize(glm::vec3(1, 0, 0)));
        unsigned int transformationLoc7 = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc7, 1, GL_FALSE, glm::value_ptr(transformation_matrix7));
        glDrawArrays(GL_TRIANGLES, 0, fullVertexDataSubmarine.size() / 14);

        //PLANE

        /*glBindTexture(GL_TEXTURE_2D, planeTexture);
        glUniform1i(tex0Address, 0);
        glBindVertexArray(VAO_plane);

        glm::mat4 transformation_matrix8 = glm::mat4(1.0f);
        transformation_matrix8 = glm::translate(transformation_matrix8, glm::vec3(0.0f, -3.0f, -10.f));
        transformation_matrix8 = glm::scale(transformation_matrix8, glm::vec3(1.f, 1.f, 1.f));
        transformation_matrix8 = glm::rotate(transformation_matrix8, glm::radians(-90.0f), glm::normalize(glm::vec3(0, 0, 1)));
        unsigned int transformationLoc8 = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc8, 1, GL_FALSE, glm::value_ptr(transformation_matrix8));
        glDrawArrays(GL_TRIANGLES, 0, fullVertexDataPlane.size() / 14);*/

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO_ship1);
    glDeleteBuffers(1, &VBO_ship2);

    glDeleteVertexArrays(1, &VAO_ship2);
    glDeleteBuffers(1, &VBO_ship2);

    glDeleteVertexArrays(1, &VAO_fighterShip);
    glDeleteBuffers(1, &VBO_fighterShip);

    glDeleteVertexArrays(1, &VAO_squid);
    glDeleteBuffers(1, &VBO_squid);

    glDeleteVertexArrays(1, &VAO_submarine);
    glDeleteBuffers(1, &VBO_submarine);

    glDeleteVertexArrays(1, &VAO_racingBoat);
    glDeleteBuffers(1, &VBO_racingBoat);

    glDeleteVertexArrays(1, &VAO_whaleShark);
    glDeleteBuffers(1, &VBO_whaleShark);

    glDeleteVertexArrays(1, &VAO_plane);
    glDeleteBuffers(1, &VBO_plane);
    //glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}


void keyInput(GLFWwindow* window) {

    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * CameraCenter;
        subPos_z -= 0.1f;
        if (pov == 2)
            planePos_z -= 0.1f;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * CameraCenter;
        subPos_z += 0.1f;
        if (pov == 2)
            planePos_z += 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        camMode=2;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

        subPos_x -= 0.1f;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

        subPos_x += 0.1f;
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {

        subPos_y -= 0.1f;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {

        if (subPos_y < 0.0f)
            subPos_y += 0.1f;
    }

}

void mouseInput(GLFWwindow* window, double xPos, double yPos) {

    if ((camMode == 1)&&(pov==1)) {
        if (firstMouse) {
            lastX = xPos;
            lastY = yPos;
            firstMouse = false;
        }

        float xoffset = xPos - lastX;
        float yoffset = lastY - yPos;
        lastX = xPos;
        lastY = yPos;

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front = glm::vec3(0.f, 0.f, 0.f);
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        CameraCenter = glm::normalize(front);
    }
}