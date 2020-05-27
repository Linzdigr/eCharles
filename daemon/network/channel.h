#include <netinet/in.h>
#include <string>

  using namespace std;

class channel {
  private:
    int server_fd;
    int new_socket;
    int valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
  public:
    channel(string addr);
    ~channel();
};
