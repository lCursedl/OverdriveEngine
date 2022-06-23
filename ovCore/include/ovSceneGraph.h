#pragma once
#include <ovPrerequisitesCore.h>
#include <ovModule.h>
#include <ovActor.h>

namespace ovEngineSDK {
  
  class Model;
  class Camera;

  class OV_CORE_EXPORT SceneNode : public std::enable_shared_from_this<SceneNode>
  {
   public:
    SceneNode() = default;
    ~SceneNode() = default;

    String m_name;
    WPtr<SceneNode> m_pParent;
    Vector<SPtr<SceneNode>> m_pChilds;
    SPtr<Actor> m_pActor;

    void
    setActor(SPtr<Actor> actor);
    void
    addChildNode(SPtr<SceneNode> node);
    void
    update(float delta);
    void
    render();

    void
    insertModels(Vector<SPtr<Model>>& modelsVector);

    SPtr<Camera>
    getCam();

    SPtr<Actor>
    getActorByName(const String& actorName);

    String
    getName();

    bool m_selected = false;

  };

  class OV_CORE_EXPORT SceneGraph : public Module<SceneGraph>
  {
   public:
    SceneGraph();
    virtual ~SceneGraph() = default;

    void
    addNode(SPtr<SceneNode> node);
    void
    update(float delta);
    void
    render();

    SPtr<Camera>
    getActiveCamera();

    Vector<SPtr<Model>>
    transferModels();

    uint32
    getActorCount();

    SPtr<Actor>
    getActorByName(const String& actorName);

    SPtr<SceneNode>
    getRoot();

    SPtr<SceneNode>
    createEmptyActor();

    SPtr<SceneNode>
    m_selectedNode;    

    private:
    SPtr<SceneNode> m_pRoot;
    uint32 m_numActors;
  };
}