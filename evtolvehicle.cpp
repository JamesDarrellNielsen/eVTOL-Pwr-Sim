#include "evtolvehicle.h"

// --- EVTOLVehicle (Abstract Base) Implementation ---

EVTOLVehicle::EVTOLVehicle(CompanySpecs s) : specs(s) {
    // Base constructor initializes common data
}

EVTOLVehicle::~EVTOLVehicle() {
}

// Note: getFlightDuration is pure virtual, so no implementation here.

// --- EVTOLModel (Concrete Subclass) Implementation ---

EVTOLModel::EVTOLModel(CompanySpecs s) : EVTOLVehicle(s) {
}

double EVTOLModel::getFlightDuration() const {
    // Time = Capacity / (Speed * kWh/mile)
    if (specs.cruiseSpeed <= 0 || specs.energyUse <= 0) return 0.0;

    return specs.batteryCap / (specs.cruiseSpeed * specs.energyUse);
}


