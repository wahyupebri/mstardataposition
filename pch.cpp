// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vcclr.h>
#include <chrono>
#include <time.h>
#include <string>
#include <ctime>
#include <cstring>
#include <sstream>
#include <iomanip>
//#include <curl/curl.h>
using namespace System;
const char* RESTIP = "http://127.0.0.1/";
int test1() {
	printf("test");
	return 0;
}
int execConsole(char* command) {
	int result = system(command);
	return result;
}
int printData(void* t) {

	return 0;
}

char* convertStrToChar(String^ result) {
	pin_ptr<const wchar_t> wch = PtrToStringChars(result);
	size_t convertedChars = 0;
	size_t  sizeInBytes = ((result->Length + 1) * 2);
	errno_t err = 0;
	char* ch = (char*)malloc(sizeInBytes);

	err = wcstombs_s(&convertedChars,
		ch, sizeInBytes,
		wch, sizeInBytes);
	if (err != 0)
		printf_s("[ERROR] wcstombs_s  failed!\n");

	//printf_s("%s\n", ch);
	return ch;
}
long long getMillisec() {
	using namespace std::chrono;
	auto duration = system_clock::now().time_since_epoch();
	long long millis = duration_cast<milliseconds>(duration).count();
	return millis;
}
int waitForMillSecs(long long s) {
	auto start = getMillisec();
	for (;;) {
		if (getMillisec() - start >= s)
			return 0;
	}
}
wchar_t* getWC(const char* c) {
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);
	return wc;
}
void convertCommaToPoint(void* src, void* dest) {
	char* src_ = (char*)src;
	char * pos=strstr(src_, ",");
	if(pos!=0){
	*pos = '.';
	}
	strcpy((char *)dest,src_);
	
}
void convertDoubleCommaToCharPoint(void* src, void* dest) {
	double* src_ = (double*)src;
	char* dest_ = (char*)dest;
	std::string stringValue = std::to_string(*src_);
	int lenString=stringValue.size()+1;
	char* charArray = (char*)malloc(lenString * sizeof(char));
	std::copy(stringValue.begin(), stringValue.end(), charArray);
	charArray[stringValue.size()] = '\0';
	convertCommaToPoint(charArray, dest_);
	//std::cout << "Char array: " << charArray << std::endl;
	free(charArray);
}
void convertDoubleToCharArray(void* src, void* dest) {
	double* src_ = (double*)src;
	char* dest_ = (char*)dest;
	std::string stringValue = std::to_string(*src_);
	int lenString = stringValue.size() + 1;
	char* charArray = (char*)malloc(lenString * sizeof(char));
	std::copy(stringValue.begin(), stringValue.end(), charArray);
	charArray[stringValue.size()] = '\0';
	strcpy(dest_, charArray); 
	//std::cout << "Char array: " << charArray << std::endl;
	free(charArray);
}
int postData(char* link) {
	CURL* curl;
	CURLcode res;
	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, RESTIP);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, link);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "[ERROR] %s, %s\n", link,
				curl_easy_strerror(res));

		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	curl = NULL;
	link = NULL;
	//free(res);
	return 0;

}
int convertDataTimeFormat(char* iFormat, char* oFormat, char* input, char* output) {
	std::tm timeinfo = {};
	std::istringstream ss(input);
	ss >> std::get_time(&timeinfo, iFormat);
	if (ss.fail()) {
		fprintf(stderr, "[ERROR] Unable to parsing input\n");
		return 0;
	}
		
	char outputDateTime[50];
	std::strftime(outputDateTime, sizeof(outputDateTime), oFormat, &timeinfo);
	strcpy(output, outputDateTime);
	return 0;
}