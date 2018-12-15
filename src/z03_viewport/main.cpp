#include "Fury/Primitives/System.h"
#include "Fury/Primitives/Vec3.h"
#include "Fury/Primitives/Quaternion.h"
#include "Fury/Primitives/Mat4.h"
#include "Fury/Graphics/Window.h"
#include "Fury/Graphics/Shader.h"
#include "Fury/Trace.h"
#include "Fury/Math.h"
#include "Fury/Path.h"

using namespace Fury;


static Float pitch = 0;
static Float yaw = 0;

static Float pitch2 = 0;
static Float yaw2 = 0;

static Float distance = 6;
static Float distance2 = 10;

struct Game1 : Window {
    Shader sh;

    GLuint vao, vbo, ebo;

    void Create() override {
        sh.create(Path::local("..", "src", "z03_viewport", "shader"));

        GLfloat vertices[] = {
                // front
                -1.0f, -1.0f, 1.0f, 0, 0, 1,
                1.0f, -1.0f, 1.0f, 0, 0, 1,
                1.0f, 1.0f, 1.0f, 0, 0, 1,
                -1.0f, 1.0f, 1.0f, 0, 0, 1,

                //right
                1.0f, 1.0f, 1.0f, 1, 0, 0,
                1.0f, 1.0f, -1.0f, 1, 0, 0,
                1.0f, -1.0f, -1.0f, 1, 0, 0,
                1.0f, -1.0f, 1.0f, 1, 0, 0,

                //back
                -1.0f, -1.0f, -1.0f, 0, 0, -1,
                1.0f, -1.0f, -1.0f, 0, 0, -1,
                1.0f, 1.0f, -1.0f, 0, 0, -1,
                -1.0f, 1.0f, -1.0f, 0, 0, -1,

                //left
                -1.0f, -1.0f, -1.0f, -1, 0, 0,
                -1.0f, -1.0f, 1.0f, -1, 0, 0,
                -1.0f, 1.0f, 1.0f, -1, 0, 0,
                -1.0f, 1.0f, -1.0f, -1, 0, 0,

                //upper
                1.0f, 1.0f, 1.0f, 0, 1, 0,
                -1.0f, 1.0f, 1.0f, 0, 1, 0,
                -1.0f, 1.0f, -1.0f, 0, 1, 0,
                1.0f, 1.0f, -1.0f, 0, 1, 0,

                //bottom
                -1.0f, -1.0f, -1.0f, 0, -1, 0,
                1.0f, -1.0f, -1.0f, 0, -1, 0,
                1.0f, -1.0f, 1.0f, 0, -1, 0,
                -1.0f, -1.0f, 1.0f, 0, -1, 0,
        };
        GLuint indices[] = {
                0, 1, 2, 0, 2, 3, //front
                4, 5, 6, 4, 6, 7, //right
                8, 9, 10, 8, 10, 11, //back
                12, 13, 14, 12, 14, 15, //left
                16, 17, 18, 16, 18, 19, //upper
                20, 21, 22, 20, 22, 23, //bottom
        };


        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);


        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

    }

    void Dispose() override {
        sh.dispose();

        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }

    void Render(Float t) override {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sh.begin();

        Vec3 center{0, 0, 0};
        Vec3 cam = Quaternion::fromEulerAngles(pitch, 0, yaw) * Vec3{0, 0, -distance} + center;
        Vec3 lightPos = Quaternion::fromEulerAngles(pitch2, 0, yaw2) * Vec3{0, 0, -distance2} + center;

        Mat4 model = Mat4::identify;
        Mat4 proj = Mat4::perspective(70, width / height, -2, 100);
        Mat4 view = Mat4::lookAt(cam, center, {0, 1, 0});

        if (glfwGetKey(window, GLFW_KEY_A)) {
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
                pitch2 += deltaTime;
            } else {
                pitch += deltaTime;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_D)) {
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
                pitch2 -= deltaTime;
            } else {
                pitch -= deltaTime;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_W)) {
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
                if (yaw2 < Math::HALF_PI)
                    yaw2 += deltaTime;

            } else {
                if (yaw < Math::HALF_PI)
                    yaw += deltaTime;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_S)) {
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
                if (yaw > -Math::HALF_PI)
                    yaw2 -= deltaTime;

            } else {
                if (yaw > -Math::HALF_PI)
                    yaw -= deltaTime;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_MINUS)) {
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
                distance2 += deltaTime*3;

            } else {
                distance += deltaTime*3;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_EQUAL)) {
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
                distance2 -= deltaTime*3;

            } else {
                distance -= deltaTime*3;
            }
        }

        sh.set("world", model);
        sh.set("view", view);
        sh.set("projection", proj);
        sh.set("cameraPosition", cam);
        sh.set("lightPosition", lightPos);
        sh.set("time", t);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        sh.end();


    }
};


int main() {
    return Game1().Run();
}