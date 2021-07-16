#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <ovDXGraphicsAPI.h>
#include <ovDXTexture.h>
#include <ovDXBuffer.h>
#include <ovDXShaderProgram.h>
#include <ovDXInputLayout.h>
#include <ovDXSamplerState.h>

#include <wincodec.h>

namespace ovEngineSDK {
  bool DXGraphicsAPI::init(void* window) {
    HWND wHandle = static_cast<HWND>(window);
    if (nullptr == wHandle) {
      return false;
    }
    D3D_DRIVER_TYPE m_DriverType = D3D_DRIVER_TYPE_NULL;
    D3D_FEATURE_LEVEL m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;
    m_device = nullptr;
    m_deviceContext = nullptr;
    m_swapChain = nullptr;

    HRESULT hr = S_OK;
    RECT rc;
    GetClientRect(wHandle, &rc);

    uint32 width = rc.right - rc.left;
    uint32 height = rc.bottom - rc.top;

    uint32 createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;

    D3D_DRIVER_TYPE driverTypes[] = {
      D3D_DRIVER_TYPE_HARDWARE,
      D3D_DRIVER_TYPE_WARP,
      D3D_DRIVER_TYPE_REFERENCE,
    };
    uint32 numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] = {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
    };
    uint32 numFeatureLevels = ARRAYSIZE(featureLevels);

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = wHandle;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = true;

    for (uint32 driverTypeIndex = 0;
      driverTypeIndex < numDriverTypes;
      driverTypeIndex++) {
      m_DriverType = driverTypes[driverTypeIndex];
      hr = D3D11CreateDeviceAndSwapChain(nullptr,
        m_DriverType,
        nullptr,
        createDeviceFlags,
        featureLevels,
        numFeatureLevels,
        D3D11_SDK_VERSION,
        &sd,
        &m_swapChain,
        &m_device,
        &m_FeatureLevel,
        &m_deviceContext);
      if (SUCCEEDED(hr))
        break;
    }
    if (FAILED(hr)) {
      return false;
    }

    SPtr<DXTexture>backBuffer(new DXTexture);

    //Create RTV
    hr = m_swapChain->GetBuffer(0,
      __uuidof(ID3D11Texture2D),
      (LPVOID*)
      &backBuffer->m_texture);
    if (FAILED(hr)) {
      return false;
    }

    hr = m_device->CreateRenderTargetView(
      backBuffer->m_texture,
      nullptr, &backBuffer->m_rtv);
    if (FAILED(hr)) {
      return false;
    }

    SPtr<DXTexture>depthTexture(new DXTexture);

    //Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory(&descDepth, sizeof(descDepth));
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = m_device->CreateTexture2D(&descDepth, nullptr, &depthTexture->m_texture);
    if (FAILED(hr)) {
      return false;
    }

    //Create depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory(&descDSV, sizeof(descDSV));
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = m_device->CreateDepthStencilView(depthTexture->m_texture,
      &descDSV,
      &depthTexture->m_dsv);
    if (FAILED(hr)) {
      return false;
    }

    //Set main RTV and DSV by default
    m_deviceContext->OMSetRenderTargets(1, &backBuffer->m_rtv, depthTexture->m_dsv);

    m_backBuffer = static_pointer_cast<Texture>(backBuffer);
    m_depthStencil = depthTexture;

    fillFormats();

