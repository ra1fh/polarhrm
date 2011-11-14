#ifndef _WATCHDATA_H
#define _WATCHDATA_H


#include <string>
using namespace std;

class Reminder;
class User;
class WatchTime;
class Bike;
class Overview;
class RawSessions;
class RawSession;
class Session;
class Time;

class WatchData {
  private:
    Reminder * reminder[5];

    User * user;

    WatchTime * time;

    Bike * bikes[2];

    Overview * overview;


  public:
    int initObjects();


  private:
    <RawSession *> rawsessions;

    <Session *> sessions;

};
class Date {
  private:
    Time * time;

    unsigned char year;

    unsigned char month;

    unsigned char day;


  public:
    string toString();

     setDate(unsigned char year, unsigned char month, unsigned char day);


  private:
    WatchTime * date;

};
#endif
