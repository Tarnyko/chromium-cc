// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_OUTPUT_BSP_TREE_H_
#define CC_OUTPUT_BSP_TREE_H_

#include <deque>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "cc/output/bsp_compare_result.h"
#include "cc/quads/draw_polygon.h"

namespace cc {

struct BspNode {
  // This represents the splitting plane.
  scoped_ptr<DrawPolygon> node_data;
  // This represents any coplanar geometry we found while building the BSP.
  std::vector<scoped_ptr<DrawPolygon>> coplanars_front;
  std::vector<scoped_ptr<DrawPolygon>> coplanars_back;

  scoped_ptr<BspNode> back_child;
  scoped_ptr<BspNode> front_child;

  explicit BspNode(scoped_ptr<DrawPolygon> data);
  ~BspNode();
};

class CC_EXPORT BspTree {
 public:
  explicit BspTree(std::deque<scoped_ptr<DrawPolygon>>* list);
  scoped_ptr<BspNode>& root() { return root_; }

  template <typename ActionHandlerType>
  void TraverseWithActionHandler(ActionHandlerType* action_handler) const {
    if (root_) {
      WalkInOrderRecursion<ActionHandlerType>(action_handler, root_.get());
    }
  }

  ~BspTree();

 private:
  scoped_ptr<BspNode> root_;

  void FromList(std::vector<scoped_ptr<DrawPolygon>>* list);
  void BuildTree(BspNode* node, std::deque<scoped_ptr<DrawPolygon>>* data);

  template <typename ActionHandlerType>
  void WalkInOrderAction(ActionHandlerType* action_handler,
                         DrawPolygon* item) const {
    (*action_handler)(item);
  }

  template <typename ActionHandlerType>
  void WalkInOrderVisitNodes(
      ActionHandlerType* action_handler,
      const BspNode* node,
      const BspNode* first_child,
      const BspNode* second_child,
      const std::vector<scoped_ptr<DrawPolygon>>& first_coplanars,
      const std::vector<scoped_ptr<DrawPolygon>>& second_coplanars) const {
    if (first_child) {
      WalkInOrderRecursion(action_handler, first_child);
    }
    for (size_t i = 0; i < first_coplanars.size(); i++) {
      WalkInOrderAction(action_handler, first_coplanars[i].get());
    }
    WalkInOrderAction(action_handler, node->node_data.get());
    for (size_t i = 0; i < second_coplanars.size(); i++) {
      WalkInOrderAction(action_handler, second_coplanars[i].get());
    }
    if (second_child) {
      WalkInOrderRecursion(action_handler, second_child);
    }
  }

  template <typename ActionHandlerType>
  void WalkInOrderRecursion(ActionHandlerType* action_handler,
                            const BspNode* node) const {
    // If our view is in front of the the polygon
    // in this node then walk back then front.
    if (GetCameraPositionRelative(*(node->node_data)) == BSP_FRONT) {
      WalkInOrderVisitNodes<ActionHandlerType>(action_handler,
                                               node,
                                               node->back_child.get(),
                                               node->front_child.get(),
                                               node->coplanars_front,
                                               node->coplanars_back);
    } else {
      WalkInOrderVisitNodes<ActionHandlerType>(action_handler,
                                               node,
                                               node->front_child.get(),
                                               node->back_child.get(),
                                               node->coplanars_back,
                                               node->coplanars_front);
    }
  }

  // Returns whether or not nodeA is on one or the other side of nodeB,
  // coplanar, or whether it crosses nodeB's plane and needs to be split
  static BspCompareResult GetNodePositionRelative(const DrawPolygon& node_a,
                                                  const DrawPolygon& node_b);
  // Returns whether or not our viewer is in front of or behind the plane
  // defined by this polygon/node
  static BspCompareResult GetCameraPositionRelative(const DrawPolygon& node);
};

}  // namespace cc

#endif  // CC_OUTPUT_BSP_TREE_H_
