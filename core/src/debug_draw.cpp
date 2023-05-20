//
// Created by aryan on 3/21/20.
//

#include "fury/debug_draw.h"

void GLRenderPoints::create() {
    sh = new Shader(R"(
#version 330 core

uniform mat4 projection;
uniform mat4 view;
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec4 v_color;
layout(location = 2) in float v_size;
out vec4 f_color;

void main(void)
{
  f_color = v_color;
  gl_Position = projection * view * vec4(v_position, 1.0f);
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
    glVertexAttribPointer(m_vertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
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
    if (m_vaoId) {
        glDeleteVertexArrays(1, &m_vaoId);
        glDeleteBuffers(2, m_vboIds);
        m_vaoId = 0;
    }
    if (m_programId) {
        glDeleteProgram(m_programId);
        m_programId = 0;
    }
    sh->~Shader();
}

void GLRenderPoints::vertex(const Vec3 &v, const Color &c, f32 size) {
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

    sh->Begin();

    sh->SetParameter("projection", projection);
    sh->SetParameter("view", view);

    glBindVertexArray(m_vaoId);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SPRITE);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(Vec3), m_vertices);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(Color), m_colors);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[2]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(float), m_sizes);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glDrawArrays(GL_POINTS, 0, m_count);
    glDisable(GL_PROGRAM_POINT_SIZE);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    sh->End();

    m_count = 0;
}

void GLRenderPoints::update(const Mat4 &v, const Mat4 &p) {
    view = v;
    projection = p;
}


