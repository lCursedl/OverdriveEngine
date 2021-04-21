#pragma once
#include <ovPrerequisitesOGL.h>
#include <ovInputLayout.h>

namespace ovEngineSDK {
  /**
  * @class       OGLInputLayout
  * @brief       Class which holds info for a D3D11's input layout.
  */
  class OGLInputLayout final : public InputLayout
  {
   public:

    /**
    * @fn        ~OGLInputLayout()
    * @brief     Class destructor.
    *
    *            If VAO was used, release memory before deleting object.
    */
    ~OGLInputLayout();

   protected:

    /** 
    * @fn        OGLInputLayout()
    * @brief     Class constructor.
    */
    OGLInputLayout() = default;

  private:

    uint32 m_vao = 0;   /**< unsigned int for OpenGL VAO index */

    friend class OGLGraphicsAPI;
  };
}