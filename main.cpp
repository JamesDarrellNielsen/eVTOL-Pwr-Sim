#include "evtolpwrsim.h"  // Includes the header for your main window class (the "View")
#include <QApplication>   // Required for the Qt Application framework and event loop

int main(int argc, char *argv[])
{
    /*
     * 1. THE APPLICATION OBJECT
     * QApplication manages GUI application control flow and main settings.
     * It handles window system integration, mouse/keyboard events, and
     * the 'look and feel' of the widgets. It must be created before any
     * GUI objects (like data tables) are instantiated.
     */
    QApplication a(argc, argv);

    /*
     * 2. THE MAIN WINDOW (The "View" Container)
     * Here, 'eVtolPwrSim' is a custom class (inherits from QMainWindow).
     * This object holds QTableView and the logic to set up the Model.
     */
    eVtolPwrSim w;

    /*
     * 3. INITIAL GEOMETRY
     * Sets the width to 750 pixels and height to 280 pixels.
     * If this isn't set, the window will default to a size based on its contents.
     */
    w.resize(750, 280);

    /*
     * 4. DISPLAYING THE WINDOW
     * Widgets are created hidden by default. Calling .show() makes the window
     * visible and schedules it to be painted on the screen.
     */
    w.show();

    /*
     * 5. THE UI EVENT LOOP
     * a.exec() starts the Qt event loop. This is a blocking call.
     * The program stays in this loop, waiting for user input (clicks, typing,
     * resizing) and dispatching those events to the appropriate widgets.
     *
     * It returns the exit code when the application closes (usually 0).
     */
    return a.exec();
}

