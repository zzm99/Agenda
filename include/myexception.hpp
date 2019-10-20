#ifndef MYEXCEPTION
#define MYEXCEPTION 

#include <string>
using namespace std;

class myexception {
public:
    myexception(const string & message ) : m_message(message){}
    virtual ~myexception(){}
    string getmsg() const { return this->m_message;}
    virtual string type() const = 0;

private:
    string m_message;
};

class error_format : public myexception {
public:
    error_format(const string &msg = "") : myexception(msg){};
    virtual string type() const { return "Error: format wrong !"; }
};

class error_date : public myexception {
public:
    error_date(const string &msg = "") : myexception(msg){};
    virtual string type() const { return "Error: date wrong !"; }
};


class no_user : public myexception {
public:
    no_user(const string &msg = "") : myexception(msg){};
    virtual string type() const { return "Error: user not find  wrong !"; }
};


class user_repeat : public myexception {
public:
    user_repeat(const string &msg = "") : myexception(msg){};
    virtual string type() const { return "Error: user repeat wrong !"; }
};




class conflict_time : public myexception {
public:
    conflict_time(const string &msg = "") : myexception(msg){};
    virtual string type() const { return "Error: conflict time wrong !"; }
};


class no_meet : public myexception {
public:
    no_meet(const string &msg = "") : myexception(msg){};
    virtual string type() const { return "Error: meet not find wrong !"; }
};


class del_none : public myexception {
public:
    del_none(const string &msg = "") : myexception(msg){};
    virtual string type() const { return "Error: delete none wrong !"; }
};


class wrong_num : public myexception {
public:
    wrong_num(const string &msg = "") : myexception(msg){};
    virtual string type() const { return "Error: num wrong !"; }
};


class unknown : public myexception {
public:
    unknown(const string &msg = "") : myexception(msg){};
    virtual string type() const { return "Error: unknown wrong !"; }
};


#endif