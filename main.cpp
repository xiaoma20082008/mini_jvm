#include "Launcher.h"

/**
 * jvm启动程序
 * @param argc
 * @param argv
 * @return 0
 */
int main(int argc, char **argv) {
  jvm::Launcher launcher;
  launcher.Start();
  launcher.Close();
  return 0;
}
