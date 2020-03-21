//
// Created by aryan on 3/21/20.
//

#ifndef GAME_DEBUG_DRAW_H
#define GAME_DEBUG_DRAW_H

#include "vec2.h"
#include "color.h"
#include "shader.h"

#define BUFFER_OFFSET(x)  ((const void*) (x))

struct GLRenderPoints {
    void create();

    void update(const Mat4 &v, const Mat4 &p);

    void render();

    void dispose();

    void vertex(const Vec2 &v, const Color &c, f32 size);

private:
    Mat4 view;
    Mat4 projection;
    Shader sh;
    enum {
        e_maxVertices = 512
    };
    Vec2 m_vertices[e_maxVertices];
    Color m_colors[e_maxVertices];
    float m_sizes[e_maxVertices];

    i32 m_count;

    GLuint m_vaoId;
    GLuint m_vboIds[3];
    GLuint m_programId;
    GLint m_vertexAttribute;
    GLint m_colorAttribute;
    GLint m_sizeAttribute;
};

struct GLRenderLines {
    void create();

    void update(const Mat4 &v, const Mat4 &p);

    void render();

    void dispose();

    void vertex(const Vec2 &v, const Color &c);

private:
    Mat4 view;
    Mat4 projection;
    Shader sh;

    enum {
        e_maxVertices = 2 * 512
    };
    Vec2 m_vertices[e_maxVertices];
    Color m_colors[e_maxVertices];

    i32 m_count;

    GLuint m_vaoId;
    GLuint m_vboIds[2];
    GLuint m_programId;
    GLint m_projectionUniform;
    GLint m_vertexAttribute;
    GLint m_colorAttribute;
};

struct GLRenderTriangles {
    void create();

    void update(const Mat4 &v, const Mat4 &p);

    void render();

    void dispose();

    void vertex(const Vec2 &v, const Color &c);

private:
    Mat4 view;
    Mat4 projection;
    Shader sh;

    enum {
        e_maxVertices = 3 * 512
    };
    Vec2 m_vertices[e_maxVertices];
    Color m_colors[e_maxVertices];

    i32 m_count;

    GLuint m_vaoId;
    GLuint m_vboIds[2];
    GLuint m_programId;
    GLint m_projectionUniform;
    GLint m_vertexAttribute;
    GLint m_colorAttribute;
};

struct DebugDraw {
public:
    void create();

    void dispose();

    void update(const Mat4 &v, const Mat4 &p);

    void drawSegment(const Vec2 &p1, const Vec2 &p2, const Color &color);

    void drawPolygon(const Vec2 *vertices, u32 vertexCount, const Color &color);

    void drawSolidPolygon(const Vec2 *vertices, u32 vertexCount, const Color &color);

    void drawCircle(const Vec2 &center, float radius, const Color &color);

    void drawSolidCircle(const Vec2 &center, float radius, const Vec2 &axis, const Color &color);

    void drawPoint(const Vec2 &p, float size, const Color &color);

    void render();

private:
    GLRenderPoints *m_points;
    GLRenderLines *m_lines;
    GLRenderTriangles *m_triangles;
};

#endif //GAME_DEBUG_DRAW_H
