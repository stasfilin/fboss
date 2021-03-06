/*
 *  Copyright (c) 2004-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#pragma once

#include "fboss/agent/gen-cpp2/switch_config_types.h"
#include "fboss/agent/FbossError.h"
#include "fboss/agent/types.h"
#include "fboss/agent/state/NodeBase.h"

#include <string>
#include <utility>
#include <folly/Optional.h>

namespace facebook { namespace fboss {

struct PortQueueFields {
  explicit PortQueueFields(uint8_t id) : id(id) {}

  template<typename Fn>
  void forEachChild(Fn) {}

  folly::dynamic toFollyDynamic() const;
  static PortQueueFields fromFollyDynamic(const folly::dynamic& json);
  uint8_t id{0};
  cfg::StreamType streamType{cfg::StreamType::UNICAST};
  folly::Optional<int> weight{folly::none};
  folly::Optional<int> reservedBytes{folly::none};
  folly::Optional<cfg::MMUScalingFactor> scalingFactor{folly::none};
};

/*
 * PortQueue defines the behaviour of the per port queues
 */
class PortQueue :
    public NodeBaseT<PortQueue, PortQueueFields> {
 public:
  explicit PortQueue(uint8_t id);
  static std::shared_ptr<PortQueue>
  fromFollyDynamic(const folly::dynamic& json) {
    const auto& fields = PortQueueFields::fromFollyDynamic(json);
    return std::make_shared<PortQueue>(fields);
  }

  static std::shared_ptr<PortQueue>
  fromJson(const folly::fbstring& jsonStr) {
    return fromFollyDynamic(folly::parseJson(jsonStr));
  }

  folly::dynamic toFollyDynamic() const override {
    return getFields()->toFollyDynamic();
  }

  bool operator==(const PortQueue& queue) {
    return getFields()->id == queue.getID() &&
           getFields()->streamType == queue.getStreamType() &&
           getFields()->weight == queue.getWeight() &&
           getFields()->reservedBytes == queue.getReservedBytes() &&
           getFields()->scalingFactor == queue.getScalingFactor();
  }

  uint8_t getID() const {
    return getFields()->id;
  }

  void setStreamType(cfg::StreamType type) {
    writableFields()->streamType = type;
  }

  cfg::StreamType getStreamType() const {
    return getFields()->streamType;
  }

  folly::Optional<int> getWeight() const {
    return getFields()->weight;
  }

  void setWeight(int weight) {
    writableFields()->weight = weight;
  }

  folly::Optional<int> getReservedBytes() const {
    return getFields()->reservedBytes;
  }

  void setReservedBytes(int reservedBytes) {
    writableFields()->reservedBytes = reservedBytes;
  }

  folly::Optional<cfg::MMUScalingFactor> getScalingFactor() const {
    return getFields()->scalingFactor;
  }

  void setScalingFactor(cfg::MMUScalingFactor scalingFactor) {
    writableFields()->scalingFactor = scalingFactor;
  }

 private:
  // Inherit the constructors required for clone()
  using NodeBaseT::NodeBaseT;
  friend class CloneAllocator;
};

}} // facebook::fboss
