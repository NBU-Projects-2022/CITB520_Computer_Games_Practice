#include <cstring>
#include <iostream>

#include "ShaderProgram.h"

// TODO: check why the function was static
void ValidateProgram( GLuint program ) {
    const uint16_t BUFFER_SIZE = 512;
    char buffer[ BUFFER_SIZE ];
    memset( buffer, 0, BUFFER_SIZE );
    GLsizei length = 0;

    glGetProgramInfoLog( program, BUFFER_SIZE, &length, buffer );
    if ( length > 0 ) {
        std::cerr << "Program " << program << " link error: " << buffer << std::endl;
    }

    glValidateProgram( program );
    GLint status;
    glGetProgramiv( program, GL_VALIDATE_STATUS, &status );
    if ( status == GL_FALSE ) {
        std::cerr << "Error validating shader " << program << std::endl;
    }
}

ShaderProgram::ShaderProgram()
    : programId( 0 ) {
}

ShaderProgram::~ShaderProgram() {
    Destroy();
}

void ShaderProgram::Bind() const {
    glUseProgram( programId );
}

void ShaderProgram::Unbind() const {
    glUseProgram( 0 );
}

uint32_t ShaderProgram::GetId() const {
    return programId;
}

int32_t ShaderProgram::GetUniformLocation(const char * uniformName) {
    return glGetUniformLocation(programId, uniformName);
}

void ShaderProgram::BindFragDataLocation(const char * name) {
    glBindFragDataLocation( programId, 0, name );
}

void ShaderProgram::AttachShader( const Shader & shader ) {
    glAttachShader( programId, shader.GetId() );
}

void ShaderProgram::Link() {
    glLinkProgram( programId );
    ValidateProgram( programId );
}

void ShaderProgram::Create() {
    programId = glCreateProgram();
}

void ShaderProgram::Destroy() {
    glDeleteProgram( programId );
}
