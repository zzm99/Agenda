#include "Storage.hpp"
#include "Path.hpp"
#include <iostream>
#include <fstream>
#include <string> 
#include <vector>
using namespace std;

std::shared_ptr<Storage> Storage::m_instance = nullptr;

Storage::Storage(){
    this->m_dirty = false;
    readFromFile();
}
 
bool Storage::readFromFile(void){
    ifstream meetingcsv(Path::meetingPath);
    ifstream usercsv(Path::userPath);
    string tmpmeeting;
    string tmpuser;

    if(!meetingcsv.is_open())
        return false;
    if(!usercsv.is_open())
        return false;
    
    while(getline(usercsv, tmpuser)){
        string username, password, email, phone;

        for(auto it = tmpuser.begin(); it != tmpuser.end(); ){
            if((*it)=='"')
                it = tmpuser.erase(it);
            else
                it++;
        }

        username = tmpuser.substr(0, tmpuser.find(','));
        tmpuser.erase(0, tmpuser.find(',')+1);
        password = tmpuser.substr(0, tmpuser.find(','));
        tmpuser.erase(0, tmpuser.find(',')+1);
        email = tmpuser.substr(0, tmpuser.find(','));
        tmpuser.erase(0, tmpuser.find(',')+1);
        phone = tmpuser.substr(0, tmpuser.find(','));

        m_userList.push_back(User(username, password, email, phone));

    }

    while(getline(meetingcsv,tmpmeeting)){
        string sponsor, participator, startTime, endTime, title;
        vector<string> participators;

        for(auto it = tmpmeeting.begin(); it!=tmpmeeting.end(); ){
            if((*it)=='"')
                it = tmpmeeting.erase(it);
            else
            {
                it++;
            }
            
        }

        sponsor = tmpmeeting.substr(0, tmpmeeting.find(','));
        tmpmeeting.erase(0, tmpmeeting.find(',')+1);
        participator = tmpmeeting.substr(0, tmpmeeting.find(','));
        tmpmeeting.erase(0, tmpmeeting.find(',')+1);

        while(participator.find('&')!=-1){
            string person = participator.substr(0, participator.find('&'));
            participators.push_back(person);
            participator.erase(0, participator.find('&')+1);
        }

        participators.push_back(participator);

        startTime = tmpmeeting.substr(0, tmpmeeting.find(','));
        tmpmeeting.erase(0, tmpmeeting.find(',')+1);
        endTime = tmpmeeting.substr(0, tmpmeeting.find(','));
        tmpmeeting.erase(0, tmpmeeting.find(',')+1);
        title = tmpmeeting;
        Date start = Date::stringToDate(startTime);
        Date end = Date::stringToDate(endTime);
        m_meetingList.push_back(Meeting(sponsor, participators, start, end, title));
    }

    meetingcsv.close();
    usercsv.close();

    return true;
}

bool Storage::writeToFile(void){
    m_dirty = false;
    ofstream meetingcsv(Path::meetingPath);
    ofstream usercsv(Path::userPath);
    
    if(!meetingcsv.is_open())
        return false;
    if(!usercsv.is_open())
        return false;
    
    for(auto i : m_meetingList){
        string tmp = "\"";
        tmp += i.getSponsor();
        tmp += "\",\"";
        vector<string> participators = i.getParticipator();
        for(auto it = participators.begin(); it != participators.end(); it++){
            tmp += *it;
            if(it + 1 != participators.end()){
                tmp += "&";
            }
        }
        tmp += "\",\"";
        Date start = i.getStartDate(), end = i.getEndDate();
        tmp += Date::dateToString(start);
        tmp += "\",\"";
        tmp += Date::dateToString(end);
        tmp += "\",\"";
        tmp += i.getTitle();
        tmp += "\"";

        meetingcsv << tmp << "\n";
    }
    meetingcsv.close();

    for(auto i : m_userList){
        string tmp = "\"";
        tmp += i.getName();
        tmp += "\",\"";
        tmp += i.getPassword();
        tmp += "\",\"";
        tmp += i.getEmail();
        tmp += "\",\"";
        tmp += i.getPhone();
        tmp += "\"";
        usercsv << tmp << "\n";
    }

    usercsv.close();
    return true;
}


std::shared_ptr<Storage> Storage::getInstance(void){
    if(m_instance == nullptr)
        m_instance = std::shared_ptr<Storage>(new Storage());
    return m_instance;
}

Storage::~Storage(){
    sync();
}


void Storage::createUser(const User &t_user){
    this->m_userList.push_back(t_user);
    m_dirty = true;
}

 
std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const{
    std::list<User> ret;
    for(auto it = m_userList.begin(); it != m_userList.end(); it++)
    {
        if(filter(*it))
        {
            ret.push_back(*it);
        }
    }
    return ret;
}
  
int Storage::updateUser(std::function<bool(const User &)> filter,
                 std::function<void(User &)> switcher){
    int ret = 0;
    for(auto it = m_userList.begin(); it != m_userList.end(); it++)
    {
        if(filter(*it))
        {
            switcher(*it);
            ret++;
        }
    }
    if(ret != 0)
        m_dirty = true;
    return ret;
}

  
int Storage::deleteUser(std::function<bool(const User &)> filter){
    int ret = 0;
    for(auto it = m_userList.begin(); it != m_userList.end(); )
    {
        if(filter(*it))
        {
            it = m_userList.erase(it);
            ret++;
        }
        else
        {
            it++;
        }
        
    }
    if(ret != 0)
        m_dirty = true;
    return ret;
}

  
void Storage::createMeeting(const Meeting &t_meeting){
    this->m_meetingList.push_back(t_meeting);
    m_dirty = true;
}

  
std::list<Meeting> Storage::queryMeeting(
      std::function<bool(const Meeting &)> filter) const{
    std::list<Meeting> ret;
    for(auto it = m_meetingList.begin(); it != m_meetingList.end(); it++)
    {
        if(filter(*it))
        {
            ret.push_back(*it);
        }
    }
    return ret;
}

 
int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                    std::function<void(Meeting &)> switcher){ 
    int ret = 0;
    for(auto it = m_meetingList.begin(); it != m_meetingList.end(); it++)
    {
        if(filter(*it))
        {
            switcher(*it);
            ret++;
        }
    }
    if(ret != 0)
        m_dirty = true;
    return ret;
}

int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter){
    int ret = 0;
    for(auto it = m_meetingList.begin(); it != m_meetingList.end(); )
    {
        if(filter(*it))
        {
            it = m_meetingList.erase(it);
            ret++;
        }
        else
        {
            it++;
        }
        
    }
    if(ret != 0)
        m_dirty = true;
    return ret;
}

bool Storage::sync(void){
    if(m_dirty == false) return false;
    else
    {
        return writeToFile();
    }
}