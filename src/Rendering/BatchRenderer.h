#pragma once

#include "BatchRecorder.h"
#include "Material.h"

class BatchRenderer {
public:
    BatchRenderer();

    // void UpdateProjection(con)

    void Draw(BatchRecorder & batchRecorder, SpriteMaterial & spriteMaterial);

private:
    // SpriteMaterial spriteMaterial;
};
