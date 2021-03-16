#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <random>

#include <unistd.h>

#include "CFileDownload.hpp"

int main(int argc, char* argv[]) {
	CFileDownload* fdload = new CFileDownload();
	std::string filename = "Forum-Zero-G-Shogun-Steel-Customizations.html";
	fdload->download("https://worldbeyblade.org/Forum-Zero-G-Shogun-Steel-Customizations", filename);
	std::vector<std::string> threads;
	bool nextpage = false;
	do {
		nextpage = false;
		std::string path = "downloads/";
		path.append(filename);
		std::ifstream infile(path);
		std::string line = "";
		while(std::getline(infile, line)) {
			if(line.find("class=\"threadlink") != std::string::npos) {
				std::string tmp = "";
				int counter = 0;
				for(int i = 0; i < line.size(); i++) {
					if(line[i] == '"') {
						counter++;
					} else {
						if(counter == 1) {
							tmp += line[i];
						}
					}
				}
				if(std::find(threads.begin(), threads.end(), tmp) == threads.end()) {
					threads.push_back(tmp);
				}
			}
			if(line.find(">Next <span aria-hidden=\"true\"><i class=\"fa fa-fw fa-chevron-circle-right\"></i></span></a>") != std::string::npos) {
				std::string tmp = "";
				int counter = 0;
				for(int i = 0; i < line.size(); i++) {
					if(line[i] == '"') {
						counter++;
					} else {
						if(counter == 3) {
							tmp += line[i];
						}
					}
				}
				nextpage = true;
				std::string filepath = "https://worldbeyblade.org/";
				filepath.append(tmp);
				filename = tmp;
				filename.append(".html");
				fdload->download(filepath, filename);
			}
		}
		infile.close();
	} while(nextpage);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(10, 20);
	for(int i = 0; i < threads.size(); i++) {
		sleep(distr(gen));
		filename = threads[i];
		filename.append(".html");
		std::string filepath = "https://worldbeyblade.org/";
		filepath.append(threads[i]);
		fdload->download(filepath, filename);
		nextpage = false;
		std::string path = "downloads/";
		path.append(filename);
		std::ifstream infile(path);
		std::string line = "";
		while(std::getline(infile, line)) {
			if(line.find(">Next <span aria-hidden=\"true\"><i class=\"fa fa-fw fa-chevron-circle-right\"></i></span></a>") != std::string::npos) {
				std::string tmp = "";
				int counter = 0;
				for(int i = 0; i < line.size(); i++) {
					if(line[i] == '"') {
						counter++;
					} else {
						if(counter == 3) {
							tmp += line[i];
						}
					}
				}
				nextpage = true;
				std::string filepath = "https://worldbeyblade.org/";
				filepath.append(tmp);
				filename = tmp;
				filename.append(".html");
				fdload->download(filepath, filename);
				if(std::find(threads.begin(), threads.end(), tmp) == threads.end()) {
					threads.push_back(tmp);
				}
			}
		}
		infile.close();
	}
	delete fdload;
	fdload = NULL;
	std::cout << "Done with " << threads.size() << " documents, please check." << std::endl;
	return 0;
}
