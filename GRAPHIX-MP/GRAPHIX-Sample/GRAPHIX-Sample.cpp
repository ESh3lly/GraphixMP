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

int camMode = 1;

glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 3.f);
glm::vec3 CameraCenter = glm::vec3(0.f, 0.f, -1.f);
glm::vec3 WorldUp = glm::vec3(0.f, 1.f, 0.f);

#include "MyCamera.h"
#include "PerspectiveCamera1.h"
#include "OrthoCamera.h"

#include "PointLight.h"
#include "DirectionLight.h"

float subPos_z = 0.0f;
void keyInput(GLFWwindow* window);
void mouseInput(GLFWwindow* window, double xPos, double yPos);

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    float screenWidth = 600.0f;
    float screenHeight = 600.0f;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(screenWidth, screenHeight, "Aguilar, Earl Angelo | Aquino, Karl Matthew | Lee, Jerickson", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    //Initialize GLAD
    gladLoadGL();

    glViewport(0, 0, 600, 600);
    //glViewport(320, 0, 320, 480);

    glfwSetCursorPosCallback(window, mouseInput);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    //Prevents pixelating
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //Prevents Tiling
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

    std::string path = "3D/Durham Boat V1/ship1.obj";
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

    //stbi_set_flip_vertically_on_load(true);
    int img_width, img_height, color_channels;
    unsigned char* tex_bytes = stbi_load("3D/brickwall.jpg", &img_width, &img_height, &color_channels, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB, //GL_RGB = for jpegs/png w/o alpha; GL_RGBA = png/images with alpha
        img_width, img_height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        tex_bytes
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);

    std::vector<GLfloat> fullVertexData;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];

        int vertexIndex = vData.vertex_index * 3;
        int uvIndex = vData.texcoord_index * 2;
        int normIndex = vData.normal_index * 3;
        //X
        fullVertexData.push_back(
            attributes.vertices[vertexIndex]

        );
        //Y
        fullVertexData.push_back(
            attributes.vertices[vertexIndex + 1]

        );
        //Z
        fullVertexData.push_back(
            attributes.vertices[vertexIndex + 2]

        );
        //Normals
        fullVertexData.push_back(attributes.normals[normIndex]);
        fullVertexData.push_back(attributes.normals[normIndex + 1]);
        fullVertexData.push_back(attributes.normals[normIndex + 2]);

        //U
        fullVertexData.push_back(
            attributes.texcoords[uvIndex]

        );
        //V
        fullVertexData.push_back(
            attributes.texcoords[uvIndex + 1]

        );

    }

    GLfloat vertices[]{
        0.f, 0.5f, 0.f, //0
        -0.5f, -0.5f,0.f, //1
        0.5f, -0.5f, 0.f //2
    };

    //EBO indices --- EBO reduces redundant indices
    GLuint indices[]{
        0,1,2 //Triangle 1
    };
    //Init VAO and VBO id vars
    GLuint VAO, VBO, VAO2, VBO2, VAO3, VBO3, VAO4, VBO4, VAO5, VBO5, VAO6, VBO6, VAO7, VBO7; //EBO, VBO_UV;
    //Generate and Assign ID to VAO
    glGenVertexArrays(1, &VAO);
    //Generate and Assign ID to VBO
    glGenBuffers(1, &VBO);


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //LOADING MESH
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GL_FLOAT) * fullVertexData.size(), //Indices ng array
        fullVertexData.data(), //array itself
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0, // Position
        3, // 3 properties: X,Y,Z
        GL_FLOAT,
        GL_FALSE,
        //XYZUV
        8 * sizeof(GL_FLOAT),
        (void*)0);

    GLintptr normPtr = 3 * sizeof(GL_FLOAT);
    glVertexAttribPointer(1,
        3,//3 props
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GL_FLOAT),//XYZUV
        (void*)normPtr
    );
    GLintptr uvPtr = 6 * sizeof(GL_FLOAT);
    glVertexAttribPointer(2,
        2,//UV
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GL_FLOAT),//XYZUV
        (void*)uvPtr
    );


    //0 Position
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*****************SHIP 2 RENDERING*********************/
    std::string path2 = "3D/Fireboat V1/ship2.obj";
    std::vector<tinyobj::shape_t> shapes2;
    std::vector<tinyobj::material_t> materials2;
    std::string warning2, error2;

    tinyobj::attrib_t attributes2;

    bool success2 = tinyobj::LoadObj(
        &attributes2,
        &shapes2,
        &materials2,
        &warning2,
        &error2,
        path2.c_str()
    );

    std::vector<GLfloat> fullVertexData2;
    for (int i = 0; i < shapes2[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData2 = shapes2[0].mesh.indices[i];

        int vertexIndex2 = vData2.vertex_index * 3;
        int uvIndex2 = vData2.texcoord_index * 2;
        int normIndex2 = vData2.normal_index * 3;
        //X
        fullVertexData2.push_back(
            attributes2.vertices[vertexIndex2]

        );
        //Y
        fullVertexData2.push_back(
            attributes2.vertices[vertexIndex2 + 1]

        );
        //Z
        fullVertexData2.push_back(
            attributes2.vertices[vertexIndex2 + 2]

        );
        //Normals
        fullVertexData2.push_back(attributes2.normals[normIndex2]);
        fullVertexData2.push_back(attributes2.normals[normIndex2 + 1]);
        fullVertexData2.push_back(attributes2.normals[normIndex2 + 2]);

        //U
        fullVertexData2.push_back(
            attributes2.texcoords[uvIndex2]

        );
        //V
        fullVertexData2.push_back(
            attributes2.texcoords[uvIndex2 + 1]

        );

    }
    //Generate and Assign ID to VAO
    glGenVertexArrays(1, &VAO2);
    //Generate and Assign ID to VBO
    glGenBuffers(1, &VBO2);


    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);

    //LOADING MESH
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GL_FLOAT) * fullVertexData2.size(), //Indices ng array
        fullVertexData2.data(), //array itself
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0, // Position
        3, // 3 properties: X,Y,Z
        GL_FLOAT,
        GL_FALSE,
        //XYZUV
        8 * sizeof(GL_FLOAT),
        (void*)0);

    GLuint normPtr2 = 3 * sizeof(GL_FLOAT);
    glVertexAttribPointer(1,
        3,//3 props
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GL_FLOAT),//XYZUV
        (void*)normPtr2
    );
    GLuint uvPtr2 = 6 * sizeof(GL_FLOAT);
    glVertexAttribPointer(2,
        2,//UV
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GL_FLOAT),//XYZUV
        (void*)uvPtr2
    );


    //0 Position
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /****************Fightership Rendering************************/
    std::string path3 = "3D/FighterShip/fightership.obj";
    std::vector<tinyobj::shape_t> shapes3;
    std::vector<tinyobj::material_t> materials3;
    std::string warning3, error3;

    tinyobj::attrib_t attributes3;

    bool success3 = tinyobj::LoadObj(
        &attributes3,
        &shapes3,
        &materials3,
        &warning3,
        &error3,
        path3.c_str()
    );

    std::vector<GLfloat> fullVertexData3;
    for (int i = 0; i < shapes3[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData3 = shapes3[0].mesh.indices[i];

        int vertexIndex3 = vData3.vertex_index * 3;
        int uvIndex3 = vData3.texcoord_index * 2;
        int normIndex3 = vData3.normal_index * 3;
        //X
        fullVertexData3.push_back(
            attributes3.vertices[vertexIndex3]

        );
        //Y
        fullVertexData3.push_back(
            attributes3.vertices[vertexIndex3 + 1]

        );
        //Z
        fullVertexData3.push_back(
            attributes3.vertices[vertexIndex3 + 2]

        );
        //Normals
        fullVertexData3.push_back(attributes3.normals[normIndex3]);
        fullVertexData3.push_back(attributes3.normals[normIndex3 + 1]);
        fullVertexData3.push_back(attributes3.normals[normIndex3 + 2]);

        //U
        fullVertexData3.push_back(
            attributes3.texcoords[uvIndex3]

        );
        //V
        fullVertexData3.push_back(
            attributes3.texcoords[uvIndex3 + 1]

        );

    }
    //Generate and Assign ID to VAO
    glGenVertexArrays(1, &VAO3);
    //Generate and Assign ID to VBO
    glGenBuffers(1, &VBO3);


    glBindVertexArray(VAO3);

    glBindBuffer(GL_ARRAY_BUFFER, VBO3);

    //LOADING MESH
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GL_FLOAT) * fullVertexData3.size(), //Indices ng array
        fullVertexData3.data(), //array itself
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0, // Position
        3, // 3 properties: X,Y,Z
        GL_FLOAT,
        GL_FALSE,
        //XYZUV
        8 * sizeof(GL_FLOAT),
        (void*)0);

    GLuint normPtr3 = 3 * sizeof(GL_FLOAT);
    glVertexAttribPointer(1,
        3,//3 props
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GL_FLOAT),//XYZUV
        (void*)normPtr3
    );
    GLuint uvPtr3 = 6 * sizeof(GL_FLOAT);
    glVertexAttribPointer(2,
        2,//UV
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GL_FLOAT),//XYZUV
        (void*)uvPtr3
    );


    //0 Position
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*****************RacingBoat Rendering********************/
    std::string path4 = "3D/RacingBoat/racingboat.obj";
    std::vector<tinyobj::shape_t> shapes4;
    std::vector<tinyobj::material_t> materials4;
    std::string warning4, error4;

    tinyobj::attrib_t attributes4;

    bool success4 = tinyobj::LoadObj(
        &attributes4,
        &shapes4,
        &materials4,
        &warning4,
        &error4,
        path4.c_str()
    );

    std::vector<GLfloat> fullVertexData4;
    for (int i = 0; i < shapes4[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData4 = shapes4[0].mesh.indices[i];

        int vertexIndex4 = vData4.vertex_index * 3;
        int uvIndex4 = vData4.texcoord_index * 2;
        int normIndex4 = vData4.normal_index * 3;
        //X
        fullVertexData4.push_back(
            attributes4.vertices[vertexIndex4]

        );
        //Y
        fullVertexData4.push_back(
            attributes4.vertices[vertexIndex4 + 1]

        );
        //Z
        fullVertexData4.push_back(
            attributes4.vertices[vertexIndex4 + 2]

        );
        //Normals
        fullVertexData4.push_back(attributes4.normals[normIndex4]);
        fullVertexData4.push_back(attributes4.normals[normIndex4 + 1]);
        fullVertexData4.push_back(attributes4.normals[normIndex4 + 2]);

        //U
        fullVertexData4.push_back(
            attributes4.texcoords[uvIndex4]

        );
        //V
        fullVertexData4.push_back(
            attributes4.texcoords[uvIndex4 + 1]

        );

    }
    //Generate and Assign ID to VAO
    glGenVertexArrays(1, &VAO4);
    //Generate and Assign ID to VBO
    glGenBuffers(1, &VBO4);


    glBindVertexArray(VAO4);

    glBindBuffer(GL_ARRAY_BUFFER, VBO4);

    //LOADING MESH
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GL_FLOAT) * fullVertexData4.size(), //Indices ng array
        fullVertexData4.data(), //array itself
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0, // Position
        3, // 3 properties: X,Y,Z
        GL_FLOAT,
        GL_FALSE,
        //XYZUV
        8 * sizeof(GL_FLOAT),
        (void*)0);

    GLuint normPtr4 = 3 * sizeof(GL_FLOAT);
    glVertexAttribPointer(1,
        3,//3 props
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GL_FLOAT),//XYZUV
        (void*)normPtr4
    );
    GLuint uvPtr4 = 6 * sizeof(GL_FLOAT);
    glVertexAttribPointer(2,
        2,//UV
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GL_FLOAT),//XYZUV
        (void*)uvPtr4
    );


    //0 Position
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*****************Squid Rendering********************/
    std::string path5 = "3D/squid.obj";
    std::vector<tinyobj::shape_t> shapes5;
    std::vector<tinyobj::material_t> materials5;
    std::string warning5, error5;

    tinyobj::attrib_t attributes5;

    bool success5 = tinyobj::LoadObj(
        &attributes5,
        &shapes5,
        &materials5,
        &warning5,
        &error5,
        path5.c_str()
    );

    std::vector<GLfloat> fullVertexData5;
    for (int i = 0; i < shapes5[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData5 = shapes5[0].mesh.indices[i];

        int vertexIndex5 = vData5.vertex_index * 3;
        int uvIndex5 = vData5.texcoord_index * 2;
        int normIndex5 = vData5.normal_index * 3;
        //X
        fullVertexData5.push_back(
            attributes5.vertices[vertexIndex5]

        );
        //Y
        fullVertexData5.push_back(
            attributes5.vertices[vertexIndex5 + 1]

        );
        //Z
        fullVertexData5.push_back(
            attributes5.vertices[vertexIndex5 + 2]

        );
        //Normals
        fullVertexData5.push_back(attributes5.normals[normIndex5]);
        fullVertexData5.push_back(attributes5.normals[normIndex5 + 1]);
        fullVertexData5.push_back(attributes5.normals[normIndex5 + 2]);

        //U
        fullVertexData5.push_back(
            attributes5.texcoords[uvIndex5]

        );
        //V
        fullVertexData5.push_back(
            attributes5.texcoords[uvIndex5 + 1]

        );

    }
    //Generate and Assign ID to VAO
    glGenVertexArrays(1, &VAO5);
    //Generate and Assign ID to VBO
    glGenBuffers(1, &VBO5);


    glBindVertexArray(VAO5);

    glBindBuffer(GL_ARRAY_BUFFER, VBO5);

    //LOADING MESH
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GL_FLOAT) * fullVertexData5.size(), //Indices ng array
        fullVertexData5.data(), //array itself
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0, // Position
        3, // 3 properties: X,Y,Z
        GL_FLOAT,
        GL_FALSE,
        //XYZUV
        8 * sizeof(GL_FLOAT),
        (void*)0);

    GLuint normPtr5 = 3 * sizeof(GL_FLOAT);
    glVertexAttribPointer(1,
        3,//3 props
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GL_FLOAT),//XYZUV
        (void*)normPtr5
    );
    GLuint uvPtr5 = 6 * sizeof(GL_FLOAT);
    glVertexAttribPointer(2,
        2,//UV
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GL_FLOAT),//XYZUV
        (void*)uvPtr5
    );


    //0 Position
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*****************WhaleShark Rendering********************/
    std::string path6 = "3D/whaleshark.obj";
    std::vector<tinyobj::shape_t> shapes6;
    std::vector<tinyobj::material_t> materials6;
    std::string warning6, error6;

    tinyobj::attrib_t attributes6;

    bool success6 = tinyobj::LoadObj(
        &attributes6,
        &shapes6,
        &materials6,
        &warning6,
        &error6,
        path6.c_str()
    );

    std::vector<GLfloat> fullVertexData6;
    for (int i = 0; i < shapes6[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData6 = shapes6[0].mesh.indices[i];

        int vertexIndex6 = vData6.vertex_index * 3;
        int uvIndex6 = vData6.texcoord_index * 2;
        int normIndex6 = vData6.normal_index * 3;
        //X
        fullVertexData6.push_back(
            attributes6.vertices[vertexIndex6]

        );
        //Y
        fullVertexData6.push_back(
            attributes6.vertices[vertexIndex6 + 1]

        );
        //Z
        fullVertexData6.push_back(
            attributes6.vertices[vertexIndex6 + 2]

        );
        //Normals
        fullVertexData6.push_back(attributes6.normals[normIndex6]);
        fullVertexData6.push_back(attributes6.normals[normIndex6 + 1]);
        fullVertexData6.push_back(attributes6.normals[normIndex6 + 2]);

        //U
        fullVertexData6.push_back(
            attributes6.texcoords[uvIndex6]

        );
        //V
        fullVertexData6.push_back(
            attributes6.texcoords[uvIndex6 + 1]

        );

    }
    //Generate and Assign ID to VAO
    glGenVertexArrays(1, &VAO6);
    //Generate and Assign ID to VBO
    glGenBuffers(1, &VBO6);


    glBindVertexArray(VAO6);

    glBindBuffer(GL_ARRAY_BUFFER, VBO6);

    //LOADING MESH
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GL_FLOAT) * fullVertexData6.size(), //Indices ng array
        fullVertexData6.data(), //array itself
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0, // Position
        3, // 3 properties: X,Y,Z
        GL_FLOAT,
        GL_FALSE,
        //XYZUV
        8 * sizeof(GL_FLOAT),
        (void*)0);

    GLuint normPtr6 = 3 * sizeof(GL_FLOAT);
    glVertexAttribPointer(1,
        3,//3 props
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GL_FLOAT),//XYZUV
        (void*)normPtr6
    );
    GLuint uvPtr6 = 6 * sizeof(GL_FLOAT);
    glVertexAttribPointer(2,
        2,//UV
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GL_FLOAT),//XYZUV
        (void*)uvPtr6
    );


    //0 Position
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*****************SUBMARINE Rendering********************/
    std::string path7 = "3D/submarine.obj";
    std::vector<tinyobj::shape_t> shapes7;
    std::vector<tinyobj::material_t> materials7;
    std::string warning7, error7;

    tinyobj::attrib_t attributes7;

    bool success7 = tinyobj::LoadObj(
        &attributes7,
        &shapes7,
        &materials7,
        &warning7,
        &error7,
        path7.c_str()
    );

    std::vector<GLfloat> fullVertexData7;
    for (int i = 0; i < shapes7[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData7 = shapes7[0].mesh.indices[i];

        int vertexIndex7 = vData7.vertex_index * 3;
        int uvIndex7 = vData7.texcoord_index * 2;
        int normIndex7 = vData7.normal_index * 3;
        //X
        fullVertexData7.push_back(
            attributes7.vertices[vertexIndex7]

        );
        //Y
        fullVertexData7.push_back(
            attributes7.vertices[vertexIndex7 + 1]

        );
        //Z
        fullVertexData7.push_back(
            attributes7.vertices[vertexIndex7 + 2]

        );
        //Normals
        fullVertexData7.push_back(attributes7.normals[normIndex7]);
        fullVertexData7.push_back(attributes7.normals[normIndex7 + 1]);
        fullVertexData7.push_back(attributes7.normals[normIndex7 + 2]);

        //U
        fullVertexData7.push_back(
            attributes7.texcoords[uvIndex7]

        );
        //V
        fullVertexData7.push_back(
            attributes7.texcoords[uvIndex7 + 1]

        );

    }
    //Generate and Assign ID to VAO
    glGenVertexArrays(1, &VAO7);
    //Generate and Assign ID to VBO
    glGenBuffers(1, &VBO7);


    glBindVertexArray(VAO7);

    glBindBuffer(GL_ARRAY_BUFFER, VBO7);

    //LOADING MESH
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GL_FLOAT) * fullVertexData7.size(), //Indices ng array
        fullVertexData7.data(), //array itself
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0, // Position
        3, // 3 properties: X,Y,Z
        GL_FLOAT,
        GL_FALSE,
        //XYZUV
        8 * sizeof(GL_FLOAT),
        (void*)0);

    GLuint normPtr7 = 3 * sizeof(GL_FLOAT);
    glVertexAttribPointer(1,
        3,//3 props
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GL_FLOAT),//XYZUV
        (void*)normPtr7
    );
    GLuint uvPtr7 = 6 * sizeof(GL_FLOAT);
    glVertexAttribPointer(2,
        2,//UV
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GL_FLOAT),//XYZUV
        (void*)uvPtr7
    );


    //0 Position
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);



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

    PerspectiveCamera1 pCam(2);   //perspective camera
    OrthoCamera oCam(-5.0f, 5.0f, -10.0f, 5.0f, -5.f, 5.0f);    //orthographic projection

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        currentTimePressed = glfwGetTime();

        glm::vec3 cameraPos = glm::vec3(0, 0, subPos_z+10.0f);
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        keyInput(window);

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
        glActiveTexture(GL_TEXTURE0);
        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(tex0Address, 0);

        if (camMode == 1)
            pCam.getPCamera(shaderProgram, viewMatrix);
        else if (camMode == 2)
            oCam.getOCamera(shaderProgram);

        /*unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));



        unsigned int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

        unsigned int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
        glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

        //Ambient
        unsigned int ambientStrLoc = glGetUniformLocation(shaderProgram, "ambientStr");
        glUniform1f(ambientStrLoc, ambientStr);

        unsigned int ambientColorLoc = glGetUniformLocation(shaderProgram, "ambientColor");
        glUniform3fv(ambientColorLoc, 1, glm::value_ptr(ambientColor));

        //Spec
        unsigned int cameraPosLoc = glGetUniformLocation(shaderProgram, "cameraPos");
        glUniform3fv(cameraPosLoc, 1, glm::value_ptr(cameraPos));
        unsigned int specStrLoc = glGetUniformLocation(shaderProgram, "specStr");
        glUniform1f(specStrLoc, specStr);
        unsigned int specPhongLoc = glGetUniformLocation(shaderProgram, "specPhog");
        glUniform1f(specPhongLoc, specPhong);

        glUniform1f(glGetUniformLocation(shaderProgram, "constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shaderProgram, "lin"), 0.1f);
        glUniform1f(glGetUniformLocation(shaderProgram, "quad"), 0.04f);

        glUseProgram(shaderProgram); // Declare in loop once
        glBindVertexArray(VAO); // Declare in loop once
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 8);

        //SHIP2
        glBindVertexArray(VAO2);
        glm::mat4 transformation_matrix2 = glm::mat4(1.0f);
        transformation_matrix2 = glm::translate(transformation_matrix2, glm::vec3(-4.0f, -3.0f, -40.0f));
        transformation_matrix2 = glm::scale(transformation_matrix2, glm::vec3(0.005f, 0.005f, 0.005f));
        transformation_matrix2 = glm::rotate(transformation_matrix2, glm::radians(-90.0f), glm::normalize(glm::vec3(1, 0, 0)));
        unsigned int transformationLoc2 = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc2, 1, GL_FALSE, glm::value_ptr(transformation_matrix2));
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData2.size() / 8);

        //FIGHTERSHIP
        glBindVertexArray(VAO3);
        glm::mat4 transformation_matrix3 = glm::mat4(1.0f);
        transformation_matrix3 = glm::translate(transformation_matrix3, glm::vec3(-4.0f, -3.0f, -80.0f));
        transformation_matrix3 = glm::scale(transformation_matrix3, glm::vec3(0.003f, 0.003f, 0.003f));
        transformation_matrix3 = glm::rotate(transformation_matrix3, glm::radians(-90.0f), glm::normalize(glm::vec3(1, 0, 0)));
        transformation_matrix3 = glm::rotate(transformation_matrix3, glm::radians(180.0f), glm::normalize(glm::vec3(0, 1, 0)));
        unsigned int transformationLoc3 = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc3, 1, GL_FALSE, glm::value_ptr(transformation_matrix3));
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData3.size() / 8);

        //RACINGBOAT
        glBindVertexArray(VAO4);
        glm::mat4 transformation_matrix4 = glm::mat4(1.0f);
        transformation_matrix4 = glm::translate(transformation_matrix4, glm::vec3(4.5f, -3.0f, -60.0f));
        transformation_matrix4 = glm::scale(transformation_matrix4, glm::vec3(0.01f, 0.01f, 0.01f));
        transformation_matrix4 = glm::rotate(transformation_matrix4, glm::radians(-90.0f), glm::normalize(glm::vec3(1, 0, 0)));
        unsigned int transformationLoc4 = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc4, 1, GL_FALSE, glm::value_ptr(transformation_matrix4));
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData4.size() / 8);

        //SQUID
        glBindVertexArray(VAO5);
        glm::mat4 transformation_matrix5 = glm::mat4(1.0f);
        transformation_matrix5 = glm::translate(transformation_matrix5, glm::vec3(-6.0f, -3.0f, -120.0f));
        transformation_matrix5 = glm::scale(transformation_matrix5, glm::vec3(0.1f, 0.1f, 0.1f));
        transformation_matrix5 = glm::rotate(transformation_matrix5, glm::radians(0.0f), glm::normalize(glm::vec3(1, 0, 0)));
        unsigned int transformationLoc5 = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc5, 1, GL_FALSE, glm::value_ptr(transformation_matrix5));
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData5.size() / 8);

        //WHALESHARK
        glBindVertexArray(VAO6);
        glm::mat4 transformation_matrix6 = glm::mat4(1.0f);
        transformation_matrix6 = glm::translate(transformation_matrix6, glm::vec3(6.0f, -3.5f, -100.0f));
        transformation_matrix6 = glm::scale(transformation_matrix6, glm::vec3(0.008f, 0.008f, 0.008f));
        transformation_matrix6 = glm::rotate(transformation_matrix6, glm::radians(-90.0f), glm::normalize(glm::vec3(1, 0, 0)));
        unsigned int transformationLoc6 = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc6, 1, GL_FALSE, glm::value_ptr(transformation_matrix6));
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData6.size() / 8);

        //SUBMARINE
        glBindVertexArray(VAO7);
        glm::mat4 transformation_matrix7 = glm::mat4(1.0f);
        transformation_matrix7 = glm::translate(transformation_matrix7, glm::vec3(0.0f, -3.0f, subPos_z));
        transformation_matrix7 = glm::scale(transformation_matrix7, glm::vec3(0.0007f, 0.0007f, 0.0007f));
        transformation_matrix7 = glm::rotate(transformation_matrix7, glm::radians(90.0f), glm::normalize(glm::vec3(1, 0, 0)));
        unsigned int transformationLoc7 = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc7, 1, GL_FALSE, glm::value_ptr(transformation_matrix7));
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData7.size() / 8);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * CameraCenter;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(CameraCenter, WorldUp)) * cameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(CameraCenter, WorldUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        camMode=2;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        camMode=1;
    }

}

void mouseInput(GLFWwindow* window, double xPos, double yPos) {

    if (camMode == 1) {
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