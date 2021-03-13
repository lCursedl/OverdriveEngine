#pragma once
#include <ovShader.h>

namespace ovEngineSDK {
  class ShaderProgram
  {
   public:
     ShaderProgram() = default;

     /**
     * @fn       virtual ~ShaderProgram()
     * @brief    Class destructor.
     *
     *	         If m_pVertexShader or m_pPixelShader was used, release memory
     *           before deleting object.
     */
     virtual ~ShaderProgram() = default;

     /**
     * @fn       CVertexShader* getVerteShader()
     * @brief    Obtains the pointer to the VertexShader member
     * @return	 CVertexshader pointer
     */
     VertexShader* getVertexShader() {
       return m_pVertexShader;
     }

     /**
     * @fn       CPixelShader* getPixelShader()
     * @brief    Obtains the pointer to the PixelShader member
     * @return   CPixelShader pointer
     */
     PixelShader* getPixelShader() {
       return m_pPixelShader;
     }

     /**
     * @fn       virtual void setVertexShader(CVertexShader* vertexShader)
     * @brief    Sets the received VS pointer as the one to store.
     * @param[in]vertexShader CVertexShader pointer to store
     */
     virtual void setVertexShader(VertexShader* vertexShader) {
       m_pVertexShader = vertexShader;
     }

     /**
     * @fn       virtual void setPixelShader(CPixelShader* pixelShader)
     * @brief    Sets the received PS pointer as the one to store.
     * @param[in]pixelShader CPixelShader pointer to store
     */
     virtual void setPixelShader(PixelShader* pixelShader) {
       m_pPixelShader = pixelShader;
     }

     /**
     * @fn       virtual void linkProgram()
     * @brief    Links the attached shaders to the program
     */
     virtual void linkProgram() {}

   protected:
    VertexShader* m_pVertexShader = nullptr;	/**< CVertexShader pointer */
    PixelShader* m_pPixelShader = nullptr;		/**< CPixelShader pointer */
  };
}