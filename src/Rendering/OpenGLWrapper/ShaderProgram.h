#pragma once

#include "Shader.h"

struct ShaderLocation {
    enum {
        VERTEX = 0,
        TEXTURE = 1,
        NORMAL = 3,

        ADDITIONAL_ATTRIBUTES = 4,

        MODEL_MATRIX = 0,
        VIEW_MATRIX = 1,
        PROJECTION_MATRIX = 2,
        MODEL_VIEW_PROJECTION_MATRIX = 3,

        LIGHT_DIRECTION = 0,
        LIGHT_COLOR = 1
    };
};

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();

    void Bind() const;

    void Unbind() const;

    uint32_t GetId() const;

    int32_t GetUniformLocation(const char * uniformName);

    void BindFragDataLocation(const char * name);

    void AttachShader( const Shader &shader );

    void Link();

    void Create();

    void Destroy();

private:
    uint32_t programId;
};
