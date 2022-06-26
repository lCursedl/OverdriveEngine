#include "ovSceneGraph.h"
#include <ovModel.h>
#include <ovCamera.h>
#include <ovRandom.h>

namespace ovEngineSDK {
  void
  SceneNode::setActor(SPtr<Actor> actor) {
    m_pActor = actor;
  }

  void
  SceneNode::addChildNode(SPtr<SceneNode> node) {
    node->m_pParent = weak_from_this();
    m_pChilds.insert(std::make_pair(Random::instance().GetRandom(0, 999), node));
  }

  void
  SceneNode::update(float delta) {
    m_pActor->update(delta);
    for (auto& childs : m_pChilds) {
      childs.second->update(delta);
    }
  }

  void
  SceneNode::render() {
    m_pActor->render();
    for (auto& childs : m_pChilds) {
      childs.second->render();
    }
  }

  void SceneNode::insertModels(Vector<SPtr<Model>>& modelsVector) {
    if (m_pActor) {
      for (auto& components : m_pActor->m_components) {
        if (COMPONENT_TYPE::kSTATIC_MESH == components->m_componentID) {
          modelsVector.push_back(static_pointer_cast<Model>(components));
        }
      }
      for (auto& childs : m_pChilds) {
        childs.second->insertModels(modelsVector);
      }
    }
  }

  SPtr<Camera> SceneNode::getCam() {
    SPtr<Camera> camera;
    if (m_pActor) {
      for (auto& components : m_pActor->m_components ) {
        if (COMPONENT_TYPE::kCAMERA == components->m_componentID) {
          camera = static_pointer_cast<Camera>(components);
        }
        if (camera.get()) {
          break;
        }
      }
      if (!camera.get()) {
        for (auto& childs : m_pChilds) {
          camera = childs.second->getCam();
          if (camera.get()) {
            break;
          }
        }
      }
    }
    return camera;
  }

  SPtr<Actor>
  SceneNode::getActorByName(const String& actorName) {
    if (actorName == m_pActor->getActorName()) {
      return m_pActor;
    }
    else {
      for (auto& node : m_pChilds) {
        auto tmpActor = node.second->getActorByName(actorName);
        if (nullptr != tmpActor) {
          if (actorName == tmpActor->getActorName()) {
            return tmpActor;
          }
        }
      }
    }
    return nullptr;
  }

  String
  SceneNode::getName() {
    if (m_name.empty()) {
      if (nullptr != m_pActor) {
        return m_pActor->getActorName();
      }
    }
    return m_name;
  }

  SceneGraph::SceneGraph() {
    m_pRoot = make_shared<SceneNode>();
    m_pRoot->m_name = "Root";
    m_numActors = 0;
  }

  void SceneGraph::addNode(SPtr<SceneNode> node) {
    if (nullptr != m_selectedNode) {
      m_selectedNode->addChildNode(node);
    }
    else {
      m_pRoot->addChildNode(node);
    } 
    m_numActors = nullptr != node->m_pActor ? m_numActors + 1 : m_numActors;
  }

  void
  SceneGraph::update(float delta) {
    deleteMarkedNodes();
    if (m_pRoot) {
      for (auto child : m_pRoot->m_pChilds) {
        child.second->update(delta);
      }
    }
  }

  void
  SceneGraph::render() {
    if (m_pRoot) {
      for (auto& child : m_pRoot->m_pChilds) {
        child.second->render();
      }
    }
  }

  SPtr<Camera>
  SceneGraph::getActiveCamera() {
    SPtr<Camera> camera;
    if (m_pRoot) {
      for (auto& node : m_pRoot->m_pChilds) {
        camera = node.second->getCam();
        if (camera.get()) {
          break;
        }
      }
    }
    return camera;
  }

  Vector<SPtr<Model>>
  SceneGraph::transferModels() {
    Vector<SPtr<Model>> modelVector;
    if (m_pRoot) {
      for (auto& node : m_pRoot->m_pChilds) {
        node.second->insertModels(modelVector);
      }
    }

    return modelVector;
  }

  uint32
  SceneGraph::getActorCount() {
    return m_numActors;
  }

  SPtr<Actor>
  SceneGraph::getActorByName(const String& actorName) {
    for (auto& child : m_pRoot->m_pChilds) {
      auto tmp = child.second->getActorByName(actorName);
      if (nullptr != tmp) {
        return tmp;
      }
    }
    return nullptr;
  }
  SPtr<SceneNode>
  SceneGraph::getRoot() {
    return m_pRoot;
  }

  SPtr<SceneNode>
  SceneGraph::createEmptyActor() {
    SPtr<SceneNode> empty = make_shared<SceneNode>();
    empty->setActor(make_shared<Actor>());
    empty->m_pActor->setActorName("Actor");
    return empty;
  }

  void
  SceneGraph::createEmptyAtNode(SPtr<SceneNode>& node) {
    SPtr<SceneNode> empty = make_shared<SceneNode>();
    empty->setActor(make_shared<Actor>());
    empty->m_pActor->setActorName("Actor");
    empty->m_pParent = node->weak_from_this();
    node->m_pChilds.insert(std::make_pair(Random::instance().GetRandom(0, 999), empty));
    m_numActors = nullptr != node->m_pActor ? m_numActors + 1 : m_numActors;
  }

  void
  SceneGraph::markNodeForDelete(int32 deleteID, SPtr<SceneNode> deleteNode) {
    m_deleteNodes.insert(std::make_pair(deleteID, deleteNode));
  }

  void
  SceneGraph::deleteMarkedNodes() {
    SPtr<SceneNode> parent;
    //Check for every node marked for deletion
    for (auto node : m_deleteNodes) {
      parent =  node.second->m_pParent.lock();
      //If node has children
      if (0 < node.second->m_pChilds.size()) {
        //Pass them to its parent
        
        for (auto& childs : node.second->m_pChilds) {
          childs.second->m_pParent = parent;
          parent->m_pChilds.insert(childs);
        }
        node.second->m_pChilds.clear();
      }
      //Find the marked node in its parent and delete it
      parent->m_pChilds[node.first].reset();
      parent->m_pChilds.erase(node.first);
    }
    m_deleteNodes.clear();
  }
}
