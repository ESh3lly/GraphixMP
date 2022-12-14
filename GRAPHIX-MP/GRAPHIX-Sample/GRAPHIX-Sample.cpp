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

#include "SpotLight.h"
#include "DirectionLight.h"

#include "Model3D.h"

using namespace std;

bool firstTimePressed = true;
double currentTimePressed, lastTimePressed;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float yaw = -90.0f;
float pitch = 0.0f;

float orthoYaw = -90.0f;
float orthoPitch = 0.0f;

bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0F / 2.0;

int camMode = 1; 
int pov = 1; 

glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 3.f);
glm::vec3 CameraCenter = glm::vec3(0.f, 0.f, -1.f);
glm::vec3 WorldUp = glm::vec3(0.f, 1.f, 0.f);

float subPos_z = 0.0f;
float subPos_x = 0.0f;
float subPos_y = -3.0f;

float camCen_x = 0.f;
float camCen_y = 0.f;
float camCen_z   = 0.f;

float planePos_z = -15.f;

float screenWidth = 1000.0f;
float screenHeight = 1000.0f;

bool leftClick = false;

GLuint VAO_ship1, VBO_ship1, VAO_ship2, VBO_ship2, VAO_fighterShip, VBO_fighterShip, VAO_racingBoat, VBO_racingBoat, VAO_whaleShark, VBO_whaleShark, VAO_submarine, VBO_submarine, VAO_squid, VBO_squid, VAO_plane, VBO_plane;

Model3D ship1, ship2, fighterShip, racingBoat, whaleShark, submarine, squid, plane;

int lightIntensity = 1;
SpotLight spotlight = SpotLight(subPos_x, subPos_y, subPos_z);
DirectionLight directionlight = DirectionLight(5.0f, 12.0f, 0.0f);

void keyInput(GLFWwindow* window);
void mouseInput(GLFWwindow* window, double xPos, double yPos);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

/*to read buttons that are used for toggling(1 press only)*/
void Key_Callback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods) {

    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        if (lightIntensity == 1) {
            lightIntensity = 2;
            spotlight.ambientStr = 3.0f;
            printf("%d", lightIntensity);
        }

        else if (lightIntensity == 2) {
            lightIntensity = 3;
            spotlight.ambientStr = 7.0f;
            printf("%d", lightIntensity);
        }

        else if (lightIntensity == 3) {
            lightIntensity = 1;
            spotlight.ambientStr = 10.0f;
            printf("%d", lightIntensity);

        }

    }
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        if (camMode == 1) {
            if (pov == 1) {
                pov = 2;
            }
            else if (pov == 2) {
                pov = 1;
            }
        }
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        if (camMode == 1) {
            camMode = 2;
            std::cout << "cam 2" << std::endl;
        }
        else if (camMode == 2) {
            camMode = 1;
            std::cout << "cam 1" << std::endl;
        }
    }
    
}

 /*updates the CameraCenter vector with yaw and pitch to pan cameras*/
void yawpitch() {
    camCen_x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    camCen_y = sin(glm::radians(pitch));
    camCen_z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    CameraCenter = glm::normalize(glm::vec3(camCen_x, camCen_y, camCen_z));
}

