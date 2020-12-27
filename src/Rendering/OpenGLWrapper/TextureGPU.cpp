#include "TextureGPU.h"

TextureGPU::TextureGPU() {
    textureId = 0;
};

TextureGPU::~TextureGPU() {
    Delete();
}

uint32_t TextureGPU::GetId() const {
    return textureId;
}

bool TextureGPU::UploadToGPU( const RawImage &rawTexture, TextureColorFormat internalFormat ) {
    if ( !textureId ) {
        glGenTextures( 1, &textureId );
        glBindTexture( GL_TEXTURE_2D, textureId );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }

    if ( internalFormat == TextureColorFormat::NOT_SET ) {
        internalFormat = rawTexture.format;
    }

    width = rawTexture.width;
    height = rawTexture.height;

    glTexImage2D( GL_TEXTURE_2D,
        0, static_cast<uint16_t>(internalFormat),
        width, height,
        0, static_cast<uint16_t>(rawTexture.format),
        GL_UNSIGNED_BYTE, rawTexture.data );

    return true;
}

void TextureGPU::Delete() {
    if ( textureId ) {
        glDeleteTextures( 1, &textureId );
        textureId = 0;
        width = 0;
        height = 0;
    }
}

void TextureGPU::Bind() {
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void TextureGPU::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

int32_t TextureGPU::GetWidth() {
    return width;
}

int32_t TextureGPU::GetHeight() {
    return height;
}