    return true;
  }

  void DXGraphicsAPI::shutdown() {
    m_deviceContext->ClearState();
    m_depthStencil.reset();
    m_backBuffer.reset();
    m_swapChain->Release();
    m_deviceContext->Release();
    m_device->Release();
  }

  Matrix4
  DXGraphicsAPI::matrix4Policy(const Matrix4& mat) {
    return mat.transpose();
  }

  SPtr<Texture>
  DXGraphicsAPI::createTexture(int32 width,
                                             int32 height,
                                             TEXTURE_BINDINGS::E binding,
                                             FORMATS::E format) {
    if (nullptr != m_device) {
      SPtr<DXTexture>texture(new DXTexture);

      //Create texture descriptor
      D3D11_TEXTURE2D_DESC Desc;
      ZeroMemory(&Desc, sizeof(Desc));

      Desc.Width = width;
      Desc.Height = height;
      Desc.MipLevels = 1;
      Desc.ArraySize = 1;
      Desc.Format = m_formats[format];
      Desc.SampleDesc.Count = 1;
      Desc.SampleDesc.Quality = 0;
      Desc.Usage = D3D11_USAGE_DEFAULT;

      if (binding & TEXTURE_BINDINGS::E::SHADER_RESOURCE) {
        Desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
      }
      if (binding & TEXTURE_BINDINGS::E::RENDER_TARGET) {
        Desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
      }
      if (binding & TEXTURE_BINDINGS::E::DEPTH_STENCIL) {
        Desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
      }
      Desc.MiscFlags = 0;

      //Create texture with descriptor
      HRESULT hr = m_device->CreateTexture2D(&Desc, nullptr, &texture->m_texture);
      if (FAILED(hr)) {
        return nullptr;
      }

      if (binding & TEXTURE_BINDINGS::E::SHADER_RESOURCE) {
        D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
        ZeroMemory(&viewDesc, sizeof(viewDesc));
        viewDesc.Format = Desc.Format;
        viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        viewDesc.Texture2D.MostDetailedMip = 0;
        viewDesc.Texture2D.MipLevels = 1;

        if (FAILED(m_device->CreateShaderResourceView(texture->m_texture,
                                                      &viewDesc,
                                                      &texture->m_srv))) {
          return nullptr;
        }
      }
      if (binding & TEXTURE_BINDINGS::E::RENDER_TARGET) {
        D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
        ZeroMemory(&rtvDesc, sizeof(rtvDesc));
        rtvDesc.Format = Desc.Format;
        rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        rtvDesc.Texture2D.MipSlice = 0;

        if (FAILED(m_device->CreateRenderTargetView(
                   texture->m_texture,
                   &rtvDesc,
                   &texture->m_rtv))) {
          return nullptr;
        }
      }
      if (binding & TEXTURE_BINDINGS::E::DEPTH_STENCIL) {
        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
        ZeroMemory(&descDSV, sizeof(descDSV));
        descDSV.Format = Desc.Format;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice = 0;

        if (FAILED(m_device->CreateDepthStencilView(
                   texture->m_texture,
                   &descDSV,
                   &texture->m_dsv))) {
          return nullptr;
        }
      }
      return texture;
    }
    else {
      OutputDebugStringA("Missing API initialization.\n");
      return nullptr;
    }
  }

  SPtr<Texture>
  DXGraphicsAPI::createTextureFromFile(String path) {
    int32 width, height, components;
    uint8* data = stbi_load(path.c_str(), &width, &height, &components, 4);
    if (data) {
      D3D11_TEXTURE2D_DESC desc;
      ZeroMemory(&desc, sizeof(desc));

      desc.Width = width;
      desc.Height = height;
      desc.MipLevels = 1;
      desc.ArraySize = 1;
      desc.SampleDesc.Count = 1;
      desc.SampleDesc.Quality = 0;
      desc.Usage = D3D11_USAGE_DEFAULT;
      desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
      desc.MiscFlags = 0;
      desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

      //Texture data
      D3D11_SUBRESOURCE_DATA initData;
      ZeroMemory(&initData, sizeof(initData));
      initData.pSysMem = data;
      initData.SysMemPitch = width * 4;

      SPtr<DXTexture>texture(new DXTexture);

      if (FAILED(m_device->CreateTexture2D(&desc,
                                           &initData,
                                           &texture->m_texture))) {
        stbi_image_free(data);
        return nullptr;
      }

      //Shader resource data
      D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
      ZeroMemory(&viewDesc, sizeof(viewDesc));
      viewDesc.Format = desc.Format;
      viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
      viewDesc.Texture2D.MostDetailedMip = 0;
      viewDesc.Texture2D.MipLevels = 1;

      if (FAILED(m_device->CreateShaderResourceView(texture->m_texture,
                                                    &viewDesc,
                                                    &texture->m_srv))) {
        stbi_image_free(data);
        return nullptr;
      }
      return texture;
    }
    return nullptr;
  }

  WString getFileName(WString vsfile)
  {
    size_t realPos = 0;
    size_t posInvSlash = vsfile.rfind('\\');
    size_t posSlash = vsfile.rfind('/');

    if (posInvSlash == std::wstring::npos) {
      //No encontramos diagonales invertidas
      if (~posSlash == std::wstring::npos) {
        //Encontramos diagonales normales
        realPos = posSlash;
      }
    }
    else {
      //Encontramos diagonales invertidas
      realPos = posInvSlash;
      if (~posSlash == std::wstring::npos) {
        if (posSlash > realPos) {
          posSlash = realPos;
        }
      }
    }
    return vsfile.substr(realPos, vsfile.length() - realPos);
  }

  SPtr<ShaderProgram> DXGraphicsAPI::createShaderProgram() {
    SPtr<DXShaderProgram> shaderProgram(new DXShaderProgram);
    return shaderProgram;
  }

  SPtr<Buffer> DXGraphicsAPI::createBuffer(const void* data,
                                           SIZE_T size,
                                           BUFFER_TYPE::E type) {
    if (size != 0) {
      D3D11_BUFFER_DESC buffDesc;
      ZeroMemory(&buffDesc, sizeof(buffDesc));

      buffDesc.Usage = D3D11_USAGE_DEFAULT;
      buffDesc.ByteWidth = static_cast<uint32>(size);
      buffDesc.CPUAccessFlags = 0;
      buffDesc.BindFlags = (D3D11_BIND_FLAG)type;

      SPtr<DXBuffer> buffer(new DXBuffer);

      if (data != nullptr) {
        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = data;
        if (FAILED(m_device->CreateBuffer(
                   &buffDesc,
                   &InitData,
                   &buffer->m_buffer))) {
          return nullptr;
        }
      }
      else {
        if (FAILED(m_device->CreateBuffer(
                   &buffDesc,
                   nullptr,
                   &buffer->m_buffer))) {
          return nullptr;
        }
      }
      return buffer;
    }
    else {
      OutputDebugStringA("Invalid size for buffer.\n");
      return nullptr;
    }
  }

  SPtr<InputLayout>
  DXGraphicsAPI::createInputLayout(SPtr<ShaderProgram> program, LAYOUT_DESC& desc) {
    Vector<D3D11_INPUT_ELEMENT_DESC> layout;

    uint32 texcoordindex = 0;
    uint32 positionindex = 0;
    uint32 normalindex = 0;
    uint32 binormalindex = 0;
    uint32 tangentindex = 0;
    uint32 blendindex = 0;
    uint32 weightindex = 0;

    D3D11_INPUT_ELEMENT_DESC D;

    for (uint32 i = 0; i < desc.v_Layout.size(); i++) {
      //SEMANTIC NAME & INDEX
      switch (desc.v_Layout[i].m_semantic) {
      case SEMANTIC::kPOSITION:
        D.SemanticName = "POSITION";
        D.SemanticIndex = positionindex;
        positionindex++;
        break;
      case SEMANTIC::kTEXCOORD:
        D.SemanticName = "TEXCOORD";
        D.SemanticIndex = texcoordindex;
        texcoordindex++;
        break;
      case SEMANTIC::kNORMAL:
        D.SemanticName = "NORMAL";
        D.SemanticIndex = normalindex;
        normalindex++;
        break;
      case SEMANTIC::kBINORMAL:
        D.SemanticName = "BINORMAL";
        D.SemanticIndex = binormalindex;
        binormalindex++;
        break;
      case SEMANTIC::kTANGENT:
        D.SemanticName = "TANGENT";
        D.SemanticIndex = tangentindex;
        tangentindex++;
        break;
      case SEMANTIC::kBLENDINDICES:
        D.SemanticName = "BLENDINDICES";
        D.SemanticIndex = blendindex;
        break;
      case SEMANTIC::kBLENDWEIGHT:
        D.SemanticName = "BLENDWEIGHT";
        D.SemanticIndex = weightindex;
        break;
      }
      //FORMAT
      D.Format = m_formats[desc.v_Layout[i].m_format];
      D.InputSlot = 0;
      D.AlignedByteOffset = desc.v_Layout[i].m_offset;
      D.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
      D.InstanceDataStepRate = 0;

      layout.push_back(D);
    }

    auto VS = static_pointer_cast<DXVertexShader>(program->getVertexShader());

    SPtr<DXInputLayout> ILayout(new DXInputLayout);

    if (FAILED(m_device->CreateInputLayout(layout.data(),
                                           static_cast<UINT>(layout.size()),
                                           VS->m_blob->GetBufferPointer(),
                                           VS->m_blob->GetBufferSize(),
                                           &ILayout->m_inputLayout))) {
      return nullptr;
    }
    return ILayout;
  }

  SPtr<SamplerState>
  DXGraphicsAPI::createSamplerState(FILTER_LEVEL::E mag,
                                    FILTER_LEVEL::E min,
                                    FILTER_LEVEL::E mip,
                                    uint32 anisotropic,
                                    WRAPPING::E wrapMode) {
    D3D11_SAMPLER_DESC desc;
    ZeroMemory(&desc, sizeof(desc));

    desc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)wrapMode;
    desc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)wrapMode;
    desc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)wrapMode;
    desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    desc.MinLOD = 0;
    desc.MaxLOD = D3D11_FLOAT32_MAX;

    desc.MaxAnisotropy = std::clamp<int32>(anisotropic, 0, D3D11_MAX_MAXANISOTROPY);

    if (mag == FILTER_LEVEL::FILTER_POINT) {
      if (min == FILTER_LEVEL::FILTER_POINT) {
        if (mip == FILTER_LEVEL::FILTER_POINT) {
          desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        }
        else {
          desc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
        }
      }
      else {
        if (mip == FILTER_LEVEL::FILTER_POINT) {
          desc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
        }
        else {
          desc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
        }
      }
    }
    else {
      if (min == FILTER_LEVEL::FILTER_POINT) {
        if (mip == FILTER_LEVEL::FILTER_POINT) {
          desc.Filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
        }
        else {
          desc.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
        }
      }
      else {
        if (mip == FILTER_LEVEL::FILTER_POINT) {
          desc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
        }
        else {
          desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        }
      }
    }

   SPtr<DXSamplerState> sampler(new DXSamplerState);

    if (FAILED(m_device->CreateSamplerState(&desc, &sampler->m_sampler))) {
      return nullptr;
    }
    return sampler;
  }

  SPtr<VertexShader>
  DXGraphicsAPI::createVertexShader(WString file) {
    WString realFileName = getFileName(file) + L"_DX.hlsl";
    SPtr<DXVertexShader> vs(new DXVertexShader);

    if (FAILED(compileShaderFromFile(realFileName,
                                     "vs_4_0",
                                     &vs->m_blob))) {
      return nullptr;
    }
    //Create vertex shader from compilation and check errors
    if (FAILED(m_device->CreateVertexShader(
               vs->m_blob->GetBufferPointer(),
               vs->m_blob->GetBufferSize(),
               nullptr,
               &vs->m_vs))) {
      return nullptr;
    }
    return vs;
  }

 SPtr<PixelShader>
 DXGraphicsAPI::createPixelShader(WString file) {
    WString realFileName = getFileName(file) + L"_DX.hlsl";
    SPtr<DXPixelShader> ps(new DXPixelShader);

    if (FAILED(compileShaderFromFile(realFileName,
               "ps_4_0",
               &ps->m_blob))) {
      return nullptr;
    }

    //Create vertex shader from compilation and check errors
    if (FAILED(m_device->CreatePixelShader(
               ps->m_blob->GetBufferPointer(),
               ps->m_blob->GetBufferSize(),
               nullptr,
               &ps->m_ps))) {
      return nullptr;
    }
    return ps;
  }

  void
  DXGraphicsAPI::setBackBuffer() {
    m_deviceContext->OMSetRenderTargets(1,
                                        &static_pointer_cast<DXTexture>(m_backBuffer)->m_rtv,
                                        static_pointer_cast<DXTexture>(m_depthStencil)->m_dsv);
  }

  void
  DXGraphicsAPI::setViewport(int32 topLeftX,
                             int32 topLeftY,
                             int32 width,
                             int32 height,
                             float minDepth,
                             float maxDepth) {
    D3D11_VIEWPORT vp;
    vp.Width = static_cast<FLOAT>(width);
    vp.Height = static_cast<FLOAT>(height);
    vp.MinDepth = minDepth;
    vp.MaxDepth = maxDepth;
    vp.TopLeftX = static_cast<FLOAT>(topLeftX);
    vp.TopLeftY = static_cast<FLOAT>(topLeftY);
    m_deviceContext->RSSetViewports(1, &vp);
  }

  void
  DXGraphicsAPI::setShaders(SPtr<ShaderProgram> program) {
    auto VS = static_pointer_cast<DXVertexShader>(program->getVertexShader());
    auto PS = static_pointer_cast<DXPixelShader>(program->getPixelShader());

    m_deviceContext->VSSetShader(VS->m_vs, 0, 0);
    m_deviceContext->PSSetShader(PS->m_ps, 0, 0);
  }

  void DXGraphicsAPI::drawIndexed(uint32 indices) {
    m_deviceContext->DrawIndexed(indices, 0, 0);
  }

  void DXGraphicsAPI::draw(uint32 count, uint32 first) {
    m_deviceContext->Draw(count, first);
  }

  void DXGraphicsAPI::clearBackBuffer(Color clearColor) {
    float Color[4] = { clearColor.red, clearColor.green, clearColor.blue, clearColor.alpha };
    m_deviceContext->ClearRenderTargetView(
                     static_pointer_cast<DXTexture>(m_backBuffer)->m_rtv,
                     Color);
    m_deviceContext->ClearDepthStencilView(
                     static_pointer_cast<DXTexture>(m_depthStencil)->m_dsv,
                     D3D11_CLEAR_DEPTH,
                     1.0f,
                     0);
  }

  void
  DXGraphicsAPI::setInputLayout(SPtr<InputLayout> layout) {
    if (layout) {
      auto dxlayout = static_pointer_cast<DXInputLayout>(layout);
      if (dxlayout->m_inputLayout) {
        m_deviceContext->IASetInputLayout(dxlayout->m_inputLayout);
      }
      else {
        OutputDebugStringA("Input layout invalid.\n");
      }

    }
    else {
      OutputDebugStringA("Received nullptr pointer.\n");
    }
  }

  void
  DXGraphicsAPI::setRenderTarget(SPtr<Texture> texture, SPtr<Texture> depth) {
    if (texture) {
      auto rtv = static_pointer_cast<DXTexture>(texture)->m_rtv;
      if (rtv) {
        if (depth) {
          auto dsv = static_pointer_cast<DXTexture>(depth)->m_dsv;
          if (dsv) {
            m_deviceContext->OMSetRenderTargets(1, &rtv, dsv);
          }
          else {
            OutputDebugStringA("Invalid DepthStencilView.\n");
          }
        }
        else {
          m_deviceContext->OMSetRenderTargets(1, &rtv, nullptr);
        }

      }
      else {
        OutputDebugStringA("Invalid RenderTargetView.\n");
      }
    }
    else {
      OutputDebugStringA("Received nullptr pointer for texture.\n");
    }
  }

  void
  DXGraphicsAPI::updateBuffer(SPtr<Buffer> buffer, const void* data) {
    if (!buffer) {
      OutputDebugStringA("Invalid buffer received.\n");
      return;
    }
    if (!data) {
      OutputDebugStringA("Invalid data received.\n");
      return;
    }

    auto buff = static_pointer_cast<DXBuffer>(buffer);
    if (!buff->m_buffer) {
      OutputDebugStringA("Buffer not initialized, can't update data.\n");
      return;
    }
    m_deviceContext->UpdateSubresource(buff->m_buffer, 0, nullptr, data, 0, 0);
  }

  void
  DXGraphicsAPI::setVertexBuffer(SPtr<Buffer> buffer,
                                 uint32 stride,
                                 uint32 offset) {
    if (buffer != nullptr) {
      auto buff = static_pointer_cast<DXBuffer>(buffer);
      if (buff->m_buffer != nullptr) {
        m_deviceContext->IASetVertexBuffers(0,
                                            1,
                                            &buff->m_buffer,
                                            &stride,
                                            &offset);
      }
      else {
        OutputDebugStringA("Buffer missing initialization.\n");
      }
    }
    else {
      OutputDebugStringA("Empty buffer received.\n");
    }
  }

  void
  DXGraphicsAPI::setIndexBuffer(SPtr<Buffer> buffer) {
    if (buffer != nullptr) {
      auto buff = static_pointer_cast<DXBuffer>(buffer);
      if (buff->m_buffer != nullptr) {
        m_deviceContext->IASetIndexBuffer(buff->m_buffer, DXGI_FORMAT_R32_UINT, 0);
      }
      else {
        OutputDebugStringA("Buffer missing initialization.\n");
      }
    }
    else {
      OutputDebugStringA("Empty buffer received.\n");
    }
  }

  void
  DXGraphicsAPI::setSamplerState(uint32 slot,
                                 SPtr<Texture> texture,
                                 SPtr<SamplerState> sampler) {
    if (!sampler) {
      OutputDebugStringA("Invalid sampler received.\n");
      return;
    }
    auto samp = static_pointer_cast<DXSamplerState>(sampler);
    if (!samp->m_sampler) {
      OutputDebugStringA("Sampler missing initialization.\n");
      return;
    }
    m_deviceContext->PSSetSamplers(slot, 1, &samp->m_sampler);
  }

  void
  DXGraphicsAPI::setConstantBuffer(uint32 slot,
                                   SPtr<Buffer> buffer,
                                   SHADER_TYPE::E shaderType) {
    if (buffer) {
      auto buff = static_pointer_cast<DXBuffer>(buffer);
      if (buff) {
        if (shaderType == SHADER_TYPE::VERTEX_SHADER) {
          m_deviceContext->VSSetConstantBuffers(slot, 1, &buff->m_buffer);
        }
        else {
          m_deviceContext->PSSetConstantBuffers(slot, 1, &buff->m_buffer);
        }
      }
      else {
        OutputDebugStringA("Buffer not initialized received.\n");
      }
    }
    else {
      OutputDebugStringA("Invalid buffer received.\n");
    }
  }

  void
  DXGraphicsAPI::clearRenderTarget(SPtr<Texture> rt, Color clearColor) {
    if (!rt) {
      OutputDebugStringA("Invalid Render Target received.\n");
      return;
    }
    auto tex = static_pointer_cast<DXTexture>(rt);
    if (!tex->m_rtv) {
      OutputDebugStringA("Render Target not initialized received.\n");
      return;
    }
    float c[4] = { clearColor.red, clearColor.green, clearColor.blue, clearColor.alpha };
    m_deviceContext->ClearRenderTargetView(tex->m_rtv, c);
  }

  void
  DXGraphicsAPI::clearDepthStencil(SPtr<Texture> ds) {
    if (!ds) {
      OutputDebugStringA("Depth Stencil received was nullptr.\n");
      return;
    }
    auto depth = static_pointer_cast<DXTexture>(ds);
    if (!depth->m_dsv) {
      OutputDebugStringA("Depth Stencil not initialized received.\n");
      return;
    }
    m_deviceContext->ClearDepthStencilView(depth->m_dsv,
                                           D3D11_CLEAR_DEPTH,
                                           1.0f,
                                           0);
  }

  void
  DXGraphicsAPI::setTexture(uint32 slot, SPtr<Texture> texture) {
    auto tex = static_pointer_cast<DXTexture>(texture);
    if (!tex) {
      OutputDebugStringA("Texture received was nullptr.\n");
      return;
    }
    if (!tex->m_texture || !tex->m_srv) {
      OutputDebugStringA("Uninitialized texture received.\n");
      return;
    }
    m_deviceContext->PSSetShaderResources(slot, 1, &tex->m_srv);
  }

  void DXGraphicsAPI::setTopology(TOPOLOGY::E topology) {
    D3D11_PRIMITIVE_TOPOLOGY T = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
    switch (topology) {
    case TOPOLOGY::E::kPOINTS:
      T = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
      break;
    case TOPOLOGY::E::kLINES:
      T = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
      break;
    case TOPOLOGY::E::kTRIANGLES:
      T = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
      break;
    case TOPOLOGY::E::kLINE_STRIP:
      T = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
      break;
    case TOPOLOGY::E::kTRIANGLE_STRIP:
      T = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
      break;
    case TOPOLOGY::E::kLINE_ADJACENCY:
      T = D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
      break;
    case TOPOLOGY::E::kTRIANGLE_ADJANCENCY:
      T = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
      break;
    case TOPOLOGY::E::kLINE_STRIP_ADJACENCY:
      T = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
      break;
    case TOPOLOGY::E::kTRIANGLE_STRIP_ADJACENCY:
      T = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
      break;
    }
    m_deviceContext->IASetPrimitiveTopology(T);
  }

  void DXGraphicsAPI::swapBuffer() {
    m_swapChain->Present(0, 0);
  }

  void DXGraphicsAPI::resizeBackBuffer(uint32 width, uint32 height) {
    m_deviceContext->OMSetRenderTargets(0, 0, 0);
    if (FAILED(m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0))) {
      OutputDebugStringA("Failed to resize back buffer.\n");
      return;
    }
    SPtr<DXTexture> backBuffer(new DXTexture);
    if (FAILED(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
               (void**)&backBuffer->m_texture))) {
      OutputDebugStringA("Failed to obtain back buffer from swap chain.\n");
      return;
    }
    if (FAILED(m_device->CreateRenderTargetView(backBuffer->m_texture,
                                                nullptr,
                                                &backBuffer->m_rtv))) {
      OutputDebugStringA("Failed to create render target view for back buffer.\n");
      return;
    }
    SPtr<DXTexture> depthTexture(new DXTexture);
    D3D11_TEXTURE2D_DESC descDepth;
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;

    if (FAILED(m_device->CreateTexture2D(&descDepth, nullptr, &depthTexture->m_texture))) {
      OutputDebugStringA("Failed to create depth stencil texture.\n");
      return;
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory(&descDSV, sizeof(descDSV));
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;

    if (FAILED(m_device->CreateDepthStencilView(depthTexture->m_texture,
               &descDSV,
               &depthTexture->m_dsv))) {
      OutputDebugStringA("Failed to create depth stencil view.\n");
      return;
    }
    m_backBuffer.reset();
    m_depthStencil.reset();
    m_backBuffer = backBuffer;
    m_depthStencil = depthTexture;
  }

  HRESULT DXGraphicsAPI::compileShaderFromFile(WString fileName,
                                               String shaderModel,
                                               ID3DBlob** ppBlobOut) {
    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
    ID3DBlob* pErrorBlob;

    HRESULT hr = D3DCompileFromFile(fileName.c_str(),
                                    0,
                                    0,
                                    "main",
                                    shaderModel.c_str(),
                                    dwShaderFlags,
                                    0,
                                    ppBlobOut,
                                    &pErrorBlob);

    if (FAILED(hr)) {
      if (pErrorBlob != nullptr) {
        OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
        pErrorBlob->Release();
        return hr;
      }
    }
    if (pErrorBlob) {
      pErrorBlob->Release();
    }
    return S_OK;
  }

  void DXGraphicsAPI::fillFormats() {
    m_formats.insert(std::make_pair(FORMATS::kR8_SNORM, DXGI_FORMAT_R8_SNORM));
    m_formats.insert(std::make_pair(FORMATS::kR16_SNORM, DXGI_FORMAT_R16_SNORM));
    m_formats.insert(std::make_pair(FORMATS::kRG8_SNORM, DXGI_FORMAT_R8G8_SNORM));
    m_formats.insert(std::make_pair(FORMATS::kRG16_SNORM, DXGI_FORMAT_R16G16_SNORM));
    m_formats.insert(std::make_pair(FORMATS::kRGB10_A2UI, DXGI_FORMAT_R10G10B10A2_UINT));
    m_formats.insert(std::make_pair(FORMATS::kR16_FLOAT, DXGI_FORMAT_R16_FLOAT));
    m_formats.insert(std::make_pair(FORMATS::kRG16_FLOAT, DXGI_FORMAT_R16G16_FLOAT));
    m_formats.insert(std::make_pair(FORMATS::kRGBA16_FLOAT, DXGI_FORMAT_R16G16B16A16_FLOAT));
    m_formats.insert(std::make_pair(FORMATS::kR32_FLOAT, DXGI_FORMAT_R32_FLOAT));
    m_formats.insert(std::make_pair(FORMATS::kRG32_FLOAT, DXGI_FORMAT_R32G32_FLOAT));
    m_formats.insert(std::make_pair(FORMATS::kRGB32_FLOAT, DXGI_FORMAT_R32G32B32_FLOAT));
    m_formats.insert(std::make_pair(FORMATS::kRGBA32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT));
    m_formats.insert(std::make_pair(FORMATS::kRG11B10_FLOAT, DXGI_FORMAT_R11G11B10_FLOAT));
    m_formats.insert(std::make_pair(FORMATS::kRGB9_E5, DXGI_FORMAT_R9G9B9E5_SHAREDEXP));
    m_formats.insert(std::make_pair(FORMATS::kR8_INT, DXGI_FORMAT_R8_SINT));
    m_formats.insert(std::make_pair(FORMATS::kR8_UINT, DXGI_FORMAT_R8_UINT));
    m_formats.insert(std::make_pair(FORMATS::kR16_INT, DXGI_FORMAT_R16_SINT));
    m_formats.insert(std::make_pair(FORMATS::kR16_UINT, DXGI_FORMAT_R16_UINT));
    m_formats.insert(std::make_pair(FORMATS::kR32_INT, DXGI_FORMAT_R32_SINT));
    m_formats.insert(std::make_pair(FORMATS::kR32_UINT, DXGI_FORMAT_R32_UINT));
    m_formats.insert(std::make_pair(FORMATS::kRG8_INT, DXGI_FORMAT_R8G8_SINT));
    m_formats.insert(std::make_pair(FORMATS::kRG8_UINT, DXGI_FORMAT_R8G8_UINT));
    m_formats.insert(std::make_pair(FORMATS::kRG16_INT, DXGI_FORMAT_R16G16_SINT));
    m_formats.insert(std::make_pair(FORMATS::kRG16_UINT, DXGI_FORMAT_R16G16_UINT));
    m_formats.insert(std::make_pair(FORMATS::kRG32_INT, DXGI_FORMAT_R32G32_SINT));
    m_formats.insert(std::make_pair(FORMATS::kRG32_UINT, DXGI_FORMAT_R32G32_UINT));
    m_formats.insert(std::make_pair(FORMATS::kRGB32_INT, DXGI_FORMAT_R32G32B32_SINT));
    m_formats.insert(std::make_pair(FORMATS::kRGB32_UINT, DXGI_FORMAT_R32G32B32_UINT));
    m_formats.insert(std::make_pair(FORMATS::kRGBA8_INT, DXGI_FORMAT_R8G8B8A8_SINT));
    m_formats.insert(std::make_pair(FORMATS::kRGBA8_UINT, DXGI_FORMAT_R8G8B8A8_UINT));
    m_formats.insert(std::make_pair(FORMATS::kRGBA16_INT, DXGI_FORMAT_R16G16B16A16_SINT));
    m_formats.insert(std::make_pair(FORMATS::kRGBA16_UINT, DXGI_FORMAT_R16G16B16A16_UINT));
    m_formats.insert(std::make_pair(FORMATS::kRGBA32_INT, DXGI_FORMAT_R32G32B32A32_SINT));
    m_formats.insert(std::make_pair(FORMATS::kRGBA32_UINT, DXGI_FORMAT_R32G32B32A32_UINT));
    m_formats.insert(std::make_pair(FORMATS::kR8_UNORM, DXGI_FORMAT_R8_UNORM));
    m_formats.insert(std::make_pair(FORMATS::kR16_UNORM, DXGI_FORMAT_R16_UNORM));
    m_formats.insert(std::make_pair(FORMATS::kRG8_UNORM, DXGI_FORMAT_R8G8_UNORM));
    m_formats.insert(std::make_pair(FORMATS::kRG16_UNORM, DXGI_FORMAT_R16G16_UNORM));
    m_formats.insert(std::make_pair(FORMATS::kRGB5A1_UNORM, DXGI_FORMAT_B5G5R5A1_UNORM));
    m_formats.insert(std::make_pair(FORMATS::kRGBA8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM));
    m_formats.insert(std::make_pair(FORMATS::kRGB10A2_UNORM, DXGI_FORMAT_R10G10B10A2_UNORM));
    m_formats.insert(std::make_pair(FORMATS::kRGBA16_UNORM, DXGI_FORMAT_R16G16B16A16_UNORM));
    m_formats.insert(std::make_pair(FORMATS::kRGBA8_SRGB_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB));
    m_formats.insert(std::make_pair(FORMATS::kD24_S8, DXGI_FORMAT_D24_UNORM_S8_UINT));
  }
}