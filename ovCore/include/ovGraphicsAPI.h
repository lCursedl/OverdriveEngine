#pragma once

#include <ovPrerequisitesCore.h>

#include <ovTexture.h>
#include <ovShader.h>
#include <ovBuffer.h>
#include <ovShaderProgram.h>
#include <ovInputLayout.h>
#include <ovSamplerState.h>
#include <ovModule.h>
#include <ovMatrix4.h>
#include <ovRasterizerState.h>
#include <ovDepthStencilState.h>
#include <ovBlendState.h>

namespace ovEngineSDK {

  /**
  * @struct      COLOR
  *	@brief       Structure which defines a color with Red, Green, Blue and
  *              Alpha components.
  */
  struct Color {
    float red;
    float green;
    float blue;
    float alpha;
  };

  /**
  * @enum        TOPOLOGY
  *	@brief       Enum which defines the different types of topologies available from the api.
  */
  namespace TOPOLOGY {
    enum E {
      kPOINTS = 0,
      kLINES,
      kTRIANGLES,
      kLINE_STRIP,
      kTRIANGLE_STRIP,
      kLINE_ADJACENCY,
      kTRIANGLE_ADJANCENCY,
      kLINE_STRIP_ADJACENCY,
      kTRIANGLE_STRIP_ADJACENCY
    };
  }

  /**
  * @class       CGraphicsAPI
  *	@brief       Class which serves as an interface for CDXGraphicsAPI & COGLGraphicsAPI.
  */
  class GraphicsAPI : public Module<GraphicsAPI>
  {
   public:
     /**
     * @fn       bool init(void* window)
     * @brief    Initializes and creates the necesary elements for basic rendering.
     * @param[in]window Window handler of the window properly created for use.
     */
     virtual bool
     init(void* window) { OV_UNREFERENCED_PARAMETER(window); return false;}

     /** 
     * @fn       void shutdown()
     * @brief    Releases memory and deletes all resources created by the API.
     */
     virtual void
     shutdown() {};

     /**
     * @fn       Matrix4 matrixPolicy(const Matrix4& mat)
     * @brief    Returns an API compatible Matrix4.
     * @param[in]mat A Matrix4 to convert.
     * @return   the received matrix in the correct order.
     */
     virtual Matrix4
     matrix4Policy(const Matrix4& mat) {
      OV_UNREFERENCED_PARAMETER(mat);
      return Matrix4();
     }

     /**
     * @fn
     */
     FORCEINLINE void
     setObject(GraphicsAPI* _api) {
      GraphicsAPI::_instance() = _api;
     }

     /**
     * @fn       Matrix4 createCompatibleOrtho(float left, float right, float top,
     *           float bottom, float near, float far)
     * @brief    Creates an API compatible orthographic Matrix4.
     * @param[in] left 
     * @param[in] right
     * @param[in] top
     * @param[in] bottom
     * @param[in] near
     * @param[in] far
     * @return   Compatible orthographic matrix por the API.
     */
     virtual Matrix4
     createCompatibleOrtho(float Left,
                           float Right,
                           float Top,
                           float Bottom,
                           float Near,
                           float Far) {
      OV_UNREFERENCED_PARAMETER(Left); OV_UNREFERENCED_PARAMETER(Right);
      OV_UNREFERENCED_PARAMETER(Top); OV_UNREFERENCED_PARAMETER(Bottom);
      OV_UNREFERENCED_PARAMETER(Near); OV_UNREFERENCED_PARAMETER(Far);
      return Matrix4();
     }

     /***********************************************************************************/
     /*----------------------------------DEVICE-----------------------------------------*/
     /***********************************************************************************/

     /**
     * @fn       SPtr<Texture> createTexture(int32 width, int32 height,
     *								                       TEXTURE_BINDINGS binding,
     *                                       FORMATS format)
     * @brief    Creates a texture with the received parameters and returns it.
     * @param[in]width	Specificates texture's width.
     * @param[in]height	Specificates texture's height.
     * @param[in]binding	Specifies type of texture to create.
     * @param[in]format	Specifies the texture format.
     * @return   Texture pointer of the corresponding API.
     */
     virtual SPtr<Texture>
     createTexture(int32 width,
                   int32 height,
                   TEXTURE_BINDINGS::E binding,
                   FORMATS::E format) {
      OV_UNREFERENCED_PARAMETER(width);
      OV_UNREFERENCED_PARAMETER(height);
      OV_UNREFERENCED_PARAMETER(binding);
      OV_UNREFERENCED_PARAMETER(format);
      return nullptr;
     }
     /**
     * @fn       Texture createTextureFromFile(std::string path)
     * @brief    Loads a texture file and returns a CTexture pointer with its data.
     * @param[in]path The path to the file.
     * @return   Texture pointer for the corresponding API with data.
     */
     virtual SPtr<Texture>
     createTextureFromFile(String){
      return nullptr;
     }

