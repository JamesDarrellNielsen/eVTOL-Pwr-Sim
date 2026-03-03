#ifndef EVTOLPWRSIM_H  // Header guard: prevents this file from being included multiple times during compilation
#define EVTOLPWRSIM_H  // Defines the macro to mark this header as "already included"

#include <QMainWindow>    // Imports the base class for a standard Qt application main window
#include <QStandardItem>   // Imports the class used for managing individual data items in a list or table
#include <QStringList>     // Imports a specialized container for handling lists of strings
#include <QtConcurrent>
#include <QFutureWatcher>
#include "evtolvehicle.h"
QT_BEGIN_NAMESPACE      // Standard Qt macro to handle potential namespace conflicts in complex builds
namespace Ui { class eVtolPwrSim; } // Forward declaration of the UI class generated from the .ui file
QT_END_NAMESPACE        // Closes the Qt namespace block

// Defines the main class, inheriting from QMainWindow to provide a standard window interface
class eVtolPwrSim : public QMainWindow
{
    Q_OBJECT // Mandatory macro for any class using Qt's signals, slots, or meta-object system

public:
    // Constructor: initializes the window; 'parent' manages memory cleanup within the Qt hierarchy
    eVtolPwrSim(QWidget *parent = nullptr);

    // Destructor: ensures resources (like the UI pointer) are properly cleaned up when the object is destroyed
    ~eVtolPwrSim();

private slots:
    // Event handler: automatically triggered by Qt when the button named 'runPwrSimBtn' is clicked
    void on_runPwrSimBtn_clicked();
    void handleSimFinished(); // slot to update the table

private:
    // Helper method: likely used to format or map string data into UI-ready QStandardItem objects
    void extracted(QStringList &rowData, QList<QStandardItem *> &items);

    // Pointer to the UI object: provides access to all visual widgets (buttons, labels, etc.) in the window
    Ui::eVtolPwrSim *ui;

    // Move these here so handleSimFinished() can see them:
    std::vector<EVTOLVehicle::CompanySpecs> fleetSpecs;
    std::vector<int> counts;
    std::vector<EVTOLVehicle::Stats> stats;
};

#endif // EVTOLPWRSIM_H // Ends the header guard conditional block



