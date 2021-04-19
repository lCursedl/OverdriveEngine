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

namespace ovEngineSDK {
  /**
  * @struct      COLOR
  *	@brief       Structure which defines a color with Red, Green, Blue and
  *              Alpha components.
  */
  struct COLOR {
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
    enum class E {
      POINTS = 0,
      LINES,
      TRIANGLES,
      LINE_STRIP,
      TRIANGLE_STRIP,
      LINE_ADJACENCY,
      TRIANGLE_ADJANCENCY,
      LINE_STRIP_ADJACENCY,
      TRIANGLE_STRIP_ADJACENCY
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
     virtual bool init(void* window) {return false;}

     /** 
     * @fn       void shutdown()
     * @brief    Releases memory and deletes all resources created by the API.
     */
     virtual void shutdown() {};

     /**
     * @fn       Matrix4 matrixPolicy(const Matrix4& mat)
     * @brief    Returns an API compatible Matrix4.
     * @param[in]mat A Matrix4 to convert.
     * @return   the received matrix in the correct order.
     */
     virtual Matrix4 matrix4Policy(const Matrix4& mat) {return Matrix4();}

     /**
     * @fn
     */
     FORCEINLINE void
     setObject(GraphicsAPI* _api) {
      GraphicsAPI::_instance() = _api;
     }

     /**
     * @fn       glm::mat4 matrix4Policy(const glm::mat4 & mat)
     * @brief    Returns an API compatible matrix of 4 x 4 dimensions
     * @param[in]mat A glm::mat4 to make compatible.
     * @return   The received matrix in the correct order.
     */
     //virtual glm::mat4 matrix4Policy(const glm::mat4& mat) = 0;

     /***********************************************************************************/
     /*----------------------------------DEVICE-----------------------------------------*/
     /***********************************************************************************/

     /** \fn     Texture* createTexture(int32 width, int32 height,
     *								                  TEXTURE_BINDINGS binding,
                                        FORMATS format)
     * @brief    Creates a texture with the received parameters and returns it.
     * @param[in]width	Specificates texture's width.
     * @param[in]height	Specificates texture's height.
     * @param[in]binding	Specifies type of texture to create.
     * @param[in]format	Specifies the texture format.
     * @return   Texture pointer of the corresponding API.
     */
     virtual Texture* createTexture(int32 width,
                                    int32 height,
                                    TEXTURE_BINDINGS::E binding,
                                    FORMATS::E format) { return nullptr;}
     /**
     * @fn       Texture* createTextureFromFile(std::string path)
     * @brief    Loads a texture file and returns a CTexture pointer with its data.
     * @param[in]path The path to the file.
     * @return   Texture pointer for the corresponding API with data.
     */
     //virtual Texture* createTextureFromFile(std::string path) = 0;

     /**
     * @fn       ShaderProgram* createShaderProgram()
     * @brief    Creates a shader program and returns it.
     * @return   ShaderProgram pointer of the corresponding API.
     */
     virtual ShaderProgram* createShaderProgram() {return nullptr;}

     /**
     * @fn       CBuffer* createBuffer(const void * data, int32 size, BUFFER_TYPE type)
     * @brief    Creates a buffer with the received parameters and optionally data.
     * @param[in]data Pointer to the data to load. Can be nullptr.
     * @param[in]size Size of the data to load.
     * @param[in]type	Type of buffer to create (VERTEX_BUFFER,
     *            INDEX_BUFFER, CONST_BUFFER).
     * @return   Buffer pointer of the corresponding API.
     */
     virtual Buffer* createBuffer(const void* data,
                                  uint32 size,
                                  BUFFER_TYPE::E type) {return nullptr;}

     /**
     * @fn       InputLayout* createInputLayout(CShaderProgram* program, LAYOUT_DESC desc)
     * @brief    Creates an input layout from a descriptor and shader program.
     * @param[in]program ShaderProgram from where to extract info.
     * @param[in]desc The layout descriptor.
     * @return   InputLayout pointer of the corresponding API.
     */
     virtual InputLayout* createInputLayout(ShaderProgram* program,
                                            LAYOUT_DESC desc) {return nullptr;}

