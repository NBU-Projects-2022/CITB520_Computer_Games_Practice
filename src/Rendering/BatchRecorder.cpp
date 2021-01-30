#include "BatchRecorder.h"

#include <glm/gtc/matrix_transform.hpp>

BatchRecorder::~BatchRecorder() {
    for (auto & ptr : subBatches) {
        delete ptr.second;
    }
}

void BatchRecorder::FinishRecording() {
    for (auto & ptr : subBatches) {
        auto drawBatch = ptr.second;
        if (!drawBatch->vao.IsStarted()) {
            drawBatch->vao.Generate();
        } else {
            drawBatch->vertexBuffer.BindData(drawBatch->vertices.data(), sizeof(SpriteVertex) * drawBatch->vertices.size(), GL_STATIC_DRAW);
            // drawBatch->vertexBuffer.MapBufferRange(0, drawBatch->vertices.size(), GL_WRITE_ONLY);
            // drawBatch->vertexBuffer.UnmapBuffer();
            continue;
        }

        drawBatch->vao.Bind();

        drawBatch->vertexBuffer.GenerateBuffer(GL_ARRAY_BUFFER);
        drawBatch->vertexBuffer.BindData(drawBatch->vertices.data(), sizeof(SpriteVertex) * drawBatch->vertices.size(), GL_STATIC_DRAW);

        // vertex
        glEnableVertexAttribArray(0);
        glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (GLvoid*)offsetof(SpriteVertex, position) );

        // uv
        glEnableVertexAttribArray(1);
        glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (GLvoid*)offsetof(SpriteVertex, uv) );

        // color
        glEnableVertexAttribArray(2);
        glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (GLvoid*)offsetof(SpriteVertex, color) );
    }
}

void BatchRecorder::Clear() {
    for (auto & ptr : subBatches) {
        ptr.second->vertices.clear();
    }
}

void BatchRecorder::Add(const Sprite & sprite, glm::vec2 position, float rotation, glm::vec2 scale) {
    Add(sprite, position.x, position.y, rotation, scale.x, scale.y);
}

void BatchRecorder::Add(const Sprite & sprite, float x, float y, float rot, float scaleX, float scaleY) {
    auto modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(x, y ,0.0f));
    modelMatrix = glm::rotate(modelMatrix, rot, glm::vec3(0.0, 0.0, 1.0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, 1.0));
    Add(sprite, modelMatrix);
}

void BatchRecorder::Add(const Sprite & sprite, const glm::mat4 & mat) {
    std::map<TextureGPU *, DrawBatch2D *>::iterator batch;
    auto lb = subBatches.find(sprite.texture);
    if (lb == subBatches.end()) {
        auto newBatch = new DrawBatch2D();
        newBatch->texture = sprite.texture;
        batch = subBatches.insert(lb, std::make_pair(sprite.texture, newBatch));
    } else {
        batch = lb;
    }

    auto & vertices = batch->second->vertices;
    SpriteVertex vecBottomLeft { { 0, 0 }, { sprite.uvMin.x, sprite.uvMax.y }, glm::vec4(1.0) };
    vecBottomLeft.position = mat * glm::vec4(vecBottomLeft.position, 0.0, 1.0);
    vertices.push_back(vecBottomLeft);

    SpriteVertex vecBottomRight { { sprite.width, 0 }, { sprite.uvMax.x, sprite.uvMax.y }, glm::vec4(1.0) };
    vecBottomRight.position = mat * glm::vec4(vecBottomRight.position, 0.0, 1.0);
    vertices.push_back(vecBottomRight);

    SpriteVertex vecTopLeft { { 0, sprite.height }, { sprite.uvMin.x, sprite.uvMin.y }, glm::vec4(1.0) };
    vecTopLeft.position = mat * glm::vec4(vecTopLeft.position, 0.0, 1.0);
    vertices.push_back(vecTopLeft);

    // second triangle
    vertices.push_back(vecTopLeft);
    vertices.push_back(vecBottomRight);

    SpriteVertex vecTopRight { { sprite.width, sprite.height }, { sprite.uvMax.x, sprite.uvMin.y }, glm::vec4(1.0) };
    vecTopRight.position = mat * glm::vec4(vecTopRight.position, 0.0, 1.0);
    vertices.push_back(vecTopRight);
}
