#include "ovOmniverse.h"
#include <ovSceneGraph.h>
#include <ovModel.h>

namespace ovEngineSDK {
  
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
      auto& scene = SceneGraph::instance();
      Vector<SPtr<Model>>sceneModels = scene.transferModels();
      int32 modelIndex = 0;
      for (auto& model : sceneModels) {
        int32 meshIndex = 0;
        String modelName("_model" + std::to_string(modelIndex));
        ++modelIndex;
        SdfPath rootPrimPath = SdfPath::AbsoluteRootPath().AppendChild(_tokens->Root);
        SdfPath modelPath = rootPrimPath.AppendChild(TfToken(modelName));
        UsdGeomMesh mesh = UsdGeomMesh::Define(gStage, modelPath);
        if (!mesh) {
          OutputDebugStringA("Couldn't create UsdGeoMesh\n");
          return;
        }
        int32 count = model->getMeshCount();
        //Model has single mesh
        if (count > 0 && count < 2) {
          
        }
        //Model has multiple meshes
        else if (count > 1) {
          for (auto& inmesh : model->m_meshes) {
            String meshName("_mesh" + std::to_string(meshIndex));
            ++meshIndex;
            SdfPath meshPath = modelPath.AppendChild(TfToken(meshName));
            UsdGeomMesh subMesh = UsdGeomMesh::Define(gStage, meshPath);
            if (!subMesh) {
              OutputDebugStringA("Couldn't create a sub UsdGeoMesh.\n");
            }
            subMesh.CreateOrientationAttr(VtValue(UsdGeomTokens->rightHanded));
            Vector<Vector3> tempVertices;
            Vector<uint32> tempIndices;
            Vector<Vector3> tempNormals;
            Vector<Vector2> tempUVs;
            inmesh->getMeshInfo(tempVertices, tempIndices, tempNormals, tempUVs);

            VtArray<GfVec3f> points;
            points.resize(tempVertices.size());
            int32 numVertices = tempVertices.size();
            for (int32 i = 0; i < numVertices; ++i) {
              points[i] = GfVec3f(tempVertices[i].x, tempVertices[i].y, tempVertices[i].z);
            }
            subMesh.CreatePointsAttr(VtValue(points));

            int32 numIndices = tempIndices.size();
            VtArray<int32> vecIndices;
            vecIndices.resize(numIndices);
            for (int32 i = 0; i < numIndices; ++i) {
              vecIndices[i] = tempIndices[i];
            }
            subMesh.CreateFaceVertexIndicesAttr(VtValue(vecIndices));

            VtArray<GfVec3f> meshNormals;
            meshNormals.resize(numVertices);
            for (int32 i = 0; i < numVertices; ++i) {
              meshNormals[i] = GfVec3f(tempNormals[i].x, tempNormals[i].y, tempNormals[i].z);
            }
            subMesh.CreateNormalsAttr(VtValue(meshNormals));

            VtArray<int32> faceVertexCounts;
            faceVertexCounts.resize(numIndices / 3);
            std::fill(faceVertexCounts.begin(), faceVertexCounts.end(), 3);
            subMesh.CreateFaceVertexCountsAttr(VtValue(faceVertexCounts));

            UsdGeomPrimvar attr2 = subMesh.CreatePrimvar(_tokens->st, SdfValueTypeNames->TexCoord2fArray);
            {
              int32 uv_count = tempUVs.size();
              VtVec2fArray valueArray;
              valueArray.resize(uv_count);
              for (int32 i = 0; i < uv_count; ++i) {
                valueArray[i].Set(tempUVs[i].x, tempUVs[i].y);
              }
              bool status = attr2.Set(valueArray);
            }
            attr2.SetInterpolation(UsdGeomTokens->vertex);
          }        
        }
      }
      gStage->Save();
      omniUsdLiveProcess();
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