#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "IRenderer.hpp"
class Shader;
class PointRenderer:public IRenderer {
public:
    PointRenderer();
   virtual ~PointRenderer();

   virtual  void initGL() override;
   virtual void setShader(Shader* shader) { m_shader = shader; }
    void setPoints(const std::vector<glm::vec2>& pts);
    void addPoint(const glm::vec2& p, const glm::vec3& color = {0.0f,1.0f,0.0f});
   virtual void clear()override;
   virtual void draw()override;

private:
    Shader* m_shader;
    GLuint m_vao{0}, m_vbo{0};
    bool m_initialized{false};

    // Í¨¹ý IRenderer ¼Ì³Ð
    void update() override;
};