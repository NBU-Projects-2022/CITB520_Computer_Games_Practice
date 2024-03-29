#include "BatchRecorder.h"

#include <algorithm>
#include <cassert>

#include <glm/gtc/matrix_transform.hpp>

BatchRecorder::~BatchRecorder() {
    for (auto & ptr : subBatches) {
        delete ptr.second;
    }
}

void BatchRecorder::FinishRecording() {
    for (auto & ptr : subBatches) {
        auto drawBatch = ptr.second;

        std::stable_sort(drawBatch->vertices.begin(),
            drawBatch->vertices.end(),
            [](const SpriteVertex & a, const SpriteVertex & b) {
                return a.position.z < b.position.z;
            });

        if (drawBatch->vao.IsStarted()) {
            drawBatch->vertexBuffer.BindData(drawBatch->vertices.data(), sizeof(SpriteVertex) * drawBatch->vertices.size(), GL_DYNAMIC_DRAW);
            // drawBatch->vertexBuffer.MapBufferRange(0, drawBatch->vertices.size(), GL_WRITE_ONLY);
            // drawBatch->vertexBuffer.UnmapBuffer();
            continue;
        }

        drawBatch->vao.Generate();
        drawBatch->vao.Bind();

        drawBatch->vertexBuffer.GenerateBuffer(GL_ARRAY_BUFFER);
        drawBatch->vertexBuffer.BindData(drawBatch->vertices.data(), sizeof(SpriteVertex) * drawBatch->vertices.size(), GL_DYNAMIC_DRAW);

        // vertex
        glEnableVertexAttribArray(0);
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (GLvoid*)offsetof(SpriteVertex, position) );

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

void BatchRecorder::Add(const DrawCircle & drawCircle, glm::vec3 position, float rotation, glm::vec2 scale) {
    Add(drawCircle, position.x, position.y, position.z, rotation, scale.x, scale.y);
}
void BatchRecorder::Add(const DrawCircle & drawCircle, float x, float y, float z, float rot, float scaleX, float scaleY) {
    auto modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(x, y, z));
    modelMatrix = glm::rotate(modelMatrix, rot, glm::vec3(0.0, 0.0, 1.0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, 1.0));
    Add(drawCircle, modelMatrix);
}
void BatchRecorder::Add(const DrawCircle & drawCircle, const glm::mat4 & mat) {
    std::map<TextureGPU *, DrawBatch2D *>::iterator batch;
    auto lb = subBatches.find(nullptr);
    if (lb == subBatches.end()) {
        auto newBatch = new DrawBatch2D();
        newBatch->texture = nullptr;
        batch = subBatches.insert(lb, std::make_pair(nullptr, newBatch));
    } else {
        batch = lb;
    }

    auto & vertices = batch->second->vertices;
    assert(drawCircle.segmentsCount > 3 && "segmentsCount must be more then 3");

    SpriteVertex v0 { { 0, 0, 0 }, { 0.0f, 0.0f }, glm::vec4(1.0) };
    v0.position = mat * glm::vec4(v0.position, 1.0);
    v0.color = drawCircle.color;

    float angleAdvance = glm::two_pi<float>() / drawCircle.segmentsCount;
    for (int i = 1; i < drawCircle.segmentsCount + 1; ++i) {
        vertices.push_back(v0);

        float prevAngle = (i-1)*angleAdvance;
        SpriteVertex v1 {
            { drawCircle.radius * glm::cos(prevAngle), drawCircle.radius * glm::sin(prevAngle), 0 },
            { 1.0f, 1.0f },
            drawCircle.color
        };
        v1.position = mat * glm::vec4(v1.position, 1.0);
        vertices.push_back(v1);

        float angle = i*angleAdvance;
        SpriteVertex v2 {
            { drawCircle.radius * glm::cos(angle), drawCircle.radius * glm::sin(angle), 0 },
            { 1.0f, 1.0f },
            drawCircle.color
        };
        v2.position = mat * glm::vec4(v2.position, 1.0);
        vertices.push_back(v2);
    }
}