#include "MyCamera.h"
#include "PerspectiveCamera1.h"
#include "PerspectiveCamera2.h"
#include "OrthoCamera.h"

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

   

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
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

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

    ship1.loadModel("3D/Durham Boat V1/ship1.obj");
    ship1.loadTexturesRGB("3D/Textures/wood.jpg");

    glGenVertexArrays(1, &VAO_ship1);
    glGenBuffers(1, &VBO_ship1);

    glBindVertexArray(VAO_ship1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ship1);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)* ship1.fullVertexData.size(), ship1.fullVertexData.data(), GL_STATIC_DRAW);

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

    ship2.loadModel("3D/Fireboat V1/ship2.obj");
    ship2.loadTexturesRGB("3D/Textures/futuristic.jpg");

    glGenVertexArrays(1, &VAO_ship2);
    glGenBuffers(1, &VBO_ship2);

    glBindVertexArray(VAO_ship2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ship2);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * ship2.fullVertexData.size(), ship2.fullVertexData.data(), GL_STATIC_DRAW);

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

    fighterShip.loadModel("3D/FighterShip/fightership.obj");
    fighterShip.loadTexturesRGB("3D/FighterShip/Fighter_Ship_body_diffuse.jpg");

    glGenVertexArrays(1, &VAO_fighterShip);
    glGenBuffers(1, &VBO_fighterShip);

    glBindVertexArray(VAO_fighterShip);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_fighterShip);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)* fighterShip.fullVertexData.size(), fighterShip.fullVertexData.data(), GL_STATIC_DRAW);

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

    racingBoat.loadModel("3D/RacingBoat/racingboat.obj");
    racingBoat.loadTexturesRGB("3D/RacingBoat/racingBoatTexture.jpg");

    glGenVertexArrays(1, &VAO_racingBoat);
    glGenBuffers(1, &VBO_racingBoat);

    glBindVertexArray(VAO_racingBoat);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_racingBoat);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)* racingBoat.fullVertexData.size(), racingBoat.fullVertexData.data(), GL_STATIC_DRAW);

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

    whaleShark.loadModel("3D/whaleshark.obj");
    whaleShark.loadTexturesRGB("3D/Textures/rust.jpg");

    glGenVertexArrays(1, &VAO_whaleShark);
    glGenBuffers(1, &VBO_whaleShark);

    glBindVertexArray(VAO_whaleShark);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_whaleShark);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)* whaleShark.fullVertexData.size(), whaleShark.fullVertexData.data(), GL_STATIC_DRAW);

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

    submarine.loadModel("3D/submarine.obj");
    submarine.loadTexturesRGB("3D/Textures/camo.jpg");

    glGenVertexArrays(1, &VAO_submarine);
    glGenBuffers(1, &VBO_submarine);

    glBindVertexArray(VAO_submarine);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_submarine);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)* submarine.fullVertexData.size(), submarine.fullVertexData.data(), GL_STATIC_DRAW);

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

    squid.loadModel("3D/squid.obj");
    squid.loadTexturesRGB("3D/Textures/floral.jpg");

    glGenVertexArrays(1, &VAO_squid);
    glGenBuffers(1, &VBO_squid);

    glBindVertexArray(VAO_squid);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_squid);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)* squid.fullVertexData.size(), squid.fullVertexData.data(), GL_STATIC_DRAW);

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

    plane.loadModel("3D/plane.obj");
    plane.loadTexturesRGBA("3D/Textures/gradient.png");

    glGenVertexArrays(1, &VAO_plane);
    glGenBuffers(1, &VBO_plane);

    glBindVertexArray(VAO_plane);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_plane);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)* plane.fullVertexData.size(), plane.fullVertexData.data(), GL_STATIC_DRAW);

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
    OrthoCamera oCam(-30.0f, 30.0f, -30.0f, 30.0f, -30.f, 30.0f);    //orthographic projection
    glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraPos + CameraCenter, WorldUp);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        spotlight.setSpotLight(shaderProgram);
        directionlight.setDirectionLight(shaderProgram);
        currentTimePressed = glfwGetTime();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        spotlight.lightPosition = glm::vec3(subPos_x, subPos_y, subPos_z - 15.0f);
        spotlight.direction = glm::vec3(CameraCenter.x, CameraCenter.y, CameraCenter.z - 12.0f);
        keyInput(window);

        glm::vec3 cameraPos = glm::vec3(subPos_x, subPos_y + 2.f, subPos_z + 10.0f);

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
        transformation_matrix = glm::translate(transformation_matrix, glm::vec3(20.0f, -3.0f, -45.0f));
        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(0.005f, 0.005f, 0.005f));
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(-90.0f), glm::normalize(glm::vec3(1, 0, 0)));
        unsigned int transformationLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));


        /* Ship 1 */

        glActiveTexture(GL_TEXTURE0);
        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        if (pov == 1) {
            glBindTexture(GL_TEXTURE_2D, ship1.newTexture);
            glUniform1i(tex0Address, 0);
        }

        if (camMode == 1) {
            if (pov == 1)
                FPVCam.getPCamera(shaderProgram, viewMatrix);
            else if (pov == 2)
                TPVCam.getPCamera2(shaderProgram, viewMatrix, tex0Address , plane);
        }
        else if (camMode == 2)
            oCam.getOCamera(shaderProgram);

        glUseProgram(shaderProgram); // Declare in loop once
        glBindVertexArray(VAO_ship1); // Declare in loop once
        glDrawArrays(GL_TRIANGLES, 0, ship1.fullVertexData.size() / 14);

        //SHIP2
        glBindVertexArray(VAO_ship2);

        if ((pov == 1)||(camMode==2)) {
            glBindTexture(GL_TEXTURE_2D, ship2.newTexture);
            glUniform1i(tex0Address, 0);
        }

        glm::mat4 transformation_matrix2 = glm::mat4(1.0f);
        transformation_matrix2 = glm::translate(transformation_matrix2, glm::vec3(-18.0f, -3.0f, -60.0f));
        transformation_matrix2 = glm::scale(transformation_matrix2, glm::vec3(0.015f, 0.015f, 0.0015f));
        transformation_matrix2 = glm::rotate(transformation_matrix2, glm::radians(-90.0f), glm::normalize(glm::vec3(1, 0, 0)));
        unsigned int transformationLoc2 = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc2, 1, GL_FALSE, glm::value_ptr(transformation_matrix2));
        glDrawArrays(GL_TRIANGLES, 0, ship2.fullVertexData.size() / 14);

        //FIGHTERSHIP
        glBindVertexArray(VAO_fighterShip);

        if ((pov == 1) || (camMode == 2)) {
            glBindTexture(GL_TEXTURE_2D, fighterShip.newTexture);
            glUniform1i(tex0Address, 0);
        }

        glm::mat4 transformation_matrix3 = glm::mat4(1.0f);
        transformation_matrix3 = glm::translate(transformation_matrix3, glm::vec3(-14.0f, -3.0f, -100.0f));
        transformation_matrix3 = glm::scale(transformation_matrix3, glm::vec3(0.010f, 0.010f, 0.010f));
        transformation_matrix3 = glm::rotate(transformation_matrix3, glm::radians(-90.0f), glm::normalize(glm::vec3(1, 0, 0)));
        transformation_matrix3 = glm::rotate(transformation_matrix3, glm::radians(180.0f), glm::normalize(glm::vec3(0, 1, 0)));
        unsigned int transformationLoc3 = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc3, 1, GL_FALSE, glm::value_ptr(transformation_matrix3));
        glDrawArrays(GL_TRIANGLES, 0, fighterShip.fullVertexData.size() / 14);

        //RACINGBOAT
        glBindVertexArray(VAO_racingBoat);

        if ((pov == 1) || (camMode == 2)) {
            glBindTexture(GL_TEXTURE_2D, racingBoat.newTexture);
            glUniform1i(tex0Address, 0);
        }

        glm::mat4 transformation_matrix4 = glm::mat4(1.0f);
        transformation_matrix4 = glm::translate(transformation_matrix4, glm::vec3(16.f, -3.0f, -80.0f));
        transformation_matrix4 = glm::scale(transformation_matrix4, glm::vec3(0.03f, 0.03f, 0.03f));
        transformation_matrix4 = glm::rotate(transformation_matrix4, glm::radians(-90.0f), glm::normalize(glm::vec3(1, 0, 0)));
        unsigned int transformationLoc4 = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc4, 1, GL_FALSE, glm::value_ptr(transformation_matrix4));
        glDrawArrays(GL_TRIANGLES, 0, racingBoat.fullVertexData.size() / 14);

        //SQUID
        glBindVertexArray(VAO_squid);

        if ((pov == 1) || (camMode == 2)) {
            glBindTexture(GL_TEXTURE_2D, squid.newTexture);
            glUniform1i(tex0Address, 0);
        }

        glm::mat4 transformation_matrix5 = glm::mat4(1.0f);
        transformation_matrix5 = glm::translate(transformation_matrix5, glm::vec3(-16.0f, -3.0f, -140.0f));
        transformation_matrix5 = glm::scale(transformation_matrix5, glm::vec3(0.3f, 0.3f, 0.3f));
        transformation_matrix5 = glm::rotate(transformation_matrix5, glm::radians(0.0f), glm::normalize(glm::vec3(1, 0, 0)));
        unsigned int transformationLoc5 = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc5, 1, GL_FALSE, glm::value_ptr(transformation_matrix5));
        glDrawArrays(GL_TRIANGLES, 0, squid.fullVertexData.size() / 14);

        //WHALESHARK
        glBindVertexArray(VAO_whaleShark);

        if ((pov == 1) || (camMode == 2)) {
            glBindTexture(GL_TEXTURE_2D, whaleShark.newTexture);
            glUniform1i(tex0Address, 0);
        }

        glm::mat4 transformation_matrix6 = glm::mat4(1.0f);
        transformation_matrix6 = glm::translate(transformation_matrix6, glm::vec3(23.0f, -16.0f, -120.0f));
        transformation_matrix6 = glm::scale(transformation_matrix6, glm::vec3(0.012f, 0.012f, 0.012f));
        transformation_matrix6 = glm::rotate(transformation_matrix6, glm::radians(-90.0f), glm::normalize(glm::vec3(1, 0, 0)));
        unsigned int transformationLoc6 = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc6, 1, GL_FALSE, glm::value_ptr(transformation_matrix6));
        glDrawArrays(GL_TRIANGLES, 0, whaleShark.fullVertexData.size() / 14);

        //SUBMARINE
        glBindVertexArray(VAO_submarine);

        if ((pov == 1) || (camMode == 2)) {
            glBindTexture(GL_TEXTURE_2D, submarine.newTexture);
            glUniform1i(tex0Address, 0);
        }
        
        glm::mat4 transformation_matrix7 = glm::mat4(1.0f);
        transformation_matrix7 = glm::translate(transformation_matrix7, glm::vec3(subPos_x, subPos_y, subPos_z));
        transformation_matrix7 = glm::scale(transformation_matrix7, glm::vec3(0.0007f, 0.0007f, 0.0007f));
        transformation_matrix7 = glm::rotate(transformation_matrix7, glm::radians(270.0f), glm::normalize(glm::vec3(1, 0, 0)));
        unsigned int transformationLoc7 = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc7, 1, GL_FALSE, glm::value_ptr(transformation_matrix7));
        glDrawArrays(GL_TRIANGLES, 0, submarine.fullVertexData.size() / 14);

        glfwSwapBuffers(window);
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

    glfwTerminate();
    return 0;
}

