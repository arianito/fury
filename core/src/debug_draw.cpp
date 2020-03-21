//
// Created by aryan on 3/21/20.
//

#include "fury/debug_draw.h"

void GLRenderPoints::create() {
    sh.fromSource(R"(
#version 330 core

uniform mat4 projection;
uniform mat4 view;
layout(location = 0) in vec2 v_position;
layout(location = 1) in vec4 v_color;
layout(location = 2) in float v_size;
out vec4 f_color;

void main(void)
{
  f_color = v_color;
  gl_Position = projection * view * vec4(v_position, 0.5f, 1.0f);
  gl_PointSize = v_size;
}
)", R"(
#version 330 core

in vec4 f_color;
out vec4 color;

void main(void)
{
  color = f_color;
}
)");
    sh.create();

    m_vertexAttribute = 0;
    m_colorAttribute = 1;
    m_sizeAttribute = 2;

    // Generate
    glGenVertexArrays(1, &m_vaoId);
    glGenBuffers(3, m_vboIds);

    glBindVertexArray(m_vaoId);
    glEnableVertexAttribArray(m_vertexAttribute);
    glEnableVertexAttribArray(m_colorAttribute);
    glEnableVertexAttribArray(m_sizeAttribute);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glVertexAttribPointer(m_vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
    glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[2]);
    glVertexAttribPointer(m_sizeAttribute, 1, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_sizes), m_sizes, GL_DYNAMIC_DRAW);

    // Cleanup
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_count = 0;
}

void GLRenderPoints::dispose() {
    sh.dispose();
    if (m_vaoId) {
        glDeleteVertexArrays(1, &m_vaoId);
        glDeleteBuffers(2, m_vboIds);
        m_vaoId = 0;
    }
    if (m_programId) {
        glDeleteProgram(m_programId);
        m_programId = 0;
    }
}

void GLRenderPoints::vertex(const Vec2 &v, const Color &c, f32 size) {
    if (m_count == e_maxVertices)
        render();

    m_vertices[m_count] = v;
    m_colors[m_count] = c;
    m_sizes[m_count] = size;
    ++m_count;
}

void GLRenderPoints::render() {

    if (m_count == 0)
        return;

    sh.begin();

    sh.setParam("projection", projection);
    sh.setParam("view", view);

    glBindVertexArray(m_vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(Vec2), m_vertices);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(Color), m_colors);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[2]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(float), m_sizes);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glDrawArrays(GL_POINTS, 0, m_count);
    glDisable(GL_PROGRAM_POINT_SIZE);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    sh.end();

    m_count = 0;
}

void GLRenderPoints::update(const Mat4 &v, const Mat4 &p) {
    view = v;
    projection = p;
}


void GLRenderLines::create() {
    sh.fromSource(R"(
#version 330 core

uniform mat4 projection;
uniform mat4 view;
layout(location = 0) in vec2 v_position;
layout(location = 1) in vec4 v_color;
out vec4 f_color;

void main(void)
{
  f_color = v_color;
  gl_Position = projection * view * vec4(v_position, 0.2f, 1.0f);
}
)", R"(
#version 330 core

in vec4 f_color;
out vec4 color;

void main(void)
{
  color = f_color;
}
)");
    sh.create();

    m_vertexAttribute = 0;
    m_colorAttribute = 1;

    // Generate
    glGenVertexArrays(1, &m_vaoId);
    glGenBuffers(2, m_vboIds);

    glBindVertexArray(m_vaoId);
    glEnableVertexAttribArray(m_vertexAttribute);
    glEnableVertexAttribArray(m_colorAttribute);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glVertexAttribPointer(m_vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
    glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors, GL_DYNAMIC_DRAW);

    // Cleanup
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_count = 0;
}

