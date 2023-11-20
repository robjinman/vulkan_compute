#include <cstdlib>
#include "application.hpp"

int main() {
  ApplicationPtr app = createApplication();
  
  app->start();

  return EXIT_SUCCESS;
}

