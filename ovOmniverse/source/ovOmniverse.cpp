#include "ovOmniverse.h"
#include <ovSceneGraph.h>
#include <ovModel.h>
#include <ovGraphicsAPI.h>

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

  SPtr<Mesh> createMeshfromGeomMesh(const UsdGeomMesh& geomMesh);

  static void
    createEmptyFolder(const String& emptyFolderPath) {
      {
        std::unique_lock<std::mutex> lk(gLogMutex);
        std::cout << "Waiting to create a new folder: " << emptyFolderPath << " ...";
      }

      OmniClientResult localResult = Count_eOmniClientResult;

      omniClientWait(omniClientCreateFolder(emptyFolderPath.c_str(),
        &localResult,
        [](void* userData,
          OmniClientResult result) noexcept {
            auto returnResult = static_cast<OmniClientResult*>(userData);
            *returnResult = result;
        }));

      {
        std::unique_lock<std::mutex> lk(gLogMutex);
        std::cout << "finished [" << omniClientGetResultString(localResult) << "]\n";
      }
  }

  static Vector<UsdPrim>
  findGeomMesh() {
    omniUsdLiveWaitForPendingUpdates();
    Vector<UsdPrim> geomMeshes;
    auto range = gStage->Traverse();
    for (const auto & node : range) {
      if ("Root" == node.GetParent().GetName()) {
        if (node.IsA<UsdGeomMesh>()) {
          geomMeshes.push_back(node);
        }
      }      
    }
    return geomMeshes;
  }

  bool
  OmniverseOV::init() {
    omniClientSetLogCallback(logCallback);
    omniClientSetLogLevel(eOmniClientLogLevel_Debug);
    if (!omniClientInitialize(kOmniClientVersion)) {
      return false;
    }
    omniClientRegisterConnectionStatusCallback(nullptr,
                                               OmniClientConnectionStatusCallbackImpl);
    omniUsdLiveSetDefaultEnabled(m_liveEditActive);
    return true;
  }

  void
  OmniverseOV::update() {
    if (m_liveEditActive) {
      liveEdit(findGeomMesh());
    }
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
      gStage->Save();
      omniUsdLiveProcess();
      shutdownOmniverse();
    }
  }

  bool
  OmniverseOV::loadUSD(const String& fileName) {
    gStage = UsdStage::Open(fileName);
    if (!gStage) {
      failNotify("Failure to open stage in Omniverse: ", fileName.c_str());
      return false;
    }

    {
      std::unique_lock<std::mutex> lk(gLogMutex);
      std::cout << "Stage is not Y-up so live xform edits will be incorrect. Stage is "
                << UsdGeomGetStageUpAxis(gStage) << "-up.\n";
    }

    auto range = gStage->Traverse();
    for (const auto& node : range) {
      if (node.IsA<UsdGeomMesh>()) {
        UsdPrim parent = node.GetParent();
        std::cout << parent.GetName() << "\n";
        if ("Root" == parent.GetName()) {
          std::unique_lock<std::mutex> lk(gLogMutex);
          std::cout << "Found UsdGeoMesh " << node.GetName() << ".\n";
          
          UsdGeomMesh geoMesh(node);
          VtArray<GfVec3f> points;
          UsdAttribute meshPointAttrib = geoMesh.GetPointsAttr();
          meshPointAttrib.Get(&points);
          
          SPtr<Model> tempModel = make_shared<Model>();
          tempModel->m_textureSampler = g_graphicsAPI().createSamplerState(
            FILTER_LEVEL::FILTER_LINEAR,
            FILTER_LEVEL::FILTER_LINEAR,
            FILTER_LEVEL::FILTER_LINEAR,
            false,
            0,
            WRAPPING::WRAP,
            COMPARISON::NEVER);

          //GeomMesh is container and has children
          if (0 == points.size() && !node.GetAllChildren().empty()) {
            for (const auto& childNode : node.GetAllChildren()) {
              if (childNode.IsA<UsdGeomMesh>()) {
                UsdGeomMesh childGM(childNode);
                tempModel->m_meshes.push_back(createMeshfromGeomMesh(childGM));
              }
            }
          }
          //GeomMesh is single and contains the mesh info
          else {
            tempModel->m_meshes.push_back(createMeshfromGeomMesh(geoMesh));
          }

          SPtr<Actor>myActor = make_shared<Actor>(node.GetName().GetString());
          myActor->addComponent(tempModel);

          SPtr<SceneNode>myNode = make_shared<SceneNode>();
          myNode->setActor(myActor);

          SceneGraph::instance().addNode(myNode);
        }
      }
    }
    return true;
  }

  bool
  OmniverseOV::connectFromOmni(const String& fileName) {
    m_liveEditActive = true;
    omniUsdLiveSetDefaultEnabled(m_liveEditActive);
    return loadUSD(fileName);
  }

  bool
  OmniverseOV::connectToOmni(const String& fileName) {
    m_liveEditActive = true;
    omniUsdLiveSetDefaultEnabled(m_liveEditActive);
    if (m_existingExample.empty()) {
      if (0 != SceneGraph::instance().getActorCount()) {
        createEmptyUSD(fileName);
        createUSD();
        return true;
      }
      else {
        createEmptyUSD(fileName);
        createOmniverseModel(m_destinationPath);
        return true;
      }
    }
    return false;
  }

  void
  OmniverseOV::createEmptyUSD(const String projectName) {
    createEmptyFolder(m_destinationPath + projectName);
    m_existingExample = m_destinationPath + projectName;
    m_destinationPath = m_existingExample;
  }

  void
  OmniverseOV::liveEdit(Vector<UsdPrim> primVector) {
    omniUsdLiveWaitForPendingUpdates();
    auto& sgraph = SceneGraph::instance();
    for (auto& prim : primVector) {
      UsdGeomMesh geomMesh(prim);
      UsdGeomXformable xForm = geomMesh;

      UsdGeomXformOp translateOp;
      UsdGeomXformOp rotateOp;
      UsdGeomXformOp scaleOp;

      GfVec3d position(0);
      GfVec3f rotZYX(0);
      GfVec3f scale(1);

      bool resetXformStack = false;
      Vector<UsdGeomXformOp> xFormOps = xForm.GetOrderedXformOps(&resetXformStack);

      for (SIZE_T i = 0; i < xFormOps.size(); ++i) {
        switch (xFormOps[i].GetOpType()) {
        case UsdGeomXformOp::TypeTranslate: {
            translateOp = xFormOps[i];
            translateOp.Get(&position);
            break;
          }
        case UsdGeomXformOp::TypeRotateZYX: {
            rotateOp = xFormOps[i];
            translateOp.Get(&rotZYX);
            break;
          }
        case UsdGeomXformOp::TypeScale: {
            scaleOp = xFormOps[i];
            scaleOp.Get(&scale);
            break;
          }
        }
      }
      //Get corresponding actor in scenegraph      
      auto tmpActor = sgraph.getActorByName(prim.GetName().GetString());
      tmpActor->m_localPosition = { (float)position.GetArray()[0],
                                    (float)position.GetArray()[1],
                                    (float)position.GetArray()[2] };
      tmpActor->m_localScale = { (float)scale.GetArray()[0],
                                 (float)scale.GetArray()[1],
                                 (float)scale.GetArray()[2] };
    }

    gStage->Save();
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
    String stageUrl = destinationPath;

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
      failNotify("Failed to create model in Omniverse ", stageUrl.c_str());
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

  SPtr<Mesh> createMeshfromGeomMesh(const UsdGeomMesh& geomMesh) {

    VtArray<GfVec3f> points;
    VtArray<GfVec3f> normals;
    VtVec2fArray uvs;
    VtArray<int32> indices;
    
    UsdAttribute meshPointAttrib = geomMesh.GetPointsAttr();
    meshPointAttrib.Get(&points);

    UsdAttribute meshIndexAttrib = geomMesh.GetFaceVertexIndicesAttr();
    meshIndexAttrib.Get(&indices);

    UsdAttribute meshNormalAttrib = geomMesh.GetNormalsAttr();
    meshNormalAttrib.Get(&normals);

    //UsdGeomPrimvarsAPI primApi = UsdGeomPrimvarsAPI::Get(gStage, geomMesh.GetPath());
    //UsdGeomPrimvar meshUVAttrib = primApi.GetPrimvar(_tokens->st);
    ////UsdAttribute meshUVAttrib = geomMesh.GetPrimvar(_tokens->st);
    //meshUVAttrib.Get(&uvs);

    uint32 numPoints = points.size();
    uint32 numIndices = indices.size();

    Vector<MeshVertex> meshVertices;
    Vector<uint32> meshIndices;

    for (int32 i = 0; i < numPoints; ++i) {
      MeshVertex v;
      v.Position = Vector3(points[i].data()[0], points[i].data()[1], points[i].data()[2]);
      v.Normal = Vector3(normals[i].data()[0], normals[i].data()[1], normals[i].data()[2]);
      v.Tangent = Vector3(1.0f, 1.0f, 1.0f);
      v.Bitangent = Vector3(1.0f, 1.0f, 1.0f);
      v.TexCoords = Vector2(0.f, 1.f);
      meshVertices.push_back(v);
    }

    for (int32 i = 0; i < numIndices; ++i) {
      meshIndices.push_back(indices[i]);
    }

    Vector<MeshTexture> emptytextures;

    MeshTexture mTexture;
    mTexture.TextureMesh = g_graphicsAPI().createTextureFromFile(
                           "resources/textures/missingtexture.png");

    emptytextures.push_back(mTexture);
    emptytextures.push_back(mTexture);
    emptytextures.push_back(mTexture);
    emptytextures.push_back(mTexture);

    return make_shared<Mesh>(meshVertices, meshIndices, emptytextures, nullptr);
  }
}