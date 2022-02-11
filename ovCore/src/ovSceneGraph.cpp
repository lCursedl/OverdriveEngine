#include "ovSceneGraph.h"
#include <ovModel.h>
#include <ovCamera.h>

namespace ovEngineSDK {
  void
  SceneNode::setActor(SPtr<Actor> actor) {
    m_pActor = actor;
  }

  void
  SceneNode::addChildNode(SPtr<SceneNode> node) {
    m_pChilds.push_back(node);
  }

  void
  SceneNode::update(float delta) {
    m_pActor->update(delta);
    SIZE_T numChilds = m_pChilds.size();
    for (SIZE_T i = 0; i < numChilds; ++i) {
      m_pChilds[i]->update(delta);
    }
  }

  void
  SceneNode::render() {
    m_pActor->render();
    SIZE_T numChilds = m_pChilds.size();
    for (SIZE_T i = 0; i < numChilds; ++i) {
      m_pChilds[i]->render();
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
        childs->insertModels(modelsVector);
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
          camera = childs->getCam();
          if (camera.get()) {
            break;
          }
        }
      }
    }
    return camera;
  }

  SceneGraph::SceneGraph() {
    m_pRoot = make_shared<SceneNode>();
  }

  void SceneGraph::addNode(SPtr<SceneNode> node) {
    m_pRoot->addChildNode(node);
  }

  void
  SceneGraph::update(float delta) {
    if (m_pRoot) {
      for (auto& child : m_pRoot->m_pChilds) {
        child->update(delta);
      }
    }
  }

  void
  SceneGraph::render() {
    if (m_pRoot) {
      for (auto& child : m_pRoot->m_pChilds) {
        child->render();
      }
    }
  }

  SPtr<Camera>
  SceneGraph::getActiveCamera() {
    SPtr<Camera> camera;
    if (m_pRoot) {
      for (auto& node : m_pRoot->m_pChilds) {
        camera = node->getCam();
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
        node->insertModels(modelVector);
      }
    }

    return modelVector;
  }
}