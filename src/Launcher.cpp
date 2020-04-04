//
// Created by machunxiao on 2020/4/4.
//

#include "Launcher.h"
#include "spdlog/spdlog.h"

namespace jvm {
void Launcher::Start() { spdlog::info("Launcher Start"); }
void Launcher::Close() { spdlog::info("Launcher Close"); }
} // namespace jvm