#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const {
        //TODO: (Req 3) Write this function
        // Step 1: Create scaling matrix
        glm::mat4 scaleMatrix(1.0f); // Initialize as the identity matrix
        //glm::scale(glm::mat4(1.0f), scale);
        // Set the diagonal elements to the scale factors matrix
        scaleMatrix[0][0] = scale.x; // Scale x
        scaleMatrix[1][1] = scale.y; // Scale y
        scaleMatrix[2][2] = scale.z; // Scale z
        // Step 2: Create rotation matrix from Euler angles using glm::yawPitchRoll
        glm::mat4 rotationMatrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
        // Step 3: Create translation matrix
        //glm::mat4 translationMatrix(1.0f); 
        //glm::translate(glm::mat4(1.0f), position);
        // Set the four column elements to the translation factors
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
        //translationMatrix[0][3] = position.x; // translate x
        //translationMatrix[1][3] = position.y; // translate y
        //translationMatrix[2][3] = position.z; // translate z
        // Step 4: Combine the matrices: scaling → rotation → translation
        return translationMatrix * rotationMatrix * scaleMatrix;
    }
    

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
    }

}