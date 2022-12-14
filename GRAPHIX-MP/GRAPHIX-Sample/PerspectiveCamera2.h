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
    void getPCamera2(GLuint shaderProg, glm::mat4 viewMatrix, GLuint tex0Address, Model3D plane) {

        cameraPos = glm::vec3(subPos_x, subPos_y, subPos_z - 10.0f);
        CameraCenter = glm::vec3(0.f, 0.f, -1.f);
        WorldUp = glm::vec3(0.f, 1.f, 0.f);
        viewMatrix = glm::lookAt(cameraPos, cameraPos + CameraCenter, WorldUp);

        unsigned int cameraPosLoc = glGetUniformLocation(shaderProg, "cameraPos");
        glUniform3fv(cameraPosLoc, 1, glm::value_ptr(cameraPos));

        unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        glm::mat4 projection_matrix = glm::perspective(glm::radians(100.0f), height / width, 0.1f, 200.f);

        unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        glBindTexture(GL_TEXTURE_2D, plane.newTexture);
        glUniform1i(tex0Address, 0);
        glBindVertexArray(VAO_plane);

        /*render plane for sonar blending*/
        glm::mat4 transformation_matrix8 = glm::mat4(1.0f);
        transformation_matrix8 = glm::translate(transformation_matrix8, glm::vec3(subPos_x, subPos_y-3.0f, subPos_z - planePos_z));
        transformation_matrix8 = glm::scale(transformation_matrix8, glm::vec3(1.f, 1.f, 1.f));
        transformation_matrix8 = glm::rotate(transformation_matrix8, glm::radians(-90.0f), glm::normalize(glm::vec3(0, 0, 1)));
        unsigned int transformationLoc8 = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformationLoc8, 1, GL_FALSE, glm::value_ptr(transformation_matrix8));
        glDrawArrays(GL_TRIANGLES, 0, plane.fullVertexData.size() / 14);


        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);
    }
};