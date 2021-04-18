#pragma once
#include <ovBaseApp.h>

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
};