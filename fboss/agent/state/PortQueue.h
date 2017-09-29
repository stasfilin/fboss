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
  explicit PortQueueFields(int8_t id, cfg::StreamType streamType)
    : id(id),
      streamType(streamType) {}

  template<typename Fn>
  void forEachChild(Fn) {}

  folly::dynamic toFollyDynamic() const;
  static PortQueueFields fromFollyDynamic(const folly::dynamic& json);
  uint8_t id{0};
  cfg::StreamType streamType{cfg::StreamType::UNICAST};
  folly::Optional<int> priority{folly::none};
  folly::Optional<int> weight{folly::none};
};

/*
 * PortQueue defines the behaviour of the per port queues
 */
class PortQueue :
    public NodeBaseT<PortQueue, PortQueueFields> {
 public:
  explicit PortQueue(int8_t id, cfg::StreamType streamType);
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
           getFields()->priority == queue.getPriority() &&
           getFields()->weight == queue.getWeight();
  }

  uint8_t getID() const {
    return getFields()->id;
  }

  cfg::StreamType getStreamType() const {
    return getFields()->streamType;
  }

  folly::Optional<int> getPriority() const {
    return getFields()->priority;
  }

  folly::Optional<int> getWeight() const {
    return getFields()->weight;
  }

  void setPriority(int priority) {
    writableFields()->priority = priority;;
  }

  void setWeight(int weight) {
    writableFields()->weight = weight;
  }

 private:
  // Inherit the constructors required for clone()
  using NodeBaseT::NodeBaseT;
  friend class CloneAllocator;
};

}} // facebook::fboss