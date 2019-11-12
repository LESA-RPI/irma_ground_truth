#include "includes.h"
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

int timedelta = 1; // seconds
int cycles = 21600; // iterations

void count_sensor(Dist500* d5, iris::uip::__counter_type countertype) {
    Dist500::CountList cl;
    d5->getCountResults(1, countertype, true, 0x00, cl);
    // output the data
    for (Dist500::CountList::const_iterator k = cl.begin(); k != cl.end(); ++k) {
        category_counts cnt = *k;
        std::cout << cnt.alighting << "," << cnt.boarding << ","
                  << cnt.alighting - cnt.boarding << "," << timedelta << std::endl;
//        std::cout << "fa " << 1 << " - category " << (int) cnt.categoryId << ": " << cnt.boarding
//             << " in / " << cnt.alighting << " out" << std::endl;
    }
}

void turn_on_sensor(Dist500* d5, iris::uip::__counter_type countertype) {
    d5->resetCounts(1, countertype);
    d5->setDoorState(1,1,100,100);
    d5->startCounting();
}

void turn_off_sensor(Dist500* d5) {
    d5->setDoorState(1,1,0,0);
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
    iris::uip::__counter_type start_counter = iris::uip::start_counter;

    // this workflow is the only one that seems to work, and it only returns one
    // sample per program trial. Currently, running this over and over produces inconsistent
    // results :(
//    turn_on_sensor(d5);
//    sleep(timedelta);
//    turn_off_sensor(d5);2563be92a6a2ba7d1f27669c62ebdd2bb7a57912
//    count_sensor(d5, buffercounter);

    // this workflow is the one we'd like to have-- but it returns 0 alighting/0 boarding
    turn_on_sensor(d5, start_counter);
    for (int i = 0; i < cycles; i++) {
        sleep(timedelta);
        count_sensor(d5, start_counter);
    };
    turn_off_sensor(d5);


}