     virtual Vector<SPtr<Texture>>
     createCompressedTexture(String) {
       return Vector<SPtr<Texture>>();
     }

     

     /**
     * @fn       SPtr<Texture> createTextureFromMemory(int32 width, int32 height,
                 TEXTURE_BINDINGS binding, FORMATS format, uint8* data)
     * @brief    Creates a texture with the received parameters and data and returns it.
     * @param[in]width Specifies texture's width.
     * @param[in]height Specifies texture's height.
     * @param[in]binding Specifies tpye of texture to create.
     * @param[in]format Specifies texture's format.
     * @param[in]data Pointer to the texture's data.
     * @return   Texture pointer of the corresponding API.
     */
     virtual SPtr<Texture>
       createTextureFromMemory(int32,
                               int32,
                               TEXTURE_BINDINGS::E,
                               FORMATS::E,
                               uint8*,
                               uint32 elements = 4) {
       OV_UNREFERENCED_PARAMETER(elements);
       return nullptr;
     }
     /**
     * @fn       SPtr<ShaderProgram> createShaderProgram()
     * @brief    Creates a shader program and returns it.
     * @return   ShaderProgram pointer of the corresponding API.
     */
     virtual SPtr<ShaderProgram>
     createShaderProgram() {
      return nullptr;
     }

     /**
     * @fn       SPtr<Buffer> createBuffer(const void * data, int32 size, BUFFER_TYPE type)
     * @brief    Creates a buffer with the received parameters and optionally data.
     * @param[in]data Pointer to the data to load. Can be nullptr.
     * @param[in]size Size of the data to load.
     * @param[in]type	Type of buffer to create (VERTEX_BUFFER,
     *            INDEX_BUFFER, CONST_BUFFER).
     * @param[in]elements Number of elements in buffer.
     * @param[in]format Format type for the buffer data.
     * @return   Buffer pointer of the corresponding API.
     */
     virtual SPtr<Buffer>
     createBuffer(const void* data,
                  SIZE_T size,
                  BUFFER_TYPE::E type,
                  uint32 elements= 0,
                  FORMATS::E format = FORMATS::kR32_FLOAT) {
      OV_UNREFERENCED_PARAMETER(data);
      OV_UNREFERENCED_PARAMETER(size);
      OV_UNREFERENCED_PARAMETER(type);
      OV_UNREFERENCED_PARAMETER(elements);
      OV_UNREFERENCED_PARAMETER(format);
      return nullptr;
     }

     /**
     * @fn       SPtr<InputLayout> createInputLayout(SPtr<ShaderProgram> program,
                                                     LAYOUT_DESC desc)
     * @brief    Creates an input layout from a descriptor and shader program.
     * @param[in]program ShaderProgram from where to extract info.
     * @param[in]desc The layout descriptor.
     * @return   InputLayout pointer of the corresponding API.
     */
     virtual SPtr<InputLayout>
     createInputLayout(SPtr<ShaderProgram> program,
                       LAYOUT_DESC& desc) {
      OV_UNREFERENCED_PARAMETER(program);
      OV_UNREFERENCED_PARAMETER(desc);
      return nullptr;
     }

     /**
     * @fn       SPtr<SamplerState> createSamplerState(FILTER_LEVEL mag, FILTER_LEVEL min,
     *										                             FILTER_LEVEL mip, uint32 anisotropic,
     *                                                 WRAPPING wrapMode,
     *                                                 COMPARISON compMode)
     * @brief    Creates a sampler state from defined parameters and returns it.
     * @param[in]mag Magnification filter ( LINEAR, POINT ).
     * @param[in]min Minification filter ( LINEAR, POINT ).
     * @param[in]mip Mipmap filter ( LINEAR, POINT ).
     * @param[in]anisotropic Anisotropic level ( 0 - 16 ).
     * @param[in]wrapMode Mode in which texture border are treated ( WRAP, MIRROR,
                 CLAMP, BORDER, MIRROR_ONCE ).
     * @param[in]compMode Function mode that compares sampled data against existing
     *           sampled data.
     * @return   SamplerState pointer of the corresponding API.
     */
     virtual SPtr<SamplerState>
     createSamplerState(FILTER_LEVEL::E mag,
                        FILTER_LEVEL::E min,
                        FILTER_LEVEL::E mip,
                        bool filterCompare,
                        uint32 anisotropic,
                        WRAPPING::E wrapMode,
                        COMPARISON::E compMode) {
      OV_UNREFERENCED_PARAMETER(mag);OV_UNREFERENCED_PARAMETER(min);
      OV_UNREFERENCED_PARAMETER(filterCompare);
      OV_UNREFERENCED_PARAMETER(mip);OV_UNREFERENCED_PARAMETER(anisotropic);
      OV_UNREFERENCED_PARAMETER(wrapMode);OV_UNREFERENCED_PARAMETER(compMode);
      return nullptr;
     }

