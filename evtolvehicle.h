#ifndef EVTOLVEHICLE_H
#define EVTOLVEHICLE_H

#include <string>

// Abstract Base Class
class EVTOLVehicle {
public:
    // Encapsulated Structures (Shared)
    struct CompanySpecs {
        std::string name;
        double cruiseSpeed;    // mph
        double batteryCap;     // kWh
        double chargeTime;     // hours
        double energyUse;      // kWh/mile
        int passengers;
        double faultProb;      // per hour
    };

    struct Stats {
        double totalFlightTime = 0;
        double totalDistance = 0;
        double totalChargeTime = 0;
        int totalFaults = 0;
        double totalPassengerMiles = 0;
        int flightCount = 0;
        int chargeCount = 0;
    };

    // Public Member Variables
    CompanySpecs specs;
    double nextAvailableTime = 0;
    bool isCharging = false;

    // Constructor
    EVTOLVehicle(CompanySpecs s);

    // Virtual Destructor
    virtual ~EVTOLVehicle();

    // Pure Virtual Method (Makes this class Abstract)
    virtual double getFlightDuration() const = 0;
};

// Concrete Subclass
class EVTOLModel : public EVTOLVehicle {
public:
    EVTOLModel(CompanySpecs s);

    // Concrete Implementation
    double getFlightDuration() const override;
};

#endif // EVTOLVEHICLE_H


