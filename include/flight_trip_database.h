#ifndef FLIGHT_MANAGEMENT_INCLUDE_FLIGHT_TRIP_DATABASE_H
#define FLIGHT_MANAGEMENT_INCLUDE_FLIGHT_TRIP_DATABASE_H

#include <map>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <optional>
#include <climits>

#include "common_data.h"
#include "flight_data.h"
#include "helper_database.h"

namespace flight_management
{

    class FlightTripDatabase final
    {
        using FlightDatabase = std::map<std::string, FlightData>;
        using HelperFlightDatabase = HelperDatabase<
            BaseDataset<Filters::kOrigin>,
            BaseDataset<Filters::kDestination>,
            BaseDataset<Filters::kFlightOperator>>;

    public:
        template <typename FlightNumber, typename Origin, typename Destination, typename Operator, typename Fare>
        bool AddTrip(FlightNumber &&flight_number, Origin &&origin_city, Destination &&destination_city,
                     Operator &&flight_operator, Fare &&fare) noexcept
        {
            auto const &add_result = flight_database_.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(std::forward<FlightNumber>(flight_number)),
                std::forward_as_tuple(
                    std::forward<FlightNumber>(flight_number),
                    std::forward<Origin>(origin_city),
                    std::forward<Destination>(destination_city),
                    std::forward<Operator>(flight_operator),
                    std::forward<Fare>(fare)));

            bool result_origin_add{helper_database_.Add<Filters::kOrigin>(std::forward<Origin>(origin_city),
                                                                          std::forward<Origin>(flight_number))};
            bool result_origin_destination{helper_database_.Add<Filters::kDestination>(std::forward<Origin>(destination_city),
                                                                                       std::forward<Origin>(flight_number))};
            bool result_operator_add{helper_database_.Add<Filters::kOrigin>(std::forward<Operator>(flight_operator),
                                                                            std::forward<Origin>(flight_number))};

            return add_result.second && result_origin_add && result_origin_destination && result_operator_add;
        }

        template <typename Origin>
        std::vector<FlightData> FindFlightsByOriginCity(Origin &&origin_city) const noexcept
        {
            std::vector<std::string> flights_by_origin{
                helper_database_.GetValuesForKey<Filters::kOrigin>(std::forward<Origin>(origin_city))};

            std::vector<FlightData> flight_data;
            flight_data.reserve(flights_by_origin.size());

            std::for_each(flights_by_origin.begin(), flights_by_origin.end(),
                          [this, &flight_data](const auto &flight_number) {
                              if (IsTripInDatabase(flight_number))
                              {
                                  flight_data.emplace_back(flight_database_.at(flight_number));
                              }
                          });

            return flight_data;
        }

        template <typename Operator>
        std::uint32_t FindMaxFareByOperator(Operator &&flight_operator) const noexcept
        {
            std::uint32_t max_fare{0U};

            std::vector<std::string> flights_by_operator{
                helper_database_.GetValuesForKey<Filters::kOrigin>(std::forward<Operator>(flight_operator))};

            std::for_each(flights_by_operator.begin(), flights_by_operator.end(),
                          [this, &max_fare](const auto &flight_number) {
                              if (IsTripInDatabase(flight_number))
                              {
                                  max_fare = std::max(max_fare, flight_database_.at(flight_number).GetAirFare());
                              }
                          });

            return max_fare;
        }

        template <typename Origin, typename Destination>
        std::uint32_t FindMinFareBetweenCities(Origin &&origin_city, Destination &&destination_city) const noexcept
        {
            std::vector<std::string> flights_by_origin{
                helper_database_.GetValuesForKey<Filters::kOrigin>(std::forward<Origin>(origin_city))};

            std::vector<std::string> flights_by_destination{
                helper_database_.GetValuesForKey<Filters::kDestination>(std::forward<Destination>(destination_city))};

            std::vector<std::string> common_flights{};

            auto it{std::set_intersection(flights_by_origin.begin(), flights_by_origin.end(),
                                          flights_by_destination.begin(), flights_by_destination.end(),
                                          std::back_inserter(common_flights))};

            std::uint32_t min_fare{UINT_MAX};

            std::for_each(common_flights.begin(), common_flights.end(),
                          [this, &min_fare](const auto &flight_number) {
                              if (IsTripInDatabase(flight_number))
                              {
                                  min_fare = std::min(min_fare, flight_database_.at(flight_number).GetAirFare());
                              }
                          });

            return min_fare;
        }

        bool IsTripInDatabase(const std::string &flightNumber) const noexcept;
        bool RemoveTrip(const std::string &flightNumber) noexcept;
        bool UpdateFareByTrip(const std::string &flightNumber, std::uint32_t fare) noexcept;
        void DisplayAllTrips() const noexcept;
        std::uint32_t FindAverageCostOfAllTrips() const noexcept;
        std::optional<FlightData> FindFlightsByNumber(const std::string &flight_number) const noexcept;

    private:
        FlightDatabase flight_database_{};
        HelperFlightDatabase helper_database_{};
    };

} // namespace flight_management

#endif //FLIGHT_MANAGEMENT_INCLUDE_FLIGHT_TRIP_DATABASE_H
