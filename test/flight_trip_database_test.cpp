#include "gtest/gtest.h"

#include <vector>
#include <optional>

#include "flight_trip_database.h"

namespace flight_management
{

class FlightTripDataBaseTests : public ::testing::Test
{
public:
	void SetUp() override
	{
		database_.reset(new FlightTripDatabase);
	}

	bool Add(const std::string &flight_number, const std::string &origin_city,
			 const std::string &destination_city, const std::string &flight_operator, std::uint32_t fare) noexcept
	{
		return database_->AddTrip(flight_number, origin_city, destination_city, flight_operator, fare);
	}

	bool Remove(const std::string &flight_number) noexcept
	{
		return database_->RemoveTrip(flight_number);
	}

	bool Update(const std::string &flight_number, std::uint32_t fare) noexcept
	{
		return database_->UpdateFareByTrip(flight_number, fare);
	}

	std::optional<FlightData> Find(const std::string &flight_number) const noexcept
	{
		return database_->FindFlightsByNumber(flight_number);
	}

	void Display() const noexcept
	{
		database_->DisplayAllTrips();
	}

	std::vector<FlightData> FindFlights(const std::string &origin_city) const noexcept
	{
		return database_->FindFlightsByOriginCity(origin_city);
	}

	std::uint32_t AverageCost() const noexcept
	{
		return database_->FindAverageCostOfAllTrips();
	}

	std::uint32_t MaxFareByOperator(const std::string &flight_operator) const noexcept
	{
		return database_->FindMaxFareByOperator(flight_operator);
	}

