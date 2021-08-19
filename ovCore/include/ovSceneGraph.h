#pragma once
#include <ovPrerequisitesCore.h>
#include <ovModule.h>
#include <ovActor.h>

namespace ovEngineSDK {
  
  class Model;

  class OV_CORE_EXPORT SceneNode
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

    Vector<SPtr<Model>>
    transferModels();

    private:
    SPtr<SceneNode> m_pRoot;
  };
}