#include "ovSceneGraph.h"

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

  SceneGraph::SceneGraph() {
    m_pRoot = make_shared<SceneNode>();
  }

  void SceneGraph::addNode(SPtr<SceneNode> node) {
    m_pRoot->addChildNode(node);
  }

  void
  SceneGraph::update(float delta) {
    if (m_pRoot) {
      SIZE_T numChilds = m_pRoot->m_pChilds.size();
      for (uint32 i = 0; i < numChilds; ++i) {
        m_pRoot->m_pChilds[i]->update(delta);
      }
    }
  }

  void
  SceneGraph::render() {
    if (m_pRoot) {
      SIZE_T numChilds = m_pRoot->m_pChilds.size();
      for (uint32 i = 0; i < numChilds; ++i) {
        m_pRoot->m_pChilds[i]->render();
      }
    }
  }
}