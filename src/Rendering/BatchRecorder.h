#pragma once

#include <vector>
#include <map>

#include <glm/glm.hpp>

#include "Sprite.h"
#include "OpenGLWrapper/VAO.h"
#include "OpenGLWrapper/TextureGPU.h"

struct SpriteVertex {
    glm::vec2 position;
    glm::vec2 uv;
    glm::vec4 color;
};

struct DrawBatch2D {
    VAO vao;
    BufferGPU vertexBuffer;
    TextureGPU* texture;
    std::vector<SpriteVertex> vertices;
};

class BatchRenderer;

class BatchRecorder {
    friend BatchRenderer;
public:
    ~BatchRecorder();

    void Add(const Sprite & sprite, float x, float y, float rot = 0.0f, float scaleX = 1, float scaleY = 1);
    void Add(const Sprite & sprite, const glm::mat4 & mat);

    void FinishRecording();
    void Clear();

private:
    std::map<TextureGPU*, DrawBatch2D*> subBatches;
};
