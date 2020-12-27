#pragma once

#include <stdint.h>
#include <GL/gl3w.h>

enum class ShaderType : uint16_t {
    VERTEX_SHADER = GL_VERTEX_SHADER,
    TESSELATTION_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
    TESSELATION_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER,
    GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
    FRAGMENT_SHADER = GL_FRAGMENT_SHADER
};

class Shader {
public:
    Shader();
    ~Shader();

    uint32_t GetId() const;

    ShaderType GetType() const;

    int InitSource( const char *shaderSource, ShaderType shaderType );

    int InitFile( const char *fileName, ShaderType shaderType );

    void Delete();

private:
    uint32_t shaderId;
    ShaderType shaderType;

    //void GetLocations();
};
