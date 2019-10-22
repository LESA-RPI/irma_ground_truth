// STL includes
#include "includes.h"

using namespace iris::dist500;
using namespace iris::drivers;
using namespace iris::uip;
using namespace std;

class SensorManager {
	private:
		DriverManager& dm = DriverManager::getInstance();
		std::vector<std::string> sensorURLs;
		int num_sensors;
		Sensor** sensors;
		Dist500** dists;
		PictureProcessor** procs;

	public:
		SensorManager(const std::string& configFile) {
			dm.createAndActivateDefaultDrivers();
			readConfig(configFile);
			sensors = new Sensor*[num_sensors];
			dists = new Dist500*[num_sensors];
			procs = new PictureProcessor*[num_sensors];
		}

		~SensorManager() {
 		    dm.shutdownAllDrivers();
			delete[] dists;
		}

		void startUpdating() {

//            for (unsigned int i = 0; i < sensorURLs.size(); ++i) {
//
//                sensors[i] = &dm.getSensorFor(sensorURLs[i]);
//                dists[i] = new Dist500(*sensors[i]);
//                procs[i] = new PictureProcessor(i);
//                procs[i]->PictureSink::attachSource(dists[i]);
//                dists[i]->setDoorState(2, 6, 100, 100);
//                dists[i]->startCounting();
////                dists[i]->startPictureAcquisision();
////                dists[i]->enableInstallationMode(InstallationModeController::Full16, 0, 0, 0, 0, 0);
//
//                iris::uip::__counter_type buffercounter = iris::uip::buffer_counter;
//                Dist500::CountList cl;
//
//                Dist500::FunctionAreaStatusList sl;
//                dists[i]->getFunctionAreaStatuses(sl);
//                std::cout << "Got this far1 - " << sl.size() << std::endl;
//
//                std::cout << "Starting counting..." << std::endl;
//                sleep(10);
//                std::cout << "Ending counting..." << std::endl;
//                dists[i]->stopCounting();
//                dists[i]->getCountResults(1, buffercounter, true, 0xff, cl);
//                for (Dist500::CountList::iterator i = cl.begin(); i != cl.end(); ++i) {
//                    iris::uip::category_counts cc = *i;
//                    std::cout << "cat " << (int) cc.categoryId << ": in " <<
//                         cc.boarding << " / out " << cc.alighting << "\r\n";}
//
//            }


            Dist500::SensorStatusSet sss;
            Sensor& s = dm.getSensorFor("udp://10.3.35.21#1");
            Dist500* d5 = new Dist500(s);
            Dist500::CountList cl;


            d5->setDoorState(1, 1, 0, 0);
            d5->stopCounting();
            iris::uip::__counter_type buffercounter = iris::uip::buffer_counter;
//            d5->resetCounts(1, buffer_counter);


            std::cout << "starting" << std::endl;
            d5->setDoorState(1, 1, 100, 100);
            std::cout << "door open" << std::endl;
            d5->startCounting();
            std::cout << "started counting" << std::endl;
            sleep(10);
            d5->setDoorState(1, 1, 0, 0);
            std::cout << "closed doors" << std::endl;
            d5->stopCounting();
            std::cout << "stopped counting" << std::endl;



            d5->getCountResults(1, buffercounter, false, 0xff, cl);
            // output the data
            for (Dist500::CountList::const_iterator k = cl.begin(); k != cl.end(); ++k) {
                category_counts cnt = *k;
                cout << "fa " << 1 << " - category " << (int) cnt.categoryId << ": "
                     << cnt.boarding
                     << " in / " << cnt.alighting << " out\n";
            }
//            d5->startCounting();

//            while (true) {
//                d5->setDoorState(1, 1, 100, 100);
//                d5->getSensorStatuses(sss);
//
//                d5->getCountResults(1, buffercounter, true, 0x01, cl);
//            }

//
//            unsigned short fa = d5->getSensorInfo().getFunctionArea();
//            std::cout << "fa:" << fa << std::endl;
//            d5->setDoorState(fa, 1, 100, 100);
//
//            d5->setWorkingMode(iris::uip::normal_activated_mode);
//            iris::uip::working_mode currentMode;
//            d5->getWorkingMode(currentMode);
//            std::cout << "Working mode" << currentMode << std::endl;
//
//            Dist500::SensorStatusSet sss;
//            d5->getSensorStatuses(sss);
//
//            d5->getSensorStatuses(sss);
//            std::cout << "Sss size before counting time: " << sss.size() << std::endl;
//
//            d5->startCounting();
//            std::cout << "Started counting" << std::endl;
//            sleep(5);
//            d5->getSensorStatuses(sss);
//            std::cout << "Sss size inside counting time: " << sss.size() << std::endl;
//            sleep(5);
//            d5->stopCounting();
//            std::cout << "Stopped counting" << std::endl;
		}

		void stopUpdating() {
			for (unsigned int i = 0; i < sensorURLs.size(); ++i) {
				dists[i]->setDoorState(2, 6, 0, 0);
				dists[i]->stopCounting();
			}
		}

	private:
		void readConfig(const std::string& s) {
			std::fstream config;
			config.open(s.c_str());
			std::string buffer;
			while (config >> buffer)
				this->sensorURLs.push_back(buffer);
			config.close();
			num_sensors = sensorURLs.size();
		}
};
