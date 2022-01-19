#include "ovOmniverse.h"

namespace ovEngineSDK {

  bool
  OmniverseOV::init() {
    omniClientSetLogCallback(logCallback);
    omniClientSetLogLevel(eOmniClientLogLevel_Debug);
    if (!omniClientInitialize(kOmniClientVersion)) {
      return false;
    }
    omniClientRegisterConnectionStatusCallback(nullptr,
                                               OmniClientConnectionStatusCallbackImpl);
    omniUsdLiveSetDefaultEnabled(false);
    createUSD();
    return true;
  }

  void
  OmniverseOV::update() {
    
  }

  void
  OmniverseOV::createUSD() {
    if (m_existingExample.empty()) {
      const String stageUrl = createOmniverseModel(m_destinationPath);
      printConnectedUsername(stageUrl);
      shutdownOmniverse();
    }
  }

  static void
  OmniClientConnectionStatusCallbackImpl(void* userData,
                                         const char* url,
                                         OmniClientConnectionStatus status) {
    {
      std::unique_lock<std::mutex> lk(gLogMutex);
      std::cout << "Connection Status: " << omniClientGetConnectionStatusString(status) << "[" << url << "]\n";
    }
    if (status == eOmniClientConnectionStatus_ConnectError) {
      std::cout << "[ERROR] Failed connection, exiting\n";
    }
  }

  static void
  logCallback(const char* threadName,
              const char* component,
              OmniClientLogLevel level,
              const char* message) {
    std::unique_lock<std::mutex> lk(gLogMutex);
    if (gOmniverseLoggingEnabled) {
      puts(message);
    }
  }

  static String
  createOmniverseModel(const String& destinationPath) {
    String stageUrl = destinationPath + "/ovEngine.usd";

    {
      std::unique_lock<std::mutex> lk(gLogMutex);
      std::cout << "Waiting for " << stageUrl << " to delete...\n";
    }
    omniClientWait(omniClientDelete(stageUrl.c_str(), nullptr, nullptr));
    {
      std::unique_lock<std::mutex> lk();
      std::cout << "finished\n";
    }

    gStage = UsdStage::CreateNew(stageUrl);
    if (!gStage) {
      failNotify("Failure to create model in Omniverse", stageUrl.c_str());
      return String();
    }

    {
      std::unique_lock<std::mutex> lk(gLogMutex);
      std::cout << "New stage created: " << stageUrl << "\n";
    }

    UsdGeomSetStageUpAxis(gStage, UsdGeomTokens->y);
    return stageUrl;
  }

  static void
  failNotify(const char *msg, const char *detail, ...) {
    std::unique_lock<std::mutex> lk(gLogMutex);
    fprintf(stderr, "%s\n", msg);
    if (detail != nullptr) {
      fprintf(stderr, "%s\n", detail);
    }
  }

  static void
  printConnectedUsername(const String& stageUrl) {
   String userName("_none_");
   omniClientWait(omniClientGetServerInfo(stageUrl.c_str(), &userName, [](void* userData, OmniClientResult result, struct OmniClientServerInfo const * info) noexcept
   {
    String* userName = static_cast<String*>(userData);
    if (userData && userName && info && info->username) {
      userName->assign(info->username);
    }
   }));
   {
     std::unique_lock<std::mutex> lk(gLogMutex);
     std::cout << "Connected username: " << userName << "\n";
   }
  }

  static void
  shutdownOmniverse() {
   omniUsdLiveWaitForPendingUpdates();
   gStage.Reset();
   omniClientShutdown();
  }
}