#include "ovGameApp.h"
#include "ovCamera.h"
#include "ovBaseInputManager.h"
#include "ovBaseRenderer.h"
#include "ovBaseOmniverse.h"
#include <ovModel.h>
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

  /*SPtr<Model>model = Model::load("resources/models/Vela/Vela2.fbx");

  SPtr<Actor>myActor = make_shared<Actor>("Vela");
  myActor->addComponent(model);
  myActor->m_localRotation = Vector3(0.f, 1.5f, 0.f);
  myActor->m_localScale = Vector3(0.5f, 0.5f, 0.5f);
  myActor->m_localPosition = {100.f, 100.f, 100.f};
  SPtr<SceneNode>myNode = make_shared<SceneNode>();
  myNode->setActor(myActor);*/

  SPtr<Camera>cam = make_shared<Camera>();
  cam->init(Vector3(0.f, 0.f, -250.f),
            Vector3(0.f, 0.f, -1.f),
            Vector3(0.f, 1.f, 0.f),
            .5f,
            PerspectiveMatrix(70.f,
                              800.f,
                              600.f,
                              0.01f,
                              3000.f));
  SPtr<Actor>camActor = make_shared<Actor>("Editor Camera");
  camActor->addComponent(cam);

  SPtr<SceneNode>camNode = make_shared<SceneNode>();
  camNode->setActor(camActor);

  /*SPtr<Model>plane = Model::load("resources/models/plano.fbx");

  SPtr<Actor>planeActor = make_shared<Actor>("Plane");
  planeActor->addComponent(plane);

  SPtr<SceneNode>planeNode = make_shared<SceneNode>();
  planeNode->setActor(planeActor);*/

  auto& scene = SceneGraph::instance();

  //scene.addNode(myNode);
  scene.addNode(camNode);
  //scene.addNode(planeNode);

  g_baseOmniverse().connectFromOmni(
    "omniverse://localhost/Users/Overdrive/OverdriveLiveShare.usd");
  //g_baseOmniverse().loadUSD("omniverse://localhost/Users/Overdrive/OverdriveLiveShare.usd");
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
  /*myActor->m_localRotation = Vector3(myActor->m_localRotation.x ,
                                     myActor->m_localRotation.y ,
                                     myActor->m_localRotation.z + .0005f);*/
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
    if (ImGui::BeginMenu("Omniverse")) {
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
  ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoNav |
                                 ImGuiWindowFlags_NoCollapse); {
    showTreeNodes(SceneGraph::instance().getRoot());
    ImGui::End();
  }
  ImGui::Begin("Browser", nullptr, ImGuiWindowFlags_NoNav |
                                   ImGuiWindowFlags_NoCollapse); {
    ImGui::Text("Placeholder for content browser.");
    ImGui::End();
  }
  ImGui::Begin("Details", nullptr, ImGuiWindowFlags_NoNav |
                                   ImGuiWindowFlags_NoCollapse); {
    showActorInfo();
    ImGui::End();
  }
  ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoNav |
                                    ImGuiWindowFlags_NoCollapse); {
    ImGui::Image(&m_finalTexture, ImGui::GetContentRegionAvail());
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

void
GameApp::showTreeNodes(SPtr<SceneNode> node) {
  ImGuiTreeNodeFlags treeflags = ImGuiTreeNodeFlags_SpanFullWidth |
                                 ImGuiTreeNodeFlags_OpenOnArrow |
                                 ImGuiTreeNodeFlags_OpenOnDoubleClick;

  bool hasChildren = 0 < node->m_pChilds.size() ? true : false;
  if (node->m_pParent.expired()) {
    treeflags |= ImGuiTreeNodeFlags_DefaultOpen;
    ImGui::SetNextItemOpen(true);
  }

  if (!hasChildren) {
    treeflags |= ImGuiTreeNodeFlags_Leaf;
  }

  String actorName = "";

  if (nullptr != node->m_pActor) {
    actorName = node->m_pActor->getActorName();
  }
  else {
    actorName = node->m_name;
  }
  if (ImGui::TreeNodeEx(actorName.c_str(), treeflags)) {
    if (ImGui::IsItemClicked()) {
      node->m_selected = true;
      auto& sgraph = SceneGraph::instance();
      if (nullptr != sgraph.m_selectedNode) {
        if (node != sgraph.m_selectedNode) {
          sgraph.m_selectedNode->m_selected = false;
        }

      }
      sgraph.m_selectedNode = node;
    }
    ImGui::AlignTextToFramePadding();
    if (hasChildren) {
      for (auto& childNode : node->m_pChilds) {
        showTreeNodes(childNode);
      }
    }
    ImGui::TreePop();
  }
  /*bool isOpen = ImGui::TreeNodeEx(actorName.c_str(), treeflags);

  if (ImGui::IsItemClicked()) {
    node->m_selected = true;
    auto& sgraph = SceneGraph::instance();
    if (nullptr != sgraph.m_selectedNode) {
      if (node != sgraph.m_selectedNode) {
        sgraph.m_selectedNode->m_selected = false;
      }
      
    }
    sgraph.m_selectedNode = node;
  }

  if (isOpen) {
    ImGui::AlignTextToFramePadding();
    if (hasChildren) {
      for (auto& childNode : node->m_pChilds) {
        showTreeNodes(childNode);
      }
    }
  }*/

  
}

void
GameApp::showActorInfo() {
  auto& scgraph = SceneGraph::instance();
  if (nullptr != scgraph.m_selectedNode) {
    auto& tmpActor = scgraph.m_selectedNode->m_pActor;
    ImGuiTreeNodeFlags myFlags = ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_AlwaysVerticalScrollbar |
                                 ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoResize;
    //Names
    String nameLabel = tmpActor->getActorName();
    float windowWidth = ImGui::GetWindowSize().x;
    float textWidth = ImGui::CalcTextSize(nameLabel.c_str()).x;

    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::Text(nameLabel.c_str());
    ImGui::Separator();
    //Transform
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
      ImGui::DragFloat3("Position", &tmpActor->m_localPosition.x);
      if (ImGui::IsItemEdited()) {
        if (g_baseOmniverse().getLiveEdit()) {
          g_baseOmniverse().setTransformOp(tmpActor->m_localPosition,
                                           OMNI_OP::kTRANSLATE,
                                           OMNI_PRECISION::kDOUBLE,
                                           tmpActor->m_omniPath);
        }
        
      }
      ImGui::DragFloat3("Rotation", &tmpActor->m_localRotation.x);
      /*if (ImGui::IsItemEdited()) {
        if (g_baseOmniverse().getLiveEdit()) {
          g_baseOmniverse().setTransformOp(tmpActor->m_localRotation,
                                           OMNI_OP::kROTATE,
                                           OMNI_PRECISION::kFLOAT,
                                           tmpActor->m_omniPath);
        }        
      }*/
      ImGui::DragFloat3("Scale", &tmpActor->m_localScale.x);
      if (ImGui::IsItemEdited()) {
        if (g_baseOmniverse().getLiveEdit()) {
          g_baseOmniverse().setTransformOp(tmpActor->m_localScale,
                                           OMNI_OP::kSCALE,
                                           OMNI_PRECISION::kFLOAT,
                                           tmpActor->m_omniPath);
        }        
      }      
    }
    ImGui::Separator();
  }
}
