// This file is part of CAF, the C++ Actor Framework. See the file LICENSE in
// the main distribution directory for license terms and copyright or visit
// https://github.com/actor-framework/actor-framework/blob/master/LICENSE.

#include "caf/test/runnable.hpp"

#include "caf/test/block_type.hpp"
#include "caf/test/context.hpp"
#include "caf/test/scenario.hpp"
#include "caf/test/scope.hpp"
#include "caf/test/test.hpp"

namespace caf::test {

runnable::~runnable() {
  // nop
}

void runnable::run() {
  switch (root_type_) {
    case block_type::scenario:
      if (auto guard = ctx_->get<scenario>(0, description_, loc_)->commit()) {
        do_run();
        return;
      }
      CAF_RAISE_ERROR(std::logic_error,
                      "failed to select  the root block for the scenario");
      break;
    case block_type::test:
      if (auto guard = ctx_->get<test>(0, description_, loc_)->commit()) {
        do_run();
        return;
      }
      CAF_RAISE_ERROR(std::logic_error,
                      "failed to select the root block for the test");
      break;
    default:
      CAF_RAISE_ERROR(std::logic_error, "invalid root type");
  }
}

void runnable::check(bool value, const detail::source_location& location) {
  if (value) {
    reporter::instance->pass(location);
    return;
  }
  reporter::instance->fail("should be true", location);
}

block& runnable::current_block() {
  if (ctx_->call_stack.empty())
    CAF_RAISE_ERROR(std::logic_error, "no current block");
  return *ctx_->call_stack.back();
}

} // namespace caf::test