#include "AgendaUI.hpp"
#include "myexception.hpp"
#include <iostream>
#include <iomanip>
#include "logger.hpp"
using namespace std;

AgendaUI::AgendaUI(){
    startAgenda();
}

void AgendaUI::OperationLoop(void){
    string op;//
    do{
        cout << "----------------------------------Agenda--------------------------------" << endl;
        cout << "Action :" << endl;
        cout << "l    - log in Agenda by user name and password" << endl;
        cout << "r    - register an Agenda account" << endl;
        cout << "q    - quit Agenda" << endl;
        cout << "------------------------------------------------------------------------" << endl;
        cout << endl;
        cout << "Agenda : ~S ";
        cin >> op;
        switch (op[0])
        {
            case 'l': userLogIn(); break;
            case 'r': userRegister(); break;
            case 'q': quitAgenda(); break;
            default: cout << "try again~" << endl; break;
        }
    }while(op[0] != 'q');
}

void AgendaUI::startAgenda(void){
    this->m_logger = Logger::get_instance();
    this->m_logger->log_system_start();
    m_agendaService.startAgenda();
}

string AgendaUI::getOperation(){
    cout << "----------------------------------Agenda--------------------------------" << endl;
    cout << "Action :" << endl;
    cout << "o    - log out Agenda" << endl;
    cout << "dc   - delete Agenda account" << endl;
    cout << "lu   - list all Agenda user" << endl;
    cout << "cm   - create a meeting" << endl;
    cout << "amp  - add meeting participator" << endl;
    cout << "rmp  - remove meeting participator" << endl;
    cout << "rqm  - request to quit meeting" << endl;
    cout << "la   - list all meetings" << endl;
    cout << "las  - list all sponsor meetings" << endl;
    cout << "lap  - list all participator meetings" << endl;
    cout << "qm   - query meeting by titile" << endl;
    cout << "qt   - query meeting by time interval" << endl;
    cout << "dm   - delete meeting by title" << endl;
    cout << "da   - delete all meetings" << endl;
    cout << "------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "Agenda@" << m_userName << " :~# ";
    string op;
    cin >> op;
    return op;
}

bool AgendaUI::executeOperation(const string t_operation){
    if( t_operation == "o"){
        userLogOut();
        return false;
    }
    else if(t_operation == "dc"){
        deleteUser();
        return false;
    }
    else if(t_operation == "lu"){
        listAllUsers();
        return true;
    }
    else if(t_operation == "cm"){
        createMeeting();
        return true;
    }
    else if(t_operation == "amp"){
        addMeetingParticipator();
        return true;
    }
    else if(t_operation == "rmp"){
        removeMeetingParticipator();
        return true;
    }
    else if(t_operation == "rqm"){
        quitMeeting();
        return true;
    }
    else if(t_operation == "la"){
        listAllMeetings();
        return true;
    }
    else if(t_operation == "las"){
        listAllSponsorMeetings();
        return true;
    }
    else if(t_operation == "lap"){
        listAllParticipateMeetings();
        return true;
    }
    else if(t_operation == "qm"){
        queryMeetingByTitle();
        return true;
    }
    else if(t_operation == "qt"){
        queryMeetingByTimeInterval();
        return true;
    }
    else if(t_operation == "dm"){
        deleteMeetingByTitle();
        return true;
    }
    else if(t_operation == "da"){
        deleteAllMeetings();
        return true;
    }
    else{
        cout << "Illegal input! Please try again. " << endl;
        return true;
    }
}


void AgendaUI::userLogIn(void){
    cout << endl;
    cout << "[log in] [username] [password]" << endl;
    cout << "[log in] ";
    string username, password;
    cin >> username >> password;
    try{
        if(m_agendaService.userLogIn(username, password)){
            cout << "[log in] succeed!" << endl << endl;
            m_userName = username;
            m_userPassword = password;
            string sth = "";
            sth += "[log in] : ";
            sth += username;
            sth += " log in success \n";
            this->m_logger->log_write_success(sth);
            while(executeOperation(getOperation()));
        }
        else
        {
            cout << "[log in] Password incorrect or user doesn't exist" << endl;
        }
    }catch(myexception & e){
        this->m_logger->log_write_error(e);
        cout << "[log in] Password incorrect or user doesn't exist" << endl;
        cout << "[" << e.type() << "]: " << e.getmsg() << endl;
    }
}

