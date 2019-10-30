#include "includes.h"
#include "pictureprocessor.h"
#include "sensormanager.h"
#include <iostream>
#include <string>

// iris includes
#include "iris/dist500/Dist500.h"
#include "iris/configuration/Configuration.h"
#include "iris/drivers/DriverManager.h"

// namespace shortcuts
using namespace iris::configuration;
using namespace iris::dist500;
using namespace iris::drivers;
using namespace iris::uip;
using namespace std;

#define SENSOR_URL "udp://10.3.35.21#1"

int timedelta = 6; // seconds

void count_sensor(Dist500* d5, iris::uip::__counter_type buffercounter) {
    Dist500::CountList cl;
    d5->getCountResults(1, buffercounter, true, 0x00, cl);
    // output the data
    for (Dist500::CountList::const_iterator k = cl.begin(); k != cl.end(); ++k) {
        category_counts cnt = *k;
        std::cout << cnt.alighting << "," << cnt.boarding << ","
            << cnt.alighting - cnt.boarding << "," << timedelta << std::endl;
//        std::cout << "fa " << 1 << " - category " << (int) cnt.categoryId << ": " << cnt.boarding
//             << " in / " << cnt.alighting << " out" << std::endl;
    }
}

void turn_on_sensor(Dist500* d5) {
//    d5->setDoorState(1,1,100,100);
    d5->startCounting();
}

void turn_off_sensor(Dist500* d5) {
//    d5->setDoorState(1,1,0,0);
    d5->stopCounting();
}

int main() {
    // create driver manager
    DriverManager& dm = DriverManager::getInstance();
    // try to register and enable default drivers (currently udp and can)
    dm.createAndActivateDefaultDrivers();
    // create DIST500 instance
    Sensor& s = dm.getSensorFor(SENSOR_URL);
    Dist500* d5 = new Dist500(s);
    iris::uip::__counter_type buffercounter = iris::uip::buffer_counter;

    turn_on_sensor(d5);
//    std::cout << "Go through the door" << std::endl;
    sleep(timedelta);
    turn_off_sensor(d5);
//    std::cout << "Done!" << std::endl;
    count_sensor(d5, buffercounter);

}