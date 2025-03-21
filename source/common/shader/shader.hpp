#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our {

    class ShaderProgram {

    private:
        //Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram(){
            program= glCreateProgram();
        }
        ~ShaderProgram(){
            glDeleteProgram(program);
        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use() { 
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name) {
            const char* nameCstr=name.c_str();
            return glGetUniformLocation(program,nameCstr);
        }

        void set(const std::string &uniform, GLfloat value) {
            GLint location = getUniformLocation(uniform);
            glUniform1f(location,value); 
        }

        void set(const std::string &uniform, GLuint value) {
            GLint location = getUniformLocation(uniform);
            glUniform1ui(location,value); 
        }

        void set(const std::string &uniform, GLint value) {
            GLint location = getUniformLocation(uniform);
            glUniform1i(location,value); 
        }

        void set(const std::string &uniform, glm::vec2 value) {
            GLint location = getUniformLocation(uniform);
            glUniform2fv(location, 1, &value[0]);	
        }

        void set(const std::string &uniform, glm::vec3 value) {
            GLint location = getUniformLocation(uniform);
            glUniform3fv(location, 1, &value[0]);	
        }

        void set(const std::string &uniform, glm::vec4 value) {
            GLint location = getUniformLocation(uniform);
            glUniform4fv(location, 1, &value[0]);	
        }

        void set(const std::string &uniform, glm::mat4 matrix) {
            GLint location = getUniformLocation(uniform);
            glUniformMatrix4fv(location, 1,GL_FALSE, &matrix[0][0]);	
        }

        //Question: Why do we delete the copy constructor and assignment operator?
        //to prevent duplicaes shader IDs for shaders 
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
    };

}

#endif