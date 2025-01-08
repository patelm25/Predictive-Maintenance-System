#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

class Sensor {
    protected:
        string sensorId;
        double minValue;
        double maxValue;
        
    public:
        Sensor(const string& id, double min, double max) {
            sensorId = id;
            minValue = min;
            maxValue = max;
        }
        
        virtual ~Sensor() {}
        
        virtual double simulateValue() = 0;
        virtual string sensorType() = 0;
        
        bool isValueValid(double value) {
            return (value >= minValue && value <= maxValue);
        }
        
        string getSensorId() {
            return sensorId;
        }
        
        double getMinValue() {
            return minValue;
        }
        
        double getMaxValue() {
            return maxValue;
        }
};

class tempSensor : public Sensor {
    public:
        tempSensor(const string& id) : Sensor(id, 20, 100) {}
        
        double simulateValue() override {
            return rand() % 121;
        }
        
        string sensorType() override {
            return "Temperature";
        }
};

class pressureSensor : public Sensor {
    public:
        pressureSensor(const string& id) : Sensor(id, 100, 500) {}
        
        double simulateValue() override {
            return rand() % 601;
        }
        
        string sensorType() override {
            return "Pressure";
        }
};

class vibrationSensor : public Sensor {
    public:
        vibrationSensor(const string& id) : Sensor(id, 10, 40) {}
        
        double simulateValue() override {
            return rand() % 61;
        }
        
        string sensorType() override {
            return "Vibration";
        }
};

class newSensor {
    public:
        static Sensor* createSensor(const string& type, const string& id) {
            if (type == "Temperature") {
                return new tempSensor(id);
            } 
            else if (type == "Pressure") {
                return new pressureSensor(id);
            } 
            else if (type == "Vibration") {
                return new vibrationSensor(id);
            }
            return nullptr;
        }
};

class logger {
    private:
        ofstream logFile;
        
    public:
        logger() {
            logFile.open("sensor_logs.txt", ios::app);
        }
        
        ~logger() {
            if (logFile.is_open()) {
                logFile.close();
            }
        }
        
        void logReading(Sensor* sensor, double value) {
            logFile << "Time: " << time(nullptr) 
                    << " - " << sensor->sensorType() << " Sensor " 
                    << sensor->getSensorId() << ": " << value;
            
            if (!sensor->isValueValid(value)) {
                logFile << " WARNING: Value outside safe range [" 
                    << sensor->getMinValue() << " - " 
                    << sensor->getMaxValue() << "]";
            }
            logFile << endl;
        }
        void lineDivider() {
            logFile << "____________________________________________________" << endl;
        }
};

class machineHealthMonitor {
    private:
        vector<Sensor*> sensors;
        logger logger;
        
    public:
        ~machineHealthMonitor() {
            for (auto sensor : sensors) {
                delete sensor;
            }
        }
        
        void addSensor(Sensor* sensor) {
            sensors.push_back(sensor);
        }
        
        void simulate() {
            cout << "\nChecking all sensors..." << endl;
            
            for (auto sensor : sensors) {
                double value = sensor->simulateValue();
                
                logger.logReading(sensor, value);
                
                cout << sensor->sensorType() << " Sensor " 
                    << sensor->getSensorId() << ": " << value;
                
                if (!sensor->isValueValid(value)) {
                    cout << " - CAUTION: Value is outside safe range!";
                }
                cout << endl;
            }
        }
        void lineDivider(){
            logger.lineDivider();
        }
};

int main() {
    srand(time(0));
    
    machineHealthMonitor monitor;
    
    Sensor* tempSensor = newSensor::createSensor("Temperature", "TEMP001");
    Sensor* pressureSensor = newSensor::createSensor("Pressure", "PRES001");
    Sensor* vibrationSensor = newSensor::createSensor("Vibration", "VIB001");
    
    monitor.addSensor(tempSensor);
    monitor.addSensor(pressureSensor);
    monitor.addSensor(vibrationSensor);
    
    for (int i = 0; i < 5; ++i) {
        monitor.simulate();
        cout << ".........................................\n";
        monitor.lineDivider();
        this_thread::sleep_for(chrono::seconds(1));
    }
    
    return 0;
}
