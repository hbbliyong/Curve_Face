#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "IRenderer.hpp"
class Shader;
class SurfaceRenderer :IRenderer{
public:
    SurfaceRenderer();
   virtual ~SurfaceRenderer();

   virtual void initGL()override;
    void setShader(Shader* shader) { m_shader = shader; }

    // 顶点：vec3, 索引三角形列表
    void setMesh(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices);
    virtual   void clear() override;
    virtual void draw()override;

private:
    Shader* m_shader;
    GLuint m_vao{0}, m_vbo{0}, m_ebo{0};
    bool m_initialized{false};

    std::vector<unsigned int> m_indices;

    // 通过 IRenderer 继承
    void update() override;
};