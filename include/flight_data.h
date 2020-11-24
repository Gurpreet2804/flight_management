#ifndef FLIGHT_MANAGEMENT_INCLUDE_FLIGHT_DATA_H
#define FLIGHT_MANAGEMENT_INCLUDE_FLIGHT_DATA_H

#include <cstdint>
#include <string>
#include <ostream>
#include <tuple>

namespace flight_management
{
class FlightData
{
public:
    FlightData(const std::string &flightNumber, const std::string &origin_city, const std::string &destination_city,
               const std::string &flight_operator, std::uint32_t fare) noexcept
        : flight_number_{flightNumber},
          origin_city_{origin_city},
          destination_city_{destination_city},
          operator_{flight_operator},
          air_fare_{fare}
    {
    }

    FlightData() noexcept = default;
    ~FlightData() noexcept = default;

    FlightData(const FlightData &) = default;
    FlightData &operator=(const FlightData &) noexcept = delete;

    FlightData(FlightData &&) noexcept = delete;
    FlightData &operator=(FlightData &&) noexcept = delete;

    void SetAirFare(const std::uint32_t fare) noexcept
    {
        air_fare_ = fare;
    }

    std::uint32_t GetAirFare() const noexcept
    {
        return air_fare_;
    }

    friend std::ostream &operator<<(std::ostream &out, const FlightData &flight_data)
    {
        out << " Flight Number: " << flight_data.flight_number_ << "\t"
            << " Origin City: " << flight_data.origin_city_ << "\t"
            << " Destination City: " << flight_data.destination_city_ << "\t"
            << " Flight Operator: " << flight_data.operator_ << "\t"
            << " Fare : " << flight_data.air_fare_;

        return out;
    }

    bool operator==(const FlightData &data) const
    {
        auto return_tie = [&](const FlightData &data) {
            return std::tie(data.flight_number_);
        };

        return return_tie(*this) == return_tie(data);
    }

private:
    std::string flight_number_{};
    std::string origin_city_{};
    std::string destination_city_{};
    std::string operator_{};
    std::uint32_t air_fare_{};
};

} // namespace flight_management

#endif //FLIGHT_MANAGEMENT_INCLUDE_FLIGHT_DATA_H
