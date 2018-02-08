#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main()
{
  uWS::Hub h;

  PID pid, pid2;
  // Initializing the pid variable
  pid.init(0.22, 0.0001, 10.0);
  pid2.init(0.8, 0.00001, 0.1);

  h.onMessage([&pid, &pid2](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
//          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = deg2rad(std::stod(j[1]["steering_angle"].get<std::string>()));
          double steer_value;
          double throttle = 0.4;

          pid.updateError(cte);
          pid2.updateError(cte);
          double err = -1 * pid.totalError();
          double err2 = fabs(pid.totalError());
          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */

          if (err > 0.65) err = 0.65;  // limiting
          else if (err < -0.65) err = -0.65;

/*
          if (fabs(cte) < 0.8)
          {
            steer_value = (angle * 5.0 + err) / 6.0;  // weighted avg.
            //throttle += 0.1; // accelerate
          }
          else if (fabs(cte) < 1.4)
          {
            steer_value = (angle * 1.5 + err) / 2.5;  // weighted avg.
            //throttle -= 0.1; // accelerate
          }
          else
          {
            //if (fabs(cte) > 2.4 && speed > 25)
            //  throttle = -0.3; // decelerate
            //else
            //  throttle = 0.3; // accelerate

            steer_value = (angle + err) / 2.0;  // weighted avg.
          }
*/
          steer_value = (err * fabs(cte) + angle) / (fabs(cte) + 1);  // weighted avg.

//          if (fabs(cte) > 0.85)
//            steer_value = (angle + err) / 2.0;  // weighted avg.
//          else
//            steer_value = (angle * 5.0 + err) / 6.0;  // weighted avg.

          if (steer_value > 0.45) steer_value = 0.45;  // limiting
          else if (steer_value < -0.45) steer_value = -0.45;

//          steer_value = (angle * 3.0 + err) / 4.0;  // weighted avg.

//          if (err2 > 3.0) err2 = 3.0;  // limiting
//          if (speed > 22 || fabs(steer_value) > 0.6)
//            throttle = (2.0 - err2) * 0.8;
//          else
//            throttle = (3.0 - err2) * 0.8;
            throttle = 0.9 - err2; // 0.9

          // DEBUG
          std::cout << "CTE: " << cte << " Steering Value: " << angle << " -> " << steer_value << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = throttle;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
