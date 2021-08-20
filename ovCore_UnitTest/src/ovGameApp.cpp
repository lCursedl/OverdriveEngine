#include "ovGameApp.h"
#if OV_PLATFORM == OV_PLATFORM_WIN32
#include <Windows.h>
#endif // OV_PLATFORM == OV_PLATFORM_WIN32

void GameApp::onCreate() {
  auto& graphicAPI = g_graphicsAPI();
  graphicAPI.setTopology(TOPOLOGY::kTRIANGLES);
  RECT rc;
  GetClientRect(m_windowHandle, &rc);
  graphicAPI.setViewport(0, 0, rc.right, rc.bottom, 0.f, 1.f);

  SPtr<Model>model = make_shared<Model>();
  model->load("resources/models/Vela/Vela2.fbx");

  SPtr<Actor>myActor = make_shared<Actor>();
  myActor->addComponent(model);

  SPtr<SceneNode>myNode = make_shared<SceneNode>();
  myNode->setActor(myActor);

  auto& scene = SceneGraph::instance();

  scene.addNode(myNode);
}

void GameApp::onUpdate(float delta) {
  /*auto& graphicAPI = g_graphicsAPI();
  Vector<Matrix4> transforms;
  myModel->transformBones(delta, transforms);
  Bones cbBone;
  SIZE_T numObjects = transforms.size();
  for (uint32 i = 0; i < numObjects; ++i) {
    if (i < MAXBONES) {
      cbBone.bones[i] = transforms[i];
    }
  }
  graphicAPI.updateBuffer(m_bBuffer, &cbBone);*/
}

void GameApp::onRender() {
 /* auto& graphicAPI = g_graphicsAPI();
  graphicAPI.clearBackBuffer(m_color);
  auto& scene = SceneGraph::instance();
  scene.render();*/
}

void GameApp::onClear() {
}