void AgendaUI::userRegister(void){
    cout << endl << "[register] [username] [password] [email] [phone]" << endl;
    cout << "[register] ";
    string name, password, email, phone;
    cin >> name >> password >> email >> phone;
    try{
        if(m_agendaService.userRegister(name, password, email, phone))
        {
            cout << "[register] succeed!" << endl;
            string sth = "";
            sth += "[register] : ";
            sth += name;
            sth += " ";
            sth += password;
            sth += " succeed\n";
            this->m_logger->log_write_success(sth);
        }
        else 
        {
            cout << "[register] This username has been registered!" << endl;
        }
    }catch(myexception & e){
        cout << endl <<  "[" << e.type() << "]: " << e.getmsg() << endl;
    }
}

void AgendaUI::userLogOut(void){
    m_userName = "";
    m_userPassword = "";
    string sth = "";
    sth += "[log out] : ";
    sth += m_userName;
    sth += " log out success \n";
    this->m_logger->log_write_success(sth);
}

void AgendaUI::quitAgenda(void){
    m_agendaService.quitAgenda();
    this->m_logger->log_system_over();
    cout << endl << "[quit Agenda] Thank you for using the Agenda! Bye-bye!" << endl;
}

void AgendaUI::deleteUser(void){
    if(m_agendaService.deleteUser(m_userName, m_userPassword))
    {
        cout << endl << "[delete agenda account] succeed!" << endl;
        string sth = "";
        sth += "[del user] : ";
        sth += m_userName;
        sth += " del success \n";
        this->m_logger->log_write_success(sth);
    }
    userLogOut();
}

void AgendaUI::listAllUsers(void){
    list<User> user = m_agendaService.listAllUsers();
    cout << endl << "[list all users]" << endl << endl;
    cout << left << setw(15) << "name" << left << setw(15) << "email" << left << setw(15) << "phone" << endl;
    for(auto i = user.begin(); i != user.end(); i++){
        cout << left << setw(15) << (*i).getName() << left << setw(15) << (*i).getEmail() << left << setw(15) << (*i).getPhone() << endl;
    }
    string tmp = "[list all users] : success\n";
    this->m_logger->log_write_success(tmp);
}

void AgendaUI::createMeeting(void){
    cout << endl << "[create meeting] [the number of participators]" << endl;
    cout << "[create meeting] ";
    int count = 0;
    cin >> count;
    vector<string> par;
    for(int i=0; i<count; i++){
        cout << "[create meeting] [pleasse enter the participator " << i+1 << " ]" << endl; 
        cout << "[create meeting] ";
        string name;
        cin >> name;
        par.push_back(name);
    }
    cout << "[create meeting] [title][start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[create meeting] ";
    string title, start, end;
    cin >> title >> start >> end;
    try{
        if(m_agendaService.createMeeting(m_userName, title, start, end, par))
        {
            cout << "[create meeting] succeed!" << endl;
            string tmp = "[create meeting] succeed \n";
            this->m_logger->log_write_success(tmp);
        }
        else
        {
            cout << "[create meeting] error!" << endl;
        }
    }catch(myexception & e){
        this->m_logger->log_write_error(e);
        cout << endl <<  "[" << e.type() << "]: " << e.getmsg() << endl;
    }
    
}

void AgendaUI::addMeetingParticipator(void){
    cout << endl << "[add participator] [meeting title] [participator username]" << endl;
    cout << "[add participator] ";
    string title, participator;
    cin >> title >> participator;
    try{
        if(m_agendaService.addMeetingParticipator(m_userName, title, participator))
        {
            cout << "[add participator] succeed!" << endl;
            string tmp = "[add participator] succeed\n";
            this->m_logger->log_write_success(tmp);
        }
        else
        {
            cout << "[add participator] error!" << endl;
        }
    }catch(myexception & e){
        this->m_logger->log_write_error(e);
        cout << endl <<  "[" << e.type() << "]: " << e.getmsg() << endl;
    }
}

void AgendaUI::removeMeetingParticipator(void){
    cout << endl << "[remove participator] [meeting title] [participator username]" << endl;
    cout << "[remove participator] ";
    string title, participator;
    cin >> title >> participator;
    try{
        if(m_agendaService.removeMeetingParticipator(m_userName, title, participator))
        {
            cout << "[remove participator] succeed!" << endl;
            string tmp = "[remove participator] succeed\n";
            this->m_logger->log_write_success(tmp);
        }
        else
        {
            cout << "[remove participator] error!" << endl;
        }
     }catch(myexception & e){
        this->m_logger->log_write_error(e);
        cout << endl <<  "[" << e.type() << "]: " << e.getmsg() << endl;
    }
}

