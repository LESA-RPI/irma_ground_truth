#include "includes.h"
#include "pictureprocessor.h"
//#include "sensormanager.h"
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

void callback(void* userData, iris::uip::ulong index, iris::uip::ulong total,
              iris::uip::ushort groupId, iris::uip::ushort paramId) {
    cout << ((index + 1) * 100.0 / total) << " %" << endl;
}

int main() {
    std::cout << "Starting" << std::endl;
    // get reference to central driver manager
    DriverManager& dm = DriverManager::getInstance();
    // try to register and enable default drivers (currently udp and can)
    dm.createAndActivateDefaultDrivers();
    // create DIST500 instance
    Sensor& s = dm.getSensorFor(SENSOR_URL);
    Dist500* d5 = new Dist500(s);
    // get information about supported categories
    unsigned short fa = d5->getSensorInfo().getFunctionArea();
    Dist500::CategoryList categories;
    d5->getCountCategories(fa, categories);
    // retrieve status bytes
    Dist500::FunctionAreaStatusList sl;
    d5->getFunctionAreaStatuses(sl);
    // let's see what we got
    for (Dist500::FunctionAreaStatusList::const_iterator i = sl.begin(); i != sl.end(); ++i) {
        functionAreaStatus stat = *i;
        std::cout << "Stat: " << stat.functionAreaStatusBytes << std::endl;
        // new result available ?
        if (stat.functionAreaStatusBytes == new_counting_result) {
            Dist500::CountList cl;
            // get available counts for all categories
            // query count results for function area
            // use buffered values
            // reset counts after transmission
            d5->getCountResults(stat.functionAreaNumber, buffer_counter, true, 0xff, cl);
            // output the data
            for (Dist500::CountList::const_iterator k = cl.begin(); k != cl.end(); ++k) {
                category_counts cnt = *k;
                cout << "fa " << stat.functionAreaNumber << " - category " << (int) cnt.categoryId << ": " << cnt.boarding
                     << " in / " << cnt.alighting << " out\n";
            }
            // alternative: iterate over cagegories
            /*for (Dist500::CategoryList::const_iterator j = categories.begin(); j != categories.end(); ++j) {
                iris::uip::category cat = *j;
                // get available counts for given category
                // query count results for function area
                // use buffered values
                // reset counts after transmission
                d5->getCountResults(stat.functionAreaNumber, buffer_counter, true, cat.id, cl);
                category_counts cnt = cl.first();
                cout << "fa " << stat.functionAreaNumber << " - category " << (int) cnt.categoryId << ": " << cnt.boarding
                     << " in / " << cnt.alighting << " out\n";
            }*/
        }
    }
    std::cout << "Ending" << std::endl;
}

int door_state_main() {
    // get reference to central driver manager
    DriverManager& dm = DriverManager::getInstance();
    // try to register and enable default drivers (currently udp and can)
    dm.createAndActivateDefaultDrivers();
    // create DIST500 instance
    Sensor& s = dm.getSensorFor(SENSOR_URL);
    Dist500* d5 = new Dist500(s);
    // read door state
    // FUNCTION AREA AND DOOR ADDRESS MAY NEED TO BE ADJUSTED FOR YOUR SENSOR !
    unsigned short fa = 1;
    unsigned short door = 1;
    unsigned char left = 0, right = 0;
    d5->getDoorState(fa, door, left, right);
    if (!left && !right) {
        cout << "door is closed\n";
    }
    else {
        cout << "door is open\n";
    }
    //d5->setDoorState(fa, door, 0, 0);
}


int config_main() {
    // get reference to central driver manager
    DriverManager& dm = DriverManager::getInstance();
    // try to register and enable default drivers (currently udp and can)
    dm.createAndActivateDefaultDrivers();
    // create DIST500 instance
    Sensor& s = dm.getSensorFor(SENSOR_URL);
    Dist500* d5 = new Dist500(s);
    // read configuration
    Dist500Configuration cfg;
    d5->readConfiguration(cfg, callback, 0);
    // get references to some parameter groups and output a few parameter values
    const FirmwareGroup& fw = cfg.getFirmwareGroup();
    FirmwareGroup::Version v;
    fw.getActiveDspFirmwareVersion(v);
    cout << "active firmware version is " << v << "\n";
    const DoorGroup& d = cfg.getDoorGroup();
    cout << "1st door address " << d.getDoorAddress(0) << "\n";
    const FunctionAreaGroup& f = cfg.getFunctionAreaGroup();
    cout << "1st fa address " << f.getFunctionArea(0) << "\n";
    const CountingGroup& c = cfg.getCountingGroup();
    cout << "mounting height is " << c.getMountingHeight() << " mm\n";
}
//
//int oldmain() {
//    printf("step1");
//	SensorManager s("config.txt");
//    printf("step2");
//	s.startUpdating();
//    printf("step3");
//    sleep(1000);
//	return 0;
//}
//
//int old_main() {
//    SensorManager s("config.txt");
//    while (true) {
//        s.startUpdating();
//    }
//    return 0;
//}
