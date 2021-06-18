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
  onUpdate(float delta) override;

  void
  onRender() override;

  void
  onClear() override;

 private:
  SPtr<VertexShader> m_vs;
  SPtr<PixelShader> m_ps;
  SPtr<ShaderProgram> m_shaderProgram;
  SPtr<InputLayout> m_layout;
  SPtr<Buffer> m_cBuffer;
  SPtr<Buffer> m_bBuffer;
  Color m_color;
  SPtr<Model> myModel;

  struct Matrices {
    Matrix4 world;
    Matrix4 view;
    Matrix4 projection;
    Vector4 color;
  };

  struct Bones {
    Matrix4 bones[MAXBONES];
  };
};