     /**
     * @fn       VertexShader* createVertexShader(WString file)
     * @brief    Creates a vertex shader from the specified file, if available.
     * @param[in]file Filename of the vertex shader.
     * @return   VertexShader pointer of the corresponding API.
     */
     virtual SPtr<VertexShader>
     createVertexShader(WString) {
      return nullptr;
     }

     /**
     * @fn       PixelShader* createPixelShader(WString file)
     * @brief    Creates a pixel shader from the specified file, if available.
     * @param[in]file Filename of the pixel shader.
     * @return   PixelShader pointer of the corresponding API.
     */
     virtual SPtr<PixelShader>
     createPixelShader(WString) {
      return nullptr;
     }

     /**
     * @fn       ComputeShader* createComputeShader(WString file)
     * @brief    Creates a commpute shader from the specified file, if available.
     * @param[in]file Filename of the compute shader.
     * @return   ComputeShader pointer of the corresponding API.
     */
     virtual SPtr<ComputeShader>
       createComputeShader(WString) {
       return nullptr;
     }

     /**
     * @fn       SPtr<RasterizerState> createRasterizerState(FILL_MODE::E fillmode,
                                                             CULL_MODE::E cullmode,
                                                             bool counterClockWise)
     * @brief    Creates a rasterizer state from the defined parameters and returns it.
     * @param[in] fillMode Determines the fill mode to use when rendering triangles
     *                    ( WIREFRAME, SOLID ).
     * @param[in] cullMode Indicates triangles facing a particular direction are not drawn
     *                    ( NONE, FRONT, BACK ).
     * @param[in] counterClockWise Determines if a triangle is front- or back-facing.
     * @param[in] scissor Determines if scissor-rectangle culling is active.
     * @return    Rasterizer state smart pointer of the corresponding API.
     */
     virtual SPtr<RasterizerState>
     createRasterizerState(FILL_MODE::E,
                           CULL_MODE::E,
                           bool,
                           bool) {
       return nullptr;
     }

     /**
     * @fn       SPtr<DepthStencilState>createDepthStencilState(bool stencilEnable,
                                                                bool depthEnable)
     * @brief    Creates a depth stencil state from the defined parameters and returns it.
     * @param[in]stencilEnable Enable depth testing.
     * @param[in]depthEnable Enable stencil testing.
     * @param[in]compMode Function mode that compares sampled data against existing
     *           sampled data.
     * @return   Depth stencil state smart pointer of the corresponding API.
     */
     virtual SPtr<DepthStencilState>
       createDepthStencilState(bool,
                               bool,
                               COMPARISON::E) {
       return nullptr;
     }

     virtual Vector2
     getViewportDimensions() {
       return Vector2();
     }

     virtual SPtr<BlendState>
     createBlendState(bool,
                      BLEND_TYPE::E,
                      BLEND_TYPE::E,
                      BLEND_OP::E,
                      BLEND_TYPE::E,
                      BLEND_TYPE::E,
                      BLEND_OP::E,
                      Vector4) {
       return nullptr;
     }

     /***********************************************************************************/
     /*----------------------------DEVICE CONTEXT---------------------------------------*/
     /***********************************************************************************/

     /**
     * @fn       void setBackBuffer()
     * @brief    Sets the default render target view and depth stencil view as current.
     */
     virtual void setBackBuffer() {}

     /**
     * @fn       setViewport(int32 topLeftX, int32 topLeftY, int32 width, int32 height)
     * @brief    Sets the viewport area
     * @param[in]topLeftX Corner on the X axis of the render area.
     * @param[in]topLeftY Corner on the Y axis of the render area.
     * @param[in]width Width of render area.
     * @param[in]height Height of render area.
     * @param[in]minDepth Minimum depth of viewport
     * @param[in]maxDepth Maximum depth of viewport
     */
     virtual void setViewport(int32,
                              int32,
                              int32,
                              int32,
                              float,
                              float) {}

