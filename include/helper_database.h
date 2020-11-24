#ifndef FLIGHT_MANAGEMENT_INCLUDE_HELPER_DATABASE_H
#define FLIGHT_MANAGEMENT_INCLUDE_HELPER_DATABASE_H

#include <map>

#include "base_dataset.h"
#include "common_data.h"

namespace flight_management
{

template <typename... Args>
class HelperDatabase : public Args...
{
public:
    template <Filters filter, class... Data>
    bool Add(Data &&... data) noexcept
    {
        return BaseDataset<filter>::Add(std::forward<Data>(data)...);
    };

    template <Filters filter, class... Data>
    std::vector<std::string> GetValuesForKey(Data &&... data) const noexcept
    {
        return BaseDataset<filter>::EqualRange(std::forward<Data>(data)...);
    }
};

} // namespace flight_management

#endif //FLIGHT_MANAGEMENT_INCLUDE_HELPER_DATABASE_H
