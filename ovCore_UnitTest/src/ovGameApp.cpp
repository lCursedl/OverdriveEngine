#include "ovGameApp.h"
#include "ovCamera.h"
#include "ovBaseInputManager.h"
#if OV_PLATFORM == OV_PLATFORM_WIN32
#include <Windows.h>
#endif // OV_PLATFORM == OV_PLATFORM_WIN32

void
GameApp::onCreate() {
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

  auto& scene = SceneGraph::instance();

  scene.addNode(myNode);
  scene.addNode(camNode);
}

void
GameApp::onUpdate(float delta) {
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
    if (g_baseInput().isKeyPressed(KEYS::kW)) {
      cam->setForward(true);
    }
    else {
      cam->setForward(false);
    }
    
    if (g_baseInput().isKeyPressed(KEYS::kS)) {
      cam->setBackward(true);
    }
    else {
      cam->setBackward(false);
    }

    if (g_baseInput().isKeyPressed(KEYS::kA)) {
      cam->setLeft(true);
    }
    else {
      cam->setLeft(false);
    }

    if (g_baseInput().isKeyPressed(KEYS::kD)) {
      cam->setRight(true);
    }
    else {
      cam->setRight(false);
    }

    if (g_baseInput().isKeyPressed(KEYS::kQ)) {
      cam->setUp(true);
    }
    else {
      cam->setUp(false);
    }

    if (g_baseInput().isKeyPressed(KEYS::kE)) {
      cam->setDown(true);
    }
    else {
      cam->setDown(false);
    }

    cam->update(delta);
  }
}

void
GameApp::onRender() {
}

void
GameApp::onClear() {
}