void GLRenderLines::dispose() {
    sh.dispose();
    if (m_vaoId) {
        glDeleteVertexArrays(1, &m_vaoId);
        glDeleteBuffers(2, m_vboIds);
        m_vaoId = 0;
    }

    if (m_programId) {
        glDeleteProgram(m_programId);
        m_programId = 0;
    }
}

void GLRenderLines::vertex(const Vec2 &v, const Color &c) {
    if (m_count == e_maxVertices)
        render();

    m_vertices[m_count] = v;
    m_colors[m_count] = c;
    ++m_count;
}

void GLRenderLines::render() {

    if (m_count == 0)
        return;


    sh.begin();

    sh.setParam("projection", projection);
    sh.setParam("view", view);

    glLineWidth(2);
    glBindVertexArray(m_vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(Vec2), m_vertices);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(Color), m_colors);

    glDrawArrays(GL_LINES, 0, m_count);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    sh.end();

    m_count = 0;
}

void GLRenderLines::update(const Mat4 &v, const Mat4 &p) {
    view = v;
    projection = p;
}


void GLRenderTriangles::create() {
    sh.fromSource(R"(
#version 330 core

uniform mat4 projection;
uniform mat4 view;
layout(location = 0) in vec2 v_position;
layout(location = 1) in vec4 v_color;
out vec4 f_color;

void main(void)
{
  f_color = v_color;
  gl_Position = projection * view * vec4(v_position, 0.0f, 1.0f);
}
)", R"(
#version 330 core

in vec4 f_color;
out vec4 color;

void main(void)
{
  color = f_color;
}
)");
    sh.create();

    m_vertexAttribute = 0;
    m_colorAttribute = 1;

    // Generate
    glGenVertexArrays(1, &m_vaoId);
    glGenBuffers(2, m_vboIds);

    glBindVertexArray(m_vaoId);
    glEnableVertexAttribArray(m_vertexAttribute);
    glEnableVertexAttribArray(m_colorAttribute);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glVertexAttribPointer(m_vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
    glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_count = 0;
}

void GLRenderTriangles::dispose() {
    sh.dispose();

    if (m_vaoId) {
        glDeleteVertexArrays(1, &m_vaoId);
        glDeleteBuffers(2, m_vboIds);
        m_vaoId = 0;
    }

    if (m_programId) {
        glDeleteProgram(m_programId);
        m_programId = 0;
    }
}

void GLRenderTriangles::vertex(const Vec2 &v, const Color &c) {
    if (m_count == e_maxVertices)
        render();

    m_vertices[m_count] = v;
    m_colors[m_count] = c;
    ++m_count;
}

void GLRenderTriangles::render() {

    if (m_count == 0)
        return;


    sh.begin();

    sh.setParam("projection", projection);
    sh.setParam("view", view);
    glBindVertexArray(m_vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(Vec2), m_vertices);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(Color), m_colors);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLES, 0, m_count);
    glDisable(GL_BLEND);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    sh.end();

    m_count = 0;
}

void GLRenderTriangles::update(const Mat4 &v, const Mat4 &p) {
    view = v;
    projection = p;
}

void DebugDraw::create() {
    m_points = new GLRenderPoints();
    m_points->create();
    m_lines = new GLRenderLines();
    m_lines->create();
    m_triangles = new GLRenderTriangles();
    m_triangles->create();
}

void DebugDraw::dispose() {
    m_points->dispose();
    delete m_points;
    m_points = NULL;

    m_lines->dispose();
    delete m_lines;
    m_lines = NULL;

    m_triangles->dispose();
    delete m_triangles;
    m_triangles = NULL;
}

void DebugDraw::render() {
    m_triangles->render();
    m_lines->render();
    m_points->render();
}

void DebugDraw::update(const Mat4 &v, const Mat4 &p) {
    m_triangles->update(v, p);
    m_lines->update(v, p);
    m_points->update(v, p);
}

void DebugDraw::drawSegment(const Vec2 &p1, const Vec2 &p2, const Color &color) {
    m_lines->vertex(p1, color);
    m_lines->vertex(p2, color);
}

