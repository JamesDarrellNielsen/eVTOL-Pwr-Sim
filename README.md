The Electric Vertical Take-Off and Landing aircraft (eVTOL) Power Simulator program solves a problem assigned during the interview process at Joby Aviation.

eVTOL SIMULATION SOLUTION:

On a laptop with an Intal 10 core CPU on the Windows 11 platform,  developed a human machine interface (HMI) program using the Qt Framework C++ 17 libraries, minimalist ((GNU) is not Unix) (GNU) C++ for windows (MinGw) compiler with the Qt Creator IDE with the buit in Qt Designer (WYSIWYG) tool.

This C++ program is a Qt Widgets application that simulates a 3-hour power and flight performance test for a fleet of 20 eVTOL (Electric Vertical Take-off and Landing) vehicles. It uses a multithreaded architecture to perform (presumed) heavy calculations in the background while maintaining a responsive user interface.

The eVTOL Power Simulator program implements the Model-View-Controller (MVC) architectural design pattern that using Qt here is an adaption of the traditinal MVC pattern where the Controller is a separate object that mediates between the Model(data and logic) and the View(presentation) whereas Qt implements more of a Model View pattern with the Controller built into the Views handling user interaction and style rendering(delegates).

The main function entry point first instantiates the eVtolPwrSim object that serves as the Model encapsulating the logic and the data for the simulations, and presents the View that is the decoupled presentation and handles user interaction, and then enters into the "GUI Loop".

The eVtolPwrSim is where the simulation and data model is processed.  The simulation is executed automatically in the construction of the object that displays the results and allows the user to press a button to re-run the simulation.

eVtolPwrSim:
Performs a multithreaded simulation by launching a QtConcurrent::run block. Inside this block, it creates a vector of 20 vehicles randomly distributed among five different companies (Alpha, Bravo, Charlie, Delta, and Echo).

Each of the 20 vehicles is assigned its own std::thread to simulate its journey independently. This includes:

Flight Phase: Calculating distance, passenger miles, and flight time based on cruise speed and battery life.

Random Faults: Using a Bernoulli distribution to simulate the chance of mechanical faults during flight.

Charging Phase: Simulating battery recharging using a shared set of three chargers, managed by a priority queue (to find the soonest available charger) and mutexes (to prevent data corruption).

Thread Safety: The program uses std::lock_guard with mutexes to safely merge individual vehicle results into global company statistics.

Asynchronous Monitoring: A QFutureWatcher monitors the background simulation. Once all threads finish and the data is ready, it triggers the handleSimFinished slot on the main UI thread.

Data Display: The program initializes a QStandardItemModel (the Model), populates it with calculated averages (average flight time, distance, and charging duration) for each company, and attaches it to a QTableView (the View) for the user to see.

Key Technical Components:
Component	        Purpose
std::mt19937	    A high-quality random number engine used 
                    to generate vehicle counts and simulation events.
QtConcurrent	    Manages the high-level background task to keep the 
                    GUI from freezing.
std::mutex	        Synchronizes access to shared resources like chargers and 
                    final statistics.
QStandardItemModel	Data source that feeds the table in the UI.

The simulation uses a discrete event loop for each vehicle, calculating flight and charging cycles until a 3-hour limit is reached.

Flight Duration & The "Over-Flying" Guard
    The simulation first determines how long a vehicle can stay in the air based on its specific battery capacity.
    Battery Life: Derived from the getFlightDuration() method of the EVTOLModel class.
    Safety Logic: To ensure a vehicle doesn't fly past the 3-hour simulation cutoff, it uses std::min:

    Actual Flight = min(Battery Life, Remaining Sim Time)

Distance & Productivity Metrics
    Once the flight time is determined, the code calculates the physical output of that flight segment:
    Distance: Calculated using the standard kinematic formula:

          Distance = Actual Flight X Cruise Speed

    Passenger Miles: Measures the total "utility" provided by the flight:

          PAX Miles = Distance X Number of Passengers

Stochastic Fault Logic
    The program uses a Bernoulli Distribution (a weighted coin flip) to simulate mechanical failures. The risk is scaled by the length of the flight, meaning longer flights have a higher cumulative chance of a fault.
	
    Fault Probability:

            Fault Probability per Hour X Actual Flight Time	

