#include "ovGameApp.h"

void GameApp::onCreate() {
  auto graphicAPI = &g_graphicsAPI();
  graphicAPI->setTopology(TOPOLOGY::kTRIANGLES);
  RECT rc;
  GetClientRect(m_windowHandle, &rc);
  graphicAPI->setViewport(0, 0, rc.right, rc.bottom, 0.f, 1.f);
  //Compile and create vertex / pixel shader
  m_vs = graphicAPI->createVertexShader(L"resources/shaders/VS");
  m_ps = graphicAPI->createPixelShader(L"resources/shaders/PS");
  //Create shader program
  m_shaderProgram = graphicAPI->createShaderProgram();
  //Attach shaders to program
  m_shaderProgram->setVertexShader(m_vs);
  m_shaderProgram->setPixelShader(m_ps);
  //Link shader program
  m_shaderProgram->linkProgram();
  //Define input layout
  LAYOUT_DESC lDesc;
  lDesc.addToDesc(SEMANTIC::kPOSITION,     FORMATS::kRGB32_FLOAT,   0, 3);
  lDesc.addToDesc(SEMANTIC::kTEXCOORD,     FORMATS::kRG32_FLOAT,   12, 2);
  lDesc.addToDesc(SEMANTIC::kNORMAL,       FORMATS::kRGB32_FLOAT,  20, 3);
  lDesc.addToDesc(SEMANTIC::kTANGENT,      FORMATS::kRGB32_FLOAT,  32, 3);
  lDesc.addToDesc(SEMANTIC::kBINORMAL,     FORMATS::kRGB32_FLOAT,  44, 3);
  lDesc.addToDesc(SEMANTIC::kBLENDINDICES, FORMATS::kRGBA32_INT,   56, 4);
  lDesc.addToDesc(SEMANTIC::kBLENDWEIGHT,  FORMATS::kRGBA32_FLOAT, 72, 4);
  //Create input layout
  m_layout = graphicAPI->createInputLayout(m_shaderProgram, lDesc);

  //Create constant buffer
  m_cBuffer = graphicAPI->createBuffer(nullptr,
                                       sizeof(Matrices),
                                       BUFFER_TYPE::kCONST_BUFFER);
  m_bBuffer = graphicAPI->createBuffer(nullptr,
                                       sizeof(Bones),
                                       BUFFER_TYPE::kCONST_BUFFER);
  //Create structure to update constant buffer
  Matrices mat;
  mat.world = Matrix4::IDENTITY;
  mat.color = Vector4(1.f, 0.f, 0.f, 1.f);
  mat.view = graphicAPI->matrix4Policy(LookAtMatrix(Vector3(0.f, 0.f, -5.f),
                                                    Vector3(0.f, 1.f, 0.f),
                                                    Vector3(0.f, 1.f, 0.f)));
  mat.projection = graphicAPI->matrix4Policy(PerspectiveMatrix(70.f,
                                                               800.f,
                                                               600.f,
                                                               0.01f,
                                                               100.f));
  graphicAPI->updateBuffer(m_cBuffer, &mat);

  graphicAPI->setInputLayout(m_layout);
  graphicAPI->setConstantBuffer(0, m_cBuffer, SHADER_TYPE::VERTEX_SHADER);
  graphicAPI->setConstantBuffer(0, m_cBuffer, SHADER_TYPE::PIXEL_SHADER);
  graphicAPI->setConstantBuffer(1, m_bBuffer, SHADER_TYPE::VERTEX_SHADER);
  graphicAPI->setConstantBuffer(1, m_bBuffer, SHADER_TYPE::PIXEL_SHADER);
  graphicAPI->setShaders(m_shaderProgram);

  m_color.red = 0.0f;
  m_color.green = 0.125f;
  m_color.blue = 0.3f;
  m_color.alpha = 1.f;

  graphicAPI->setBackBuffer();

  //initialize model and load it
  myModel = make_shared<Model>();
  myModel->load("resources/models/silly_dancing.fbx");
}

void GameApp::onUpdate(float delta) {
  auto graphicAPI = &g_graphicsAPI();
  Vector<Matrix4> transforms;
  myModel->transformBones(delta, transforms);
  Bones cbBone;
  SIZE_T numObjects = transforms.size();
  for (uint32 i = 0; i < numObjects; ++i) {
    if (i < MAXBONES) {
      cbBone.bones[i] = transforms[i];
    }
  }
  graphicAPI->updateBuffer(m_bBuffer, &cbBone);
}

void GameApp::onRender() {
  auto graphicAPI = &g_graphicsAPI();
  graphicAPI->clearBackBuffer(m_color);
  myModel->draw();
}

void GameApp::onClear() {
}