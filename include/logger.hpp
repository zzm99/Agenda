#ifndef LOGGER
#define LOGGER

#include "myexception.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <memory>
#include <vector>
//#include <io.h>
using namespace std;


class Logger{
public:
	ofstream log_file;
	string m_username;
	int count = 0;
	static shared_ptr<Logger> m_logger;
	static shared_ptr<Logger> get_instance();


	Logger(string path){
		log_file.open(path, std::ofstream::app);		
		log_file.seekp(0, ios::end);
		long ps = log_file.tellp();
		// if(ps >= 10485760){
		// 	log_file.close();
		// }
		count = 1;
		//while(ps >= 10485760)
		while(ps >= 500)
		{
			log_file.close();
			string tmp = "./log-";
			tmp += '0' + count;
			tmp += ".txt";
			log_file.open(tmp, std::ofstream::app);		
			log_file.seekp(0, ios::end);
			ps = log_file.tellp();
			count++;
		}
	}
	
	~Logger(){
		log_file.close();
	}
	
	void check_file_MB(){
		log_file.seekp(0, ios::end);
		long ps = log_file.tellp();
		//while(ps >= 10485760)
		while(ps >= 500)
		{
			log_file.close();
			string tmp = "./log-";
			tmp += '0' + count;
			tmp += ".txt";
			log_file.open(tmp, std::ofstream::app);		
			log_file.seekp(0, ios::end);
			ps = log_file.tellp();
			count++;
		}
	}

	void log_in(string user);
	void log_out();

	void log_system_start();
	void log_system_over();

	void log_write_success(string& sth);
	void log_write_error(myexception &e);

};

#endif