void DebugDraw::drawPolygon(const Vec2 *vertices, u32 vertexCount, const Color &color) {
    Vec2 p1 = vertices[vertexCount - 1];
    for (u32 i = 0; i < vertexCount; ++i) {
        Vec2 p2 = vertices[i];
        m_lines->vertex(p1, color);
        m_lines->vertex(p2, color);
        p1 = p2;
    }
}

void DebugDraw::drawSolidPolygon(const Vec2 *vertices, u32 vertexCount, const Color &color) {
    Color fillColor(0.5f * color.r(), 0.5f * color.g(), 0.5f * color.b(), 0.5f);

    for (u32 i = 1; i < vertexCount - 1; ++i) {
        m_triangles->vertex(vertices[0], fillColor);
        m_triangles->vertex(vertices[i], fillColor);
        m_triangles->vertex(vertices[i + 1], fillColor);
    }

    Vec2 p1 = vertices[vertexCount - 1];
    for (u32 i = 0; i < vertexCount; ++i) {
        Vec2 p2 = vertices[i];
        m_lines->vertex(p1, color);
        m_lines->vertex(p2, color);
        p1 = p2;
    }
}

void DebugDraw::drawCircle(const Vec2 &center, float radius, const Color &color) {
    const float k_segments = 16.0f;
    const float k_increment = 2.0f * Math::PI / k_segments;
    float sinInc = sinf(k_increment);
    float cosInc = cosf(k_increment);
    Vec2 r1(1.0f, 0.0f);
    Vec2 v1 = center + radius * r1;
    for (u32 i = 0; i < k_segments; ++i) {
        Vec2 r2;
        r2.x() = cosInc * r1.x() - sinInc * r1.y();
        r2.y() = sinInc * r1.x() + cosInc * r1.y();
        Vec2 v2 = center + radius * r2;
        m_lines->vertex(v1, color);
        m_lines->vertex(v2, color);
        r1 = r2;
        v1 = v2;
    }
}

void DebugDraw::drawSolidCircle(const Vec2 &center, float radius, const Vec2 &axis, const Color &color) {
    const float k_segments = 16.0f;
    const float k_increment = 2.0f * Math::PI / k_segments;
    float sinInc = sinf(k_increment);
    float cosInc = cosf(k_increment);
    Vec2 v0 = center;
    Vec2 r1(cosInc, sinInc);
    Vec2 v1 = center + radius * r1;
    Color fillColor(0.5f * color.r(), 0.5f * color.g(), 0.5f * color.b(), 0.5f);

    for (u32 i = 0; i < k_segments; ++i) {
        // Perform rotation to avoid additional trigonometry.
        Vec2 r2;
        r2.x() = cosInc * r1.x() - sinInc * r1.y();
        r2.y() = sinInc * r1.x() + cosInc * r1.y();
        Vec2 v2 = center + radius * r2;
        m_triangles->vertex(v0, fillColor);
        m_triangles->vertex(v1, fillColor);
        m_triangles->vertex(v2, fillColor);
        r1 = r2;
        v1 = v2;
    }

    r1.set(1.0f, 0.0f);
    v1 = center + radius * r1;
    for (u32 i = 0; i < k_segments; ++i) {
        Vec2 r2;
        r2.x() = cosInc * r1.x() - sinInc * r1.y();
        r2.y() = sinInc * r1.x() + cosInc * r1.y();
        Vec2 v2 = center + radius * r2;
        m_lines->vertex(v1, color);
        m_lines->vertex(v2, color);
        r1 = r2;
        v1 = v2;
    }

    // Draw a line fixed in the circle to animate rotation.
    Vec2 p = center + radius * axis;
    m_lines->vertex(center, color);
    m_lines->vertex(p, color);
}

void DebugDraw::drawPoint(const Vec2 &p, float size, const Color &color) {
    m_points->vertex(p, color, size);
}
