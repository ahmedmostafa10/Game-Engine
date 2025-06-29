#include "shader.hpp"

#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//Forward definition for error checking functions
std::string checkForShaderCompilationErrors(GLuint shader);
std::string checkForLinkingErrors(GLuint program);

bool our::ShaderProgram::attach(const std::string &filename, GLenum type) const {
    // Here, we open the file and read a string from it containing the GLSL code of our shader
    std::ifstream file(filename);
    if(!file){
        std::cerr << "ERROR: Couldn't open shader file: " << filename << std::endl;
        return false;
    }
    std::string sourceString = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const char* sourceCStr = sourceString.c_str();
    file.close();

    //Note: The function "checkForShaderCompilationErrors" checks if there is
    // an error in the given shader. You should use it to check if there is a
    // compilation error and print it so that you can know what is wrong with
    // the shader. The returned string will be empty if there is no errors.

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    glCompileShader(shader);
    std::string errorLog =checkForShaderCompilationErrors(shader);
    if(!errorLog.empty()){
        std::cerr << "Shader Compilation Error in " << filename << ":\n" << errorLog << std::endl;
        glDeleteShader(shader); // Clean up if there's an error
        return false;
    }

    glAttachShader(program, shader);
    return true;
}



bool our::ShaderProgram::link() const {
    //Note: The function "checkForLinkingErrors" checks if there is
    // an error in the given program. You should use it to check if there is a
    // linking error and print it so that you can know what is wrong with the
    // program. The returned string will be empty if there is no errors.
    glLinkProgram(program);

    std::string errorLog=checkForLinkingErrors(program);
    if(!errorLog.empty()){
        std::cerr << "Linking Error: " << errorLog << std::endl;
        return false;
    }

    GLint shaderCount = 0;
    glGetProgramiv(program, GL_ATTACHED_SHADERS, &shaderCount); // Get the count of attached shaders

    if (shaderCount > 0) {
        std::vector<GLuint> shaders(shaderCount);
        glGetAttachedShaders(program, shaderCount, nullptr, shaders.data()); // Get shader IDs

        for (GLuint shader : shaders) {
            glDeleteShader(shader);
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////
// Function to check for compilation and linking error in shaders //
////////////////////////////////////////////////////////////////////

std::string checkForShaderCompilationErrors(GLuint shader){
     //Check and return any error in the compilation process
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, logStr);
        std::string errorLog(logStr);
        delete[] logStr;
        return errorLog;
    }
    return std::string();
}

std::string checkForLinkingErrors(GLuint program){
     //Check and return any error in the linking process
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetProgramInfoLog(program, length, nullptr, logStr);
        std::string error(logStr);
        delete[] logStr;
        return error;
    }
    return std::string();
}