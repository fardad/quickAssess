#ifndef SICT_USER_H__
#define SICT_USER_H__
#include <string>
#include <iostream>
#include "debug.h"

#ifndef _MSC_VER
#include <unistd.h>
#include <pwd.h>
#endif

  class User {
    unsigned int m_uid;
    std::string m_userid;
    std::string m_ip;
    std::string m_homedir;
    bool m_multipleIp;
    void remParentheses(std::string& whoIP)const;
  public:
    User();
    void getIP();
    bool operator==(unsigned int uid)const;
    bool operator==(std::string userid)const;
    bool operator==(const char* userid)const;
    bool operator==(const User& U)const;
    bool valid()const;
    bool multipleLogins()const;
    const std::string& ip()const;
    std::ostream& print(std::ostream& os)const;
    const char* userid()const;
    const char* homedir()const;
  };
  std::ostream& operator<<(std::ostream& os, const User& U);

  extern User theUser;
#endif