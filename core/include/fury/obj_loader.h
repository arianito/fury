#pragma once


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "vec2.h"
#include "vec3.h"
#include "vertex.h"
#include "mesh.h"
#include "material.h"

namespace algorithm {
    bool SameSide(Vec3 p1, Vec3 p2, Vec3 a, Vec3 b) {
        Vec3 cp1 = Vec3::cross(b - a, p1 - a);
        Vec3 cp2 = Vec3::cross(b - a, p2 - a);
        if (Vec3::dot(cp1, cp2) >= 0)
            return true;
        else
            return false;
    }

    Vec3 GenTriNormal(Vec3 t1, Vec3 t2, Vec3 t3) {
        Vec3 u = t2 - t1;
        Vec3 v = t3 - t1;
        Vec3 normal = Vec3::cross(u, v);
        return normal;
    }

    bool inTriangle(Vec3 point, Vec3 tri1, Vec3 tri2, Vec3 tri3) {
        bool within_tri_prisim = SameSide(point, tri1, tri2, tri3) && SameSide(point, tri2, tri1, tri3)
                                 && SameSide(point, tri3, tri1, tri2);
        if (!within_tri_prisim)
            return false;
        Vec3 n = GenTriNormal(tri1, tri2, tri3);
        Vec3 proj = Vec3::project(point, n);
        if (proj.sqrMagnitude() == 0)
            return true;
        else
            return false;
    }

    inline void split(const std::string &in,
                      std::vector<std::string> &out,
                      std::string token) {
        out.clear();
        std::string temp;
        for (int i = 0; i < int(in.size()); i++) {
            std::string test = in.substr(i, token.size());
            if (test == token) {
                if (!temp.empty()) {
                    out.push_back(temp);
                    temp.clear();
                    i += (int) token.size() - 1;
                } else {
                    out.push_back("");
                }
            } else if (i + token.size() >= in.size()) {
                temp += in.substr(i, token.size());
                out.push_back(temp);
                break;
            } else {
                temp += in[i];
            }
        }
    }

    inline std::string tail(const std::string &in) {
        size_t token_start = in.find_first_not_of(" \t");
        size_t space_start = in.find_first_of(" \t", token_start);
        size_t tail_start = in.find_first_not_of(" \t", space_start);
        size_t tail_end = in.find_last_not_of(" \t");
        if (tail_start != std::string::npos && tail_end != std::string::npos) {
            return in.substr(tail_start, tail_end - tail_start + 1);
        } else if (tail_start != std::string::npos) {
            return in.substr(tail_start);
        }
        return "";
    }

    inline std::string firstToken(const std::string &in) {
        if (!in.empty()) {
            size_t token_start = in.find_first_not_of(" \t");
            size_t token_end = in.find_first_of(" \t", token_start);
            if (token_start != std::string::npos && token_end != std::string::npos) {
                return in.substr(token_start, token_end - token_start);
            } else if (token_start != std::string::npos) {
                return in.substr(token_start);
            }
        }
        return "";
    }

    template<class T>
    inline const T &getElement(const std::vector<T> &elements, std::string &index) {
        int idx = std::stoi(index);
        if (idx < 0)
            idx = int(elements.size()) + idx;
        else
            idx--;
        return elements[idx];
    }
}

