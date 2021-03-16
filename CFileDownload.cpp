#include "CFileDownload.hpp"

static size_t size_written(void* ptr, size_t size, size_t nmemb, void* stream) {
	size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
	return written;
}

CFileDownload::CFileDownload() {
	curl_global_init(CURL_GLOBAL_ALL);
	curl_handle = curl_easy_init();
}

CFileDownload::~CFileDownload() {
	curl_easy_cleanup(curl_handle);
	curl_global_cleanup();
}

void CFileDownload::download(std::string url, std::string file_name) {
	curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 0L);
	curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, size_written);
	std::string fname = "downloads/";
	fname.append(file_name);
	FILE* pagefile = fopen(fname.c_str(), "wb");
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);
	curl_easy_perform(curl_handle);
	fclose(pagefile);
	char time_buffer[128];
	time_t t = time(NULL);
	struct tm *lt = localtime(&t);
	snprintf(time_buffer, 128, "%04d-%02d-%02d %02d:%02d:%02d", lt->tm_year + 1900, lt->tm_mon+1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
	std::reverse(file_name.begin(), file_name.end());
	std::string yaml_file_name = "lmay.";
	int counter = 0;
	for(int i = 0; i < file_name.size(); i++) {
		if(counter == 0) {
			if(file_name[i] == '.') {
				counter++;
			}
		} else {
			yaml_file_name += file_name[i];
		}
	}
	std::reverse(yaml_file_name.begin(), yaml_file_name.end());
	std::string yaml_fname = "downloads/";
	yaml_fname.append(yaml_file_name);
	std::ofstream yaml_file(yaml_fname);
	yaml_file << "origin: " << url << std::endl;
	yaml_file << "timestamp: " << time_buffer << std::endl;
	yaml_file.close();
}
