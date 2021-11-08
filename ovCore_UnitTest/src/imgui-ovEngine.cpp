#include "imgui-ovEngine.h"
#include <imgui.h>
#include <ovBaseInputManager.h>
#include <ovGraphicsAPI.h>
#include <ovMatrix4.h>
#if OV_PLATFORM == OV_PLATFORM_WIN32
#include <Windows.h>
#endif // OV_PLATFORM == OV_PlATFORM_WIN32

using ovEngineSDK::int32;
using ovEngineSDK::uint32;
using ovEngineSDK::uint8;
using ovEngineSDK::SPtr;
using ovEngineSDK::Matrix4;
using ovEngineSDK::Vector;
using ovEngineSDK::static_pointer_cast;

struct ImGui_ImplOV_Data {
  SPtr<ovEngineSDK::Buffer> m_pVB;
  SPtr<ovEngineSDK::Buffer> m_pIB;
  SPtr<ovEngineSDK::ShaderProgram> m_pSP;
  SPtr<ovEngineSDK::InputLayout> m_pIL;
  SPtr<ovEngineSDK::Buffer> m_pVertexCB;
  SPtr<ovEngineSDK::SamplerState> m_pFontSS;
  SPtr<ovEngineSDK::RasterizerState> m_pRS;
  SPtr<ovEngineSDK::DepthStencilState> m_pDS;
  SPtr<ovEngineSDK::BlendState>m_pBS;
  SPtr<ovEngineSDK::Texture>m_pTex;
  int32 m_vertexSize = 5000;
  int32 m_indexSize = 1000;
};

namespace ImGui {
  
  static ImGui_ImplOV_Data* ImGui_ImplOV_GetBackendData() {
    return ImGui::GetCurrentContext() ?
      static_cast<ImGui_ImplOV_Data*>(ImGui::GetIO().BackendRendererUserData)
      : nullptr;
  }

