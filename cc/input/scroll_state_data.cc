// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "cc/input/scroll_state_data.h"

namespace cc {

ScrollStateData::ScrollStateData(double delta_x,
                                 double delta_y,
                                 int start_position_x,
                                 int start_position_y,
                                 bool should_propagate,
                                 bool delta_consumed_for_scroll_sequence,
                                 bool is_direct_manipulation)
    : delta_x(delta_x),
      delta_y(delta_y),
      start_position_x(start_position_x),
      start_position_y(start_position_y),
      should_propagate(should_propagate),
      current_native_scrolling_layer(nullptr),
      delta_consumed_for_scroll_sequence(delta_consumed_for_scroll_sequence),
      is_direct_manipulation(is_direct_manipulation),
      caused_scroll_x(false),
      caused_scroll_y(false) {}

ScrollStateData::ScrollStateData()
    : ScrollStateData(0, 0, 0, 0, true, false, false) {}

}  // namespace cc
