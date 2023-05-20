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

        Mat4 view{0};
        Mat4 proj{0};
        f32 xrot = 0;
        f32 yrot = 0;

        Camera2d *camera = nullptr;
        ObjLoader *loader = nullptr;
        Shader *shader = nullptr;
        Texture2d *texture = nullptr;
        GLuint *model = nullptr;
        unsigned int size = 0;

        void Create() override {
            camera = new Camera2d();
            loader = new ObjLoader("../temp/cars.obj");
            texture = new Texture2d("../temp/texture.jpg");
            model = new GLuint[loader->m_Meshes.size()];
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

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)", R"(
#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
)");
        }

        void Update() override {
            camera->Update();
            DebugDraw::SetCamera(camera->GetView(), camera->GetProjection());
        }

        void Render() override {

            xrot -= Input::GetAxis(AXIS_HORIZONTAL) * Time::DeltaTime * 4;
            yrot -= Input::GetAxis(AXIS_VERTICAL) * Time::DeltaTime * 4;

            Vec3 pos = Quaternion::fromEulerAngles(xrot, 0.0f, yrot) * (10.0f * Vec3::forward);
            view = Mat4::lookAt(pos, Vec3::zero, Vec3::up);
            proj = Mat4::perspective(45 * Math::DEG2RAD, Display::GetRatio(), 0.1f, 100.0f);

            shader->Begin();

            shader->SetParameter("lightPos", pos);
            shader->SetParameter("viewPos", pos);
            shader->SetParameter("lightColor", Vec3(1, 1, 1));
            shader->SetParameter("objectColor", Vec3(1.0f, 0.5f, 0.31f));

            shader->SetParameter("projection", proj);
            shader->SetParameter("view", view);
            shader->SetParameter("model", Mat4::createTranslation(Vec3(0, 0, 0)));

            for (int i = 0; i < 5; i++) {
                glBindVertexArray(model[i]);
                glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
            }

            shader->End();


        }
    };
    return Win1().run();
}