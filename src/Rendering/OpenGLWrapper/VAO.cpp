#include "VAO.h"

VAO::VAO() : id( 0 ) {
}

VAO::~VAO() {
    Delete();
}

void VAO::Bind() const {
    glBindVertexArray( id );
}

void VAO::Unbind() const {
    glBindVertexArray( 0 );
}

bool VAO::IsStarted() const {
    return ( id != 0 );
}

uint32_t VAO::GetID() const {
    return id;
}

void VAO::Generate() {
    glGenVertexArrays( 1, &id );
}

void VAO::Delete() {
    if ( id ) {
        Unbind();
        glDeleteVertexArrays( 1, &id );
        id = 0;
    }
}
