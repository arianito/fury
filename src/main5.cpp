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
        Vec3 targetPos{0};

        Camera2d *camera = nullptr;
        ObjLoader *loader = nullptr;
        Shader *shader = nullptr;
        Texture2d *texture = nullptr;
        u32 count = 0;
        GLuint *model = nullptr;
        u32 size = 0;

        void Create() override {
            camera = new Camera2d();
            loader = new ObjLoader("../temp/box.obj");
            texture = new Texture2d("../temp/texture.jpg");
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

uniform sampler2D texture0;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.8;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 result = (ambient + specular) * objectColor * (floor(distance(viewPos, FragPos)) / 20.0f);
    FragColor = vec4(result, 1) * texture(texture0, Coord);
}
)");
        }

        void Update() override {
            camera->Update();   
        }

        void Render() override {
            xrot += Input::GetAxis(AXIS_MOUSE_X) * Time::DeltaTime * 1;
            yrot += Input::GetAxis(AXIS_MOUSE_Y) * Time::DeltaTime * 1;
            yrot = Math::clamp(yrot, -Math::HALF_PI+0.01f, Math::HALF_PI-0.01f);
            auto cameraPos = targetPos + Quaternion::fromEulerAngles(xrot, 0.0f, yrot) * (6.0f * Vec3::back);

            auto plane = Plane(Vec3::up, targetPos);
            auto cameraProject = plane.closestPointOnPlane(cameraPos);


            auto norm = (targetPos - cameraPos).normal();
            targetPos = targetPos + norm * Input::GetAxis(AXIS_VERTICAL) * Time::DeltaTime * 8 + Vec3::cross(norm, Vec3::up).normal() * Input::GetAxis(AXIS_HORIZONTAL) * Time::DeltaTime * 8;

            view = Mat4::lookAt(cameraPos, targetPos, Vec3::up);
            proj = Mat4::perspective(75 * Math::DEG2RAD, Display::GetRatio(), 0.01f, 100.0f);

            DebugDraw::SetCamera(view, proj);
            shader->Begin();

            shader->SetParameter("lightPos", Vec3(0, 0, 0));
            shader->SetParameter("viewPos", cameraPos);
            shader->SetParameter("lightColor", Vec3(1, 1, 1));
            shader->SetParameter("objectColor", Vec3(1.0f, 1.0f, 1.0f));

            shader->SetParameter("projection", proj);
            shader->SetParameter("view", view);
            texture->Begin();

            int mx = 10;
            float sz = 5.0;

            float a = 5;


            auto x0 = Math::floor(cameraPos.x() / a) * a;
            auto y0 = Math::floor(cameraPos.y() / a) * a;
            auto z0 = Math::floor(cameraPos.z() / a) * a;

            for(int i = -mx; i < mx; i++) {
                for(int j = -mx; j < mx; j++) {
                    for(int k = -mx; k < mx; k++) {
                        shader->SetParameter("model", Mat4::createTranslation(Vec3(x0 + i*sz, y0 + k*sz, z0 + j*sz)));
                        for (int i = 0; i < count; i++) {
                            glBindVertexArray(model[i]);
                            glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
                        }
                    }
                }
            }
            texture->End();
            shader->End();

            DebugDraw::Pivot(Vec3::zero);


        }
    };
    return Win1().run();
}