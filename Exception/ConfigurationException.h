//
// Created by anowikowski on 16.12.15.
//

#ifndef TIN_IPTABLES_CONFIGURATIONEXCEPTION_H
#define TIN_IPTABLES_CONFIGURATIONEXCEPTION_H

#include <exception>

namespace exception
{
    namespace configuration
    {
        class configuration_exception : std::exception { };
        class config_field_not_set : configuration_exception { };
    }
}

#endif //TIN_IPTABLES_CONFIGURATIONEXCEPTION_H
