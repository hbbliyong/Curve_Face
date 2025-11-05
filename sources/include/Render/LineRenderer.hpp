#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "IRenderer.hpp"

class Shader;
class LineRenderer : public IRenderer {
public:
    LineRenderer();
    virtual ~LineRenderer() override;

    virtual void initGL() override;
    virtual void setShader(Shader* shader) override { m_shader = shader; }
    virtual  void setPolyline(const std::vector<glm::vec2>& pts, const glm::vec3& color = {1,1,1});
    virtual void clear() override;
    virtual  void draw() override;
    virtual void cleanup() override {};

private:

    Shader* m_shader{nullptr};
    GLuint m_vao{0}, m_vbo{0};
    bool m_initialized{false};

    // Í¨¹ý IRenderer ¼Ì³Ð
    void update() override;
};