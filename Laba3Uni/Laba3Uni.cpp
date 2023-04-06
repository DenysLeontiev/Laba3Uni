#include <iostream>
#include <vector>
#include <string>
#include <fstream>  
#include <sstream>

using namespace std;

class Driver
{
public:
    string DriverName;
private:
    bool isAvailable = true; // by default driver must be avaliable
public: // cosntructors
    Driver()
    {
        DriverName.empty();
        isAvailable = true;
    }

    Driver(string driverName)
    {
        DriverName = driverName;
        isAvailable = true;
    }

public:
    bool IsDriverAvailable()
    {
        if (isAvailable)
        {
            return true;
        }

        return false;
    }

    void SetAvailability(bool isAvailable)
    {
        this->isAvailable = isAvailable;
    }
};

class Taxi
{
public: // fields
    string Passenger;
private:
    vector<Driver> Drivers;
protected:
    vector<string> Addresses;

public:
    vector<Driver> GetDrivers()
    {
        return Drivers;
    }

    vector<string> GetAddresses()
    {
        return Addresses;
    }

public: // order methods
    bool OrderTaxi(string address)
    {
        string searchAddress = GetAddress(address);
        if (searchAddress.empty())
        {
            Addresses.push_back(address);
        }

        Driver* availableDriver = nullptr;
        for (int i = 0; i < Drivers.size(); i++)
        {
            if (Drivers[i].IsDriverAvailable())
            {
                availableDriver = &Drivers[i];
            }
        }

        if (availableDriver == nullptr)
        {
            cout << "There are no avaliable drives" << endl;
            return false;
        }

        availableDriver->SetAvailability(false);
        cout << "Order is accepted" << endl << "Your driver`s name is " 
            << availableDriver->DriverName << " | Your destination is " << address << endl;

        return true;
    }

    bool OrderTaxi(string address, int driverId)
    {
        if (driverId < 0)
        {
            driverId = 0;
        } 
        else if (driverId > Drivers.size() - 1)
        {
            driverId = Drivers.size() - 1;
        }

        string searchingAddress = GetAddress(address);

        if (searchingAddress.empty())
        {
            SetAddress(searchingAddress);
        }

        Driver targetDriver = Drivers[driverId];
        if (targetDriver.IsDriverAvailable() == false)
        {
            cout << "Sorry, but driver called " << targetDriver.DriverName << " is not available" << endl;
            return false;
        }

        targetDriver.SetAvailability(false);
        cout << "Your driver`s name is " << targetDriver.DriverName << " | Your destination is " << address << endl;
        return true;
    }

    bool OrderTaxi(int driverId) // isDriverAvailable
    {
        if (driverId < 0 || driverId > Drivers.size() - 1) {
            cout << "Driver is not found" <<  endl;
            return false;
        }

        Driver targetDriver = Drivers[driverId];
        return targetDriver.IsDriverAvailable();
        
    }

public: // constructors
    Taxi() 
    {
        Passenger = "";
        Drivers = vector<Driver>();
        Addresses = vector<string>();
    }

    Taxi(string passenger, vector<Driver> drivers, vector<string> addresses) 
    {
        Passenger = passenger;
        Drivers = drivers;
        Addresses = addresses;
    }

    Taxi(const Taxi& other) 
    {
        Passenger = other.Passenger;
        Drivers = other.Drivers;
        Addresses = other.Addresses;
    }

    ~Taxi()
    {
        Passenger.clear(); // destroys string
        // vectors are self destruct;
    }

public: 
#pragma region Driver Getters and Setters
    Driver* GetDriver(string driverName)
    {
        for (int i = 0; i < Drivers.size(); i++)
        {
            if (Drivers[i].DriverName == driverName)
            {
                return &Drivers[i];
            }
        }
    }

    bool SetDriver(string driverName)
    {
        Driver* driver = nullptr;
        if (Drivers.size() > 0)
        {
            driver = GetDriver(driverName);
        }

        if (driver)
        {
            cout << "Driver with name" << driverName << "already exists" << endl;
            return false;
        }

        Driver newDriver = Driver(driverName);
        Drivers.push_back(newDriver);
        return true;
    }
#pragma endregion

#pragma region Addresses Getters and Setters
    string GetAddress(string address) 
    {
        for (int i = 0; i < Addresses.size(); i++)
        {
            if (Addresses[i] == address)
            {
                cout << "Address is found (" << address << ")" << endl;
                return address;
            }
            else
            {
                cout << "Address is not found" << endl;
                return "";
            }
        }
    }

