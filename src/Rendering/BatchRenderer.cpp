#include "BatchRenderer.h"

BatchRenderer::BatchRenderer() {}

void BatchRenderer::Draw(BatchRecorder & batchRecorder) {
    // bind shader here?
    for (auto & ptr : batchRecorder.subBatches) {
        auto drawBatch = ptr.second;
        if (!drawBatch->vao.IsStarted()) {
            continue;
        }

        drawBatch->texture->Bind();
        drawBatch->vao.Bind();

        glDrawArrays(GL_TRIANGLES, 0, drawBatch->vertices.size());

        drawBatch->vao.Unbind();
    }
}
