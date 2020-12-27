#include "BufferGPU.h"

BufferGPU::BufferGPU()
    : id( 0 ), target( 0 ), dataSize( 0 ) {
}

BufferGPU::~BufferGPU() {
    // DeleteBuffer();
}

GLenum BufferGPU::GenerateBuffer( GLenum target ) {
    if ( id ) {
        return 0;
    }

    glGenBuffers( 1, &id );
    this->target = target;
    return id;
}

void *BufferGPU::MapBufferRange( GLintptr offset, GLsizeiptr length, GLbitfield access ) {
    BindBuffer();
    return glMapBufferRange( target, offset, length, access );
}

void BufferGPU::UnmapBuffer() {
    BindBuffer();
    glUnmapBuffer( target );
}

void BufferGPU::Resize( uint32_t newSize, GLenum usage ) {
    Append(nullptr, dataSize, usage);
}

void BufferGPU::Append( void *dataToAdd, uint32_t dataToAddSize, GLenum usage ) {
    uint32_t newSize = dataToAddSize + dataSize;

    if ( dataSize >= newSize ) {
        return;
    }

    if ( !dataSize ) {
        BindBuffer();
        glBufferData( target, newSize, dataToAdd, usage );
    } else {
        uint32_t tempID;
        glGenBuffers( 1, &tempID );
        glBindBuffer( GL_COPY_WRITE_BUFFER, tempID );
        glBufferData( GL_COPY_WRITE_BUFFER, newSize, NULL, usage );
        //glVertexAttribPointer( loc, size, GL_FLOAT, GL_FALSE, 0, 0 );

        glBindBuffer( GL_COPY_READ_BUFFER, id );
        glCopyBufferSubData( GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, dataSize );

        if ( dataToAdd ) {
            glBufferSubData( GL_COPY_WRITE_BUFFER, dataSize, newSize - dataSize, dataToAdd );
        }

        glBindBuffer( GL_COPY_READ_BUFFER, 0 );
        glBindBuffer( GL_COPY_WRITE_BUFFER, 0 );
        glDeleteBuffers( 1, &id );
        glBindBuffer( target, tempID );
        id = tempID;
    }

    //UnbindBuffer();
    dataSize = newSize;
}

void BufferGPU::BindData( void *data, uint32_t dataSize, GLenum usage ) {
    this->dataSize = dataSize;

    BindBuffer();
    glBufferData( target, dataSize, data, usage );
}

uint32_t BufferGPU::DataSize() const {
    return dataSize;
}

void BufferGPU::BindBuffer() {
    glBindBuffer( target, id );
}

void BufferGPU::UnbindBuffer() {
    glBindBuffer( target, 0 );
}

bool BufferGPU::IsStarted() {
    return id ? true : false;
}

void BufferGPU::DeleteBuffer() {
    if ( id ) {
        UnbindBuffer();
        glDeleteBuffers( 1, &id );
        id = 0;
    }
}
