#include "evtolpwrsim.h"
#include "ui_evtolpwrsim.h"
#include "evtolvehicle.h"

#include <QStandardItemModel>
#include <QStringList>
#include <vector>
#include <random>
#include <queue>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <thread> // Added for threading
#include <mutex>  // Added for synchronization

eVtolPwrSim::eVtolPwrSim(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::eVtolPwrSim)
{
    ui->setupUi(this);
    on_runPwrSimBtn_clicked();
}

eVtolPwrSim::~eVtolPwrSim()
{
    delete ui;
}

void eVtolPwrSim::extracted(QStringList &rowData,
                            QList<QStandardItem *> &items) {
    for (const QString &text : rowData) {
        items << new QStandardItem(text);
    }
}

void eVtolPwrSim::on_runPwrSimBtn_clicked() {
    // Setup Data & UI

    // Main thread RNG for initial setup
    std::random_device rd;
    std::mt19937 gen(rd());

    // 1. Data Initialization
    std::vector<EVTOLVehicle::CompanySpecs> fleetSpecs = {
                                                          {"Alpha", 120, 320, 0.6, 1.6, 4, 0.25},
                                                          {"Bravo", 100, 100, 0.2, 1.5, 5, 0.10},
                                                          {"Charlie", 160, 220, 0.8, 2.2, 3, 0.05},
                                                          {"Delta", 90, 120, 0.62, 0.8, 2, 0.22},
                                                          {"Echo", 150, 30, 0.3, 5.8, 2, 0.61}};

    // Random Distribution of 20 vehicles
    std::vector<int> counts(5, 0);
    for (int i = 0; i < 20; ++i) {
        counts[std::uniform_int_distribution<>(0, 4)(gen)]++;
    }

    std::vector<EVTOLModel> vehicles;
    std::vector<EVTOLVehicle::Stats> stats(5);

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < counts[i]; ++j) {
            vehicles.push_back(EVTOLModel(fleetSpecs[i]));
        }
    }

    // Simulation Parameters
    double simTimeLimit = 3.0; // 3 hours

    // Shared Resources
    std::priority_queue<double, std::vector<double>, std::greater<double>> chargers;
    for (int i = 0; i < 3; ++i)
        chargers.push(0.0);

    // Mutexes for synchronization
    std::mutex chargerMutex;
    std::mutex statsMutex;

    // Thread Container
    std::vector<std::thread> threads;

    // Run Simulation
    for (unsigned long i = 0; i < vehicles.size(); ++i) {
        // Launch a thread for each vehicle
        threads.emplace_back([&, i]() {
            // Each thread needs its own Random Number Generator to be thread-safe
            std::random_device threadRd;
            std::mt19937 threadGen(threadRd());

            double currentTime = 0;
            int typeIdx = -1;
            for (int k = 0; k < 5; ++k)
                if (vehicles[i].specs.name == fleetSpecs[k].name)
                    typeIdx = k;

            // Local accumulation variables to minimize mutex locking duration
            double localFlightTime = 0;
            double localDistance = 0;
            double localPassMiles = 0;
            int localFlights = 0;
            int localFaults = 0;
            double localChargeTime = 0;
            int localCharges = 0;

            while (currentTime < simTimeLimit) {
                // --- FLIGHT PHASE ---
                double flightDuration = vehicles[i].getFlightDuration();
                double actualFlight =
                    std::min(flightDuration, simTimeLimit - currentTime);

                if (actualFlight <= 0)
                    break;

                double distance = actualFlight * vehicles[i].specs.cruiseSpeed;

                // Update Local Stats
                localFlightTime += actualFlight;
                localDistance += distance;
                localPassMiles += (distance * vehicles[i].specs.passengers);
                localFlights++;

                // Random Faults (Thread-safe generation)
                std::bernoulli_distribution faultDist(vehicles[i].specs.faultProb * actualFlight);
                if (faultDist(threadGen))
                    localFaults++;

                currentTime += actualFlight;

                // --- CHARGING PHASE ---
                if (currentTime < simTimeLimit) {
                    double startCharge = 0;
                    double endCharge = 0;

                    // LOCK: Accessing shared Charger Queue
                    {
                        std::lock_guard<std::mutex> lock(chargerMutex);
                        double chargerReadyTime = chargers.top();
                        chargers.pop();

                        startCharge = std::max(currentTime, chargerReadyTime);
                        endCharge = startCharge + vehicles[i].specs.chargeTime;

                        // Push the new availability time back immediately so others can see it
                        chargers.push(endCharge);
                    } // UNLOCK

                    double actualChargeTime =
                        std::min(endCharge, simTimeLimit) - startCharge;

                    if (actualChargeTime > 0) {
                        localChargeTime += actualChargeTime;
                        localCharges++;
                    }

                    currentTime = endCharge;
                }
            }

            // LOCK: Merging local thread stats into global stats vector
            {
                std::lock_guard<std::mutex> lock(statsMutex);
                stats[typeIdx].totalFlightTime += localFlightTime;
                stats[typeIdx].totalDistance += localDistance;
                stats[typeIdx].totalPassengerMiles += localPassMiles;
                stats[typeIdx].flightCount += localFlights;
                stats[typeIdx].totalFaults += localFaults;
                stats[typeIdx].totalChargeTime += localChargeTime;
                stats[typeIdx].chargeCount += localCharges;
            } // UNLOCK
        });
    }

    // Wait for all threads to finish
    for (auto &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    // Initialize the model (UI Logic - runs on main thread after simulation)
    if (ui->simReportHdr)
        ui->simReportHdr->setText("eVTOL Simulation Results (3 Hours, 20 Vehicles)");

    QStandardItemModel *simResultsModel = new QStandardItemModel(this);
    QStringList colHdrLbls;
    colHdrLbls << "Company" << "Count" << "Avg Flight" << "Avg Dist"
               << "Avg Charge"
               << "Faults" << "PAX Miles";
    simResultsModel->setHorizontalHeaderLabels(colHdrLbls);

    for (int i = 0; i < 5; ++i) {
        double avgFlight = stats[i].flightCount > 0
                               ? stats[i].totalFlightTime / stats[i].flightCount
                               : 0;
        double avgDist = stats[i].flightCount > 0
                             ? stats[i].totalDistance / stats[i].flightCount
                             : 0;
        double avgCharge = stats[i].chargeCount > 0
                               ? stats[i].totalChargeTime / stats[i].chargeCount
                               : 0;
        QStringList rowData;
        rowData << fleetSpecs[i].name.c_str() << QString::number(counts[i])
                << QString::number(avgFlight) << QString::number(avgDist)
                << QString::number(avgCharge)
                << QString::number(stats[i].totalFaults)
                << QString::number(stats[i].totalPassengerMiles);

        std::cout << std::left << std::setw(10) << fleetSpecs[i].name
                  << std::setw(8) << counts[i]
                  << std::setw(15) << avgFlight
                  << std::setw(15) << avgDist
                  << std::setw(12) << avgCharge
                  << std::setw(10) << stats[i].totalFaults
                  << stats[i].totalPassengerMiles << std::endl << std::flush;

        QList<QStandardItem *> items;
        extracted(rowData, items);
        simResultsModel->appendRow(items);
    }

    if(ui->resultsTblView) {
        ui->resultsTblView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->resultsTblView->setModel(simResultsModel);
    }
}
