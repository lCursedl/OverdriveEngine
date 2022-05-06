#include "ovGameApp.h"
#include "ovCamera.h"
#include "ovBaseInputManager.h"
#include "ovBaseRenderer.h"
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

  SPtr<Model>model = Model::loadOVFile("resources/models/stormtrooper.ovFile");

  SPtr<Actor>myActor = make_shared<Actor>();
  myActor->addComponent(model);

  SPtr<SceneNode>myNode = make_shared<SceneNode>();
  myNode->setActor(myActor);

  SPtr<Model>box = Model::createCylinder(25.f, 25.f, 25.f);
  SPtr<Actor>boxActor = make_shared<Actor>();
  boxActor->addComponent(box);
  SPtr<SceneNode>boxNode = make_shared<SceneNode>();
  boxNode->setActor(boxActor);

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

  SPtr<Model>plane = Model::load("resources/models/plano.fbx");

  SPtr<Actor>planeActor = make_shared<Actor>();
  planeActor->addComponent(plane);

  SPtr<SceneNode>planeNode = make_shared<SceneNode>();
  planeNode->setActor(planeActor);

  auto& scene = SceneGraph::instance();

  scene.addNode(myNode);
  scene.addNode(boxNode);
  scene.addNode(camNode);
  scene.addNode(planeNode);

  //m_vTextures = graphicAPI.createCompressedTexture("resources/textures/Compress.png");
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
  m_finalTexture = g_baseRenderer().getOutputImage();
  
  SPtr<Camera> cam = SceneGraph::instance().getActiveCamera();
  if (cam) {
    if (g_baseInput().isMouseKeyPressed(KEYSM::kRIGHT)) {
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
  ImGui::NewFrame();
  ImGui::update(m_windowHandle, delta);
  //ImGui window bar
  ImGui::BeginMainMenuBar(); {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New")) {}
      if (ImGui::MenuItem("Open", "Ctrl+O")) {}
      if (ImGui::MenuItem("Save", "Ctrl+S")) {}
      if (ImGui::MenuItem("Save As..")) {}
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) {
      if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
      if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}
      ImGui::Separator();
      if (ImGui::MenuItem("Cut", "CTRL+X")) {}
      if (ImGui::MenuItem("Copy", "CTRL+C")) {}
      if (ImGui::MenuItem("Paste", "CTRL+V")) {}
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Window")) {
      if (ImGui::MenuItem("Browser")){}
      if (ImGui::MenuItem("Details")){}
      if (ImGui::MenuItem("Viewport")){}
      if (ImGui::MenuItem("Scene")) {}
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help")) {
      if (ImGui::MenuItem("About")) {}
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
  //Imgui docking space for windows
  ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
  //Windows
  ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoCollapse); {
    ImGui::Text("Placeholder for scenegraph content.");
    ImGui::End();
  }
  ImGui::Begin("Browser", nullptr, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoCollapse); {
    ImGui::Text("Placeholder for content browser.");
    ImGui::End();
  }
  ImGui::Begin("Details", nullptr, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoCollapse); {
    ImGui::Text("Placeholder for object details (Transform, Components, etc)");
    ImGui::End();
  }
  ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoCollapse); {
    ImGui::Image(&m_finalTexture, ImGui::GetWindowSize());
    ImGui::End();
  }
}

void
GameApp::onRender() {
  ImGui::render(m_windowHandle);
}

void
GameApp::onClear() {
  ImGui::shutDown();
}