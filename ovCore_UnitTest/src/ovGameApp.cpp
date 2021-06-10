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

  //Define vertex buffer

  /*Vector<Vertex>vertices;
  vertices.push_back({ Vector3(-1.0f, 1.0f, -1.0f),	Vector2(0.0f, 0.0f) });
  vertices.push_back({ Vector3(1.0f, 1.0f, -1.0f),  Vector2(1.0f, 0.0f) });
  vertices.push_back({ Vector3(1.0f, 1.0f, 1.0f),	  Vector2(1.0f, 1.0f) });
  vertices.push_back({ Vector3(-1.0f, 1.0f, 1.0f),	Vector2(0.0f, 1.0f) });

  vertices.push_back({ Vector3(-1.0f, -1.0f, -1.0f),Vector2(0.0f, 0.0f) });
  vertices.push_back({ Vector3(1.0f, -1.0f, -1.0f),	Vector2(1.0f, 0.0f) });
  vertices.push_back({ Vector3(1.0f, -1.0f, 1.0f),	Vector2(1.0f, 1.0f) });
  vertices.push_back({ Vector3(-1.0f, -1.0f, 1.0f), Vector2(0.0f, 1.0f) });

  vertices.push_back({ Vector3(-1.0f, -1.0f, 1.0f), Vector2(0.0f, 0.0f) });
  vertices.push_back({ Vector3(-1.0f, -1.0f, -1.0f),Vector2(1.0f, 0.0f) });
  vertices.push_back({ Vector3(-1.0f, 1.0f, -1.0f), Vector2(1.0f, 1.0f) });
  vertices.push_back({ Vector3(-1.0f, 1.0f, 1.0f),	Vector2(0.0f, 1.0f) });

  vertices.push_back({ Vector3(1.0f, -1.0f, 1.0f),	Vector2(0.0f, 0.0f) });
  vertices.push_back({ Vector3(1.0f, -1.0f, -1.0f), Vector2(1.0f, 0.0f) });
  vertices.push_back({ Vector3(1.0f, 1.0f, -1.0f),	Vector2(1.0f, 1.0f) });
  vertices.push_back({ Vector3(1.0f, 1.0f, 1.0f),	  Vector2(0.0f, 1.0f) });

  vertices.push_back({ Vector3(-1.0f, -1.0f, -1.0f),Vector2(0.0f, 0.0f) });
  vertices.push_back({ Vector3(1.0f, -1.0f, -1.0f), Vector2(1.0f, 0.0f) });
  vertices.push_back({ Vector3(1.0f, 1.0f, -1.0f),	Vector2(1.0f, 1.0f) });
  vertices.push_back({ Vector3(-1.0f, 1.0f, -1.0f), Vector2(0.0f, 1.0f) });

  vertices.push_back({ Vector3(-1.0f, -1.0f, 1.0f), Vector2(0.0f, 0.0f) });
  vertices.push_back({ Vector3(1.0f, -1.0f, 1.0f),	Vector2(1.0f, 0.0f) });
  vertices.push_back({ Vector3(1.0f, 1.0f, 1.0f),	  Vector2(1.0f, 1.0f) });
  vertices.push_back({ Vector3(-1.0f, 1.0f, 1.0f),	Vector2(0.0f, 1.0f) });*/

  //Define index buffer

  /*Vector<uint32>indices;
  indices.push_back(3); indices.push_back(1); indices.push_back(0);
  indices.push_back(2); indices.push_back(1); indices.push_back(3);

  indices.push_back(6); indices.push_back(4); indices.push_back(5);
  indices.push_back(7); indices.push_back(4); indices.push_back(6);

  indices.push_back(11); indices.push_back(9); indices.push_back(8);
  indices.push_back(10); indices.push_back(9); indices.push_back(11);

  indices.push_back(14); indices.push_back(12); indices.push_back(13);
  indices.push_back(15); indices.push_back(12); indices.push_back(14);

  indices.push_back(19); indices.push_back(17); indices.push_back(16);
  indices.push_back(18); indices.push_back(17); indices.push_back(19);

  indices.push_back(22); indices.push_back(20); indices.push_back(21);
  indices.push_back(23); indices.push_back(20); indices.push_back(22);*/

  //Create vertex buffer

  /*m_vertexBuffer = m_graphicsAPI->createBuffer(vertices.data(),
    static_cast<int32>(sizeof(Vertex) * vertices.size()),
    BUFFER_TYPE::VERTEX_BUFFER);*/

  //Create index buffer

  /*m_indexBuffer = m_graphicsAPI->createBuffer(indices.data(),
    static_cast<int32>(sizeof(uint32) * indices.size()),
    BUFFER_TYPE::INDEX_BUFFER);*/

  //Create constant buffer
  m_cBuffer = m_graphicsAPI->createBuffer(nullptr,
                                          static_cast<int32>(sizeof(Matrices)),
                                          BUFFER_TYPE::CONST_BUFFER);
  //Create structure to update constant buffer
  Matrices mat{};
  mat.World = Matrix4::IDENTITY;
  mat.Color = Vector4(1.f, 0.f, 0.f, 1.f);
  mat.View = m_graphicsAPI->matrix4Policy(LookAtMatrix(Vector3(0.f, 3.f, -6.f),
                                                       Vector3(0.f, 1.f, 0.f),
                                                       Vector3(0.f, 1.f, 0.f)));
  mat.Projection = m_graphicsAPI->matrix4Policy(PerspectiveMatrix(70.f,
                                                                  800.f,
                                                                  600.f,
                                                                  0.01f,
                                                                  100.f));  
  m_graphicsAPI->updateBuffer(m_cBuffer, &mat);

  m_graphicsAPI->setInputLayout(m_layout);
  //m_graphicsAPI->setVertexBuffer(m_vertexBuffer, static_cast<int32>(sizeof(Vertex)), 0);
  //m_graphicsAPI->setIndexBuffer(m_indexBuffer);
  m_graphicsAPI->setConstantBuffer(0, m_cBuffer, SHADER_TYPE::VERTEX_SHADER);
  m_graphicsAPI->setConstantBuffer(0, m_cBuffer, SHADER_TYPE::PIXEL_SHADER);
  m_graphicsAPI->setShaders(m_shaderProgram);

  m_color.red = 0.0f;
  m_color.green = 0.125f;
  m_color.blue = 0.3f;
  m_color.alpha = 1.f;

  m_graphicsAPI->setBackBuffer();

  //initialize model and load it
  myModel = new Model();
  myModel->load("D:/UAD/ovEngine/bin/resources/models/silly_dancing.fbx");
}

void GameApp::onUpdate() {
}

void GameApp::onRender() {
  m_graphicsAPI->clearBackBuffer(m_color);
  myModel->draw();
}

void GameApp::onClear() {
  delete m_vertexBuffer;
  delete m_indexBuffer;
  delete m_cBuffer;
  delete m_layout;
  delete m_shaderProgram;
  delete m_vs;
  delete m_ps;
  delete myModel;
}