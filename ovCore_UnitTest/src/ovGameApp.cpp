#include "ovGameApp.h"

void GameApp::onCreate() {
  m_graphicsAPI->setTopology(TOPOLOGY::E::TRIANGLES);
  RECT rc;
  GetClientRect(m_windowHandle, &rc);
  m_graphicsAPI->setViewport(0, 0, rc.right, rc.bottom);
  //Compile and create vertex / pixel shader
  //m_vs = m_graphicsAPI->createVertexShader(L"VS");
  //m_ps = m_graphicsAPI->createPixelShader(L"PS");
  ////Create shader program
  //m_shaderProgram = m_graphicsAPI->createShaderProgram();
  ////Attach shaders to program
  //m_shaderProgram->setVertexShader(m_vs);
  //m_shaderProgram->setPixelShader(m_ps);
  ////Link shader program
  //m_shaderProgram->linkProgram();
  ////Define input layout
  //LAYOUT_DESC lDesc;
  //lDesc.addToDesc(SEMANTIC::POSITION, FORMATS::RGB32_FLOAT, 0, 3);
  //lDesc.addToDesc(SEMANTIC::TEXCOORD, FORMATS::RG32_FLOAT, 12, 2);
  ////Create input layout
  //m_layout = m_graphicsAPI->createInputLayout(m_shaderProgram, lDesc);
  ////Create constant buffer
  //m_cBuffer = m_graphicsAPI->createBuffer(nullptr,
  //                                        sizeof(Matrices),
  //                                        BUFFER_TYPE::CONST_BUFFER);
  ////Create structure to update constant buffer
  //Matrices mat{};
  //mat.World = Matrix4::IDENTITY;
  //mat.Color = Vector4(1.f, 0.f, 0.f, 1.f);
  ////Define vertex buffer
  //Vector<Vertex>vertices;
  //vertices.push_back({ Vector3(-1.0f, 1.0f, -1.0f),	Vector2(0.0f, 0.0f) });
  //vertices.push_back({ Vector3(1.0f, 1.0f, -1.0f),  Vector2(1.0f, 0.0f) });
  //vertices.push_back({ Vector3(1.0f, 1.0f, 1.0f),	  Vector2(1.0f, 1.0f) });
  //vertices.push_back({ Vector3(-1.0f, 1.0f, 1.0f),	Vector2(0.0f, 1.0f) });

  //vertices.push_back({ Vector3(-1.0f, -1.0f, -1.0f),Vector2(0.0f, 0.0f) });
  //vertices.push_back({ Vector3(1.0f, -1.0f, -1.0f),	Vector2(1.0f, 0.0f) });
  //vertices.push_back({ Vector3(1.0f, -1.0f, 1.0f),	Vector2(1.0f, 1.0f) });
  //vertices.push_back({ Vector3(-1.0f, -1.0f, 1.0f), Vector2(0.0f, 1.0f) });

  //vertices.push_back({ Vector3(-1.0f, -1.0f, 1.0f), Vector2(0.0f, 0.0f) });
  //vertices.push_back({ Vector3(-1.0f, -1.0f, -1.0f),Vector2(1.0f, 0.0f) });
  //vertices.push_back({ Vector3(-1.0f, 1.0f, -1.0f), Vector2(1.0f, 1.0f) });
  //vertices.push_back({ Vector3(-1.0f, 1.0f, 1.0f),	Vector2(0.0f, 1.0f) });

  //vertices.push_back({ Vector3(1.0f, -1.0f, 1.0f),	Vector2(0.0f, 0.0f) });
  //vertices.push_back({ Vector3(1.0f, -1.0f, -1.0f), Vector2(1.0f, 0.0f) });
  //vertices.push_back({ Vector3(1.0f, 1.0f, -1.0f),	Vector2(1.0f, 1.0f) });
  //vertices.push_back({ Vector3(1.0f, 1.0f, 1.0f),	  Vector2(0.0f, 1.0f) });

  //vertices.push_back({ Vector3(-1.0f, -1.0f, -1.0f),Vector2(0.0f, 0.0f) });
  //vertices.push_back({ Vector3(1.0f, -1.0f, -1.0f), Vector2(1.0f, 0.0f) });
  //vertices.push_back({ Vector3(1.0f, 1.0f, -1.0f),	Vector2(1.0f, 1.0f) });
  //vertices.push_back({ Vector3(-1.0f, 1.0f, -1.0f), Vector2(0.0f, 1.0f) });

  //vertices.push_back({ Vector3(-1.0f, -1.0f, 1.0f), Vector2(0.0f, 0.0f) });
  //vertices.push_back({ Vector3(1.0f, -1.0f, 1.0f),	Vector2(1.0f, 0.0f) });
  //vertices.push_back({ Vector3(1.0f, 1.0f, 1.0f),	  Vector2(1.0f, 1.0f) });
  //vertices.push_back({ Vector3(-1.0f, 1.0f, 1.0f),	Vector2(0.0f, 1.0f) });
  ////Define index buffer
  //Vector<uint32>indices;
  //indices.push_back(3); indices.push_back(1); indices.push_back(0);
  //indices.push_back(2); indices.push_back(1); indices.push_back(3);

  //indices.push_back(6); indices.push_back(4); indices.push_back(5);
  //indices.push_back(7); indices.push_back(4); indices.push_back(6);

  //indices.push_back(11); indices.push_back(9); indices.push_back(8);
  //indices.push_back(10); indices.push_back(9); indices.push_back(11);

  //indices.push_back(14); indices.push_back(12); indices.push_back(13);
  //indices.push_back(15); indices.push_back(12); indices.push_back(14);

  //indices.push_back(19); indices.push_back(17); indices.push_back(16);
  //indices.push_back(18); indices.push_back(17); indices.push_back(19);

  //indices.push_back(22); indices.push_back(20); indices.push_back(21);
  //indices.push_back(23); indices.push_back(20); indices.push_back(22);
  ////Create vertex buffer
  //m_vertexBuffer = m_graphicsAPI->createBuffer(vertices.data(),
  //                                             sizeof(Vertex) * vertices.size(),
  //                                             BUFFER_TYPE::VERTEX_BUFFER);
  ////Create index buffer
  //m_indexBuffer = m_graphicsAPI->createBuffer(indices.data(),
  //                                            sizeof(uint32) * indices.size(),
  //                                            BUFFER_TYPE::INDEX_BUFFER);
  m_color.red = 0.0f;
  m_color.green = 0.125f;
  m_color.blue = 0.3f;
  m_color.alpha = 1.f;
}

void GameApp::onUpdate() {
}

void GameApp::onRender() {
 m_graphicsAPI->clearBackBuffer(m_color);
 m_graphicsAPI->swapBuffer();
}

void GameApp::onClear() {
}