class ObjLoader {
public:
    ObjLoader(const std::string& name) {
        LoadFile(name);
    }
    ~ObjLoader() {
        m_Meshes.clear();
    }
    bool LoadFile(const std::string& name) {
        std::ifstream file(name);

        if (!file.is_open())
            return false;

        m_Meshes.clear();
        m_Vertices.clear();
        m_Indices.clear();

        std::vector<Vec3> Positions;
        std::vector<Vec2> TCoords;
        std::vector<Vec3> Normals;

        std::vector<Vertex> Vertices;
        std::vector<unsigned int> Indices;

        std::vector<std::string> MeshMatNames;

        bool listening = false;
        std::string meshname;

        Mesh tempMesh;

#ifdef OBJL_CONSOLE_OUTPUT
        const unsigned int outputEveryNth = 1000;
            unsigned int outputIndicator = outputEveryNth;
#endif

        std::string curline;
        while (std::getline(file, curline)) {
#ifdef OBJL_CONSOLE_OUTPUT
            if ((outputIndicator = ((outputIndicator + 1) % outputEveryNth)) == 1)
                {
                    if (!meshname.empty())
                    {
                        std::cout
                            << "\r- " << meshname
                            << "\t| vertices > " << Positions.size()
                            << "\t| texcoords > " << TCoords.size()
                            << "\t| normals > " << Normals.size()
                            << "\t| triangles > " << (Vertices.size() / 3)
                            << (!MeshMatNames.empty() ? "\t| material: " + MeshMatNames.back() : "");
                    }
                }
#endif

            // Generate a Mesh Object or Prepare for an object to be created
            if (algorithm::firstToken(curline) == "o" || algorithm::firstToken(curline) == "g" || curline[0] == 'g') {
                if (!listening) {
                    listening = true;

                    if (algorithm::firstToken(curline) == "o" || algorithm::firstToken(curline) == "g") {
                        meshname = algorithm::tail(curline);
                    } else {
                        meshname = "unnamed";
                    }
                } else {
                    // Generate the mesh to put into the array

                    if (!Indices.empty() && !Vertices.empty()) {
                        // Create Mesh
                        tempMesh = Mesh(Vertices, Indices);
                        tempMesh.m_Name = meshname;

                        // Insert Mesh
                        m_Meshes.push_back(tempMesh);

                        // Cleanup
                        Vertices.clear();
                        Indices.clear();
                        meshname.clear();

                        meshname = algorithm::tail(curline);
                    } else {
                        if (algorithm::firstToken(curline) == "o" || algorithm::firstToken(curline) == "g") {
                            meshname = algorithm::tail(curline);
                        } else {
                            meshname = "unnamed";
                        }
                    }
                }
#ifdef OBJL_CONSOLE_OUTPUT
                std::cout << std::endl;
                    outputIndicator = 0;
#endif
            }
            // Generate a Vertex Position
            if (algorithm::firstToken(curline) == "v") {
                std::vector<std::string> spos;
                Vec3 vpos;
                algorithm::split(algorithm::tail(curline), spos, " ");

                vpos.x() = std::stof(spos[0]);
                vpos.y() = std::stof(spos[1]);
                vpos.z() = std::stof(spos[2]);

                Positions.push_back(vpos);
            }
            // Generate a Vertex Texture Coordinate
            if (algorithm::firstToken(curline) == "vt") {
                std::vector<std::string> stex;
                Vec2 vtex;
                algorithm::split(algorithm::tail(curline), stex, " ");

                vtex.x() = std::stof(stex[0]);
                vtex.y() = std::stof(stex[1]);

                TCoords.push_back(vtex);
            }
            // Generate a Vertex Normal;
            if (algorithm::firstToken(curline) == "vn") {
                std::vector<std::string> snor;
                Vec3 vnor;
                algorithm::split(algorithm::tail(curline), snor, " ");

                vnor.x() = std::stof(snor[0]);
                vnor.y() = std::stof(snor[1]);
                vnor.z() = std::stof(snor[2]);

                Normals.push_back(vnor);
            }
            // Generate a Face (vertices & indices)
            if (algorithm::firstToken(curline) == "f") {
                // Generate the vertices
                std::vector<Vertex> vVerts;
                GenVerticesFromRawOBJ(vVerts, Positions, TCoords, Normals, curline);

                // Add Vertices
                for (int i = 0; i < int(vVerts.size()); i++) {
                    Vertices.push_back(vVerts[i]);

                    m_Vertices.push_back(vVerts[i]);
                }

                std::vector<unsigned int> iIndices;

                VertexTriangluation(iIndices, vVerts);

                // Add Indices
                for (int i = 0; i < int(iIndices.size()); i++) {
                    unsigned int indnum = (unsigned int) ((Vertices.size()) - vVerts.size()) + iIndices[i];
                    Indices.push_back(indnum);

                    indnum = (unsigned int) ((m_Vertices.size()) - vVerts.size()) + iIndices[i];
                    m_Indices.push_back(indnum);

                }
            }
            // Get Mesh Material Name
            if (algorithm::firstToken(curline) == "usemtl") {
                MeshMatNames.push_back(algorithm::tail(curline));

                // Create new Mesh, if Material changes within a group
                if (!Indices.empty() && !Vertices.empty()) {
                    // Create Mesh
                    tempMesh = Mesh(Vertices, Indices);
                    tempMesh.m_Name = meshname;
                    int i = 2;
                    while (1) {
                        tempMesh.m_Name = meshname + "_" + std::to_string(i);

                        for (auto &m : m_Meshes)
                            if (m.m_Name == tempMesh.m_Name)
                                continue;
                        break;
                    }

                    // Insert Mesh
                    m_Meshes.push_back(tempMesh);

                    // Cleanup
                    Vertices.clear();
                    Indices.clear();
                }

#ifdef OBJL_CONSOLE_OUTPUT
                outputIndicator = 0;
#endif
            }
            // Load Materials
            if (algorithm::firstToken(curline) == "mtllib") {
                // Generate LoadedMaterial

                // Generate a path to the material file
                std::vector<std::string> temp;
                algorithm::split(name, temp, "/");

                std::string pathtomat = "";

                if (temp.size() != 1) {
                    for (int i = 0; i < temp.size() - 1; i++) {
                        pathtomat += temp[i] + "/";
                    }
                }


                pathtomat += algorithm::tail(curline);

#ifdef OBJL_CONSOLE_OUTPUT
                std::cout << std::endl << "- find materials in: " << pathtomat << std::endl;
#endif

                // Load Materials
                LoadMaterials(pathtomat);
            }
        }

#ifdef OBJL_CONSOLE_OUTPUT
        std::cout << std::endl;
#endif

        // Deal with last mesh

        if (!Indices.empty() && !Vertices.empty()) {
            // Create Mesh
            tempMesh = Mesh(Vertices, Indices);
            tempMesh.m_Name = meshname;

            // Insert Mesh
            m_Meshes.push_back(tempMesh);
        }

        file.close();

        // Set Materials for each Mesh
        for (int i = 0; i < MeshMatNames.size(); i++) {
            std::string matname = MeshMatNames[i];

            // Find corresponding material name in loaded materials
            // when found copy material variables into mesh material
            for (int j = 0; j < m_Materials.size(); j++) {
                if (m_Materials[j].m_Name == matname) {
                    m_Meshes[i].m_Material = m_Materials[j];
                    break;
                }
            }
        }

        return !(m_Meshes.empty() && m_Vertices.empty() && m_Indices.empty());
    }

