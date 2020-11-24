#ifndef FLIGHT_MANAGEMENT_INCLUDE_COMMON_DATA_H
#define FLIGHT_MANAGEMENT_INCLUDE_COMMON_DATA_H

#include <cstdint>

namespace flight_management
{

enum class Filters : unsigned int
{
    kOrigin = 0U,
    kDestination = 1U,
    kFlightOperator = 2U
};

} // namespace flight_management

#endif //FLIGHT_MANAGEMENT_INCLUDE_COMMON_DATA_H