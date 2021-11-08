#include "ovGameApp.h"
#include "ovCamera.h"
#include "ovBaseInputManager.h"
#if OV_PLATFORM == OV_PLATFORM_WIN32
#include <Windows.h>
#endif // OV_PLATFORM == OV_PLATFORM_WIN32
#include <imgui.h>
#include "imgui-ovEngine.h"

void
GameApp::onCreate() {
  auto& graphicAPI = g_graphicsAPI();
  graphicAPI.setTopology(TOPOLOGY::kTRIANGLES);
  RECT rc;
  GetClientRect(m_windowHandle, &rc);
  graphicAPI.setViewport(0, 0, rc.right, rc.bottom, 0.f, 1.f);

  ImGui::init(m_windowHandle);

  SPtr<Model>model = make_shared<Model>();
  model->load("resources/models/Vela/Vela2.fbx");

  SPtr<Actor>myActor = make_shared<Actor>();
  myActor->addComponent(model);

  SPtr<SceneNode>myNode = make_shared<SceneNode>();
  myNode->setActor(myActor);

  SPtr<Camera>cam = make_shared<Camera>();
  cam->init(Vector3(0.f, 0.f, -250.f),
            Vector3(0.f, 0.f, -1.f),
            Vector3(0.f, 1.f, 0.f),
            0.01f,
            PerspectiveMatrix(70.f,
                              800.f,
                              600.f,
                              0.01f,
                              3000.f));
  SPtr<Actor>camActor = make_shared<Actor>();
  camActor->addComponent(cam);

  SPtr<SceneNode>camNode = make_shared<SceneNode>();
  camNode->setActor(camActor);

  SPtr<Model>plane = make_shared<Model>();
  plane->load("resources/models/plano.fbx");

  SPtr<Actor>planeActor = make_shared<Actor>();
  planeActor->addComponent(plane);

  SPtr<SceneNode>planeNode = make_shared<SceneNode>();
  planeNode->setActor(planeActor);

  auto& scene = SceneGraph::instance();

  scene.addNode(myNode);
  scene.addNode(camNode);
  scene.addNode(planeNode);
}

void
GameApp::onUpdate(float delta) {
  ImGui::NewFrame();
  ImGui::ShowDemoWindow(&m_showDemo);
  //auto& graphicAPI = g_graphicsAPI();
  /*Vector<Matrix4> transforms;
  myModel->transformBones(delta, transforms);
  Bones cbBone;
  SIZE_T numObjects = transforms.size();
  for (uint32 i = 0; i < numObjects; ++i) {
    if (i < MAXBONES) {
      cbBone.bones[i] = transforms[i];
    }
  }
  graphicAPI.updateBuffer(m_bBuffer, &cbBone);*/
  SPtr<Camera> cam = SceneGraph::instance().getActiveCamera();
  if (cam) {
    //Get forward input
    cam->setForward(g_baseInput().isKeyPressed(KEYS::kW));
    //Get backard input
    cam->setBackward(g_baseInput().isKeyPressed(KEYS::kS));
    //Get left input
    cam->setLeft(g_baseInput().isKeyPressed(KEYS::kA));
    //Get right input
    cam->setRight(g_baseInput().isKeyPressed(KEYS::kD));
    //Get up input
    cam->setUp(g_baseInput().isKeyPressed(KEYS::kQ));
    //Get down input
    cam->setDown(g_baseInput().isKeyPressed(KEYS::kE));
    //Get roll left input
    cam->setRotateLeft(g_baseInput().isKeyPressed(KEYS::kLEFT));
    //Get roll right input
    cam->setRotateRight(g_baseInput().isKeyPressed(KEYS::kRIGHT));

    float mouseX, mouseY;
    g_baseInput().getMouseAxis(mouseX, mouseY);
    if (mouseX != 0 && mouseY != 0) {
      cam->rotate(Vector2(mouseX, mouseY), delta);
    }
    cam->roll(delta);
    cam->update(delta);
  }
}

void
GameApp::onRender() {
  //ImGui::Render();
  ImGui::render(m_windowHandle);
}

void
GameApp::onClear() {
  ImGui::shutDown();
}