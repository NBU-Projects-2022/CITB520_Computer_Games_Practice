#pragma once

#include <stdint.h>
#include <GL/gl3w.h>

class BufferGPU {
public:
    BufferGPU();
    ~BufferGPU();

    unsigned int GenerateBuffer( GLenum target );

    void *MapBufferRange( GLintptr offset, GLsizeiptr length, GLbitfield access );
    void UnmapBuffer();

    void Append( void *dataToAdd, uint32_t dataToAddSize, GLenum usage );
    void Resize( uint32_t newDataSize, GLenum usage );
    void BindData( void *data, uint32_t dataSize, GLenum usage );

    uint32_t DataSize() const;

    void BindBuffer();
    void UnbindBuffer();

    bool IsStarted();
    void DeleteBuffer();

private:
    uint32_t id, dataSize;
    GLenum target;
};
