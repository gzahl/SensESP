#ifndef _http_H_
#define _http_H_

#include <ESPAsyncWebServer.h>

#include <functional>

/**
 * @brief Handles external interactions with the device via the web interface.
 */
class HTTPServer {
 public:
  HTTPServer(std::function<void()> reset_device, std::function<void(AsyncWebServerRequest* request)> handle_info);
  ~HTTPServer() { delete server; }
  void enable() { server->begin(); }
  void handle_not_found(AsyncWebServerRequest* request);
  void handle_config(AsyncWebServerRequest* request);
  void handle_device_reset(AsyncWebServerRequest* request);
  void handle_device_restart(AsyncWebServerRequest* request);

 private:
  AsyncWebServer* server;
  std::function<void()> reset_device;
  std::function<void(AsyncWebServerRequest* request)> handle_info;
  void handle_config_list(AsyncWebServerRequest* request);
};

#endif
