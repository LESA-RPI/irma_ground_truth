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

            for (unsigned int i = 0; i < sensorURLs.size(); ++i) {

                sensors[i] = &dm.getSensorFor(sensorURLs[i]);
                dists[i] = new Dist500(*sensors[i]);
                procs[i] = new PictureProcessor(i);
                procs[i]->PictureSink::attachSource(dists[i]);
                dists[i]->setDoorState(1, 1, 100, 100);
                dists[i]->startCounting();
//                dists[i]->startPictureAcquisision();
//                dists[i]->enableInstallationMode(InstallationModeController::Full16, 0, 0, 0, 0, 0);

                iris::uip::__counter_type buffercounter = iris::uip::buffer_counter;
                Dist500::CountList cl;

                Dist500::FunctionAreaStatusList sl;
                dists[i]->getFunctionAreaStatuses(sl);
                // let's see what we got
//                std::cout << "Got this far1 - " << sl.size() << std::endl;

                std::cout << "Starting counting..." << std::endl;
                sleep(10);
                std::cout << "Ending counting..." << std::endl;
                dists[i]->stopCounting();
                dists[i]->getCountResults(1, buffercounter, true, 0xff, cl);
                for (Dist500::CountList::iterator i = cl.begin(); i != cl.end(); ++i) {
                    iris::uip::category_counts cc = *i;
                    std::cout << "cat " << (int) cc.categoryId << ": in " <<
                         cc.boarding << " / out " << cc.alighting << "\r\n";}

            }

			/* -------
            Sensor& s = dm.getSensorFor("udp://10.3.35.21#1");
            Dist500* d5 = new Dist500(s);
            d5->setDoorState(1, 1, 100, 100);
            d5->startCounting();
            sleep(10);
            d5->stopCounting();

            unsigned short fa = d5->getSensorInfo().getFunctionArea();

            Dist500::CategoryList categories;
            d5->getCountCategories(fa, categories);

            // retrieve status bytes
            Dist500::FunctionAreaStatusList sl;
            d5->getFunctionAreaStatuses(sl);
            // let's see what we got
            std::cout << "Got this far1 - " << sl.size() << std::endl;
            Dist500::CountList cl;
            // get available counts for all categories
            // query count results for function area
            // use buffered values
            // reset counts after transmission
            d5->getCountResults(fa, buffer_counter, true, 0xff, cl);
            // output the data
            for (Dist500::CountList::const_iterator k = cl.begin(); k != cl.end(); ++k) {
                category_counts cnt = *k;
                cout << "fa " << fa << " - category " << (int) cnt.categoryId << ": " << cnt.boarding
                     << " in / " << cnt.alighting << " out\n";
                }
			// -------
//			for (unsigned int i = 0; i < sensorURLs.size(); ++i) {
//			    printf("started cycle\r\n");
//
//				sensors[i] = &dm.getSensorFor(sensorURLs[i]);
//				dists[i] = new Dist500(*sensors[i]);
//
//                unsigned short fa = 1;
//                unsigned short door = 1;
//				dists[i]->setDoorState(fa, door, 100, 100);
//
//				Dist500::CountList cl;
//                Dist500::CountList crl;
//                iris::uip::__counter_type buffercounter = iris::uip::buffer_counter;
//
//                int count = 0;
//
//                while (1){
//                    dists[i]->setDoorState(fa, door, 100, 100);
//                    dists[i]->startCounting();
//                    std::cout << "Starting counting..." << std::endl;
//                    sleep(10);
//                    std::cout << "Ending counting..." << std::endl;
//                    dists[i]->stopCounting();
//                    count++;
//
//                    dists[i]->getCountResults(fa, buffercounter, true, 0xff, cl);
//                    for (Dist500::CountList::iterator i = cl.begin(); i != cl.end(); ++i) {
//                        iris::uip::category_counts cc = *i;
//                        std::cout << "cat " << (int) cc.categoryId << ": in " <<
//                             cc.boarding << " / out " << cc.alighting << "\r\n";}
//                    printf("%u counts\r\n", count);
//                }
//			} */

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
