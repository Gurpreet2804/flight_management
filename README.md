## Problem Statement

In this problem you are supposed to design a solution for Flight Management as described below. Let's not consider date & time of flight journey to make the problem simpler for now. Let's assume flight number is unique in entire database

#### Design a class Flight as follows:

   * Attributes
      * Flight number
      * Origin City
      * Destination city
      * Operator (AirIndia, Indigo etc)
      * Air Fare
      * Suitable constructor(s)
      * Any additional member functions as required

#### Design another class to maintain Flight database
   * Use suitable container(s) to store all flight details
   * Constructor, Destructor if required
   * Operations
      * AddTrip with given attributes
      * RemoveTrip by flight number
      * Update air fare by flight number
      * Find flight details by number
      * Find all flights departing from a particular city
      * Find all flights by a specific operator
      * Find average air fare from all trips
      * Find minimum fare between two cities
      * Find maximum fare by a particular operator
      * Update the air fare for all flights by a specific operator, say decrease by 10%

### To Compile:

#### 1) Clone gtest in lib as:
   - mkdir lib
   - cd lib
   - git clone https://github.com/google/googletest/
   - cd ..

#### 2) Create build folder:
   - mkdir build
   - cd build
   - cmake ..
   - make

#### 3) Run target from build folder:
   - ./flight_management_test
