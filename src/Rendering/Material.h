#pragma once

#include <glm/glm.hpp>

#include "Rendering/OpenGLWrapper/ShaderProgram.h"
#include "Rendering/OpenGLWrapper/TextureGPU.h"

class Material {
public:
    Material() = default;
    virtual ~Material() = default;

    virtual void Bind() {}
    virtual void Unbind() {}
};

class SpriteMaterial
    : public Material
{
public:
    SpriteMaterial() {
        Init();
    }

    virtual void Bind() override {
        shaderProgram.Bind();
    }

    virtual void Unbind() override {
        shaderProgram.Unbind();
    }

    void UpdateTexture(TextureGPU * texture) {
        texture->Bind();
    }

    void UpdateProjection(glm::mat4 projectionMatrix) {
        glUniformMatrix4fv(projectionMatrixId, 1, GL_FALSE, &projectionMatrix[0][0]);
    }

    void UpdateModelView(glm::mat4 modelViewMatrix) {
        glUniformMatrix4fv(modelViewMatrixId, 1, GL_FALSE, &modelViewMatrix[0][0]);
    }

private:
    ShaderProgram shaderProgram;

    GLint projectionMatrixId;
    GLint modelViewMatrixId;

    void Init() {
        shaderProgram.Create();
        Shader vertShader;
        vertShader.InitSource(R"(
            #version 330 core

            uniform mat4 modelView;
            uniform mat4 projection;

            layout( location = 0 ) in vec2 vertex;
            layout( location = 1 ) in vec2 uv;
            layout( location = 2 ) in vec4 color;

            out vec4 v_color;
            out vec2 v_uv;

            void main()
            {
                v_color = color;
                v_uv = uv;
                gl_Position = projection * (modelView * vec4(vertex, 0.0, 1.0));
                // gl_Position = projection * vec4(vertex, 0.0, 1.0);
            }
        )", ShaderType::VERTEX_SHADER);
        shaderProgram.AttachShader(vertShader);
        Shader fragShader;
        fragShader.InitSource(R"(
            #version 330 core

            in vec4 v_color;
            in vec2 v_uv;

            out vec4 fragColor;

            uniform sampler2D spriteSampler;

            void main()
            {
                fragColor = v_color * texture( spriteSampler, v_uv );
                // fragColor = v_color * vec4(v_uv, 1, 1);
            }
        )", ShaderType::FRAGMENT_SHADER);
        shaderProgram.AttachShader(fragShader);
        shaderProgram.Link();
        shaderProgram.Bind();

        projectionMatrixId = shaderProgram.GetUniformLocation("projection");
        modelViewMatrixId = shaderProgram.GetUniformLocation("modelView");
        shaderProgram.BindFragDataLocation( "fragColor" );
    }
};