void GLRenderLines::create() {
    sh = new Shader(R"(
#version 330 core

uniform mat4 projection;
uniform mat4 view;
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec4 v_color;
out vec4 f_color;

void main(void)
{
  f_color = v_color;
  gl_Position = projection * view * vec4(v_position, 1.0f);
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
    glVertexAttribPointer(m_vertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
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
    if (m_vaoId) {
        glDeleteVertexArrays(1, &m_vaoId);
        glDeleteBuffers(2, m_vboIds);
        m_vaoId = 0;
    }

    if (m_programId) {
        glDeleteProgram(m_programId);
        m_programId = 0;
    }
    sh->~Shader();
}

void GLRenderLines::vertex(const Vec3 &v, const Color &c) {
    if (m_count == e_maxVertices)
        render();

    m_vertices[m_count] = v;
    m_colors[m_count] = c;
    ++m_count;
}

void GLRenderLines::render() {

    if (m_count == 0)
        return;


    sh->Begin();

    sh->SetParameter("projection", projection);
    sh->SetParameter("view", view);

    glLineWidth(2);
    glBindVertexArray(m_vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(Vec3), m_vertices);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(Color), m_colors);

    glDrawArrays(GL_LINES, 0, m_count);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    sh->End();

    m_count = 0;
}

void GLRenderLines::update(const Mat4 &v, const Mat4 &p) {
    view = v;
    projection = p;
}


void GLRenderTriangles::create() {
    sh = new Shader(R"(
#version 330 core

uniform mat4 projection;
uniform mat4 view;
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec4 v_color;
out vec4 f_color;

void main(void)
{
  f_color = v_color;
  gl_Position = projection * view * vec4(v_position, 1.0f);
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

    m_vertexAttribute = 0;
    m_colorAttribute = 1;

    // Generate
    glGenVertexArrays(1, &m_vaoId);
    glGenBuffers(2, m_vboIds);
    glLineWidth(2);
    glBindVertexArray(m_vaoId);
    glEnableVertexAttribArray(m_vertexAttribute);
    glEnableVertexAttribArray(m_colorAttribute);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glVertexAttribPointer(m_vertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
    glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_count = 0;
}

void GLRenderTriangles::dispose() {
    if (m_vaoId) {
        glDeleteVertexArrays(1, &m_vaoId);
        glDeleteBuffers(2, m_vboIds);
        m_vaoId = 0;
    }

    if (m_programId) {
        glDeleteProgram(m_programId);
        m_programId = 0;
    }
    sh->~Shader();
}

void GLRenderTriangles::vertex(const Vec3 &v, const Color &c) {
    if (m_count == e_maxVertices)
        render();

    m_vertices[m_count] = v;
    m_colors[m_count] = c;
    ++m_count;
}

void GLRenderTriangles::render() {

    if (m_count == 0)
        return;


    sh->Begin();

    sh->SetParameter("projection", projection);
    sh->SetParameter("view", view);
    glBindVertexArray(m_vaoId);

    glLineWidth(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(Vec3), m_vertices);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(Color), m_colors);

    glDrawArrays(GL_TRIANGLES, 0, m_count);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    sh->End();
    m_count = 0;
}

void GLRenderTriangles::update(const Mat4 &v, const Mat4 &p) {
    view = v;
    projection = p;
}

GLRenderLines* DebugDraw::m_Lines = nullptr;
GLRenderPoints* DebugDraw::m_Points = nullptr;
GLRenderTriangles* DebugDraw::m_Triangles = nullptr;

void DebugDraw::Create() {
    m_Points = new GLRenderPoints();
    m_Points->create();
    m_Lines = new GLRenderLines();
    m_Lines->create();
    m_Triangles = new GLRenderTriangles();
    m_Triangles->create();
}

void DebugDraw::Destroy() {
    m_Points->dispose();
    delete m_Points;
    m_Points = nullptr;

    m_Lines->dispose();
    delete m_Lines;
    m_Lines = nullptr;

    m_Triangles->dispose();
    delete m_Triangles;
    m_Triangles = nullptr;
}

void DebugDraw::Render() {
    m_Triangles->render();
    m_Lines->render();
    m_Points->render();
}

void DebugDraw::SetCamera(const Mat4 &v, const Mat4 &p) {
    m_Triangles->update(v, p);
    m_Lines->update(v, p);
    m_Points->update(v, p);
}

void DebugDraw::Segment(const Vec3 &p1, const Vec3 &p2, const Color &color) {
    m_Lines->vertex(p1, color);
    m_Lines->vertex(p2, color);
}

void DebugDraw::Polygon(const Vec3 *vertices, u32 vertexCount, const Color &color) {
    Vec3 p1 = vertices[vertexCount - 1];
    for (u32 i = 0; i < vertexCount; ++i) {
        Vec3 p2 = vertices[i];
        m_Lines->vertex(p1, color);
        m_Lines->vertex(p2, color);
        p1 = p2;
    }
}

void DebugDraw::SolidPolygon(const Vec3 *vertices, u32 vertexCount, const Color &color) {

    for (u32 i = 1; i < vertexCount - 1; ++i) {
        m_Triangles->vertex(vertices[0], color);
        m_Triangles->vertex(vertices[i], color);
        m_Triangles->vertex(vertices[i + 1], color);
    }
}

void DebugDraw::Circle(const Vec3 &center, float radius, const Color &color) {
    const float k_segments = 8.0f;
    const float k_increment = 2.0f * Math::PI / k_segments;
    float sinInc = sinf(k_increment);
    float cosInc = cosf(k_increment);
    Vec3 r1(1.0f, 0.0f);
    Vec3 v1 = center + radius * r1;
    for (u32 i = 0; i < k_segments; ++i) {
        Vec3 r2;
        r2.x() = cosInc * r1.x() - sinInc * r1.y();
        r2.y() = sinInc * r1.x() + cosInc * r1.y();
        Vec3 v2 = center + radius * r2;
        m_Lines->vertex(v1, color);
        m_Lines->vertex(v2, color);
        r1 = r2;
        v1 = v2;
    }
}

void DebugDraw::SolidCircle(const Vec3 &center, float radius, const Color &color) {
    const float k_segments = 8.0f;
    const float k_increment = 2.0f * Math::PI / k_segments;
    float sinInc = sinf(k_increment);
    float cosInc = cosf(k_increment);
    const auto &v0 = center;
    Vec3 r1(cosInc, sinInc);
    Vec3 v1 = center + radius * r1;

    for (u32 i = 0; i < k_segments; ++i) {
        Vec3 r2;
        r2.x() = cosInc * r1.x() - sinInc * r1.y();
        r2.y() = sinInc * r1.x() + cosInc * r1.y();
        Vec3 v2 = center + radius * r2;
        m_Triangles->vertex(v0, color);
        m_Triangles->vertex(v1, color);
        m_Triangles->vertex(v2, color);
        r1 = r2;
        v1 = v2;
    }

}

void DebugDraw::Point(const Vec3 &p, float size, const Color &color) {
    m_Points->vertex(p, color, size);
}

void DebugDraw::Pivot(const Vec3 &p) {
    auto red = Color(1, 0, 0, 0.5);
    auto green = Color(0, 1, 0, 0.5);
    auto blue = Color(0, 0, 1, 0.5);

    m_Lines->vertex(p, red);
    m_Lines->vertex(p + Vec3::right, red);

    m_Lines->vertex(p, green);
    m_Lines->vertex(p + Vec3::back, green);

    m_Lines->vertex(p, blue);
    m_Lines->vertex(p + Vec3::up, blue);
}
