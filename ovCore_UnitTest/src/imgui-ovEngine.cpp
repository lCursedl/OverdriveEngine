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

struct ImGui_ImplWin32_Data {
  bool MouseTracked = false;
  ImGuiMouseCursor LastMouseCursor = ImGuiMouseCursor_COUNT;
};

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
  
  static ImGui_ImplWin32_Data* ImGui_ImplWin32_GetBackendData() {
    return ImGui::GetCurrentContext() ? 
      static_cast<ImGui_ImplWin32_Data*>(ImGui::GetIO().BackendPlatformUserData) : 
      nullptr;
  }

  static ImGui_ImplOV_Data* ImGui_ImplOV_GetBackendData() {
    return ImGui::GetCurrentContext() ?
      static_cast<ImGui_ImplOV_Data*>(ImGui::GetIO().BackendRendererUserData) :
      nullptr;
  }

  void
  init(void* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplWin32_Data* be = IM_NEW(ImGui_ImplWin32_Data);
    io.BackendPlatformUserData = (void*)be;
    io.BackendPlatformName = "imgui_impl_win32";
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    main_viewport->PlatformHandle = main_viewport->PlatformHandleRaw = window;

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
    io.KeyMap[ImGuiKey_LeftCtrl] = ovEngineSDK::KEYS::kLCONTROL;

    RECT rc;
    GetClientRect(static_cast<HWND>(window), &rc);
    io.DisplaySize = ImVec2(static_cast<float>(rc.right),
                            static_cast<float>(rc.bottom));

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
    //Set style
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    /// 0 = FLAT APPEARENCE
    /// 1 = MORE "3D" LOOK
    int is3D = 1;

    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.12f, 0.71f);
    colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.42f, 0.42f, 0.42f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.42f, 0.42f, 0.42f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.17f, 0.17f, 0.17f, 0.90f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.335f, 0.335f, 0.335f, 1.000f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.54f, 0.54f, 0.54f, 0.35f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.52f, 0.52f, 0.52f, 0.59f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.76f, 0.76f, 0.76f, 0.77f);
    colors[ImGuiCol_Separator] = ImVec4(0.000f, 0.000f, 0.000f, 0.137f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.700f, 0.671f, 0.600f, 0.290f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.702f, 0.671f, 0.600f, 0.674f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.73f, 0.73f, 0.73f, 0.35f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

    style.PopupRounding = 3;

    style.WindowPadding = ImVec2(4, 4);
    style.FramePadding = ImVec2(6, 4);
    style.ItemSpacing = ImVec2(6, 2);

    style.ScrollbarSize = 18;

    style.WindowBorderSize = 1;
    style.ChildBorderSize = 1;
    style.PopupBorderSize = 1;
    style.FrameBorderSize = is3D;

    style.WindowRounding = 3;
    style.ChildRounding = 3;
    style.FrameRounding = 3;
    style.ScrollbarRounding = 2;
    style.GrabRounding = 3;

#ifdef IMGUI_HAS_DOCK 
    style.TabBorderSize = is3D;
    style.TabRounding = 3;

    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.85f, 0.85f, 0.85f, 0.28f);

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      style.WindowRounding = 0.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
#endif
    style.WindowMenuButtonPosition = ImGuiDir_None;
  }

  void
  update(void* window, float delta) {
    //Update OS mouse position
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Data* be = ImGui_ImplWin32_GetBackendData();
    IM_ASSERT(be != nullptr && "Failed backend platform initialization.\n");
    const ImVec2 mouse_pos_prev = io.MousePos;
    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
    io.MouseHoveredViewport = 0;

    if (io.WantSetMousePos) {
      ovEngineSDK::Vector2 pos(mouse_pos_prev.x, mouse_pos_prev.y);
      ::SetCursorPos(static_cast<int32>(pos.x), static_cast<int32>(pos.y));
    }
    //Set Dear ImGui mouse position from OS position
    POINT mouse_screen_pos;
    if (!::GetCursorPos(&mouse_screen_pos)) {
      return;
    }
    ::ScreenToClient((HWND)window, &mouse_screen_pos);
    io.MousePos = ImVec2((float)mouse_screen_pos.x, (float)mouse_screen_pos.y);
    //Update OS mouse cursor with the cursor requested by imgui
    ImGuiMouseCursor mouse_cursor = io.MouseDrawCursor ? ImGuiMouseCursor_None : 
                                                         ImGui::GetMouseCursor();
    if (be->LastMouseCursor != mouse_cursor) {
      be->LastMouseCursor = mouse_cursor;
      if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) {
        return;
      }
      ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
      if (imgui_cursor != ImGuiMouseCursor_None || io.MouseDrawCursor) {
        //Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
        ::SetCursor(0);
      }
      else {
        //Show OS mouse cursor
        LPTSTR win32_cursor = IDC_ARROW;
        switch (imgui_cursor) {
        case ImGuiMouseCursor_Arrow:        win32_cursor = IDC_ARROW; break;
        case ImGuiMouseCursor_TextInput:    win32_cursor = IDC_IBEAM; break;
        case ImGuiMouseCursor_ResizeAll:    win32_cursor = IDC_SIZEALL; break;
        case ImGuiMouseCursor_ResizeEW:     win32_cursor = IDC_SIZEWE; break;
        case ImGuiMouseCursor_ResizeNS:     win32_cursor = IDC_SIZENS; break;
        case ImGuiMouseCursor_ResizeNESW:   win32_cursor = IDC_SIZENESW; break;
        case ImGuiMouseCursor_ResizeNWSE:   win32_cursor = IDC_SIZENWSE; break;
        case ImGuiMouseCursor_Hand:         win32_cursor = IDC_HAND; break;
        case ImGuiMouseCursor_NotAllowed:   win32_cursor = IDC_NO; break;
        }
        ::SetCursor(::LoadCursor(0, win32_cursor));
      }
    }
    OV_UNREFERENCED_PARAMETER(delta);
    inputCallback();
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
    ImGui_ImplWin32_Data* be = ImGui_ImplWin32_GetBackendData();
    IM_ASSERT(be != nullptr && "No platform backend to shutdown, or already shutdown?");
    ImGui_ImplOV_Data* bd = ImGui_ImplOV_GetBackendData();
    IM_ASSERT(bd != nullptr && "No renderer backend to shitdon, or already shutdown?");
    ImGuiIO& io = ImGui::GetIO();

    ImGui::DestroyPlatformWindows();
    io.BackendRendererName = nullptr;
    io.BackendRendererUserData = nullptr;
    io.BackendPlatformName = nullptr;
    io.BackendPlatformUserData = nullptr;
    IM_DELETE(bd);
    IM_DELETE(be);
    ImGui::DestroyContext();
  }

  void
  inputCallback() {
    ImGuiIO& io = ImGui::GetIO();
    //Mouse Pressed case
    {
      int32 button = -1;
      button = ovEngineSDK::g_baseInput().isMouseKeyPressed(ovEngineSDK::KEYSM::kLEFT) ?
                                                            0 : button;
      button = ovEngineSDK::g_baseInput().isMouseKeyPressed(ovEngineSDK::KEYSM::kRIGHT) ?
                                                            1 : button;
      button = ovEngineSDK::g_baseInput().isMouseKeyPressed(ovEngineSDK::KEYSM::kMIDDLE) ?
                                                            2 : button;
      button = ovEngineSDK::g_baseInput().isMouseKeyPressed(ovEngineSDK::KEYSM::kBUTTON3) ?
                                                            3 : button;
      button = ovEngineSDK::g_baseInput().isMouseKeyPressed(ovEngineSDK::KEYSM::kBUTTON4) ?
                                                            4 : button;
      if (button > -1) {
        io.MouseDown[button] = true;
      }
    }
    //Mouse released case
    {
      int32 button = -1;
      button = ovEngineSDK::g_baseInput().isMouseKeyReleased(ovEngineSDK::KEYSM::kLEFT) ?
                                                            0 : button;
      button = ovEngineSDK::g_baseInput().isMouseKeyReleased(ovEngineSDK::KEYSM::kRIGHT) ?
                                                            1 : button;
      button = ovEngineSDK::g_baseInput().isMouseKeyReleased(ovEngineSDK::KEYSM::kMIDDLE) ?
                                                            2 : button;
      button = ovEngineSDK::g_baseInput().isMouseKeyReleased(ovEngineSDK::KEYSM::kBUTTON3) ?
                                                            3 : button;
      button = ovEngineSDK::g_baseInput().isMouseKeyReleased(ovEngineSDK::KEYSM::kBUTTON4) ?
                                                            4 : button;
      if (button > -1) {
        io.MouseDown[button] = false;
      }
    }
  }
}
