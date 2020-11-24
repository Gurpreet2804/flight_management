#ifndef FLIGHT_MANAGEMENT_INCLUDE_BASE_DATA_SET_H
#define FLIGHT_MANAGEMENT_INCLUDE_BASE_DATA_SET_H

#include <map>
#include <vector>

#include "flight_data.h"
#include "common_data.h"

namespace flight_management
{

template <Filters filter, typename data_type = std::string>
class BaseDataset
{
public:
    using Type = data_type;
    using Container = std::multimap<std::string, Type>;

    bool Add(const std::string &key, const std::string &value) noexcept
    {
        auto const &result{container_.emplace(std::make_pair(key, value))};

        return result != container_.cend();
    }

    std::vector<Type> EqualRange(const std::string &key) const noexcept
    {
        auto range{container_.equal_range(key)};

        std::vector<Type> values{};

        for (auto element = range.first; element != range.second; ++element)
        {
            values.push_back(element->second);
        }

        return values;
    }

private:
    Container container_;
};

} // namespace flight_management

#endif //FLIGHT_MANAGEMENT_INCLUDE_BASE_DATA_SET_H
