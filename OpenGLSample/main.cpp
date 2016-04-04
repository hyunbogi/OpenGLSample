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
        glPointSize(5.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        GLfloat offsetAttribute[] = {
                (float) sin(currentTime) * 0.5f,
                (float) cos(currentTime) * 0.6f,
                0.0f,
                0.0f
        };
        glVertexAttrib4fv(0, offsetAttribute);

        glDrawArrays(GL_PATCHES, 0, 3);
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
            "   color = vec4(0.0, 0.0, 1.0, 1.0);                       \n"
            "}                                                          \n"
    };

    static const char *tessControlShaderCode[] = {"#version 410 core        \n"
            "layout (vertices = 3) out;                                     \n"
            "                                                               \n"
            "void main(void)                                                \n"
            "{                                                              \n"
            "   if (gl_InvocationID == 0)                                   \n"
            "   {                                                           \n"
            "       gl_TessLevelInner[0] = 5.0;                             \n"
            "       gl_TessLevelOuter[0] = 5.0;                             \n"
            "       gl_TessLevelOuter[1] = 5.0;                             \n"
            "       gl_TessLevelOuter[2] = 5.0;                             \n"
            "   }                                                           \n"
            "   gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;   \n"
            "}                                                              \n"
    };

    static const char *tessEvaluationShaderCode[] = {"#version 410 core     \n"
            "layout (triangles, equal_spacing, cw) in;                      \n"
            "                                                               \n"
            "void main(void)                                                \n"
            "{                                                              \n"
            "   gl_Position = gl_TessCoord.x * gl_in[0].gl_Position +       \n"
            "                 gl_TessCoord.y * gl_in[1].gl_Position +       \n"
            "                 gl_TessCoord.z * gl_in[2].gl_Position;        \n"
            "}                                                              \n"
    };

    static const char *geometryShaderCode[] = {"#version 410 core       \n"
            "layout (triangles) in;                                     \n"
            "layout (points, max_vertices = 3) out;                     \n"
            "                                                           \n"
            "void main(void)                                            \n"
            "{                                                          \n"
            "   int i;                                                  \n"
            "                                                           \n"
            "   for (i = 0; i < gl_in.length(); i++)                    \n"
            "   {                                                       \n"
            "       gl_Position = gl_in[i].gl_Position;                 \n"
            "       EmitVertex();                                       \n"
            "   }                                                       \n"
            "}                                                          \n"
    };

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, vertexShaderCode, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);

    GLuint tessControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
    glShaderSource(tessControlShader, 1, tessControlShaderCode, NULL);
    glCompileShader(tessControlShader);

    GLuint tessEvaluationShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
    glShaderSource(tessEvaluationShader, 1, tessEvaluationShaderCode, NULL);
    glCompileShader(tessEvaluationShader);

    GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometryShader, 1, geometryShaderCode, NULL);
    glCompileShader(geometryShader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glAttachShader(program, tessControlShader);
    glAttachShader(program, tessEvaluationShader);
    glAttachShader(program, geometryShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(tessControlShader);
    glDeleteShader(tessEvaluationShader);
    glDeleteShader(geometryShader);

    return program;
}