     /**
     * @fn void  drawIndexed(uint32 indices)
     * @brief    Makes a draw call with the specified amount of indices.
     * @param[in]indices Amount of indices in geometry to draw.
     * @param[in]indexlocation Location of the first index to read.
     * @param[in]vertexlocation Value added to each index before reading a vertex from
     *           vertex buffer.
     */
     virtual
     void drawIndexed(uint32 indices,
                      uint32 indexlocation = 0,
                      uint32 vertexlocation = 0) {
       OV_UNREFERENCED_PARAMETER(indices);
       OV_UNREFERENCED_PARAMETER(indexlocation);
       OV_UNREFERENCED_PARAMETER(vertexlocation);
     }

     /**	\fn void draw()
     *	\brief Makes a draw call with the currently bound vertex buffer.
     *	@param[in] count Amount of vertices.
     *	@param[in] first Start location to read the vertices.
     */
     virtual void draw(uint32, uint32) {}

     virtual void drawIndexedInstanced(uint32, uint32) {}

     virtual void drawInstanced(uint32, uint32, uint32) {}

     /**
     * @fn       void setShaders(CShaderProgram* program)
     * @brief    Receives a shader program and sets both its vertex and pixel shader
     *	         as current.
     * @param[in]program CShaderProgram pointer from where to take shaders.
     */
     virtual void setShaders(SPtr<ShaderProgram>) {}

     /**
     * @fn       void setInputLayout(CInputLayout* layout)
     * @brief    Receives an input layout and sets it as current.
     * @param[in] layout CInputLayout pointer from where to take the layout.
     */
     virtual void setInputLayout(SPtr<InputLayout>) {}

     /**
     * @fn       void clearBackBuffer(COLOR color)
     * @brief    Clears default render target with specified color and depth stencil.
     * @param[in]color Color in which to clear the render target.
     */
     virtual void clearBackBuffer(Color) {}

     /** 
     * @fn       void setRenderTarget(CTexture* texture, CTexture* depth)
     * @brief    Sets the specified render targets and depth stencil as current.
     * @param[in] amount Number of render targets to set
     * @param[in] texture Texture pointer which contains the render target.
     * @param[in] depth Texture pointer which contains the depth stencil.
     *	         depth can be nullptr if only render target wants to be set.
     */
     virtual void setRenderTarget(int32 , Vector<SPtr<Texture>>, SPtr<Texture>) {}

     /**
     * @fn       void updateBuffer(CBuffer* buffer, const void * data)
     * @brief    Updates the specified buffer with the received data
     * @param[in]buffer Buffer to update.
     * @param[in]data Pointer of the data to assign to buffer.
     */
     virtual void updateBuffer(SPtr<Buffer>, const void*) {}

     /**
     * @fn       void setVertexBuffer(CBuffer* buffer)
     * @brief    Sets the specified buffer as the current vertex buffer.
     * @param[in]buffer Buffer to set.
     * @param[in]stride Lenght of the buffer data.
     * @param[in]offset Buffer offset from where to take data.
     *           warning If buffer is nullptr, no operations are done.
     */
     virtual void setVertexBuffer(SPtr<Buffer>,
                                  uint32,
                                  uint32) {}

     /**
     * @fn       void setIndexBuffer(CBuffer* buffer)
     * @brief    Sets the specified buffer as the current index buffer.
     * @param[in]buffer Buffer to set.
     * @param[in]format Format of the index buffer (R16 - short, R32 - normal).
     * @warning  If buffer is nullptr, no operations are done.
     */
     virtual void setIndexBuffer(SPtr<Buffer> buffer, FORMATS::E format) {
      OV_UNREFERENCED_PARAMETER(buffer); OV_UNREFERENCED_PARAMETER(format);
     }

     /** 
     * @fn       void setSamplerState(CTexture* texture, CSamplerState* sampler)
     * @brief    Sets the sampler state and binds it to a texture and slot.
     * @param[in]slot	   Slot of the sampler in the shader.
     * @param[in]texture Texture to which bind the sampler to.
     * @param[in]sampler Sampler State to bind.
     */
     virtual void setSamplerState(uint32,
                                  SPtr<Texture>,
                                  SPtr<SamplerState>,
                                  SHADER_TYPE::E) {}

     /**
     * @fn       void setConstantBuffer(uint32 slot, SPtr<Buffer> buffer,
     *							                    SHADER_TYPE shaderType)
     * @brief    Sets the specified buffer to use for the shaders.
     * @param[in]slot Index of the constant buffer in the shader.
     * @param[in]buffer Buffer to set.
     * @param[in]shaderType Shader where the buffer is used ( VERTEX_SHADER,
     *				   PIXEL_SHADER )
     */
     virtual void setConstantBuffer(uint32,
                                    SPtr<Buffer>,
                                    SHADER_TYPE::E) {}

