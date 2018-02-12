#include <uWS/uWS.h>
#include <iostream>
#include <thread>
#include <algorithm>

#define LISTEN_PORT 3000

int main(int argc, char* argv[]) {
  (void) argc; (void) argv;
  std::vector<std::thread *> threads(std::thread::hardware_concurrency());
  std::transform(threads.begin(), threads.end(), threads.begin(), [](std::thread *t) {
    (void) t;
    return new std::thread([]() {
      uWS::Hub h;
      h.onMessage([](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
        std::cout << "Thread ID: " << std::this_thread::get_id() << ", Addr: " << ws->getAddress().address << ", Port: " << ws->getAddress().port << ": ";
        std::cout.write(message, length);
        std::cout.flush();
        ws->send(message, length, opCode);
      });
      if (!h.listen(LISTEN_PORT, nullptr, uS::ListenOptions::REUSE_PORT)) {
        std::cerr << "Failed to listen on port " <<  LISTEN_PORT << std::endl;
      }
      h.run();
    });
  });

  std::for_each(threads.begin(), threads.end(), [](std::thread *t) {
    t->join();
  });
}
