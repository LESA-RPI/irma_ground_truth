// ***************************************************************************
// *    _      _       ____                                 _    ____ ___    *
// *   (_)_ __(_)___  / ___|  ___ _ __  ___  ___  _ __     / \  |  _ \_ _|   *
// *   | | '__| / __| \___ \ / _ \ '_ \/ __|/ _ \| '__|   / _ \ | |_) | |    *
// *   | | |  | \__ \  ___) |  __/ | | \__ \ (_) | |     / ___ \|  __/| |    *
// *   |_|_|  |_|___/ |____/ \___|_| |_|___/\___/|_|    /_/   \_\_|  |___|   *
// *                                                                         *
// *   Copyright (c) 2010 by iris-GmbH, Berlin         All rights reserved   *
// *                                                                         *
// ***************************************************************************

// ---------------------------------------------------------------------------
// Please refer to LICENSE.TXT for more information on copyright and licensing
// terms with respect to this library and its source codes.
// ---------------------------------------------------------------------------

// STL includes
#include <iostream>
#include <string>

// iris includes
#include "iris/dist500/Dist500.h"
#include "iris/drivers/DriverManager.h"
#include "iris/Common.h"

// namespace shortcuts
using namespace iris::dist500;
using namespace iris::drivers;
using namespace std;

// PLEASE REPLACE x WITH THE URL OF A CONNECTED SENSOR AND UNCOMMENT !
// (THE URL MUST BE PLACED In QUOTES, EG "udp://10.3.1.145#1")
#define SENSOR_URL "udp://10.3.6.189#1"

#ifndef SENSOR_URL
#error NO VALUE FOR #define SENSOR_URL, CHECK SOURCE CODE
#else

// callback used to get feedback on the update progress
void callback(void* userData, Dist500* sensor,
        CONTAINER_UPDATE_STATE state, unsigned int containerStep, unsigned int containerTotalSteps,
        double stepPercentage, double totalPercentage) {
    IRIS_UNUSED(userData)
    IRIS_UNUSED(sensor)
    IRIS_UNUSED(state)
    IRIS_UNUSED(containerStep)
    IRIS_UNUSED(containerTotalSteps)
    IRIS_UNUSED(stepPercentage)
    cout << (int) totalPercentage << " %" << endl;
}

// PLEASE NOTE:
// - You need administrator rights to run FirmwareUpdate.exe under MS Windows Vista, MS Windows 7 and MS Windows 8.
//   Run your IDE (e.g. Qt Creator) as administrator to accomplish this.
// - If the callback function outputs only "0%" and FirmwareUpdate.exe is then terminated with message "Error" probably D5FC file is not found.
// - On finish of container update sensor is resetted automatically to run updated firmware.
int main(int argc, char* argv[]) {
    IRIS_UNUSED(argc)
    IRIS_UNUSED(argv)

    // get reference to central driver manager
    DriverManager& dm = DriverManager::getInstance();
    // try to register and enable default drivers (currently udp and can)
    dm.createAndActivateDefaultDrivers();
    // create DIST500 instance
    Sensor& s = dm.getSensorFor(SENSOR_URL);
    Dist500* d5 = new Dist500(s);
    // simply call the appropriate method and enjoy
    string containerfile = "D5FC - DIST500_Application_2.0.0.5072 Single Count Category - 3521_CE_CF_D0_D1_D4_D5.d5fc";
    FirmwareController::UpdateResultMap resultMap;
    cout << "Firmware of sensor with URL " << SENSOR_URL << " is updated using " << containerfile << "..." << endl;
    if (d5->performContainerUpdate(containerfile, resultMap, true, 0, callback)) {
        cout << "Success" << endl;
    }
    else {
        cout << "Error" << endl;
    }
    // release the DIST500 object
    delete d5;
    dm.shutdownAllDrivers();
    return 0;
}

#endif