    bool SetAddress(string address)
    {
        string searchAddress = GetAddress(address);

        if (!searchAddress.empty()) // to avoid adding same addresses
        {
            cout << "Such address already exists (" << address << ")" << endl;
            return false;
        }

        Addresses.push_back(address);
        return true;
    }
#pragma endregion

public: // Print class method
    void PrintClass()
    {
        cout << "Passenger name: " << Passenger << endl;

        cout << "All Drivers: " << endl;
        for (int i = 0; i < Drivers.size(); i++)
        {
            cout << "DriverName: " << Drivers[i].DriverName << " | IsAvailable: " << Drivers[i].IsDriverAvailable() << endl;
        }

        cout << "All addresses: " << endl;
        for (int i = 0; i < Addresses.size(); i++)
        {
            cout << "Address #" << i + 1 << " is " << Addresses[i] << endl;
        }
    }

public: // Save to File and Load from file
    void SaveToFile(string filename)
    {
        ofstream outputFile(filename);
        if (outputFile.is_open())
        {
            // Save passenger name
            outputFile << "Passenger:" << endl << Passenger << endl;

            // Save drivers
            outputFile << "Drivers:" << endl;
            for (int i = 0; i < Drivers.size(); i++)
            {
                outputFile << "DriverName:" << Drivers[i].DriverName << ",IsAvailable:" << Drivers[i].IsDriverAvailable() << endl;
            }

            // Save addresses
            outputFile << "Addresses:" << endl;
            for (int i = 0; i < Addresses.size(); i++)
            {
                outputFile << Addresses[i] << endl;
            }

            outputFile.close();
            cout << "File " << filename << " is saved." << endl;
        }
        else
        {
            cout << "Unable to open file " << filename << endl;
        }
    }
};

class TaxiManager
{
public:
    static Taxi CreateTaxiFromInput()
    {
        // Get passenger name from input
        cout << "Enter passenger name: ";
        string passengerName;
        cin >> passengerName;

        // Get number of drivers from input
        cout << "Enter number of drivers: ";
        int numDrivers;
        cin >> numDrivers;

        vector<Driver> drivers;
        for (int i = 0; i < numDrivers; i++)
        {
            // Get driver name from input
            cout << "Enter driver #" << i + 1 << " name: ";
            string driverName;
            cin >> driverName;

            drivers.push_back(Driver(driverName));
        }

        // Get number of addresses from input
        cout << "Enter number of addresses: ";
        int numAddresses;
        cin >> numAddresses;

        vector<string> addresses;
        for (int i = 0; i < numAddresses; i++)
        {
            // Get address from input
            cout << "Enter address #" << i + 1 << ": ";
            string address;
            cin >> address;

            addresses.push_back(address);
        }

        return Taxi(passengerName, drivers, addresses);
    }

    static Taxi LoadFromFile(const string& filename)
    {
        ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            cout << "Failed to open file " + filename << endl;
        }

        string line;
        string passenger;
        vector<Driver> drivers;
        vector<string> addresses;

        while (getline(inputFile, line))
        {
            if (line.find("Passenger:") != string::npos)
            {
                getline(inputFile, passenger);
            }
            else if (line.find("DriverName:") != string::npos)
            {
                string driverName;
                bool isAvailable;

                // Parse driver name and availability from line
                size_t driverNameStart = line.find("DriverName:") + 11;
                size_t driverNameEnd = line.find(",IsAvailable:");
                driverName = line.substr(driverNameStart, driverNameEnd - driverNameStart);

                size_t isAvailableStart = driverNameEnd + 13;
                isAvailable = (line.substr(isAvailableStart) == "true");

                drivers.push_back(Driver(driverName));
                if (isAvailable)
                {
                    drivers.back().SetAvailability(true);
                }
            }
            else if (line.find("Addresses:") != string::npos)
            {
                while (getline(inputFile, line))
                {
                    addresses.push_back(line);
                }
            }
        }

        inputFile.close();

        return Taxi(passenger, drivers, addresses);
    }
};

int main()
{

    Taxi taxi;
    taxi = TaxiManager::CreateTaxiFromInput();

    taxi.PrintClass();

    cout << "Save to File" << endl;
    taxi.SaveToFile("firstFile.txt");
    cout << "========================== Init from file ==========================" << endl;
    Taxi myTaxi = TaxiManager::LoadFromFile("firstFile.txt");
    myTaxi.PrintClass();





    /*cout << "Welcome to Taxi" << endl << endl;
    cout << "Enter your name :";
    string myName;
    getline(cin, myName);*/

    /*vector<Driver> allDrivers{ Driver("Tom"), Driver("Bill"), Driver("Denys") };
    vector<string> allAddresses{ "Brody", "Lviv", "Kyjiv"};

    Taxi myTaxi(myName, allDrivers, allAddresses);
    myTaxi.PrintClass();
    
    myTaxi.OrderTaxi("Brodyyy");
    cout << "================================" << endl;
    myTaxi.OrderTaxi("Brodyyy", 1);*/
}
