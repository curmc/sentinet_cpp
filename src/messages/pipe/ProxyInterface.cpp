/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : ProxyInterface
 * @created     : Friday Sep 13, 2019 16:53:20 MDT
 */

#include "core/messages/pipe/ProxyInterface.hpp"

// Quick note, exit_signal has a constructor:
// future(future &&other) which takes an rvalue that ensures exit_signal has a non_shareable state
// Instead, we set the value to exit_signal
ProxyInterface::ProxyInterface(const std::string& id_, std::future<void> futureObj, const std::string& frontend_, const std::string& backend_)
  : id(id_), 
    exit_signal(std::move(futureObj)), 
    frontend(frontend_), 
    backend(backend_) 
{
    register_signal(CLEAN_STOP, [this] (void) -> int {
      this->stop();
      return CLEAN_STOP;
    });

    register_signal(KILL, [this] (void) -> int {
      this->kill();
      return KILL;
    });

    register_signal(START, [this] (void) -> int {
      this->start();
      return START;
    });
}

ProxyInterface::~ProxyInterface()
{
  bool ret;
  ret = stop();
  if(!ret)
    LOG_WARN("CLEAN STOP FAILED");
    ret = kill();
  if(!ret)
    LOG_ERROR("KILL SIGNAL FAILED");
}

bool ProxyInterface::start(const std::chrono::microseconds t) {
  LOG_INFO("Proxy Interface Starting up of type %s and id %s", id.c_str(), __get_type__().c_str());

  // Get the starting time
  auto start = std::chrono::steady_clock::now();

  // A global status value that we will return
  auto status = true;

  // If zero period, the steady clock call is one extra operation that we don't need
  auto period = (t == std::chrono::microseconds(0));

  // Excecution of the program
  while(exit_signal.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout) {

    // Grab the current starting time
    start = std::chrono::steady_clock::now();

    // Spin once and record that as status
    if(!(status = __spin__()))
      LOG_WARN("Spin returned false on proxy of type %s and id %s", id.c_str(), __get_type__().c_str());

    // If there's a period, sleep, otherwise, we're just wasting operations
    if(period) {
      std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - start;
      if(elapsed > std::chrono::milliseconds(0))
        std::this_thread::sleep_for(elapsed);
    }
  }

  LOG_INFO("Proxy Interface Quitting of type %s and id %s", id.c_str(), __get_type__().c_str());

  return status;
}

bool ProxyInterface::stop() {
  LOG_INFO("Attempting to stop proxy interface of type %s and id %s", __get_type__().c_str(), id.c_str());
  if(__stop__())
    return true;
  LOG_INFO("Error, couldn't stop proxy interface of type %s and id %s", __get_type__().c_str(), id.c_str());
  return false;
}

bool ProxyInterface::kill() {
  // Try to stop one more time
  LOG_INFO("Attempting to stop one more time");
  stop();
  
  // Killing a proxy is unhealthy anyways, maybe this is punishment,
  // We are going to wait 10 seconds in case stop has an asynchronous call (which it shouldn't,
  // but some developers are stupid)
  std::this_thread::sleep_for(std::chrono::seconds(10));

  LOG_ERROR("Couldn't Kill all resources");

  LOG_INFO("Killing proxy interface of type %s and id %s", __get_type__().c_str(), id.c_str());
  std::terminate();
  LOG_ERROR("I have no idea why you're here. Something broke");

  return false;
}

int ProxyInterface::signal(const int32_t signal_val) {
  auto found = signal_table.find(signal_val);
  if(found == signal_table.end()){
    LOG_ERROR("Invalid signal (%d) recieved from Proxy of type %s and id %s", signal_val, __get_type__().c_str(), id.c_str());
    return -1;
  }
  LOG_INFO("Proxy of type %s and id %s recieved sucessful signal %d", __get_type__().c_str(), id.c_str(), signal_val);
  return signal_table[signal_val]();
}

bool ProxyInterface::register_signal(const int32_t signal_val, std::function<int(void)> func) {
  auto found = signal_table.find(signal_val);

  if(found == signal_table.end()) {
    LOG_ERROR("Proxy of type %s and id %s failed to register signal %d, signal already exists", __get_type__().c_str(), id.c_str(), signal_val);
    return false;
  }

  // Return the emplace function emplace status
  if(signal_table.emplace(signal_val, func).second){
    LOG_ERROR("Proxy of type %s and id %s failed to emplace signal for some reason, it doesn't exist in the map though", __get_type__().c_str(), id.c_str());
    return false;
  }
  return true;
}
