#pragma once

class PerspectiveCamera2 : public MyCamera {

public:
    int placeholder;

    /*initialize perespective camera by camera position coordinates
    overloads MyCamera constructor*/
    PerspectiveCamera2(int n) {
        placeholder = 1;
    };

    /*perspective camera draw part*/
    void getPCamera2(GLuint shaderProg, glm::mat4 viewMatrix, GLuint tex0Address) {
        /*glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_COLOR, GL_SRC_ALPHA);
        glBlendEquation(GL_FUNC_SUBTRACT);*/

        cameraPos = glm::vec3(0, -3.f, subPos_z - 10.0f);
        CameraCenter = glm::vec3(0.f, 0.f, -1.f);
        WorldUp = glm::vec3(0.f, 1.f, 0.f);
        viewMatrix = glm::lookAt(cameraPos, cameraPos + CameraCenter, WorldUp);

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
            glm::radians(100.0f),
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

        glBindTexture(GL_TEXTURE_2D, planeTexture);
        glUniform1i(tex0Address, 0);
        glBindVertexArray(VAO_plane);

        glm::mat4 transformation_matrix8 = glm::mat4(1.0f);
        transformation_matrix8 = glm::translate(transformation_matrix8, glm::vec3(0.0f, -3.0f, subPos_z - planePos_z));
        transformation_matrix8 = glm::scale(transformation_matrix8, glm::vec3(1.f, 1.f, 1.f));
        transformation_matrix8 = glm::rotate(transformation_matrix8, glm::radians(-90.0f), glm::normalize(glm::vec3(0, 0, 1)));
        unsigned int transformationLoc8 = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformationLoc8, 1, GL_FALSE, glm::value_ptr(transformation_matrix8));
        glDrawArrays(GL_TRIANGLES, 0, fullVertexDataPlane.size() / 14);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_COLOR, GL_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);
    }
};