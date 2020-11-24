#include <iostream>

#include "flight_trip_database.h"

namespace flight_management
{

    bool FlightTripDatabase::RemoveTrip(const std::string &flight_number) noexcept
    {
        return flight_database_.erase(flight_number) == 1;
    }

    bool FlightTripDatabase::IsTripInDatabase(const std::string &flight_number) const noexcept
    {
        return flight_database_.count(flight_number) == 1;
    }

    bool FlightTripDatabase::UpdateFareByTrip(const std::string &flightNumber, std::uint32_t fare) noexcept
    {
        bool result{false};

        if (IsTripInDatabase(flightNumber))
        {
            flight_database_[flightNumber].SetAirFare(fare);
            result = true;
        }

        return result;
    }

    void FlightTripDatabase::DisplayAllTrips() const noexcept
    {
        std::cout << "Flight Details: "
                  << "\n";

        for (auto const &flight_detail : flight_database_)
        {
            std::cout << "Flight Number: " << flight_detail.first << " " << flight_detail.second << std::endl;
        }
        std::cout << "\n";
    }

    std::uint32_t FlightTripDatabase::FindAverageCostOfAllTrips() const noexcept
    {
        std::uint32_t average_cost{0U};

        for (auto const &flight_detail : flight_database_)
        {
            average_cost += flight_detail.second.GetAirFare();
        }

        return average_cost / flight_database_.size();
    }

    std::optional<FlightData> FlightTripDatabase::FindFlightsByNumber(const std::string &flight_number) const noexcept
    {
        auto flight_data_iterator = flight_database_.find(flight_number);
        return (flight_data_iterator != flight_database_.cend()) ? std::make_optional(flight_data_iterator->second) : std::nullopt;
    }

} // namespace flight_management
