#pragma once

#include "vec3.h"
#include "color.h"
#include "shader.h"

#define BUFFER_OFFSET(x)  ((const void*) (x))

struct GLRenderPoints {
    void create();

    void update(const Mat4 &v, const Mat4 &p);

    void render();

    void dispose();

    void vertex(const Vec3 &v, const Color &c, f32 size);

private:
    Mat4 view;
    Mat4 projection;
    Shader *sh;
    enum {
        e_maxVertices = 2048
    };
    Vec3 m_vertices[e_maxVertices];
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

    void vertex(const Vec3 &v, const Color &c);

private:
    Mat4 view;
    Mat4 projection;
    Shader *sh;

    enum {
        e_maxVertices = 2 * 2048
    };
    Vec3 m_vertices[e_maxVertices];
    Color m_colors[e_maxVertices];

    i32 m_count;

    GLuint m_vaoId;
    GLuint m_vboIds[2];
    GLuint m_programId;
    GLint m_vertexAttribute;
    GLint m_colorAttribute;
};

struct GLRenderTriangles {
    void create();

    void update(const Mat4 &v, const Mat4 &p);

    void render();

    void dispose();

    void vertex(const Vec3 &v, const Color &c);

private:
    Mat4 view;
    Mat4 projection;
    Shader *sh;

    enum {
        e_maxVertices = 3 * 2048
    };
    Vec3 m_vertices[e_maxVertices];
    Color m_colors[e_maxVertices];

    i32 m_count;

    GLuint m_vaoId;
    GLuint m_vboIds[2];
    GLuint m_programId;
    GLint m_vertexAttribute;
    GLint m_colorAttribute;
};

struct DebugDraw {
public:
    static void Create();

    static void Render();

    static void Destroy();

    static void SetCamera(const Mat4 &v, const Mat4 &p);

    static void Segment(const Vec3 &p1, const Vec3 &p2, const Color &color);

    static void Polygon(const Vec3 *vertices, u32 vertexCount, const Color &color);

    static void SolidPolygon(const Vec3 *vertices, u32 vertexCount, const Color &color);

    static void Circle(const Vec3 &center, float radius, const Color &color);

    static void SolidCircle(const Vec3 &center, float radius, const Color &color);

    static void Point(const Vec3 &p, float size, const Color &color);

    static void Pivot(const Vec3 &p);

private:
    static GLRenderPoints *m_Points;
    static GLRenderLines *m_Lines;
    static GLRenderTriangles *m_Triangles;
};