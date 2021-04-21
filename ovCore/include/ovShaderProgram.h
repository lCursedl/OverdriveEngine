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
     * @fn       VertexShader* getVerteShader()
     * @brief    Obtains the pointer to the VertexShader member
     * @return	 Vertexshader pointer
     */
     VertexShader* getVertexShader() {
       return m_pVertexShader;
     }

     /**
     * @fn       PixelShader* getPixelShader()
     * @brief    Obtains the pointer to the PixelShader member
     * @return   PixelShader pointer
     */
     PixelShader* getPixelShader() {
       return m_pPixelShader;
     }

     /**
     * @fn       virtual void setVertexShader(VertexShader* vertexShader)
     * @brief    Sets the received VS pointer as the one to store.
     * @param[in]vertexShader VertexShader pointer to store
     */
     virtual void setVertexShader(VertexShader* vertexShader) {
       m_pVertexShader = vertexShader;
     }

     /**
     * @fn       virtual void setPixelShader(PixelShader* pixelShader)
     * @brief    Sets the received PS pointer as the one to store.
     * @param[in]pixelShader PixelShader pointer to store
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
    VertexShader* m_pVertexShader = nullptr;	/**< VertexShader pointer */
    PixelShader* m_pPixelShader = nullptr;		/**< PixelShader pointer */
  };
}