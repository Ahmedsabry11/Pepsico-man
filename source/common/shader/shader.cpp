#include "shader.hpp"

#include <cassert>
#include <iostream>
#include <fstream>
#include <string>

//Forward definition for error checking functions
std::string checkForShaderCompilationErrors(GLuint shader);

std::string checkForLinkingErrors(GLuint program);

bool our::ShaderProgram::attach(const std::string &filename, GLenum type) const {
    // Here, we open the file and read a string from it containing the GLSL code of our shader
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "ERROR: Couldn't open shader file: " << filename << std::endl;
        return false;
    }
    std::string sourceString = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const char *sourceCStr = sourceString.c_str();
    file.close();

    //DONE: Complete this function
    //Note: The function "checkForShaderCompilationErrors" checks if there is
    // an error in the given shader. You should use it to check if there is a
    // compilation error and print it so that you can know what is wrong with
    // the shader. The returned string will be empty if there is no errors.

    // Create shader with received type (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
    uint8_t shaderID = glCreateShader(type);

    // Send shader source code
    glShaderSource(shaderID, 1, &sourceCStr, NULL);

    // Compile the shader
    glCompileShader(shaderID);

    // Check for errors in shader
    std::string error = checkForShaderCompilationErrors(shaderID);

    // If there is an error message
    if (!error.empty()) {
        // Print the error message
        std::cerr << "ERROR::SHADER::" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n"
                  << error << std::endl;

        // Delete the shader
        glDeleteShader(shaderID);

        // We return false if the compilation failed
        return false;
    }

    // Attach the shader to the program
    glAttachShader(this->program, shaderID);

    // Delete the shader
    glDeleteShader(shaderID);
    //We return true if the compilation succeeded
    return true;
}


bool our::ShaderProgram::link() const {
    //DONE: Complete this function
    //Note: The function "checkForLinkingErrors" checks if there is
    // an error in the given program. You should use it to check if there is a
    // linking error and print it so that you can know what is wrong with the
    // program. The returned string will be empty if there is no errors.

    // Link the program
    glLinkProgram(this->program);

    // Check for linking errors
    std::string error = checkForLinkingErrors(this->program);

    // If there is an error message
    if (!error.empty()) {
        // Print the error message
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << error << std::endl;

        // We return false if the linking failed
        return false;
    }

    // We return false if the linking succeeded
    return true;
}

////////////////////////////////////////////////////////////////////
// Function to check for compilation and linking error in shaders //
////////////////////////////////////////////////////////////////////

std::string checkForShaderCompilationErrors(GLuint shader) {
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

std::string checkForLinkingErrors(GLuint program) {
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