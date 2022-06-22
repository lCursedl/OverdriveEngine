#pragma once

#include <ovBaseOmniverse.h>

#include <filesystem>
#include <conio.h>

#include <OmniClient.h>
#include <OmniUsdLive.h>
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usdGeom/mesh.h>
#include <pxr/usd/usdGeom/metrics.h>
#include <pxr/base/gf/matrix4f.h>
#include <pxr/base/gf/vec2f.h>
#include <pxr/usd/usdUtils/pipeline.h>
#include <pxr/usd/usdUtils/sparseValueWriter.h>
#include <pxr/usd/usdShade/material.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/primRange.h>
#include <pxr/usd/usdShade/input.h>
#include <pxr/usd/usdShade/output.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usdShade/materialBindingAPI.h>
#include <pxr/usd/usdLux/distantLight.h>
#include <pxr/usd/usdLux/domeLight.h>
#include <pxr/usd/usdShade/shader.h>
#include <pxr/usd/usd/modelAPI.h>
#include <pxr/usd/usdGeom/primvarsAPI.h>

PXR_NAMESPACE_USING_DIRECTIVE

// Private tokens for building up SdfPaths. We recommend
  // constructing SdfPaths via tokens, as there is a performance
  // cost to constructing them directly via strings (effectively,
  // a table lookup per path element). Similarly, any API which
  // takes a token as input should use a predefined token
  // rather than one created on the fly from a string.
  TF_DEFINE_PRIVATE_TOKENS(
    _tokens,
    (box)
    (Light)
    (Looks)
    (Root)
    (Shader)
    (st)

    // These tokens will be reworked or replaced by the official MDL schema for USD.
    // https://developer.nvidia.com/usd/MDLschema
    (Material)
    ((_module, "module"))
    (name)
    (out)
    ((shaderId, "mdlMaterial"))
    (mdl)

    // Tokens used for USD Preview Surface
    (diffuseColor)
    (normal)
    (file)
    (result)
    (varname)
    (rgb)
    (RAW)
    (sRGB)
    (surface)
    (PrimST)
    (UsdPreviewSurface)
    ((UsdShaderId, "UsdPreviewSurface"))
    ((PrimStShaderId, "UsdPrimvarReader_float2"))
    (UsdUVTexture)
  );

namespace ovEngineSDK {

  #define HW_ARRAY_COUNT(array) (sizeof(array) / sizeof(array[0]))

  class OmniverseOV final : public BaseOmniverse
  {
   public:
    OmniverseOV() = default;
    ~OmniverseOV() = default;

    bool
    init()                                                            override;

    void
    update()                                                          override;

    void
    createUSD()                                                       override;

    bool
    loadUSD(const String& fileName)                                   override;

    bool
    connectFromOmni(const String& fileName)                           override;

    bool
    connectToOmni(const String& fileName)                             override;

    void
    createEmptyUSD(const String projectName);

    void
    liveEdit();

    void
    setTransformOp(Vector3 data,
                   OMNI_OP::E operation,
                   OMNI_PRECISION::E precision,
                   String omniPath)                                   override;
    
    bool
    getLiveEdit()                                                     override;

    String m_existingExample;
    String m_destinationPath = "omniverse://localhost/Users/Overdrive/";

   protected:

    const uint64 kOmniClientVersion = (uint64)OMNICLIENT_VERSION_MAJOR << 48 |
                                      (uint64)OMNICLIENT_VERSION_MINOR << 32 |
                                      (uint64)OMNICLIENT_VERSION_PATCH;
  };

  static void
  OmniClientConnectionStatusCallbackImpl(void* userData,
                                         const char* url,
                                         OmniClientConnectionStatus status);

  static void
  logCallback(const char* threadName,
              const char* component,
              OmniClientLogLevel level,
              const char* message);

  static String
  createOmniverseModel(const String& destinationPath);

  static void
  printConnectedUsername(const String& stageUrl);

  static void
  shutdownOmniverse();

  static void
  failNotify(const char *msg, const char *detail = nullptr, ...);

  static UsdStageRefPtr gStage;
  static std::mutex gLogMutex;
  static bool gOmniverseLoggingEnabled = false;

  extern "C" OV_PLUGIN_EXPORT BaseOmniverse*
  createOmniverse() {
    auto pOV = new OmniverseOV();
    return pOV;
  }

  class SetOp {
   public:
     SetOp(UsdGeomXformable& xForm,
           UsdGeomXformOp& op,
           UsdGeomXformOp::Type opType,
           const GfVec3d& value,
           const UsdGeomXformOp::Precision precision) {
       if (!op) {
         op = xForm.AddXformOp(opType, precision);
       }

       if (op.GetPrecision() == UsdGeomXformOp::Precision::PrecisionFloat)
         op.Set(GfVec3f(value));
       else
         op.Set(value);
     }
  };
}