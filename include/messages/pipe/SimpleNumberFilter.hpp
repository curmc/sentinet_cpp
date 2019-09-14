/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : SimpleNumberFilter
 * @created     : Friday Sep 13, 2019 16:06:40 MDT
 */

#ifndef SIMPLENUMBERFILTER_HPP

#define SIMPLENUMBERFILTER_HPP

// C++ Includes

// Local Includes
#include "core/messages/pipe/FilterInterface.hpp"
#include "messages/common/SimpleNumber.hpp"

namespace messages {
namespace pipes {

class SimpleNumberFilter : public FilterInterface
{
    public:
        SimpleNumberFilter ();
        virtual ~SimpleNumberFilter () = default;

        std::string convert(const std::string& incomming_message);

    private:
        std::string message_str;
        std::unique_ptr<::numbers::simple_number> message;
};


}
}
#endif /* end of include guard SIMPLENUMBERFILTER_HPP */

