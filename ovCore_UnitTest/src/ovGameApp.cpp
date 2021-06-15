#include "ovGameApp.h"

void GameApp::onCreate() {
  m_graphicsAPI->setTopology(TOPOLOGY::E::TRIANGLES);
  RECT rc;
  GetClientRect(m_windowHandle, &rc);
  m_graphicsAPI->setViewport(0, 0, rc.right, rc.bottom);
  //Compile and create vertex / pixel shader
  m_vs = m_graphicsAPI->createVertexShader(L"D:/UAD/ovEngine/bin/resources/shaders/VS");
  m_ps = m_graphicsAPI->createPixelShader(L"D:/UAD/ovEngine/bin/resources/shaders/PS");
  //Create shader program
  m_shaderProgram = m_graphicsAPI->createShaderProgram();
  //Attach shaders to program
  m_shaderProgram->setVertexShader(m_vs);
  m_shaderProgram->setPixelShader(m_ps);
  //Link shader program
  m_shaderProgram->linkProgram();
  //Define input layout
  LAYOUT_DESC lDesc;
  lDesc.addToDesc(SEMANTIC::POSITION, FORMATS::RGB32_FLOAT, 0, 3);
  lDesc.addToDesc(SEMANTIC::TEXCOORD, FORMATS::RG32_FLOAT, 12, 2);
  lDesc.addToDesc(SEMANTIC::NORMAL,   FORMATS::RGB32_FLOAT, 20, 3);
  lDesc.addToDesc(SEMANTIC::TANGENT,  FORMATS::RGB32_FLOAT, 36, 3);
  lDesc.addToDesc(SEMANTIC::BINORMAL, FORMATS::RGB32_FLOAT, 48, 3);
  lDesc.addToDesc(SEMANTIC::BLENDINDICES, FORMATS::RGBA32_INT, 60, 4);
  lDesc.addToDesc(SEMANTIC::BLENDWEIGHT, FORMATS::RGBA32_FLOAT, 72, 4);
  //Create input layout
  m_layout = m_graphicsAPI->createInputLayout(m_shaderProgram, lDesc);

  //Create constant buffer
  m_cBuffer = m_graphicsAPI->createBuffer(nullptr,
                                          static_cast<int32>(sizeof(Matrices)),
                                          BUFFER_TYPE::CONST_BUFFER);
  m_bBuffer = m_graphicsAPI->createBuffer(nullptr,
                                          static_cast<int32>(sizeof(Bones)),
                                          BUFFER_TYPE::CONST_BUFFER);
  //Create structure to update constant buffer
  Matrices mat{};
  mat.World = Matrix4::IDENTITY;
  mat.Color = Vector4(1.f, 0.f, 0.f, 1.f);
  mat.View = m_graphicsAPI->matrix4Policy(LookAtMatrix(Vector3(0.f, 0.f, -5.f),
                                                       Vector3(0.f, 1.f, 0.f),
                                                       Vector3(0.f, 1.f, 0.f)));
  mat.Projection = m_graphicsAPI->matrix4Policy(PerspectiveMatrix(70.f,
                                                                  800.f,
                                                                  600.f,
                                                                  0.01f,
                                                                  100.f));
  m_graphicsAPI->updateBuffer(m_cBuffer, &mat);

  m_graphicsAPI->setInputLayout(m_layout);
  m_graphicsAPI->setConstantBuffer(0, m_cBuffer, SHADER_TYPE::VERTEX_SHADER);
  m_graphicsAPI->setConstantBuffer(0, m_cBuffer, SHADER_TYPE::PIXEL_SHADER);
  m_graphicsAPI->setConstantBuffer(1, m_bBuffer, SHADER_TYPE::VERTEX_SHADER);
  m_graphicsAPI->setConstantBuffer(1, m_bBuffer, SHADER_TYPE::PIXEL_SHADER);
  m_graphicsAPI->setShaders(m_shaderProgram);

  m_color.red = 0.0f;
  m_color.green = 0.125f;
  m_color.blue = 0.3f;
  m_color.alpha = 1.f;

  m_graphicsAPI->setBackBuffer();

  //initialize model and load it
  myModel = make_shared<Model>();
  myModel->load("resources/models/silly_dancing.fbx");
}

void GameApp::onUpdate(float delta) {
  Vector<Matrix4> transforms;
  myModel->transformBones(delta, transforms);
  Bones cbBone;
  for (uint32 i = 0; i < transforms.size(); i++) {
    if (i < MAXBONES) {
      cbBone.gBones[i] = transforms[i];
    }
  }
  m_graphicsAPI->updateBuffer(m_bBuffer, &cbBone);
}

void GameApp::onRender() {
  m_graphicsAPI->clearBackBuffer(m_color);
  myModel->draw();
}

void GameApp::onClear() {
}