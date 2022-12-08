#pragma once

class PerspectiveCamera1 : public MyCamera {

public:
    int placeholder;

    /*initialize perespective camera by camera position coordinates
    overloads MyCamera constructor*/
    PerspectiveCamera1(int n) {
        placeholder = 1;
    };

    /*perspective camera draw part*/
    void getPCamera(GLuint shaderProg, glm::mat4 viewMatrix) {
        glDisable(GL_BLEND);

        unsigned int cameraPosLoc = glGetUniformLocation(shaderProg, "cameraPos");
        glUniform3fv(cameraPosLoc, 1, glm::value_ptr(cameraPos));

        unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
        glUniformMatrix4fv(
            viewLoc,  //address of transform variable
            1,  //matrices to assign
            GL_FALSE,   //Transpose?
            glm::value_ptr(viewMatrix)
        );

        glm::mat4 projection_matrix = glm::perspective(
            glm::radians(60.0f),
            height / width,
            0.1f,
            100.f
        );

        unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(
            projectionLoc,  //address of transform variable
            1,  //matrices to assign
            GL_FALSE,   //Transpose?
            glm::value_ptr(projection_matrix)
        );

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_DST_COLOR);
        glBlendEquation(GL_FUNC_ADD);

    }

    void loadTextures(GLuint currTexture, GLuint texAdd) {
        glBindTexture(GL_TEXTURE_2D, currTexture);
        glUniform1i(texAdd, 0);
    }
};