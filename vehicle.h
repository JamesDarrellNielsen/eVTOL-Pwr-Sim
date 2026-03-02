#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

struct CompanySpecs {
    std::string name;
    double cruiseSpeed, batteryCap, chargeTime, energyUse, faultProb;
    int passengers;
};

class EVTOLVehicle {
public:
    CompanySpecs specs;
    int typeIdx;
    double nextEventTime = 0.0;
    bool isCharging = false;

    EVTOLVehicle(CompanySpecs s, int idx) : specs(s), typeIdx(idx) {}

    double getFlightDuration() const {
        return specs.batteryCap / (specs.cruiseSpeed * specs.energyUse);
    }
};
#endif // VEHICLE_H
