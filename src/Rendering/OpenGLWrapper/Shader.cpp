#include <cstring>
#include <string>
#include <fstream>
#include <iostream>

#include "Shader.h"

std::string LoadTextFile( const char *fileName ) {
    std::ifstream textFile;
    std::string text;

    textFile.open( fileName, std::ios::in | std::ios::binary );
    if ( !textFile.is_open() ) {
        std::cerr << "Missing text file" << std::endl;
        return "";
    }

    textFile.seekg( 0, std::ios::end );
    text.resize( static_cast<std::string::size_type>( textFile.tellg() ) );
    textFile.seekg( 0, std::ios::beg );
    textFile.read( &text[ 0 ], text.size() );

    textFile.close();

    return text;
}

void ValidateShader( GLuint shaderId, const char *file = 0 ) {
    const unsigned int BUFFER_SIZE = 512;
    char buffer[ BUFFER_SIZE ];
    memset( buffer, 0, BUFFER_SIZE );
    GLsizei length = 0;

    glGetShaderInfoLog( shaderId, BUFFER_SIZE, &length, buffer );
    if ( length > 0 ) {
        std::cerr << "Shader " << shaderId << " (" << ( file ? file : "" ) << ") compile error: " << buffer << std::endl;
    }
}

Shader::Shader()
    : shaderId( 0 ) {
}

Shader::~Shader() {
    Delete();
}

uint32_t Shader::GetId() const {
    return shaderId;
}

ShaderType Shader::GetType() const {
    return shaderType;
}

int Shader::InitSource( const char *shaderSource, ShaderType shaderType ) {
    if ( !shaderSource ) {
        std::cerr << "Empty shader source" << std::endl;
        return -1;
    }

    this->shaderId = glCreateShader( static_cast<uint16_t>( shaderType ) );
    glShaderSource( this->shaderId, 1, &shaderSource, 0 );
    glCompileShader( this->shaderId );

    ValidateShader( this->shaderId, shaderSource );

    this->shaderType = shaderType;
    return 0;
}

// should throw
int Shader::InitFile( const char *fileName, ShaderType shaderType ) {
    std::string shaderSource = LoadTextFile( fileName );
    if ( !shaderSource.size() ) {
        std::cerr << "Empty shader file" << std::endl;
        return -2;
    }

    int res = InitSource( shaderSource.c_str(), shaderType );

    return res;
}

void Shader::Delete() {
    if ( shaderId ) {
        glDeleteShader( shaderId );
        shaderId = 0;
    }
}
