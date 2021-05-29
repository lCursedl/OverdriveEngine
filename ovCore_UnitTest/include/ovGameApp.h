#pragma once
#include <ovBaseApp.h>
#include <ovVector4.h>
#include <ovVector3.h>
#include <ovVector2.h>
#include <ovMatrix4.h>
#include <ovModel.h>

using namespace ovEngineSDK;

class GameApp : public BaseApp
{
 public:
  GameApp() = default;
  ~GameApp() = default;

  void
  onCreate() override;

  void
  onUpdate() override;

  void
  onRender() override;

  void
  onClear() override;

 private:
  VertexShader* m_vs = nullptr;
  PixelShader* m_ps = nullptr;
  ShaderProgram* m_shaderProgram = nullptr;
  InputLayout* m_layout = nullptr;
  Buffer* m_cBuffer = nullptr;
  Buffer* m_vertexBuffer = nullptr;
  Buffer* m_indexBuffer = nullptr;
  COLOR m_color;
  Model* myModel = nullptr;

  struct Matrices {
    Matrix4 World;
    Matrix4 View;
    Matrix4 Projection;
    Vector4 Color;
  };

  struct Vertex {
    Vector3 Position;
    Vector2 Texcoord;
  };
};