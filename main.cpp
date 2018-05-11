#include <uWS/uWS.h>
#include <iostream>
#include <thread>
#include <algorithm>

#define LISTEN_PORT 3000

int main(int argc, char* argv[]) {
  (void) argc; (void) argv;
  uS::TLS::Context tls_context = uS::TLS::createContext("./cert.pem", "./pkey.pem");
  if (!tls_context) {
    std::cerr << "Cannot create tls context" << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<std::thread *> threads(std::thread::hardware_concurrency());
  std::transform(threads.begin(), threads.end(), threads.begin(), [&](std::thread *t) {
    (void) t;
    return new std::thread([&]() -> int {
      uWS::Hub h;
      uWS::Group<uWS::SERVER> *ssl_group = h.createGroup<uWS::SERVER>(uWS::PERMESSAGE_DEFLATE);
      ssl_group->onMessage([](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
        std::cout << "Thread ID: " << std::this_thread::get_id() << ", Addr: " << ws->getAddress().address << ", Port: " << ws->getAddress().port << ": ";
        std::cout.write(message, length);
        std::cout << std::endl;
        ws->send(message, length, opCode);
      });
      if (!h.listen(LISTEN_PORT, tls_context, uS::ListenOptions::REUSE_PORT, ssl_group)) {
        std::cerr << "Failed to listen to Websocket Secure on port " <<  LISTEN_PORT << std::endl;
        return EXIT_FAILURE;
      }
      h.run();
      return EXIT_SUCCESS;
    });
  });

  std::for_each(threads.begin(), threads.end(), [](std::thread *t) {
    t->join();
  });
}