     /**
     * @fn       SamplerState* createSamplerState(FILTER_LEVEL mag, FILTER_LEVEL min,
     *										                        FILTER_LEVEL mip, uint32 anisotropic,
     *                                            WRAPPING wrapMode)
     * @brief    Creates a sampler state from defined parameters and returns it.
     * @param[in]mag Magnification filter ( LINEAR, POINT ).
     * @param[in]min Minification filter ( LINEAR, POINT ).
     * @param[in]mip Mipmap filter ( LINEAR, POINT ).
     * @param[in]anisotropic Anisotropic level ( 0 - 16 ).
     * @param[in]wrapMode Mode in which texture border are treated ( WRAP, MIRROR,
                 CLAMP, BORDER, MIRROR_ONCE ).
     * @return   SamplerState pointer of the corresponding API.
     */
     virtual SamplerState* createSamplerState(FILTER_LEVEL::E mag,
                                              FILTER_LEVEL::E min,
                                              FILTER_LEVEL::E mip,
                                              uint32 anisotropic,
                                              WRAPPING::E wrapMode) {return nullptr;}

     /**
     * @fn       CVertexShader* createVertexShader(std::wstring file)
     * @brief    Creates a vertex shader from the specified file, if available.
     * @param[in]file Filename of the vertex shader.
     * @return   VertexShader pointer of the corresponding API.
     */
     virtual VertexShader* createVertexShader(std::wstring file) {return nullptr;};

     /**
     * @fn       PixelShader* createPixelShader(std::wstring file)
     * @brief    Creates a pixe shader from the specified file, if available.
     * @param[in]file Filename of the pixel shader.
     * @return   PixelShader pointer of the corresponding API.
     */
     virtual PixelShader* createPixelShader(std::wstring file) {return nullptr;}

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
     */
     virtual void setViewport(int32 topLeftX,
                              int32 topLeftY,
                              int32 width,
                              int32 height) {}

     /**
     * @fn void  drawIndexed(uint32 indices)
     * @brief    Makes a draw call with the specified amount of indices.
     * @param[in]indices Amount of indices in geometry to draw.
     */
     virtual void drawIndexed(uint32 indices) {}

     /**	\fn void draw()
     *	\brief Makes a draw call with the currently bound vertex buffer.
     *	@param[in] count Amount of vertices.
     *	@param[in] first Start location to read the vertices.
     */
     virtual void draw(uint32 count, uint32 first) {}

     /**
     * @fn       void setShaders(CShaderProgram* program)
     * @brief    Receives a shader program and sets both its vertex and pixel shader
     *	         as current.
     * @param[in]program CShaderProgram pointer from where to take shaders.
     */
     virtual void setShaders(ShaderProgram* program) {}

     /**
     * @fn       void setInputLayout(CInputLayout* layout)
     * @brief    Receives an input layout and sets it as current.
     * @param[in] layout CInputLayout pointer from where to take the layout.
     */
     virtual void setInputLayout(InputLayout* layout) {}

     /**
     * @fn       void clearBackBuffer(COLOR color)
     * @brief    Clears default render target with specified color and depth stencil.
     * @param[in]color Color in which to clear the render target.
     */
     virtual void clearBackBuffer(COLOR color) {}

     /** 
     * @fn       void setRenderTarget(CTexture* texture, CTexture* depth)
     * @brief    Sets the specified render target and depth stencil as current.
     * @param[in]texture CTexture pointer which contains the render target.
     * @param[in]depth CTexture pointer which contains the depth stencil.
     *	         depth can be nullptr if only render target wants to be set.
     */
     virtual void setRenderTarget(Texture* texture, Texture* depth) {}

