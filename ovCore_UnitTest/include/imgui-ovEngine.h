#pragma once
#ifndef IMGUI_OVENGINE_H
#define IMGUI_OVENGINE_H



namespace ovEngineSDK {
  class RasterizerState;
  class DepthStencilState;
}

namespace ImGui {
  void init(void* window);
  void update(void* window, float delta);
  void render(void* window);
  void shutDown();
}

#endif // !IMGUI_OVENGINE_H
