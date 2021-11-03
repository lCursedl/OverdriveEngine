#pragma once
#include <ovShader.h>
#include <ovPrerequisitesDX.h>

namespace ovEngineSDK {
  /**
   * @class       DXComputeShader
   * @brief       Class which holds info for a D3D11's compute shader.
   */
  class DXComputeShader final : public ComputeShader
  {
   public:

    /**
    * @fn        ~DXComputeShader()
    * @brief     Class destructor.
    *            If m_cs or m_blob was used, release memory before deleting object.
    */
    ~DXComputeShader();

   protected:

    /**
    * @fn        DXComputeShader()
    * @brief     Class constructor.
    *            Sets m_cs & m_blob as nullptr.
    */
    DXComputeShader();

   private:
    ID3D11ComputeShader* m_cs;        /**< ID3D11ComputeShader pointer */
    ID3DBlob* m_blob;               /**< ID3D11Blob pointer */

    friend class DXGraphicsAPI;
  };
}