     /**
     * @fn       void updateBuffer(CBuffer* buffer, const void * data)
     * @brief    Updates the specified buffer with the received data
     * @param[in]buffer Buffer to update.
     * @param[in]data Pointer of the data to assign to buffer.
     */
     virtual void updateBuffer(Buffer* buffer, const void* data) {}

     /**
     * @fn       void setVertexBuffer(CBuffer* buffer)
     * @brief    Sets the specified buffer as the current vertex buffer.
     * @param[in]buffer Buffer to set.
     * @param[in]stride Lenght of the buffer data.
     * @param[in]offset Buffer offset from where to take data.
     *           warning If buffer is nullptr, no operations are done.
     */
     virtual void setVertexBuffer(Buffer* buffer,
                                  uint32 stride,
                                  uint32 offset) {}

     /**
     * @fn       void setIndexBuffer(CBuffer* buffer)
     * @brief    Sets the specified buffer as the current index buffer.
     * @param[in]buffer Buffer to set.
     * @warning  If buffer is nullptr, no operations are done.
     */
     virtual void setIndexBuffer(Buffer* buffer) {}

     /** 
     * @fn       void setSamplerState(CTexture* texture, CSamplerState* sampler)
     * @brief    Sets the sampler state and binds it to a texture and slot.
     * @param[in]slot	   Slot of the sampler in the shader.
     * @param[in]texture Texture to which bind the sampler to.
     * @param[in]sampler Sampler State to bind.
     */
     virtual void setSamplerState(uint32 slot,
                                  Texture* texture,
                                  SamplerState* sampler) {}

     /**
     * @fn       void setConstantBuffer(uint32 slot, CBuffer* buffer,
     *							                    SHADER_TYPE shaderType)
     * @brief    Sets the specified buffer to use for the shaders.
     * @param[in]slot Index of the constant buffer in the shader.
     * @param[in]buffer Buffer to set.
     * @param[in]shaderType Shader where the buffer is used ( VERTEX_SHADER,
     *				   PIXEL_SHADER )
     */
     virtual void setConstantBuffer(uint32 slot,
                                    Buffer* buffer,
                                    SHADER_TYPE::E shaderType) {}

     /**
     * @fn       void clearRenderTarget(CTexture* rt, COLOR color)
     * @brief    Clears the specified render target.
     * @param[in] rt Render target to clear.
     * @param[in] color COLOR to use for clearing.
     */
     virtual void clearRenderTarget(Texture* rt, COLOR color) {}

     /**
     * @fn       void clearDepthStencil(CTexture* ds)
     * @brief    Clears the specified depth stencil.
     */
     virtual void clearDepthStencil(Texture* ds) {}

     /**
     * @fn       void setTexture(uint32 slot, CTexture* texture)
     * @brief    Sets the texture for use in the pixel shader.
     * @param[in]slot Index of the texture in the shader.
     * @param[in]texture Texture to set.
     */
     virtual void setTexture(uint32 slot, Texture* texture) {}

     /**
     * @fn       void setTopology(TOPOLOGY topology)
     * @brief    Sets the topology type for vertex data.
     * @param[in]topology TOPOLOGY type to set.
     */
     virtual void setTopology(TOPOLOGY::E topology) {}

     //SWAPCHAIN

     /**
     * @fn       void swapBuffer()
     * @brief    Swaps the back buffer with the front buffer and presents it.
     */
     virtual void swapBuffer() {}

     /**
     * @fn       void resizeBackBuffer(uint32 width, uint32 height)
     * @brief    Resizes both backbuffer and its depth stencil.
     * @param[in]width New X dimension for back buffer.
     * @param[in]height New Y dimension for back buffer.
     */
     virtual void resizeBackBuffer(uint32 width, uint32 height) {}
  };

  OV_CORE_EXPORT GraphicsAPI&
  g_graphicsAPI();

  using funCreateGraphicsAPI = GraphicsAPI * (*)();
}