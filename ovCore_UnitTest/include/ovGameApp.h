#pragma once
#include <ovBaseApp.h>
#include <ovMatrix4.h>

using namespace ovEngineSDK;

class GameApp : public BaseApp
{
 public:
  GameApp() = default;
  ~GameApp() = default;

  void
  onCreate()                                                          override;

  void
  onUpdate(float delta)                                               override;

  void
  onRender()                                                          override;

  void
  onClear()                                                           override;

  void
  resize(int32 width, int32 height)                                   override;

 private:
  
  void
  showTreeNodes(SPtr<SceneNode> node);

  void
  showActorInfo();
  
  Color m_color;
  SPtr<Texture> m_pBack;
  SPtr<Texture> m_finalTexture;
  Vector<SPtr<Texture>> m_deferredTextures;
  bool m_showDemo = true;
  bool m_showOmniWindow = false;

  /*struct Bones {
    Matrix4 bones[MAXBONES];
  };*/
};