#include "DebugGL.h"

#include <cstdio>
#include <cassert>
#include <string>
#include <iostream>
#include <map>

#include <GL/gl3w.h>

void MessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const GLvoid* userParam
) {
    // assert(false);

    std::string sev;
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH: sev = "CRITICAL"; break;
        case GL_DEBUG_SEVERITY_MEDIUM: sev = "ERROR"; break;
        case GL_DEBUG_SEVERITY_LOW: sev = "WARN"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: sev = "TRACE"; break;
    }

    if (sev.empty()) {
        assert(false && "Unknown severity level!");
    }

    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x(%s), message = %s\n",
            ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
                type, severity, sev.c_str(), message );
}

void EnableOpenGLDebugging() {
    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback( (GLDEBUGPROCARB)MessageCallback, nullptr );
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
}

struct ErrorDescription {
    ErrorDescription(const char * text, const char * description)
        : text(text), description(description)
    {}

    const char * text;
    const char * description;
};

std::map<GLenum, ErrorDescription> ErrorDescriptons = {
    {
        GL_INVALID_ENUM,
        ErrorDescription(
            "GL_INVALID_ENUM",
            "Given when an enumeration parameter is not a legal enumeration for that function. This is given only for local problems; if the spec allows the enumeration in certain circumstances, where other parameters or state dictate those circumstances, then GL_INVALID_OPERATION is the result instead."
        )
    },
    {
        GL_INVALID_VALUE,
        ErrorDescription(
            "GL_INVALID_VALUE",
            "Given when a value parameter is not a legal value for that function. This is only given for local problems; if the spec allows the value in certain circumstances, where other parameters or state dictate those circumstances, then GL_INVALID_OPERATION is the result instead."
        )
    },
    {
        GL_INVALID_OPERATION,
        ErrorDescription(
            "GL_INVALID_OPERATION",
            "Given when the set of state for a command is not legal for the parameters given to that command. It is also given for commands where combinations of parameters define what the legal parameters are."
        )
    },
    {
        GL_STACK_OVERFLOW,
        ErrorDescription(
            "GL_STACK_OVERFLOW",
            "Given when a stack pushing operation cannot be done because it would overflow the limit of that stack's size."
        )
    },
    {
        GL_STACK_UNDERFLOW,
        ErrorDescription(
            "GL_STACK_UNDERFLOW",
            "Given when a stack popping operation cannot be done because the stack is already at its lowest point."
        )
    },
    {
        GL_OUT_OF_MEMORY,
        ErrorDescription(
            "GL_OUT_OF_MEMORY",
            "Given when performing an operation that can allocate memory, and the memory cannot be allocated. The results of OpenGL functions that return this error are undefined; it is allowable for partial execution of an operation to happen in this circumstance."
        )
    },
    {
        GL_INVALID_FRAMEBUFFER_OPERATION,
        ErrorDescription(
            "GL_INVALID_FRAMEBUFFER_OPERATION",
            "Given when doing anything that would attempt to read from or write/render to a framebuffer that is not complete."
        )
    },
    // ErrorDescription(
    //     GL_CONTEXT_LOST,
    //     "GL_CONTEXT_LOST (with OpenGL 4.5 or ARB_KHR_robustness)",
    //     "Given if the OpenGL context has been lost, due to a graphics card reset."
    // ),
    // ErrorDescription(
    //     GL_TABLE_TOO_LARGE1,
    //     "GL_TABLE_TOO_LARGE1",
    //     "Part of the ARB_imaging extension."
    // )
};

void CheckForGLErrors() {
    GLenum glError;
    while((glError = glGetError()) != GL_NO_ERROR)
    {
        // Process/log the error.
        auto err = ErrorDescriptons.find(glError);
        if (err != ErrorDescriptons.end()) {
            std::cerr << "gl Error 0x"
                << std::hex << glError
                << "(" << err->second.text << ") "
                << err->second.description
                << std::endl;
        } else {
            std::cerr << "gl Error " << std::hex << glError << std::endl;
        }

        // assert(false);
    }
}
