#pragma once

#include <memory>
#include <vector>

#include "../Aoce.hpp"
#include "BaseLayer.hpp"

namespace aoce {

typedef std::shared_ptr<class PipeNode> PipeNodePtr;

// 对应layer,管理layer的连接
class ACOE_EXPORT PipeNode {
   private:
    /* data */
    friend class PipeGraph;
    BaseLayer* layer = nullptr;
    // 如果为true,当前节点不使用
    bool bInvisible = false;
    // 如果为true,包含这个节点之后子节点不使用
    bool bDisable = false;
    // 在graph的索引
    int32_t graphIndex = 0;
    std::weak_ptr<PipeNode> startNode;
    std::weak_ptr<PipeNode> endNode;
    // PipeNode* startNode = nullptr;

   public:
    PipeNode(BaseLayer* _layer);
    virtual ~PipeNode();

   public:
    void setVisable(bool bvisable);
    void setEnable(bool benable);
    inline BaseLayer* getLayer() { return layer; };
    inline int32_t getNodeIndex() { return graphIndex; };
    void setStartNode(PipeNodePtr node);
    void setEndNode(PipeNodePtr node);
    PipeNodePtr getStartNode();
    PipeNodePtr getEndNode();

   public:
    // 有一个隐藏的line关系,当前节点第一个输出连接下一节点的第一个输入
    PipeNodePtr addNode(BaseLayer* layer);

    PipeNodePtr addNode(ILayer* layer);

    PipeNodePtr addLine(PipeNodePtr to, int32_t formOut = 0, int32_t toIn = 0);
};

template <typename T>
struct TNodeLayer {
    ITLayer<T>* layer = nullptr;
    PipeNode* node = nullptr;
};

class PipeLine {
   public:
    PipeLine(){};
    ~PipeLine(){};

   public:
    int32_t fromNode = -1;
    int32_t fromOutIndex = -1;
    int32_t toNode = -1;
    int32_t toInIndex = -1;

   public:
    bool valid() {
        return toNode >= 0 && fromNode >= 0 && fromOutIndex >= 0 &&
               toInIndex >= 0;
    }

    inline bool operator==(const PipeLine& right) {
        return this->fromNode == right.fromNode &&
               this->fromOutIndex == right.fromOutIndex &&
               this->toNode == right.toNode &&
               this->toInIndex == right.toInIndex;
    }

    inline void operator=(const PipeLine& right) {
        this->fromNode = right.fromNode;
        this->fromOutIndex = right.fromOutIndex;
        this->toNode = right.toNode;
        this->toInIndex = right.toInIndex;
    }
};

typedef std::shared_ptr<PipeLine> PipeLinePtr;

}  // namespace aoce