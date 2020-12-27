#pragma once

#include <stdint.h>

#include "BufferGPU.h"

class VAO {
public:
    VAO();
    ~VAO();

    void Bind() const;
    void Unbind() const;

    bool IsStarted() const;

    uint32_t GetID() const;

    void Generate();
    void Delete();

private:
    uint32_t id;
};