    // Loaded Mesh Objects
    std::vector<Mesh> m_Meshes;
    // Loaded Vertex Objects
    std::vector<Vertex> m_Vertices;
    // Loaded Index Positions
    std::vector<unsigned int> m_Indices;
    // Loaded Material Objects
    std::vector<Material> m_Materials;

private:
    // Generate vertices from a list of positions,
    //	tcoords, normals and a face line
    void GenVerticesFromRawOBJ(std::vector<Vertex> &oVerts,
                               const std::vector<Vec3> &iPositions,
                               const std::vector<Vec2> &iTCoords,
                               const std::vector<Vec3> &iNormals,
                               std::string icurline) {
        std::vector<std::string> sface, svert;
        Vertex vVert;
        algorithm::split(algorithm::tail(icurline), sface, " ");

        bool noNormal = false;

        // For every given vertex do this
        for (int i = 0; i < int(sface.size()); i++) {
            // See What type the vertex is.
            int vtype;

            algorithm::split(sface[i], svert, "/");

            // Check for just position - v1
            if (svert.size() == 1) {
                // Only position
                vtype = 1;
            }

            // Check for position & texture - v1/vt1
            if (svert.size() == 2) {
                // Position & Texture
                vtype = 2;
            }

            // Check for Position, Texture and Normal - v1/vt1/vn1
            // or if Position and Normal - v1//vn1
            if (svert.size() == 3) {
                if (svert[1] != "") {
                    // Position, Texture, and Normal
                    vtype = 4;
                } else {
                    // Position & Normal
                    vtype = 3;
                }
            }

            // Calculate and store the vertex
            switch (vtype) {
                case 1: // P
                {
                    vVert.m_Position = algorithm::getElement(iPositions, svert[0]);
                    vVert.m_Texcoord = Vec2(0, 0);
                    noNormal = true;
                    oVerts.push_back(vVert);
                    break;
                }
                case 2: // P/T
                {
                    vVert.m_Position = algorithm::getElement(iPositions, svert[0]);
                    vVert.m_Texcoord = algorithm::getElement(iTCoords, svert[1]);
                    noNormal = true;
                    oVerts.push_back(vVert);
                    break;
                }
                case 3: // P//N
                {
                    vVert.m_Position = algorithm::getElement(iPositions, svert[0]);
                    vVert.m_Texcoord = Vec2(0, 0);
                    vVert.m_Normal = algorithm::getElement(iNormals, svert[2]);
                    oVerts.push_back(vVert);
                    break;
                }
                case 4: // P/T/N
                {
                    vVert.m_Position = algorithm::getElement(iPositions, svert[0]);
                    vVert.m_Texcoord = algorithm::getElement(iTCoords, svert[1]);
                    vVert.m_Normal = algorithm::getElement(iNormals, svert[2]);
                    oVerts.push_back(vVert);
                    break;
                }
                default: {
                    break;
                }
            }
        }
        if (noNormal) {
            Vec3 A = oVerts[0].m_Position - oVerts[1].m_Position;
            Vec3 B = oVerts[2].m_Position - oVerts[1].m_Position;
            Vec3 normal = Vec3::cross(A, B);
            for (int i = 0; i < int(oVerts.size()); i++) {
                oVerts[i].m_Normal = normal;
            }
        }
    }

