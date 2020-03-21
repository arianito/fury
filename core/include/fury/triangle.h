//
// Created by aryan on 3/20/20.
//

#ifndef GAME_TRIANGLE_H
#define GAME_TRIANGLE_H

struct Triangle {
    void create() {

        GLfloat vertices[] = {
                -0.5f, -0.5, 0.0f,
                0.5f, -0.5, 0.0f,
                0, 0.5f, 0.0f,
        };
        GLuint indices[] = {
                0, 1, 2
        };
        //generate
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        // bind
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        // attr
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        // clear
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void render() {
        glBindVertexArray(vao);
        glLineWidth(3);
        glDrawElements(GL_LINES, 3, GL_UNSIGNED_INT, nullptr);
    }

    void dispose() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }

private:
    GLuint vbo;
    GLuint vao;
    GLuint ebo;
};

#endif //GAME_TRIANGLE_H