/*to read buttons that should work when pressed simultaneously*/
void keyInput(GLFWwindow* window) {

    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && (camMode != 2)) {
        cameraPos += cameraSpeed * CameraCenter;
        subPos_z -= 0.1f;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && (camMode != 2)) {
        cameraPos -= cameraSpeed * CameraCenter;
        subPos_z += 0.1f;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && (camMode != 2)) {

        subPos_x -= 0.05f;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && (camMode != 2)) {
        cameraPos += glm::normalize(glm::cross(CameraCenter, WorldUp)) * cameraSpeed;
        subPos_x += 0.05f;
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && (camMode != 2)) {
        subPos_y -= 0.1f;

        std::cout << "Depth: " << subPos_y << "\n";
    }

    if ((glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) && (camMode!=2)) {

        if (subPos_y < 0.0f)
            subPos_y += 0.1f;

        std::cout << "Depth: " << subPos_y << "\n";
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && (camMode == 2)) {
        orthoPitch += 1.f;
        pitch += 1.f;
        if (orthoPitch > 89.0f)
            orthoPitch = 89.0f;
        if (orthoPitch < -89.0f)
            orthoPitch = -89.0f;
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
        yawpitch();
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && (camMode == 2)) {
        orthoPitch -= 1.f;
        pitch -= 1.f;
        if (orthoPitch > 89.0f)
            orthoPitch = 89.0f;
        if (orthoPitch < -89.0f)
            orthoPitch = -89.0f;
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
        yawpitch();
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && (camMode == 2)) {

        orthoYaw -= 1.f;
        yaw += 1.f;
        yawpitch();
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && (camMode == 2)) {

        orthoYaw += 1.f;
        yaw -= 1.f;
        yawpitch();
    }

}

/*to read xand y offsets of mouse for panning the camera*/
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
        
        yawpitch();
    }

    if ((camMode == 2)&&(leftClick == true)) {
        if (firstMouse) {
            lastX = xPos;
            lastY = yPos;
            firstMouse = false;
        }

        float xoffset = -(xPos - lastX);
        float yoffset = lastY - yPos;
        lastX = xPos;
        lastY = yPos;

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        orthoYaw += xoffset;
        orthoPitch += yoffset;

        if (orthoPitch > 89.0f)
            orthoPitch = 89.0f;
        if (orthoPitch < -89.0f)
            orthoPitch = -89.0f;

        yaw -= xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        yawpitch();
            
    }
}

/*to read mouse clicks for drag feature*/
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        leftClick = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        leftClick = false;
    }
}