     /**
     * @fn       void clearRenderTarget(CTexture* rt, COLOR color)
     * @brief    Clears the specified render target.
     * @param[in] rt Render target to clear.
     * @param[in] color COLOR to use for clearing.
     */
     virtual void clearRenderTarget(SPtr<Texture>, Color) {}

     /**
     * @fn       void clearDepthStencil(CTexture* ds)
     * @brief    Clears the specified depth stencil.
     */
     virtual void clearDepthStencil(SPtr<Texture>) {}

     /**
     * @fn       void setTexture(uint32 slot, CTexture* texture)
     * @brief    Sets the texture for use in the pixel shader.
     * @param[in]slot Index of the texture in the shader.
     * @param[in]texture Texture to set.
     */
     virtual void setTexture(uint32, SPtr<Texture>) {}

     /**
     * @fn       void setTopology(TOPOLOGY topology)
     * @brief    Sets the topology type for vertex data.
     * @param[in]topology TOPOLOGY type to set.
     */
     virtual
     void setTopology(TOPOLOGY::E) {}

     /**
     * @fn       void setBufferShaderResource(uint32 )
     * @brief    
     */
     virtual
     void setBufferShaderResource(uint32, SPtr<Buffer>, SHADER_TYPE::E) {}

     virtual
     void setTextureShaderResource(uint32, SPtr<Texture>, SHADER_TYPE::E) {}

     virtual
     void setBufferUnorderedAccess(uint32, SPtr<Buffer>) {}

     virtual
     void setTextureUnorderedAccess(uint32, SPtr<Texture>) {}

     virtual
     void setRasterizerState(SPtr<RasterizerState>) {}

     virtual
     void setDepthStencilState(SPtr<DepthStencilState>) {}

     virtual
     void dispatch(uint32, uint32, uint32) {}

     virtual
     void getRaterizerState(SPtr<RasterizerState>&) {}

     virtual
     void getBlendState(SPtr<BlendState>&) {}

     virtual
     void getDepthStencilState(SPtr<DepthStencilState>&) {}

     virtual
     void getTextureShaderResource(uint32,
                                   SPtr<Texture>&,
                                   SHADER_TYPE::E) {}

     virtual
     void getSampler(uint32, SPtr<SamplerState>&, SHADER_TYPE::E) {}

     virtual
     void getShaderProgram(SPtr<ShaderProgram>&) {}

     virtual
     void getConstantBuffer(SPtr<Buffer>&, uint32, SHADER_TYPE::E) {}

     virtual
     void getBuffer(SPtr<Buffer>&, BUFFER_TYPE::E) {}

     virtual
     void getVertexBuffer(SPtr<Buffer>& buffer, uint32& stride, uint32& offset) {
      OV_UNREFERENCED_PARAMETER(buffer);
      OV_UNREFERENCED_PARAMETER(stride);
      OV_UNREFERENCED_PARAMETER(offset);
     }

     virtual
     void getIndexBuffer(SPtr<Buffer>& buffer) {
      OV_UNREFERENCED_PARAMETER(buffer);
     }

     virtual
     void getInputLayout(SPtr<InputLayout>&) {}

     virtual
     void setScissorRects(float left, float right, float top, float bottom) {
       OV_UNREFERENCED_PARAMETER(left); OV_UNREFERENCED_PARAMETER(right);
       OV_UNREFERENCED_PARAMETER(top); OV_UNREFERENCED_PARAMETER(bottom);
     }

     //SWAPCHAIN

     /**
     * @fn       void swapBuffer()
     * @brief    Swaps the back buffer with the front buffer and presents it.
     */
     virtual
     void swapBuffer() {}

     /**
     * @fn       void resizeBackBuffer(uint32 width, uint32 height)
     * @brief    Resizes both backbuffer and its depth stencil.
     * @param[in]width New X dimension for back buffer.
     * @param[in]height New Y dimension for back buffer.
     */
     virtual
     void resizeBackBuffer(uint32, uint32) {}

     //OUTPUT MERGER

     virtual
     void setBlendState(SPtr<BlendState> blend, uint32 mask) {
      OV_UNREFERENCED_PARAMETER(blend);
      OV_UNREFERENCED_PARAMETER(mask);
     }

     virtual
     void getBackBuffer(SPtr<Texture>& tex) {
      OV_UNREFERENCED_PARAMETER(tex);
     }

     virtual bool
     isInitialized() { return false; }

    protected:
     bool m_initialized;
  };

  OV_CORE_EXPORT GraphicsAPI&
  g_graphicsAPI();

  using funCreateGraphicsAPI = GraphicsAPI * (*)();
}