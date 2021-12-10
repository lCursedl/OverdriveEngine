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
  Color m_color;
  SPtr<Texture> m_pBack;
  SPtr<Texture> m_finalTexture;
  SPtr<Texture> m_histogramTexture;
  Vector<SPtr<Texture>> m_vTextures;
  bool m_showDemo = true;

  struct Bones {
    Matrix4 bones[MAXBONES];
  };
};