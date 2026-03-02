#include <QtTest>
#include "../evtolvehicle.h"

class TestEVTOL : public QObject
{
    Q_OBJECT

private slots:
    // --- SMOKE TESTS (Sanity Checks) ---

    void initTestCase() {
        qDebug() << "Initializing QMake Test Suite...";
    }

    void smokeTest_Instantiation() {
        // Can we create the object without crashing?
        EVTOLVehicle::CompanySpecs specs = {"Smoke", 100, 100, 1, 1, 1, 0};
        EVTOLModel vehicle(specs);

        QCOMPARE(vehicle.specs.name, std::string("Smoke"));
        QVERIFY(vehicle.specs.batteryCap > 0);
    }

    // --- UNIT TESTS (Logic Verification) ---

    void unitTest_FlightCalc() {
        // Formula: Time = Battery / (Speed * Energy)
        // Data: 1000 / (100 * 2) = 5.0 hours
        EVTOLVehicle::CompanySpecs specs = {"Unit", 100, 1000, 1, 2.0, 1, 0};
        EVTOLModel vehicle(specs);

        QVERIFY(qFuzzyCompare(vehicle.getFlightDuration(), 5.0));
    }

    void unitTest_ZeroSpeedSafety() {
        // If speed is 0, result should be 0 (not infinity/NaN)
        EVTOLVehicle::CompanySpecs specs = {"Safety", 0, 100, 1, 1, 1, 0};
        EVTOLModel vehicle(specs);

        QCOMPARE(vehicle.getFlightDuration(), 0.0);
    }

    // --- REGRESSION TESTS (Fixed Scenarios) ---

    void regressionTest_AlphaProfile() {
        // "Alpha" Profile from requirements
        // 320 kWh, 120 mph, 1.6 kWh/mi
        // Expected: 1.66666...
        EVTOLVehicle::CompanySpecs alpha = {"Alpha", 120, 320, 0.6, 1.6, 4, 0.25};
        EVTOLModel vehicle(alpha);

        double expected = 1.6666666666666667;
        QVERIFY(qAbs(vehicle.getFlightDuration() - expected) < 0.00001);
    }

    void cleanupTestCase() {
        qDebug() << "Tests Finished.";
    }
};

QTEST_APPLESS_MAIN(TestEVTOL)

#include "tst_evtoltests.moc"
