/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : ProxyInterface
 * @created     : Friday Sep 13, 2019 16:26:31 MDT
 */

#ifndef PROXYINTERFACE_HPP

#define PROXYINTERFACE_HPP

// C++ Includes
#include <string>
#include <future>
#include <map>

// Local Includes
#include "core/utils/logging.hpp"

class ProxyInterface
{
    public:
        ProxyInterface () = delete;

        ProxyInterface(const std::string&, std::future<void>, const std::string&, const std::string&);

        virtual ~ProxyInterface ();

        /**
         * @brief Start the message pipe
         *
         * @param A period run everytime spin is called, zero by default
         *
         * @return Status of exit
         */
        bool start(const std::chrono::microseconds t = std::chrono::microseconds(0));

        /**
         * @brief Clean stop
         *
         * @return Status
         */
        bool stop();

        /**
         * @brief Kill it
         *
         * @return Status
         */
        bool kill();

        /**
         * @brief Signal the thing
         *
         * @param signal_val The value of the signal
         *
         * @return Status code
         */
        int signal(const int32_t signal_val);

        /**
         * @brief Register a signal function
         *
         * @note There are a few signals allocated that do stuff like stop and start that you cannot register
         *
         * @param signal_val The value of the signal
         * @param std::function What to do on the signal
         *
         * @return The status code
         */
        bool register_signal(const int32_t signal_val, std::function<int(void)>);

    private:
        enum signals {
          CLEAN_STOP = 0,
          KILL = 1,
          START = 2,
        };

        /**
         * @brief Executed every clock tick
         *
         * @return Status of clock tick
         */
        virtual bool __spin__() = 0;

        /**
         * @brief Any specific start up stuff to do
         *
         * @return 
         */
        virtual bool __start__() = 0;

        /**
         * @brief Stop the processes
         *
         * @return Status of stop
         */
        virtual bool __stop__() = 0;

        /**
         * @brief Get the type of proxy
         *
         * @return The type
         */
        virtual std::string __get_type__() = 0;

        std::map<int, std::function<int(void)>> signal_table;
        const std::string id;
        std::future<void> exit_signal;
        const std::string frontend;
        const std::string backend;

};

#endif /* end of include guard PROXYINTERFACE_HPP */

