#include "AgendaService.hpp"
#include <iostream>
#include "myexception.hpp"
using namespace std;


/**
   * constructor
   */
AgendaService::AgendaService(){
    startAgenda();
}
//

  /**
   * destructor
   */
AgendaService::~AgendaService(){
    quitAgenda();
}

  /**
   * check if the username match password
   * @param userName the username want to login
   * @param password the password user enter
   * @return if success, true will be returned
   */
bool AgendaService::userLogIn(const std::string &userName, const std::string &password){
    std::list<User> user = m_storage->queryUser([&](const User& userT)->bool{
        return (userT.getName() == userName && userT.getPassword()==password);
        });
    //return (user.size() != 0);
    if(user.size() == 0) 
        throw no_user("User: " + userName);
    
    return true;
}

  /**
   * regist a user
   * @param userName new user's username
   * @param password new user's password
   * @param email new user's email
   * @param phone new user's phone
   * @return if success, true will be returned
   */
bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                    const std::string &email, const std::string &phone){
    std::list<User> user = m_storage->queryUser([&](const User& userT)->bool{
        return (userT.getName() == userName);
        });
    if(!user.empty()){
        //m_storage->createUser(User(userName, password, email, phone));
       // return true;
       throw user_repeat("Username: " + userName + " repeated");
    }
    m_storage->createUser(User(userName, password, email, phone));
    return true;
}

  /**
   * delete a user
   * @param userName user's username
   * @param password user's password
   * @return if success, true will be returned
   */
bool AgendaService::deleteUser(const std::string &userName, const std::string &password){
    int count = m_storage->deleteUser([&](const User& userA){
        return (userA.getName()==userName && userA.getPassword() == password);
        });
    if(count == 0)
        throw no_user("User: " + userName);
        //return false;
    else
    {
        m_storage->deleteMeeting([&](const Meeting& metA){
            return (metA.getSponsor() == userName);
            });
        m_storage->updateMeeting([&](const Meeting& meeting){
            return meeting.isParticipator(userName);},
            [&]( Meeting& meeting){
                 meeting.removeParticipator(userName);
                 }
        );
        m_storage->deleteMeeting([&](const Meeting& meeting){ 
            return meeting.getParticipator().size() == 0;
             });
    }
    return true;
}

/**
   * list all users from storage
   * @return a user list result
   */
std::list<User> AgendaService::listAllUsers(void) const{
    auto filter =  [](const User& user){
        return true;
    };
    return m_storage->queryUser(filter);
}

  /**
   * create a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @param startData the meeting's start date
   * @param endData the meeting's end date
   * @return if success, true will be returned
   */
bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                     const std::string &startDate, const std::string &endDate,
                     const std::vector<std::string> &participator){
Date start(startDate);
Date end(endDate);
if(!(Date::isValid(start) && Date::isValid(end))) 
throw error_date("Date: " + startDate + " " + endDate);
//return false;
if(m_storage->queryMeeting([&](const Meeting &metA){ return metA.getTitle() == title; }).size() == 0){
    if(start < end){
        list<User> invole = m_storage->queryUser([&](const User &userA){
            bool flag = false;
            for(auto it = participator.begin(); it != participator.end(); it++){
                if((*it) == userA.getName()) flag = true;
            }
            if(userName == userA.getName()) flag = true;
            return flag;
        });
        if(invole.size() != participator.size() + 1) 
            throw wrong_num("Num people in createMeeting");

        //return false;
        for(auto it = participator.begin(); it != participator.end(); it++){
            if((*it)==userName) 
                throw error_format("participator can not be sponsor");
            //return false;
            list<Meeting> m = m_storage->queryMeeting([&](const Meeting& metA){
                if(metA.getSponsor() == (*it) || metA.isParticipator((*it))){
                    if(end <= metA.getStartDate() || start >= metA.getEndDate()) 
                    return false;
                    else 
                    return true;
                }
                else
                return false;
            });
            if(m.size() != 0) 
                throw conflict_time("createMeeting: people in shedule conflict !");
            //return false;
        }
        list<Meeting> m = m_storage->queryMeeting([&](const Meeting& metA){
            if(metA.getSponsor() == userName || metA.isParticipator(userName))
            {
                if(end <= metA.getStartDate() || start >= metA.getEndDate()) 
                return false;
                else 
                return true;
            }
            else 
            return false;
        });
        if(m.size()!=0)
            throw conflict_time("createMeeting: people in shedule conflict !");
            //return false;

    Meeting newmeeting(userName, participator, start, end, title);
    m_storage->createMeeting(newmeeting);
    return true; 
    }
}
return false;
}

  /**
   * add a participator to a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @return if success, true will be returned
   */
bool AgendaService::addMeetingParticipator(const std::string &userName,
                              const std::string &title,
                              const std::string &participator){
    list<User> person = m_storage->queryUser([&](const User& userA){
        return userA.getName() == participator;
    });
    if(person.size() != 1) 
        throw no_user("addMeetingparticipator: no user: " + userName);
    //return false;
    list<Meeting> target = m_storage->queryMeeting([&](const Meeting& metA){
        return  !metA.isParticipator(participator) && metA.getTitle() == title;
    });
    if(target.size() == 0 || target.size()> 1) 
        throw no_meet("addMeetingParticipator: no meeting: title: " + title );
    //return false;
    else{
        Meeting m = target.front();
        Date start = m.getStartDate();
        Date end = m.getEndDate();
        list<Meeting> meetings = m_storage->queryMeeting([&](const Meeting& metA){
            if(metA.getSponsor() == participator || metA.isParticipator(participator)){
                if(end <= metA.getStartDate() || start >= metA.getEndDate()) 
                return false;
                else 
                return true;
            }
            else 
            return false;
        });
        if(meetings.size() != 0) 
            throw conflict_time("meeting time conflict! ");
        //return false;
        m_storage->updateMeeting([&](const Meeting& metA){return metA.getTitle()==title;},
                                [&](Meeting& metB){metB.addParticipator(participator);});
        return true;
    }
    return false;
}

  /**
   * remove a participator from a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @return if success, true will be returned
   */
bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                 const std::string &title,
                                 const std::string &participator){
        // jide yao gengxin 
        int count = m_storage->updateMeeting([&](const Meeting& metA){ 
            return metA.getTitle() == title && metA.isParticipator(participator);
            },
                                    [&](Meeting & metB){ 
                                        metB.removeParticipator(participator);});
    if(count == 0) 
        throw del_none("delnone : " + participator);
        //return false;

    m_storage->deleteMeeting([&](const Meeting &metA){
        return metA.getParticipator().size() == 0;
        });
    return true;
    }

  /**
   * quit from a meeting
   * @param userName the current userName. need to be the participator (a sponsor can not quit his/her meeting)
   * @param title the meeting's title
   * @return if success, true will be returned
   */
bool AgendaService::quitMeeting(const std::string &userName, const std::string &title){
    // ! buneng shi sponsor 
    int ret = m_storage->updateMeeting(
        [&](const Meeting& meet)->bool{
        if(meet.isParticipator(userName) && meet.getTitle() == title && meet.getSponsor() != userName) 
            return true;
        return false;
        },
        [&](Meeting& m)->void{
            m.removeParticipator(userName);
        }
    );
     m_storage->deleteMeeting([&](const Meeting & meeting){ 
         return meeting.getParticipator().size()==0;
         });
     
    if(ret) return true;
    //return false;
    throw del_none("none quit from meeting!");
}

  /**
   * search a meeting by username and title
   * @param userName as a sponsor OR a participator
   * @param title the meeting's title
   * @return a meeting list result
   */
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &title) const{
        auto filter = [userName, title](const Meeting &meeting){
            if(meeting.getTitle() == title)
            return meeting.isParticipator(userName) || meeting.getSponsor() == userName;
        return false;
        };

        return m_storage->queryMeeting(filter);
    }
  /**
   * search a meeting by username, time interval
   * @param userName as a sponsor OR a participator
   * @param startDate time interval's start date
   * @param endDate time interval's end date
   * @return a meeting list result
   */
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &startDate,
                                  const std::string &endDate) const{
            std::list<Meeting> ret;//chansheng yige kong de ret
            // yanzheng shijian shifou zhengque 
            Date start = Date::stringToDate(startDate);
            Date end = Date::stringToDate(endDate);
            if(!Date::isValid(start) || !Date::isValid(end)) 
            //    throw error_date("error in date: " + startDate + " " + endDate);
            return ret;
            if(start > end) 
            //    throw error_date("error in date: " + startDate + " " + endDate);
            return ret;
            // chenggong 
            return  m_storage->queryMeeting([&](const Meeting& meet)->bool{
                if(meet.getSponsor()==userName || meet.isParticipator(userName)){
                if(meet.getEndDate() < start || meet.getStartDate() > end) 
                    return false;
                return true;
                }
                return false;
            });
        }

  /**
   * list all meetings the user take part in
   * @param userName user's username
   * @return a meeting list result
   */
std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const{
    std::list<Meeting> ret = m_storage->queryMeeting([&](const Meeting& meet)->bool{
        if(meet.getSponsor()==userName || meet.isParticipator(userName))
            return true; 
        return false; });
    return ret;
}

  /**
   * list all meetings the user sponsor
   * @param userName user's username
   * @return a meeting list result
   */
std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const{
    std::list<Meeting> ret = m_storage->queryMeeting([&](const Meeting& meet)->bool{
        if(meet.getSponsor()==userName)return true; else return false; 
        });
    return ret;
}

  /**
   * list all meetings the user take part in and sponsor by other
   * @param userName user's username
   * @return a meeting list result
   */
std::list<Meeting> AgendaService::listAllParticipateMeetings(
      const std::string &userName) const{
          std::list<Meeting> ret = m_storage->queryMeeting([&](const Meeting& meet)->bool{
              if(meet.isParticipator(userName) && meet.getSponsor() != userName)return true; else return false; 
              });
          return ret;
      }

  /**
   * delete a meeting by title and its sponsor
   * @param userName sponsor's username
   * @param title meeting's title
   * @return if success, true will be returned
   */
bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title){
    int ret = m_storage->deleteMeeting([&](const Meeting& meet)->bool{
        if(meet.getSponsor()==userName && meet.getTitle()==title)return true; else return false;
        });
    if(ret)
        return true;
    else
        throw del_none("del no meeting : "+ title);
        //return false;
}

  /**
   * delete all meetings by sponsor
   * @param userName sponsor's username
   * @return if success, true will be returned
   */
bool AgendaService::deleteAllMeetings(const std::string &userName){
    int ret = m_storage->deleteMeeting([&](const Meeting& meet)->bool{
        if(meet.getSponsor()==userName)
            return true;
        else 
            return false;});
    if(ret)
        return true;
    else
         throw del_none("del no meeting : "+ userName);
        //return false;
}

  /**
   * start Agenda service and connect to storage
   */
void AgendaService::startAgenda(void){
    // huoqu instance-> new and read
    m_storage = Storage::getInstance();
}

  /**
   * quit Agenda service
   */
void AgendaService::quitAgenda(void){
    // jieshu  gengxin shujv 
    m_storage->sync();
}
