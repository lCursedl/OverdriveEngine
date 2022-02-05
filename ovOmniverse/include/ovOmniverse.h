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
#include <pxr/usd/usdGeom/primvar.h>
#include <pxr/usd/usdShade/input.h>
#include <pxr/usd/usdShade/output.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usdShade/materialBindingAPI.h>
#include <pxr/usd/usdLux/distantLight.h>
#include <pxr/usd/usdLux/domeLight.h>
#include <pxr/usd/usdShade/shader.h>
#include <pxr/usd/usd/modelAPI.h>

namespace ovEngineSDK {

  PXR_NAMESPACE_USING_DIRECTIVE

  class OmniverseOV final : public BaseOmniverse
  {
   public:
    OmniverseOV() = default;
    ~OmniverseOV() = default;

    bool
    init()       override;

    void
    update()     override;

    void
    createUSD()  override;

    bool
    loadUSD(const String& fileName)                                    override;

    String m_existingExample;
    String m_destinationPath = "omniverse://localhost/Users/Overdrive";

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

}