    void VertexTriangluation(std::vector<unsigned int> &oIndices,
                             const std::vector<Vertex> &iVerts) {
        if (iVerts.size() < 3) {
            return;
        }
        if (iVerts.size() == 3) {
            oIndices.push_back(0);
            oIndices.push_back(1);
            oIndices.push_back(2);
            return;
        }
        std::vector<Vertex> tVerts = iVerts;
        while (true) {
            for (int i = 0; i < int(tVerts.size()); i++) {
                Vertex pPrev;
                if (i == 0) {
                    pPrev = tVerts[tVerts.size() - 1];
                } else {
                    pPrev = tVerts[i - 1];
                }
                Vertex pCur = tVerts[i];
                Vertex pNext;
                if (i == tVerts.size() - 1) {
                    pNext = tVerts[0];
                } else {
                    pNext = tVerts[i + 1];
                }
                if (tVerts.size() == 3) {
                    for (int j = 0; j < int(tVerts.size()); j++) {
                        if (iVerts[j].m_Position == pCur.m_Position)
                            oIndices.push_back(j);
                        if (iVerts[j].m_Position == pPrev.m_Position)
                            oIndices.push_back(j);
                        if (iVerts[j].m_Position == pNext.m_Position)
                            oIndices.push_back(j);
                    }

                    tVerts.clear();
                    break;
                }
                if (tVerts.size() == 4) {
                    // Create a triangle from pCur, pPrev, pNext
                    for (int j = 0; j < int(iVerts.size()); j++) {
                        if (iVerts[j].m_Position == pCur.m_Position)
                            oIndices.push_back(j);
                        if (iVerts[j].m_Position == pPrev.m_Position)
                            oIndices.push_back(j);
                        if (iVerts[j].m_Position == pNext.m_Position)
                            oIndices.push_back(j);
                    }

                    Vec3 tempVec;
                    for (int j = 0; j < int(tVerts.size()); j++) {
                        if (tVerts[j].m_Position != pCur.m_Position
                            && tVerts[j].m_Position != pPrev.m_Position
                            && tVerts[j].m_Position != pNext.m_Position) {
                            tempVec = tVerts[j].m_Position;
                            break;
                        }
                    }
                    for (int j = 0; j < int(iVerts.size()); j++) {
                        if (iVerts[j].m_Position == pPrev.m_Position)
                            oIndices.push_back(j);
                        if (iVerts[j].m_Position == pNext.m_Position)
                            oIndices.push_back(j);
                        if (iVerts[j].m_Position == tempVec)
                            oIndices.push_back(j);
                    }
                    tVerts.clear();
                    break;
                }
                float angle = Vec3::angle(pPrev.m_Position - pCur.m_Position, pNext.m_Position - pCur.m_Position) *
                              (180 / 3.14159265359);
                if (angle <= 0 && angle >= 180)
                    continue;
                bool inTri = false;
                for (int j = 0; j < int(iVerts.size()); j++) {
                    if (algorithm::inTriangle(iVerts[j].m_Position, pPrev.m_Position, pCur.m_Position, pNext.m_Position)
                        && iVerts[j].m_Position != pPrev.m_Position
                        && iVerts[j].m_Position != pCur.m_Position
                        && iVerts[j].m_Position != pNext.m_Position) {
                        inTri = true;
                        break;
                    }
                }
                if (inTri)
                    continue;
                for (int j = 0; j < int(iVerts.size()); j++) {
                    if (iVerts[j].m_Position == pCur.m_Position)
                        oIndices.push_back(j);
                    if (iVerts[j].m_Position == pPrev.m_Position)
                        oIndices.push_back(j);
                    if (iVerts[j].m_Position == pNext.m_Position)
                        oIndices.push_back(j);
                }
                for (int j = 0; j < int(tVerts.size()); j++) {
                    if (tVerts[j].m_Position == pCur.m_Position) {
                        tVerts.erase(tVerts.begin() + j);
                        break;
                    }
                }
                i = -1;
            }
            if (oIndices.size() == 0)
                break;
            if (tVerts.size() == 0)
                break;
        }
    }
    bool LoadMaterials(std::string path) {
        if (path.substr(path.size() - 4, path.size()) != ".mtl")
            return false;
        std::ifstream file(path);
        if (!file.is_open())
            return false;
        Material tempMaterial;
        bool listening = false;
        std::string curline;
        while (std::getline(file, curline)) {
            if (algorithm::firstToken(curline) == "newmtl") {
                if (!listening) {
                    listening = true;

                    if (curline.size() > 7) {
                        tempMaterial.m_Name = algorithm::tail(curline);
                    } else {
                        tempMaterial.m_Name = "none";
                    }
                } else {
                    m_Materials.push_back(tempMaterial);
                    tempMaterial = Material();
                    if (curline.size() > 7) {
                        tempMaterial.m_Name = algorithm::tail(curline);
                    } else {
                        tempMaterial.m_Name = "none";
                    }
                }
            }
            if (algorithm::firstToken(curline) == "Ka") {
                std::vector<std::string> temp;
                algorithm::split(algorithm::tail(curline), temp, " ");

                if (temp.size() != 3)
                    continue;

                tempMaterial.m_Ka.x() = std::stof(temp[0]);
                tempMaterial.m_Ka.y() = std::stof(temp[1]);
                tempMaterial.m_Ka.z() = std::stof(temp[2]);
            }
            if (algorithm::firstToken(curline) == "Kd") {
                std::vector<std::string> temp;
                algorithm::split(algorithm::tail(curline), temp, " ");
                if (temp.size() != 3)
                    continue;
                tempMaterial.m_Kd.x() = std::stof(temp[0]);
                tempMaterial.m_Kd.y() = std::stof(temp[1]);
                tempMaterial.m_Kd.z() = std::stof(temp[2]);
            }
            if (algorithm::firstToken(curline) == "Ks") {
                std::vector<std::string> temp;
                algorithm::split(algorithm::tail(curline), temp, " ");
                if (temp.size() != 3)
                    continue;
                tempMaterial.m_Ks.x() = std::stof(temp[0]);
                tempMaterial.m_Ks.y() = std::stof(temp[1]);
                tempMaterial.m_Ks.z() = std::stof(temp[2]);
            }
            if (algorithm::firstToken(curline) == "Ns") {
                tempMaterial.m_Ns = std::stof(algorithm::tail(curline));
            }
            if (algorithm::firstToken(curline) == "Ni") {
                tempMaterial.m_Ni = std::stof(algorithm::tail(curline));
            }
            if (algorithm::firstToken(curline) == "d") {
                tempMaterial.m_D = std::stof(algorithm::tail(curline));
            }
            if (algorithm::firstToken(curline) == "illum") {
                tempMaterial.m_Illuminant = std::stoi(algorithm::tail(curline));
            }
            if (algorithm::firstToken(curline) == "map_Ka") {
                tempMaterial.m_MapKa = algorithm::tail(curline);
            }
            if (algorithm::firstToken(curline) == "map_Kd") {
                tempMaterial.m_MapKd = algorithm::tail(curline);
            }
            if (algorithm::firstToken(curline) == "map_Ks") {
                tempMaterial.m_MapKs = algorithm::tail(curline);
            }
            if (algorithm::firstToken(curline) == "map_Ns") {
                tempMaterial.m_MapNs = algorithm::tail(curline);
            }
            if (algorithm::firstToken(curline) == "map_d") {
                tempMaterial.m_MapD = algorithm::tail(curline);
            }
            if (algorithm::firstToken(curline) == "map_Bump" || algorithm::firstToken(curline) == "map_bump" ||
                algorithm::firstToken(curline) == "bump") {
                tempMaterial.m_MapBump = algorithm::tail(curline);
            }
        }
        m_Materials.push_back(tempMaterial);
        return !m_Materials.empty();
    }
};