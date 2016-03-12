// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_TEST_SOLID_COLOR_CONTENT_LAYER_CLIENT_H_
#define CC_TEST_SOLID_COLOR_CONTENT_LAYER_CLIENT_H_

#include "base/compiler_specific.h"
#include "cc/layers/content_layer_client.h"
#include "third_party/skia/include/core/SkColor.h"

namespace cc {

class SolidColorContentLayerClient : public ContentLayerClient {
 public:
  explicit SolidColorContentLayerClient(SkColor color, gfx::Size size)
      : color_(color), size_(size) {}

  gfx::Rect PaintableRegion() override;

  // ContentLayerClient implementation.
  scoped_refptr<DisplayItemList> PaintContentsToDisplayList(
      PaintingControlSetting painting_control) override;
  bool FillsBoundsCompletely() const override;
  size_t GetApproximateUnsharedMemoryUsage() const override;

 private:
  SkColor color_;
  gfx::Size size_;
};

}  // namespace cc

#endif  // CC_TEST_SOLID_COLOR_CONTENT_LAYER_CLIENT_H_
