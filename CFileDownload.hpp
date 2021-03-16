#ifndef CFILEDOWNLOAD_HPP
#define CFILEDOWNLOAD_HPP

#include <string>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <curl/curl.h>

static size_t size_written(void* ptr, size_t size, size_t nmemb, void* stream);

class CFileDownload {
	public:
		CFileDownload();
		virtual ~CFileDownload();
		void download(std::string url, std::string file_name);
	private:
		CURL* curl_handle;
};

#endif