Charging & Waiting

    Charging is handled using a Min-Heap (Priority Queue) of three chargers. This simulates real-world "queuing" where a vehicle must wait if all chargers are occupied.

    Start Charge Time: If a vehicle arrives at a charger that is still busy, it must wait:

		Start Charge = max(Arrival Time, Charger Ready Time)
		
    End Charge Time:

        End Charge = Start Charge + Vehicle Charge Time Specification
		
Final Averaging (Post-Simulation)
    After all threads join, the handleSimFinished function calculates the final statistics shown in your QTableView:

					Total Metric (Time/Dist)
		Averages:   ------------------------
		               Total Flight Count
					   
Output results from one run:

![Alt text](https://github.com/JamesDarrellNielsen/eVTOL-Pwr-Sim/blob/main/images/EVTOLPwrSimResults.png)

**** Appedix **** 
eVTOL SIMULATION ASSIGNMENT:

Objective - Show us your awesome skills! We want to see your design, coding, documentation, testing, and communication abilties first hand! 
Details - This work must be coded in C++, and submitted through GitHub, BitBucket, or Gitlab. Upload the documented code to your personal account and send us a link. We may look at the git history to see how you use version control. 
At Joby we love good OOP design. Please exercise the basic tenets of OOP, using relevant principles where appropriate. We’re looking for robust, testable, modular, and well-documented code. 
Unit Testing is a big part of our development process. Please include just a few examples of unit tests or how your code is testable. Tests don't necessarily need to run in any test framework, but please be ready to talk about what and how you tested your code. 
To dos and notes are OK. If you want to incorporate an idea that makes sense in a real project, but would take too much time to implement, just leave a note.

Problem Statement 
There are five companies developing eVTOL aircraft. The vehicle produced by each 
manufacturer has different characteristics. Six distinct properties are laid out in the below table: 

![Alt text](https://github.com/JamesDarrellNielsen/eVTOL-Pwr-Sim/blob/main/images/assignmentSpecs.png)

You will simulate using these vehicle for 3 hours. Of course your simulation should take much less time than that. 20 total vehicles should be deployed, and a random number of each type of vehicle should be used (with the total between all five types being 20). 
There are only three chargers available for all 20 vehicles! A single charger can only be used by one vehicle at a time. Assume the chargers can charge the batteries in the Time to Charge time listed for each vehicle.  
Keep track of the following statistics per vehicle type:  
	- average flight time per flight 
	- average distance traveled per flight 
	- average time charging per charge session 
	- total number of faults - total number of passenger miles.  

For example, if there are 2 vehicles carrying 4 passengers on a vehicle that cruises 
for 1 hour at 100 mph, total number of passenger miles is 2 * 4 * 1 * 100 = 800. 
Assume that: - Each vehicle starts the simulation with a fully-charged battery - Each vehicle instantaneously reaches Cruise Speed - Each vehicle is airborne for the full use of the battery, and is immediately in line for the charger after running out of battery power. 
Please include the statistics recorded during at least one run of the simulation in your submission. 
Please do not hesitate to reach out to ask any questions about the problem! However, if you find yourself making an assumption that we haven’t explained explicitly, you may simply document your assumption. It’s interesting to us to hear what assumptions you made and why. 

eVTOL SIMULATION SOLUTION:

On a laptop with an Intal 10 core CPU on the Windows 11 platform,  developed a human machine interface (HMI) program using the Qt Framework C++ 17 libraries, minimalist ((GNU) is not Unix) (GNU) C++ for windows (MinGw) compiler with the Qt Creator IDE with the buit in Qt Designer (WYSIWYG) tool.

The eVTOL Power Simulator program implements the Model-View-Controller (MVC) architectural design pattern that using Qt here is an adaption of the traditinal MVC pattern where the Controller is a separate object that mediates between the Model(data and logic) and the View(presentation) whereas Qt implements more of a Model View pattern with the Controller built into the Views handling user interaction and style rendering(delegates).

The main function entry point first instantiates the eVtolPwrSim object that serves as the Model encapsulating the logic and the data for the simulations, and presents the View that is the decoupled presentation and handles user interaction, and then enters into the "GUI Loop".

The eVtolPwrSim is where the simulation and data model is processed.  The simulation is executed automatically in the construction of the object that displays the results and allows the user to press a button to re-run the simulation.



  
