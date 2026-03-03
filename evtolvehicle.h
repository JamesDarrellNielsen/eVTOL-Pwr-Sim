#ifndef EVTOLVEHICLE_H    // Header Guard: Prevents circular inclusion errors
#define EVTOLVEHICLE_H    // during the compilation process.

#include <string>         // Included for the std::string name in CompanySpecs

/* --- EVTOLVehicle Abstract Base Class ---
 * Defines the Core Data Model for the simulation. It uses C++ OOP concepts
 * such as Abstract Base Class, Pure Virtual Function, and Nested Structs.
 * CONCEPT: An Abstract Base Class (ABC) cannot be instantiated on its own.
 * It serves as a "Blue Print" or "Contract" for all vehicle types in the system.
 * It defines the shared attributes and mandatory behaviors (methods).
 */
class EVTOLVehicle {
public:
    // --- ENCAPSULATED STRUCTURES (Nested Structs) ---
    /*
     * These structs are "Nested" inside the class to keep the namespace clean.
     * They define the 'Inputs' (CompanySpecs) and 'Outputs' (Stats) for a vehicle.
     */
    struct CompanySpecs {
        std::string name;      // Manufacturer Name (e.g., "Alpha")
        double cruiseSpeed;    // Speed in mph
        double batteryCap;     // Total energy storage in kWh
        double chargeTime;     // Time required for a full charge in hours
        double energyUse;      // Consumption rate in kWh per mile
        int passengers;        // Seating capacity
        double faultProb;      // Probability of a mechanical fault per flight hour
    };

    /*
     * Stats: Used to track the cumulative data for a specific company
     * during the 3-hour simulation. Note the default values (= 0).
     */
    struct Stats {
        double totalFlightTime = 0;
        double totalDistance = 0;
        double totalChargeTime = 0;
        int totalFaults = 0;
        double totalPassengerMiles = 0;
        int flightCount = 0;
        int chargeCount = 0;
    };

    // --- MEMBER VARIABLES ---
    CompanySpecs specs;        // Stores the specific performance constants for this vehicle
    double nextAvailableTime = 0; // Internal clock used to track when a vehicle is done charging/flying
    bool isCharging = false;   // State flag to determine if the vehicle is currently at a charger

    // --- CONSTRUCTOR ---
    // Initializes the vehicle with a specific set of company specifications.
    EVTOLVehicle(CompanySpecs s);

    // --- VIRTUAL DESTRUCTOR ---
    /*
     * CONCEPT: Polymorphism. When you delete a pointer to a base class (EVTOLVehicle*)
     * that actually points to a child (EVTOLModel), the 'virtual' keyword ensures
     * the child's destructor is called first, preventing memory leaks.
     */
    virtual ~EVTOLVehicle();

    // --- PURE VIRTUAL METHOD ---
    /*
     * SYNTAX: '= 0' makes this a Pure Virtual Function.
     * CONCEPT: This function has no implementation in the Base class.
     * It FORCE any class inheriting from EVTOLVehicle to implement its own
     * version of getFlightDuration(). This is what makes the class "Abstract."
     */
    virtual double getFlightDuration() const = 0;
};

// --- CONCRETE SUBCLASS ---
/*
 * CONCEPT: Inheritance. EVTOLModel is a "Concrete" class because it
 * fulfills the contract by implementing the pure virtual method.
 */
class EVTOLModel : public EVTOLVehicle {
public:
    // Constructor: Passes the specs up to the Base class constructor
    EVTOLModel(CompanySpecs s);

    // --- CONCRETE IMPLEMENTATION ---
    /*
     * KEYWORD: override. This tells the compiler "I am specifically
     * implementing the virtual function from my parent."
     * This is where the actual math (Battery / (Speed * EnergyUse)) happens.
     */
    double getFlightDuration() const override;
};

#endif // EVTOLVEHICLE_H
