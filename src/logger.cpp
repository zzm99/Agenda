#include "logger.hpp"
#include "myexception.hpp"

using namespace std;

shared_ptr<Logger> Logger::m_logger = nullptr;

shared_ptr<Logger> Logger::get_instance(){
	if(m_logger == nullptr)
		m_logger = shared_ptr<Logger>(new Logger("./log-0.txt"));

	return m_logger;
}

void Logger::log_in(string user){
	check_file_MB();

	time_t now = time(0);
	string tmp = ctime(&now);
	

	tmp += "[";
	m_username = user;
	tmp += user;
	tmp += "] Log In\n";
	log_file << tmp;
}

void Logger::log_out(){
	check_file_MB();
	time_t now = time(0);
	string tmp = ctime(&now);
	

	tmp += "[";
	tmp += m_username;
	m_username = "";
	tmp += "] Log Out\n";
	log_file << tmp;
}

void Logger::log_system_start(){
	check_file_MB();
	time_t now = time(0);
	string tmp = ctime(&now);
	
	
	tmp += "[Agenda System start] \n";
	log_file << tmp << endl;
}

void Logger::log_system_over(){
	check_file_MB();
	time_t now = time(0);
	string tmp = ctime(&now);
	
	
	tmp += "[Agenda System over] \n";
	log_file << tmp << endl;
}

void Logger::log_write_success(string& sth){
	check_file_MB();
	time_t now = time(0);
	string tmp = ctime(&now);
	
	log_file << tmp << "[Success]" << sth << endl;
}

void Logger::log_write_error(myexception & e){
	check_file_MB();
	time_t now = time(0);
	string tmp = ctime(&now);
	
	log_file << tmp << "[Wrong]: " << endl << e.type() << endl << e.getmsg() << endl << endl;
}
