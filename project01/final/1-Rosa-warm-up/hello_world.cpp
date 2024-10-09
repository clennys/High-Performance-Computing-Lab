#include <cstdlib>
#include <iostream>

int main() {

  const char *hostname = std::getenv("HOSTNAME");

  if (hostname) {
    std::cout << "This program is running on " << hostname << "." << std::endl;
  } else {
    std::cout << "Environment variable for HOSTNAME was not found on your maschine." << std::endl;
  }

  return 0;
}