void AgendaUI::quitMeeting(void){
    cout << endl << "[quit meeting] [meeting title]" << endl;
    cout << "[quit meeting] ";
    string title;
    cin >> title;
    try{
        if(m_agendaService.quitMeeting(m_userName, title))
        {
            cout << "[quit meeting] succeed!" << endl;
            string tmp = "[quit meeting] succeed\n";
            this->m_logger->log_write_success(tmp);
        }
        else
        {
            cout << "[quit meeting] error!" << endl;
        }
    }catch(myexception & e){
        this->m_logger->log_write_error(e);
        cout << endl <<  "[" << e.type() << "]: " << e.getmsg() << endl;
    }
}

void AgendaUI::listAllMeetings(void){
    cout << endl << "[list all meetings]" << endl;
    cout << endl;
    list<Meeting> m = m_agendaService.listAllMeetings(m_userName);
    printMeetings(m);
    string tmp = "[list all meetings] : success\n";
    this->m_logger->log_write_success(tmp);
}

void AgendaUI::listAllSponsorMeetings(void){
    cout << endl << "[list all sponsor meetings]" << endl;
    cout << endl;
    list<Meeting> m = m_agendaService.listAllSponsorMeetings(m_userName);
    printMeetings(m);
    string tmp = "[list all sponsor meetings] : success\n";
    this->m_logger->log_write_success(tmp);
}

void AgendaUI::listAllParticipateMeetings(void){
    cout << endl << "[list all participator meetings]" << endl;
    cout << endl;
    list<Meeting> m = m_agendaService.listAllParticipateMeetings(m_userName);
    printMeetings(m);
    string tmp = "[list all participator meetings] : success\n";
    this->m_logger->log_write_success(tmp);
}

void AgendaUI::queryMeetingByTitle(void){
    cout << endl << "[query meetings] [title]:" << endl;
    cout << "[query meeting] ";
    string title;
    cin >> title;
    list<Meeting> m = m_agendaService.meetingQuery(m_userName, title);
    printMeetings(m);
    string tmp = "[query meetings by title] : success\n";
    this->m_logger->log_write_success(tmp);
}

void AgendaUI::queryMeetingByTimeInterval(void){
    cout << endl << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[query meeting] ";
    string start, end;
    cin >> start >> end;
    cout << "[query meeting] " << endl; 
    list<Meeting> m = m_agendaService.meetingQuery(m_userName, start, end);
    printMeetings(m);
    string tmp = "[query meetings by time] : success\n";
    this->m_logger->log_write_success(tmp);
}

void AgendaUI::deleteMeetingByTitle(void){
    cout << endl << "[delete meeting] [title]" << endl;
    cout << "[delete meeting] ";
    string title;
    cin >> title;
    try{
        if(m_agendaService.deleteMeeting(m_userName, title)) 
        {    
            cout << "[delete meeting] succeed!" << endl;
            string tmp = "[delete meeting] succeed\n";
            this->m_logger->log_write_success(tmp);
        }
        else
        {
            cout << "[error] delete meeting fail!" << endl;
        }
    }catch(myexception & e){
        this->m_logger->log_write_error(e);
        cout << endl <<  "[" << e.type() << "]: " << e.getmsg() << endl;
    }
}

void AgendaUI::deleteAllMeetings(void){
    try{
        if(m_agendaService.deleteAllMeetings(m_userName))
        {
            cout << "[delete all meeting] succeed!" << endl;
            string tmp = "[delete all meeting] succeed\n";
            this->m_logger->log_write_success(tmp);
        }
        else
        {
            cout << "[delete all meeting] Fail!" << endl;
        }
    }catch(myexception & e){
        this->m_logger->log_write_error(e);
        cout << endl <<  "[" << e.type() << "]: " << e.getmsg() << endl;
    }
}

void AgendaUI::printMeetings(const list<Meeting> t_meetings){
    cout << left << setw(15) << endl << "Title" << left << setw(12) << "sponsor" << left << setw(20) << "start time" << left << setw(20) << "end time" << left << setw(15) << "participators" << endl;
    for(auto i = t_meetings.begin(); i != t_meetings.end(); i++){
        vector<string> p = (*i).getParticipator();
        string participator;
        if(p.size() == 0) participator = "";
        else
        {
            for(int j = 0; j < p.size()-1; j++){
                participator += p[j];
                participator += ", ";
            }
            participator += p[p.size()-1];
        }
        cout << left << setw(15) << (*i).getTitle() << left << setw(12) << (*i).getSponsor() << left << setw(20) << Date::dateToString((*i).getStartDate()) << left << setw(20) << Date::dateToString((*i).getEndDate()) << left << setw(15) << participator << endl;
    }
}