  void
  init(void* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    io.KeyMap[ImGuiKey_Tab] = ovEngineSDK::KEYS::kTAB;
    io.KeyMap[ImGuiKey_LeftArrow] = ovEngineSDK::KEYS::kLEFT;
    io.KeyMap[ImGuiKey_RightArrow] = ovEngineSDK::KEYS::kRIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = ovEngineSDK::KEYS::kUP;
    io.KeyMap[ImGuiKey_DownArrow] = ovEngineSDK::KEYS::kDOWN;
    io.KeyMap[ImGuiKey_PageUp] = ovEngineSDK::KEYS::kPGUP;
    io.KeyMap[ImGuiKey_PageDown] = ovEngineSDK::KEYS::kPGDOWN;
    io.KeyMap[ImGuiKey_Home] = ovEngineSDK::KEYS::kHOME;
    io.KeyMap[ImGuiKey_End] = ovEngineSDK::KEYS::kEND;
    io.KeyMap[ImGuiKey_Insert] = ovEngineSDK::KEYS::kINSERT;
    io.KeyMap[ImGuiKey_Delete] = ovEngineSDK::KEYS::kDELETE;
    io.KeyMap[ImGuiKey_Backspace] = ovEngineSDK::KEYS::kBACK;
    io.KeyMap[ImGuiKey_Space] = ovEngineSDK::KEYS::kSPACE;
    io.KeyMap[ImGuiKey_Enter] = ovEngineSDK::KEYS::kRETURN;
    io.KeyMap[ImGuiKey_Escape] = ovEngineSDK::KEYS::kESCAPE;
    io.KeyMap[ImGuiKey_A] = ovEngineSDK::KEYS::kA;
    io.KeyMap[ImGuiKey_C] = ovEngineSDK::KEYS::kC;
    io.KeyMap[ImGuiKey_V] = ovEngineSDK::KEYS::kV;
    io.KeyMap[ImGuiKey_X] = ovEngineSDK::KEYS::kX;
    io.KeyMap[ImGuiKey_Y] = ovEngineSDK::KEYS::kY;
    io.KeyMap[ImGuiKey_Z] = ovEngineSDK::KEYS::kZ;

    RECT rc;
    GetClientRect(static_cast<HWND>(window), &rc);
    io.DisplaySize = ImVec2(static_cast<float>(rc.right),
                            static_cast<float>(rc.bottom));

    ImGui::StyleColorsDark();

    ImGui_ImplOV_Data* bd = IM_NEW(ImGui_ImplOV_Data);
    io.BackendRendererName = "imgui_impl_ov";
    io.BackendRendererUserData = static_cast<void*>(bd);
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

    auto& graphicApi = ovEngineSDK::g_graphicsAPI();

    //Create and compile VS and PS
    bd->m_pSP = graphicApi.createShaderProgram();
    bd->m_pSP->setVertexShader(graphicApi.createVertexShader(L"resources/shaders/ImguiVS"));
    bd->m_pSP->setPixelShader(graphicApi.createPixelShader(L"resources/shaders/ImguiPS"));
    bd->m_pSP->linkProgram();
    //Fill input layout
    ovEngineSDK::LAYOUT_DESC lDesc;
    lDesc.addToDesc(ovEngineSDK::SEMANTIC::kPOSITION,
                    ovEngineSDK::FORMATS::kRG32_FLOAT,
                    IM_OFFSETOF(ImDrawVert, pos),
                    2);
    lDesc.addToDesc(ovEngineSDK::SEMANTIC::kTEXCOORD,
                    ovEngineSDK::FORMATS::kRG32_FLOAT,
                    IM_OFFSETOF(ImDrawVert, uv),
                    2);
    lDesc.addToDesc(ovEngineSDK::SEMANTIC::kCOLOR,
                    ovEngineSDK::FORMATS::kRGBA8_UNORM,
                    IM_OFFSETOF(ImDrawVert, col),
                    4);
    //Create input layout
    bd->m_pIL = graphicApi.createInputLayout(bd->m_pSP, lDesc);
    //Create vertex shader CS
    bd->m_pVertexCB = graphicApi.createBuffer(nullptr,
                                              sizeof(Matrix4),
                                              ovEngineSDK::BUFFER_TYPE::kCONST_BUFFER);
    //Create blend state
    bd->m_pBS = graphicApi.createBlendState(true,
                                            ovEngineSDK::BLEND_TYPE::SRC_ALPHA,
                                            ovEngineSDK::BLEND_TYPE::INV_SRC_ALPHA,
                                            ovEngineSDK::BLEND_OP::ADD,
                                            ovEngineSDK::BLEND_TYPE::ONE,
                                            ovEngineSDK::BLEND_TYPE::INV_SRC_ALPHA,
                                            ovEngineSDK::BLEND_OP::ADD,
                                            ovEngineSDK::Vector4(0.f, 0.f, 0.f, 0.f));
    //Create rasterizer state
    bd->m_pRS = graphicApi.createRasterizerState(ovEngineSDK::FILL_MODE::kSOLID,
                                                 ovEngineSDK::CULL_MODE::kNONE,
                                                 false, true);
    //Create depth-stencil state
    bd->m_pDS = graphicApi.createDepthStencilState(false,
                                                   false,
                                                   ovEngineSDK::COMPARISON::ALWAYS);
    //Build texture atlas
    uint8* pixels;
    int32 width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    //Upload texture tu graphics system
    bd->m_pTex = graphicApi.createTextureFromMemory(width,
                                          height,
                                          ovEngineSDK::TEXTURE_BINDINGS::E::SHADER_RESOURCE,
                                          ovEngineSDK::FORMATS::kRGBA8_UNORM,
                                          pixels);
    io.Fonts->SetTexID(static_cast<ImTextureID>(&bd->m_pTex));
    //Create sampler
    bd->m_pFontSS = graphicApi.createSamplerState(ovEngineSDK::FILTER_LEVEL::FILTER_LINEAR,
                                                  ovEngineSDK::FILTER_LEVEL::FILTER_LINEAR,
                                                  ovEngineSDK::FILTER_LEVEL::FILTER_LINEAR,
                                                  false,
                                                  0,
                                                  ovEngineSDK::WRAPPING::WRAP,
                                                  ovEngineSDK::COMPARISON::ALWAYS);
  }

  void
  update(void* window, float delta) {
    window;
    delta;
  }

