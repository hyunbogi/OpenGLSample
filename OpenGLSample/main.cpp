//
//  main.cpp
//  OpenGLSample
//
//  Created by Hyunbok Lee on 2016. 3. 29..
//  Copyright © 2016년 Hyunbok Lee. All rights reserved.
//

#include <math.h>
#include "sb6.h"


/**
 * Function Prototypes
 */
GLuint compileShader();


/**
 * Main Rendering Application
 */
class SampleApplication : public sb6::application
{
public:
    void startup()
    {
        renderingProgram = compileShader();
        glGenVertexArrays(1, &vertexArrayObject);
        glBindVertexArray(vertexArrayObject);
    }

    void shutdown()
    {
        glDeleteVertexArrays(1, &vertexArrayObject);
        glDeleteProgram(renderingProgram);
        glDeleteVertexArrays(1, &vertexArrayObject);
    }

    void render(double currentTime)
    {
        const GLfloat color[] = {
                (float) sin(currentTime) * 0.5f + 0.5f,
                (float) cos(currentTime) * 0.5f + 0.5f,
                0.0f,
                1.0f
        };
        glClearBufferfv(GL_COLOR, 0, color);

        glUseProgram(renderingProgram);

        GLfloat attributes[] = {
                (float) sin(currentTime) * 0.5f,
                (float) cos(currentTime) * 0.6f,
                0.0f,
                0.0f
        };
        glVertexAttrib4fv(0, attributes);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

private:
    GLuint renderingProgram;
    GLuint vertexArrayObject;
};


DECLARE_MAIN(SampleApplication);


GLuint compileShader()
{
    static const char *vertexShaderCode[] = {"#version 410 core         \n"
            "layout (location = 0) in vec4 offset;                      \n"
            "                                                           \n"
            "void main(void)                                            \n"
            "{                                                          \n"
            "   const vec4 vertices[3] = vec4[3]                        \n"
            "   (                                                       \n"
            "       vec4(0.25, -0.25, 0.5, 1.0),                        \n"
            "       vec4(-0.25, -0.25, 0.5, 1.0),                       \n"
            "       vec4(0.25, 0.25, 0.5, 1.0)                          \n"
            "   );                                                      \n"
            "                                                           \n"
            "   gl_Position = vertices[gl_VertexID] + offset;           \n"
            "}                                                          \n"
    };

    static const char *fragmentShaderCode[] = {"#version 410 core       \n"
            "out vec4 color;                                            \n"
            "                                                           \n"
            "void main(void)                                            \n"
            "{                                                          \n"
            "   color = vec4(0.0, 0.8, 1.0, 1.0);                       \n"
            "}                                                          \n"
    };

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, vertexShaderCode, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}
