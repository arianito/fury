#include <fury/core.h>
#include  <cstddef>

GLuint prepareMesh(Mesh mesh) {
    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.m_Vertices.size() * sizeof(Vertex), &mesh.m_Vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.m_Indices.size() * sizeof(u32), &mesh.m_Indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, m_Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, m_Texcoord));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return VAO;
}

int main() {
    class Win1 : public Window {
        ObjLoader *loader = nullptr;
        Shader *shader = nullptr;
        Texture2d *texture = nullptr;
        Texture2d *normal = nullptr;
        u32 count = 0;
        GLuint *model = nullptr;
        u32 size = 0;

        void Create() override {
            loader = new ObjLoader("../temp/box.obj");
            texture = new Texture2d("../temp/texture.jpg");
            normal = new Texture2d("../temp/normal.jpeg");
            count = loader->m_Meshes.size();
            model = new GLuint[count];
            int i = 0;
            for (auto &m: loader->m_Meshes) {
                model[i] = prepareMesh(m);
                i++;
            }
            size = loader->m_Meshes.at(0).m_Indices.size();

            shader = new Shader(R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 Coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));

    Normal = mat3(transpose(inverse(model))) * aNormal;

    Coord = aCoord;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)", R"(
#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 Coord;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 normal = normalize(texture(texture2, Coord).rgb * 2.0 - 1.0);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1) * texture(texture1, Coord);
}
)");
        }

        void Update() override {
        }

        Mat4 view{0};
        Mat4 proj{0};
        f32 distance = 10;
        f32 lastDistance = 0;
        Vec3 targetPos{0};
        Vec3 lastPos{0};

        f32 xrot = 0;
        f32 yrot = 0;
        f32 lastX{0};
        f32 lastY{0};
        Vec2 lastMouse{};
        i32 mode = 0;

        void Render() override {

            if (Input::KeyPress(KEY_LEFT_ALT)) {
                if (Input::MouseDown(MOUSE_BUTTON_LEFT)) {
                    lastMouse = Input::GetXY();
                    lastX = xrot;
                    lastY = yrot;
                    mode = 1;
                }
                if (Input::MousePress(MOUSE_BUTTON_LEFT) && mode == 1) {
                    auto pos = Input::GetXY();
                    xrot = Math::lerp(xrot, lastX + (pos.x() - lastMouse.x()) / 100.0, Time::DeltaTime * 5);
                    yrot = Math::lerp(yrot, lastY - (pos.y() - lastMouse.y()) / 100.0, Time::DeltaTime * 5);
                    yrot = Math::clamp(yrot, -Math::HALF_PI + 0.01f, Math::HALF_PI - 0.01f);
                }
            }
            auto cameraPos = targetPos + Quaternion::fromEulerAngles(yrot, 0.0f, xrot) * (distance * Vec3::back);
            if (Input::KeyPress(KEY_LEFT_SHIFT)) {

                if (Input::MouseDown(MOUSE_BUTTON_LEFT)) {
                    lastMouse = Input::GetXY();
                    lastPos = targetPos;
                    mode = 2;
                }
                if (Input::MousePress(MOUSE_BUTTON_LEFT) && mode == 2) {
                    auto pos = Input::GetXY();
                    auto norm = (targetPos - cameraPos).normal();
                    auto y = Vec3::cross(norm, Vec3::up).normal();
                    targetPos = Vec3::lerp(targetPos, lastPos +
                                y * (lastMouse.x() - pos.x()) / 200.0 +
                                Vec3::cross(norm, y) * (lastMouse.y() - pos.y()) / 200.0, Time::DeltaTime * 10);
                }

                if (Input::MousePress(MOUSE_BUTTON_RIGHT)) {
                    distance = Math::lerp(distance, distance -Input::GetAxis(AXIS_MOUSE_Y) * Time::DeltaTime * 5, Time::DeltaTime * 10);
                    if (distance < 0.1f)
                        distance = 0.1f;
                }
            }
            view = Mat4::lookAt(cameraPos, targetPos, Vec3::up);
            proj = Mat4::perspective(45 * Math::DEG2RAD, Display::GetRatio(), 0.01f, 300.0f);

            DebugDraw::SetCamera(view, proj);
            shader->Begin();

            shader->SetParameter("lightPos", Vec3(3, 3, 3));
            shader->SetParameter("viewPos", cameraPos);
            shader->SetParameter("lightColor", Vec3(1, 1, 1));
            shader->SetParameter("objectColor", Vec3(1.0f, 1.0f, 1.0f));

            shader->SetParameter("projection", proj);
            shader->SetParameter("view", view);
            glActiveTexture(GL_TEXTURE0);
            texture->Begin();
            glActiveTexture(GL_TEXTURE1);
            normal->Begin();

            shader->SetParameter("texture1", 0);
            shader->SetParameter("texture2", 1);



            shader->SetParameter("model", Mat4::createTranslation(Vec3(-1.3f, 0)));
            for (int i = 0; i < count; i++) {
                glBindVertexArray(model[i]);
                glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
            }

            shader->SetParameter("model", Mat4::createTranslation(Vec3(1.3, 0, 0)) * Mat4::fromQuaternion(Quaternion::fromEulerAngles(0, 0, 0.3f)));
            for (int i = 0; i < count; i++) {
                glBindVertexArray(model[i]);
                glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
            }

            shader->SetParameter("model", Mat4::createTranslation(Vec3(0, 2, 0)) * Mat4::fromQuaternion(Quaternion::fromEulerAngles(0, 0, -0.3f)));
            for (int i = 0; i < count; i++) {
                glBindVertexArray(model[i]);
                glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
            }
            shader->End();


            DebugDraw::Pivot(Vec3::zero);


        }
    };
    return Win1().run();
}