  void
  render(void* window) {
    window;
    ImGui::Render();
    ImDrawData* data = ImGui::GetDrawData();
    if (0 == data->CmdListsCount) {
      return;
    }
    ImGuiIO& io = ImGui::GetIO();
    assert(io.Fonts->TexID != NULL);

    int32 width = static_cast<int32>(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int32 height = static_cast<int32>(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (0 == width || 0 == height) {
      return;
    }
    data->ScaleClipRects(io.DisplayFramebufferScale);
    ImGui_ImplOV_Data* bd = ImGui_ImplOV_GetBackendData();

    auto& graphicAPI = ovEngineSDK::g_graphicsAPI();

    //Create and grow vertex/index buffer if needed
    if (!bd->m_pVB || bd->m_vertexSize < data->TotalVtxCount) {
      if (bd->m_pVB) {
        bd->m_pVB.reset();
      }
      bd->m_vertexSize = data->TotalVtxCount;
      bd->m_pVB = graphicAPI.createBuffer(nullptr,
                                          bd->m_vertexSize * sizeof(ImDrawVert),
                                          ovEngineSDK::BUFFER_TYPE::kVERTEX_BUFFER);
    }
    if (!bd->m_pIB || bd->m_indexSize < data->TotalIdxCount) {
      if (bd->m_pIB) {
        bd->m_pIB.reset();
      }
      bd->m_indexSize = data->TotalIdxCount;
      bd->m_pIB = graphicAPI.createBuffer(nullptr,
                                          bd->m_indexSize * sizeof(ImDrawIdx),
                                          ovEngineSDK::BUFFER_TYPE::kINDEX_BUFFER);
    }
    //Upload vertex/index data
    Vector<ImDrawVert> vertices;
    Vector<ImDrawIdx> indices;
    for (int32 n = 0; n < data->CmdListsCount; ++n) {
      const ImDrawList* cmd_list = data->CmdLists[n];
      for (int32 x = 0; x < cmd_list->VtxBuffer.Size; ++x) {
        vertices.push_back(cmd_list->VtxBuffer[x]);
      }
      for (int32 y = 0; y < cmd_list->IdxBuffer.Size; ++y) {
        indices.push_back(cmd_list->IdxBuffer[y]);
      }
    }
    graphicAPI.updateBuffer(bd->m_pVB, vertices.data());
    graphicAPI.updateBuffer(bd->m_pIB, indices.data());
    //Setup ortographic projection matrix into constant buffer
    /*Matrix4 ortho = graphicAPI.matrix4Policy(graphicAPI.createCompatibleOrtho(
                                     data->DisplayPos.x,
                                     data->DisplayPos.x + data->DisplaySize.x,
                                     data->DisplayPos.y,
                                     data->DisplayPos.y + data->DisplaySize.y,
                                     0.01f,
                                     1.0f));*/
    float L = data->DisplayPos.x;
    float R = data->DisplayPos.x + data->DisplaySize.x;
    float T = data->DisplayPos.y;
    float B = data->DisplayPos.y + data->DisplaySize.y;
    float mvp[4][4] =
    {
        { 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
        { 0.0f,         2.0f / (T - B),     0.0f,       0.0f },
        { 0.0f,         0.0f,           0.5f,       0.0f },
        { (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
    };
    graphicAPI.updateBuffer(bd->m_pVertexCB, &mvp);
    //Backup state that will be modified to restore it later
    struct BACKUP_STATE {
      SPtr<ovEngineSDK::RasterizerState> RS;
      SPtr<ovEngineSDK::BlendState> BS;
      SPtr<ovEngineSDK::DepthStencilState> DS;
      SPtr<ovEngineSDK::SamplerState> PSSampler;
      SPtr<ovEngineSDK::ShaderProgram> Shaders;
      SPtr<ovEngineSDK::Buffer> indexBuffer;
      SPtr<ovEngineSDK::Buffer> vertexBuffer;
      SPtr<ovEngineSDK::Buffer> vConstantBuffer;
      SPtr<ovEngineSDK::InputLayout> iLayout;
      SPtr<ovEngineSDK::Texture> shaderResource;
      uint32 vertexStride = 0;
      uint32 vertexOffset = 0;
    };
    BACKUP_STATE old = {};
    graphicAPI.getRaterizerState(old.RS);
    graphicAPI.getBlendState(old.BS);
    graphicAPI.getDepthStencilState(old.DS);
    graphicAPI.getTextureShaderResource(0,
                                        old.shaderResource,
                                        ovEngineSDK::SHADER_TYPE::PIXEL_SHADER);
    graphicAPI.getSampler(0, old.PSSampler, ovEngineSDK::SHADER_TYPE::PIXEL_SHADER);
    graphicAPI.getShaderProgram(old.Shaders);
    graphicAPI.getConstantBuffer(old.vConstantBuffer,
                                 0,
                                 ovEngineSDK::SHADER_TYPE::VERTEX_SHADER);
    graphicAPI.getIndexBuffer(old.indexBuffer);
    graphicAPI.getVertexBuffer(old.vertexBuffer, old.vertexStride, old.vertexOffset);
    graphicAPI.getInputLayout(old.iLayout);
    //Setup desired state
    {
      //Setup shader and buffers
      uint32 stride = static_cast<uint32>(sizeof(ImDrawVert));
      graphicAPI.setInputLayout(bd->m_pIL);
      graphicAPI.setVertexBuffer(bd->m_pVB, stride, 0);
      graphicAPI.setIndexBuffer(bd->m_pIB, ovEngineSDK::FORMATS::kR16_UINT);
      graphicAPI.setShaders(bd->m_pSP);
      graphicAPI.setConstantBuffer(0,
                                   bd->m_pVertexCB,
                                   ovEngineSDK::SHADER_TYPE::VERTEX_SHADER);
      graphicAPI.setSamplerState(0,
                                 bd->m_pTex,
                                 bd->m_pFontSS,
                                 ovEngineSDK::SHADER_TYPE::PIXEL_SHADER);
      //Setup blend state
      graphicAPI.setBlendState(bd->m_pBS, 0xffffffff);
      graphicAPI.setDepthStencilState(bd->m_pDS);
      graphicAPI.setRasterizerState(bd->m_pRS);
    }
    //Render command lists
    {
      int32 global_idx_offset = 0;
      int32 global_vtx_offset = 0;
      ImVec2 clip_off = data->DisplayPos;
      for (int32 n = 0; n < data->CmdListsCount; ++n) {
        const ImDrawList* cmd_list = data->CmdLists[n];

        for (int32 cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; ++cmd_i) {
          const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
          //Project scissor/clipping rectangles into framebuffer space
          ImVec2 clip_min(pcmd->ClipRect.x - clip_off.x, pcmd->ClipRect.y - clip_off.y);
          ImVec2 clip_max(pcmd->ClipRect.z - clip_off.x, pcmd->ClipRect.w - clip_off.y);
          if (clip_max.x < clip_min.x || clip_max.y < clip_min.y)
            continue;
          //Apply scissor / clipping rectangle
          graphicAPI.setScissorRects(clip_min.x, clip_max.x, clip_min.y, clip_max.y);
          //Bind texture, Draw
          SPtr<ovEngineSDK::Texture>* texture = static_cast<SPtr<ovEngineSDK::Texture>*>(pcmd->GetTexID());
          graphicAPI.setTextureShaderResource(0,
                                              *texture,
                                              ovEngineSDK::SHADER_TYPE::PIXEL_SHADER);
          graphicAPI.drawIndexed(pcmd->ElemCount,
                                 pcmd->IdxOffset + global_idx_offset,
                                 pcmd->VtxOffset + global_vtx_offset);
        }
        global_idx_offset += cmd_list->IdxBuffer.Size;
        global_vtx_offset += cmd_list->VtxBuffer.Size;
      }
      
    }
    //Restore modified state
    {
      graphicAPI.setRasterizerState(old.RS);
      graphicAPI.setBlendState(old.BS, 0xffffffff);
      graphicAPI.setDepthStencilState(old.DS);
      graphicAPI.setShaders(old.Shaders);
      graphicAPI.setTextureShaderResource(0,
                                          old.shaderResource,
                                          ovEngineSDK::SHADER_TYPE::PIXEL_SHADER);
      graphicAPI.setSamplerState(0,
                                 old.shaderResource,
                                 old.PSSampler,
                                 ovEngineSDK::SHADER_TYPE::PIXEL_SHADER);
      graphicAPI.setConstantBuffer(0,
                                   old.vConstantBuffer,
                                   ovEngineSDK::SHADER_TYPE::VERTEX_SHADER);
      graphicAPI.setVertexBuffer(old.vertexBuffer, old.vertexStride, old.vertexOffset);
      graphicAPI.setIndexBuffer(old.indexBuffer, ovEngineSDK::FORMATS::kR32_UINT);
      graphicAPI.setInputLayout(old.iLayout);
    }
  }

  void
  shutDown() {
    ImGui_ImplOV_Data* bd = ImGui_ImplOV_GetBackendData();
    IM_ASSERT(bd != nullptr && "No renderer backend to shitdon, or already shutdown?");
    ImGuiIO& io = ImGui::GetIO();

    ImGui::DestroyPlatformWindows();
    io.BackendRendererName = nullptr;
    io.BackendRendererUserData = nullptr;
    IM_DELETE(bd);
    ImGui::DestroyContext();
  }
}