void BatchRecorder::Add(const DrawQuad & drawQuad, glm::vec3 position, float rotation, glm::vec2 scale) {
    Add(drawQuad, position.x, position.y, position.z, rotation, scale.x, scale.y);
}
void BatchRecorder::Add(const DrawQuad & drawQuad, float x, float y, float z, float rot, float scaleX, float scaleY) {
    auto modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(x, y, z));
    modelMatrix = glm::rotate(modelMatrix, rot, glm::vec3(0.0, 0.0, 1.0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, 1.0));
    Add(drawQuad, modelMatrix);
}
void BatchRecorder::Add(const DrawQuad & drawQuad, const glm::mat4 & mat) {
    std::map<TextureGPU *, DrawBatch2D *>::iterator batch;
    auto lb = subBatches.find(nullptr);
    if (lb == subBatches.end()) {
        auto newBatch = new DrawBatch2D();
        newBatch->texture = nullptr;
        batch = subBatches.insert(lb, std::make_pair(nullptr, newBatch));
    } else {
        batch = lb;
    }

    auto & vertices = batch->second->vertices;
    SpriteVertex vecBottomLeft { { 0, 0, 0 }, { 0.0f, 1.0f }, glm::vec4(1.0) };
    vecBottomLeft.position = mat * glm::vec4(vecBottomLeft.position, 1.0);
    vecBottomLeft.color = drawQuad.color;
    vertices.push_back(vecBottomLeft);

    SpriteVertex vecBottomRight { { drawQuad.size.x, 0, 0 }, { 1.0f, 1.0f }, glm::vec4(1.0) };
    vecBottomRight.position = mat * glm::vec4(vecBottomRight.position, 1.0);
    vecBottomRight.color = drawQuad.color;
    vertices.push_back(vecBottomRight);

    SpriteVertex vecTopLeft { { 0, drawQuad.size.y, 0 }, { 0.0f, 0.0f }, glm::vec4(1.0) };
    vecTopLeft.position = mat * glm::vec4(vecTopLeft.position, 1.0);
    vecTopLeft.color = drawQuad.color;
    vertices.push_back(vecTopLeft);

    // second triangle
    vertices.push_back(vecTopLeft);
    vertices.push_back(vecBottomRight);

    SpriteVertex vecTopRight { { drawQuad.size.x, drawQuad.size.y, 0 }, { 1.0f, 0.0f }, glm::vec4(1.0) };
    vecTopRight.position = mat * glm::vec4(vecTopRight.position, 1.0);
    vecTopRight.color = drawQuad.color;
    vertices.push_back(vecTopRight);
}

void BatchRecorder::Add(const Sprite & sprite, glm::vec3 position, float rotation, glm::vec2 scale) {
    Add(sprite, position.x, position.y, position.z, rotation, scale.x, scale.y);
}
void BatchRecorder::Add(const Sprite & sprite, float x, float y, float z, float rot, float scaleX, float scaleY) {
    auto modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(x, y, z));
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
    SpriteVertex vecBottomLeft {
        { sprite.drawOffset.x, sprite.drawOffset.y, 0 },
        { sprite.uvMin.x, sprite.uvMax.y },
        sprite.color
    };
    vecBottomLeft.position = mat * glm::vec4(vecBottomLeft.position, 1.0);
    vertices.push_back(vecBottomLeft);

    SpriteVertex vecBottomRight {
        { sprite.width + sprite.drawOffset.x, sprite.drawOffset.y, 0 },
        { sprite.uvMax.x, sprite.uvMax.y },
        sprite.color
    };
    vecBottomRight.position = mat * glm::vec4(vecBottomRight.position, 1.0);
    vertices.push_back(vecBottomRight);

    SpriteVertex vecTopLeft {
        { sprite.drawOffset.y, sprite.height + sprite.drawOffset.y, 0 },
        { sprite.uvMin.x, sprite.uvMin.y },
        sprite.color
    };
    vecTopLeft.position = mat * glm::vec4(vecTopLeft.position, 1.0);
    vertices.push_back(vecTopLeft);

    // second triangle
    vertices.push_back(vecTopLeft);
    vertices.push_back(vecBottomRight);

    SpriteVertex vecTopRight {
        { sprite.width + sprite.drawOffset.x, sprite.height + sprite.drawOffset.y, 0 },
        { sprite.uvMax.x, sprite.uvMin.y },
        sprite.color
    };
    vecTopRight.position = mat * glm::vec4(vecTopRight.position, 1.0);
    vertices.push_back(vecTopRight);
}