	std::uint32_t MinFareBetweenCities(const std::string &origin, const std::string &destination) const noexcept
	{
		return database_->FindMinFareBetweenCities(origin, destination);
	}

private:
	std::unique_ptr<flight_management::FlightTripDatabase> database_;
};

TEST_F(FlightTripDataBaseTests, TestAddTripToDataBase)
{
	bool result{Add("AI-854", "Pune", "Delhi", "Air India", 5000)};
	EXPECT_TRUE(result);

	result = Add("SJ854", "Pune", "Delhi", "Spice", 5000);
	EXPECT_TRUE(result);
}

TEST_F(FlightTripDataBaseTests, TestAddTripToDataBaseCheckDuplicateInsert)
{
	bool result{Add("AI-855", "Mumbai", "Delhi", "Air India", 4500)};
	EXPECT_TRUE(result);

	result = Add("AI-855", "Mumbai", "Delhi", "Air India", 4500);
	EXPECT_FALSE(result);
};

TEST_F(FlightTripDataBaseTests, TestRemoveExisting)
{
	bool result{Add("AI-855", "Mumbai", "Delhi", "Air India", 4500)};
	if (result)
	{
		result = Remove("AI-855");
		EXPECT_TRUE(Find("AI-855") == std::nullopt);
	}
	EXPECT_TRUE(result);
};

TEST_F(FlightTripDataBaseTests, TestRemoveNotExisting)
{
	bool result{Add("AI-855", "Mumbai", "Delhi", "Air India", 4500)};
	if (result)
	{
		result = Remove("AI-854");
	}
	EXPECT_FALSE(result);
};

TEST_F(FlightTripDataBaseTests, TestUpdateFareByTripExisting)
{
	bool result{Add("AI-855", "Mumbai", "Delhi", "Air India", 4500)};
	Add("AI-856", "Mumbai", "Pune", "Air India", 4500);
	Add("SJ-900", "Mumbai", "Delhi", "Spice", 4550);
	if (result)
	{
		result = Update("AI-855", 5600);
		EXPECT_TRUE(Find("AI-855").value().GetAirFare() == 5600);
	}
	EXPECT_TRUE(result);

	if (result)
	{
		result = Update("SJ-900", 5500);
		EXPECT_TRUE(Find("SJ-900").value().GetAirFare() == 5500);
	}
	EXPECT_TRUE(result);
};

TEST_F(FlightTripDataBaseTests, TestUpdateFareByTripNotExisting)
{
	bool result{Update("AI-855", 5600)};
	EXPECT_FALSE(result);
};

TEST_F(FlightTripDataBaseTests, TestDisplayDatabase)
{
	Add("AI-855", "Mumbai", "Delhi", "Air India", 4500);
	Add("AI-856", "Pune", "Delhi", "Air India", 4600);
	Add("AI-857", "Delhi", "Chennai", "Air India", 4700);
	Add("AI-858", "Banglore", "Delhi", "Air India", 4800);
	Display();
}

TEST_F(FlightTripDataBaseTests, TestFindFlightsByOriginCity)
{
	const std::string origin_city{"Delhi"};
	std::uint8_t count_origin_city_to_be_matched_in_database{0U};

	std::vector<FlightData> input_data;

	Add("AI-855", origin_city, "Delhi", "Air India", 4500);
	input_data.emplace_back("AI-855", origin_city, "Delhi", "Air India", 4500);
	++count_origin_city_to_be_matched_in_database;

	Add("AI-856", "Pune", "Delhi", "Air India", 4600);

	Add("AI-857", origin_city, "Chennai", "Air India", 4700);
	input_data.emplace_back("AI-857", origin_city, "Chennai", "Air India", 4700);
	++count_origin_city_to_be_matched_in_database;

	Add("AI-858", origin_city, "Delhi", "Air India", 4800);
	input_data.emplace_back("AI-858", origin_city, "Delhi", "Air India", 4800);
	++count_origin_city_to_be_matched_in_database;

	auto output_data{FindFlights(origin_city)};

	EXPECT_TRUE(std::equal(output_data.begin(), output_data.end(), input_data.begin()));
	EXPECT_EQ(count_origin_city_to_be_matched_in_database, output_data.size());
}

TEST_F(FlightTripDataBaseTests, TestFindFlightByNumber)
{
	FlightData input{"AI-855", "Mumbai", "Delhi", "Air India", 4500};

	bool result{Add("AI-855", "Mumbai", "Delhi", "Air India", 4500)};
	if (result)
	{
		auto FlightData{Find("AI-855")};
		EXPECT_TRUE(input == FlightData);
		EXPECT_FALSE(std::nullopt == FlightData);
	}
}

TEST_F(FlightTripDataBaseTests, TestFindFlightByNumberDataNotExistsInDatabase)
{
	FlightData input_not_added_to_databse{"AI-855", "Mumbai", "Delhi", "Air India", 4500};
	auto FlightData{Find("AI-855")};
	EXPECT_FALSE(input_not_added_to_databse == FlightData);
	EXPECT_TRUE(std::nullopt == FlightData);
}

TEST_F(FlightTripDataBaseTests, TestFindAverageCostOfAllTrips)
{
	std::uint8_t count_in_database{0U};

	const std::uint32_t cost_first_flight = 4500U;
	Add("AI-855", "Pune", "Delhi", "Air India", cost_first_flight);
	++count_in_database;

	const std::uint32_t cost_second_flight = 4600U;
	Add("AI-856", "Pune", "Delhi", "Air India", cost_second_flight);
	++count_in_database;

	const std::uint32_t cost_third_flight = 3000U;
	Add("AI-857", "Mumbai", "Chennai", "Air India", cost_third_flight);
	++count_in_database;

	const std::uint32_t cost_fourth_flight = 4500U;
	Add("AI-858", "Banglore", "Delhi", "Air India", cost_fourth_flight);
	++count_in_database;

	std::uint32_t average_cost{
		(cost_first_flight + cost_second_flight + cost_third_flight + cost_fourth_flight) / count_in_database};

	const std::uint32_t average_cost_from_database{AverageCost()};

	EXPECT_TRUE(average_cost == average_cost_from_database);
};

TEST_F(FlightTripDataBaseTests, TestFindMaxFareByOperator)
{
	const std::uint32_t max_cost_operator_1 = 9900U;
	const std::string operator_1 = "Air India";

	Add("AI-855", "Pune", "Delhi", operator_1, 2345);
	Add("AI-856", "Pune", "Delhi", operator_1, 7646);
	Add("AI-858", "Banglore", "Delhi", operator_1, max_cost_operator_1);
	Add("AI-855", "Pune", "Delhi", operator_1, 8233);

	const std::uint32_t max_cost_operator_2 = 11000U;
	const std::string operator_2 = "Spice";

	Add("SJ-155", "Chennai", "Delhi", operator_2, 6545);
	Add("SJ-356", "Pune", "Delhi", operator_2, 7646);
	Add("SJ-458", "Banglore", "Delhi", operator_2, max_cost_operator_2);
	Add("SJ-855", "Pune", "Delhi", operator_2, 10023);

	std::uint32_t max_cost_operator_1_from_database{MaxFareByOperator(operator_1)};
	EXPECT_TRUE(max_cost_operator_1 == max_cost_operator_1_from_database);

	std::uint32_t max_cost_operator_2_from_database{MaxFareByOperator(operator_2)};
	EXPECT_TRUE(max_cost_operator_2 == max_cost_operator_2_from_database);
}

TEST_F(FlightTripDataBaseTests, TestFindMinFareBetweenCities)
{
	const std::string origin_city{"Delhi"};
	const std::string destination_city{"Pune"};

	const std::uint32_t minimum_fare = 1000U;

	Add("AI-855", "Delhi", "Pune", "Air India", 2345);
	Add("AI-856", "Pune", "Delhi", "Air India", 7646);
	Add("AI-858", "Banglore", "Delhi", "Air India", 5677);
	Add("AI-855", "Delhi", "Pune", "Air India", 8233);
	Add("SJ-155", "Pune", "Delhi", "Spice", 6545);

	Add("SJ-356", "Delhi", "Pune", "Spice", minimum_fare);

	Add("SJ-458", "Banglore", "Delhi", "Spice", 4567);
	Add("SJ-855", "Pune", "Delhi", "Indigo", 10023);
	Add("IG-855", "Pune", "Delhi", "Indigo", 4274);
	Add("IG-856", "Delhi", "Pune", "Indigo", 4699);
	Add("IG-892", "Delhi", "Banglore", "Indigo", 4699);

	std::uint32_t min_cost_from_database{MinFareBetweenCities(origin_city, destination_city)};
	EXPECT_TRUE(minimum_fare == min_cost_from_database);
}

} // namespace flight_management
