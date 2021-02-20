#include "BatchRenderer.h"

BatchRenderer::BatchRenderer() {}

void BatchRenderer::Draw(BatchRecorder & batchRecorder, SpriteMaterial & spriteMaterial) {
    // bind shader here?
    for (auto & ptr : batchRecorder.subBatches) {
        auto drawBatch = ptr.second;
        if (!drawBatch->vao.IsStarted()) {
            continue;
        }

        if (drawBatch->texture == nullptr) {
            spriteMaterial.UpdateIsDebug(true);
        } else {
            drawBatch->texture->Bind();
            spriteMaterial.UpdateIsDebug(false);
        }

        drawBatch->vao.Bind();

        glDrawArrays(GL_TRIANGLES, 0, drawBatch->vertices.size());

        drawBatch->vao.Unbind();
    }
}
