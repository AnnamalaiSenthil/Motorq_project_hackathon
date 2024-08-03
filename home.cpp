// This is a cpp version that proviodes a terminal based system to maintain data

// I have added functions for each of the operations instead of maintaing if else cases

#include<iostream>
#include<cmath>
#include<vector>
#include<string>
#include<algorithm>
#include<set>
#include<map>
#include<queue>
#include<stack>
#include <iomanip>
#include <functional>
#include<cstring>
#include<cassert>
#include <ext/pb_ds/assoc_container.hpp> // Common file
#include <ext/pb_ds/tree_policy.hpp>
#include <functional> // for less
#include <chrono>
#include <iomanip> // For std::put_time
#include <ctime>   // For std::localtime
#include<cstdlib>
    
using namespace __gnu_pbds;
using namespace std;
#define ll long long
#define int long long
#define high 1000000007
    //#define mod 1000000007 // (1e9+7)
#define mod 998244353
#define f first
#define s second
#define vi vector<int> 
#define vvi vector<vector<int>>
#define vpii vector<pair<int,int>>
#define spii set<pair<int,int>>
#define mp make_pair
#define all(s) s.begin(),s.end()
#define pb push_back
#define mt make_tuple
#define fast                      \
    ios_base::sync_with_stdio(0); \
    cin.tie(NULL);                \
    cout.tie(NULL)
 
typedef tree<int, null_type, less_equal<int>, rb_tree_tag,
			tree_order_statistics_node_update>
	ordered_multiset;
typedef tree<int, null_type, less<int>, rb_tree_tag,
            tree_order_statistics_node_update>
    ordered_set;
    
    
// #pragma GCC optimize("O2")                // use as last resort
// #pragma GCC optimize("Ofast")
// #pragma GCC target("avx,avx2,fma")




class Conference {
public:
    std::string name;
    std::string location;
    std::vector<std::string> topics;
    int conference_id;

    std::string startDate;
    std::string startTime;
    std::string endDate;
    std::string endTime;
    int available_slots;
    std::vector<int> Waiting_list;

    // Default constructor
    Conference()
        : name(""), location(""), conference_id(0), startDate(""), startTime(""),
          endDate(""), endTime(""), available_slots(0), Waiting_list() {}

    // Parameterized constructor
    Conference(const std::string& name, const std::string& location, const std::vector<std::string>& topics,
               const std::string& startDate, const std::string& startTime,
               const std::string& endDate, const std::string& endTime,
               int available_slots, int conference_id, const std::vector<int>& Waiting_list)
        : name(name), location(location), topics(topics),
          startDate(startDate), startTime(startTime), endDate(endDate), endTime(endTime),
          available_slots(available_slots), conference_id(conference_id), Waiting_list(Waiting_list) {}
};


class User {
public:
    std::string username;
    int id;
    std::vector<std::string> topics;
    std::set<int> myBookings;

    // Default constructor
    User() : id(0) {}

    // Parameterized constructor
    User(const std::string& username, int id, const std::vector<std::string>& topics, const std::set<int>& myBookings)
        : username(username), id(id), topics(topics), myBookings(myBookings) {}
};

class Booking {
public:
    int booking_id;
    int status; // 1-Confirmed, 2-Waiting-List, 3-Waiting_for_Confirmation, 4-Deleted
    int start_time; // Consider using a more precise time representation if needed
    int user_id;
    int conference_id;

    // Default constructor
    Booking()
        : booking_id(0), status(0), start_time(0), user_id(0), conference_id(0) {}

    // Parameterized constructor
    Booking(int booking_id, int user_id, int conference_id, int status, int start_time)
        : booking_id(booking_id), user_id(user_id), conference_id(conference_id), status(status), start_time(start_time) {}
};

int conference_id=1;
int user_id=1;
int booking_id=1;

map<string,int> Username_userid_translation;
map<string,int> ConferenceName_ConferenceId_translation;
map<int,Conference> ID_conference_mapping;
map<int,User> ID_user_mapping;
map<int,Booking> ID_booking_mapping;


bool isValidTimestamp(const std::string& date,const std::string& time) {
    // struct tm tm;
    // return strptime(timestamp.c_str(), "%Y-%m-%lld %H:%M:%S", &tm) != NULL;
    // Check if the date string is exactly 10 characters long
    if (date.size() != 10) {
        return false;
    }

    // Check if the delimiters are correct
    if (date[2] != '-' || date[5] != '-') {
        return false;
    }

    // Extract the day, month, and year parts
    std::string dayStr = date.substr(0, 2);
    std::string monthStr = date.substr(3, 2);
    std::string yearStr = date.substr(6, 4);

    // Convert them to integers
    int day, month, year;
    std::istringstream(dayStr) >> day;
    std::istringstream(monthStr) >> month;
    std::istringstream(yearStr) >> year;

    // Basic range checks for day, month, and year
    if (month < 1 || month > 12 || day < 1 || day > 31 || year < 1) {
        return false;
    }

    // Check for valid days in the month
    bool leap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
    if (month == 2) {
        if (leap) {
            if (day > 29) return false;
        } else {
            if (day > 28) return false;
        }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) return false;
    } else {
        if (day > 31) return false;
    }
    if (time.size() != 5) {
        return false;
    }

    // Check if the delimiter is correct
    if (time[2] != ':') {
        return false;
    }

    // Extract the hour and minute parts
    std::string hourStr = time.substr(0, 2);
    std::string minuteStr = time.substr(3, 2);

    // Convert them to integers
    int hour, minute;
    std::istringstream(hourStr) >> hour;
    std::istringstream(minuteStr) >> minute;

    // Check if hours and minutes are within valid ranges
    if (hour < 0 || hour >= 24 || minute < 0 || minute >= 60) {
        return false;
    }
    return 1;
}

bool isValidDuration(const std::string& start, const std::string& end) {
    struct tm tm_start, tm_end;
    strptime(start.c_str(), "%Y-%m-%lld %H:%M:%S", &tm_start);
    strptime(end.c_str(), "%Y-%m-%lld %H:%M:%S", &tm_end);

    time_t start_time = mktime(&tm_start);
    time_t end_time = mktime(&tm_end);
    double duration = difftime(end_time, start_time) / 3600; // Duration in hours
    cout << duration << "\n";
    return duration > 0 && duration <= 12;
}

int toTotalMinutes(int day, int month, int year, int hour, int minute) {
    // Days per month for non-leap years
    static const int daysPerMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Calculate total days from years
    int days = (year - 1) * 365 + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400;

    // Calculate total days for months in the given year
    // for (int m = 1; m < month; ++m) {
    //     days += daysPerMonth[m - 1];
    //     if (m == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)) {
    //         days++; // Leap year day
    //     }
    // }
    
    // Add the days in the current month
    days += (day - 1);

    // Total minutes
    int totalMinutes = days * 1440 + hour * 60 + minute;
    return 60;
}

// Calculate duration in hours between two date-time instances
double calculateDurationInHours(const std::string& startDate, const std::string& startTime, const std::string& endDate, const std::string& endTime) {
    // Parse start date-time
    int day1, month1, year1, hour1, minute1;
    std::sscanf(startDate.c_str(), "%lld-%lld-%lld", &day1, &month1, &year1);
    std::sscanf(startTime.c_str(), "%lld:%lld", &hour1, &minute1);

    // Parse end date-time
    int day2, month2, year2, hour2, minute2;
    std::sscanf(endDate.c_str(), "%lld-%lld-%lld", &day2, &month2, &year2);
    std::sscanf(endTime.c_str(), "%lld:%lld", &hour2, &minute2);

    // Convert to total minutes
    int totalMinutes1 = toTotalMinutes(day1, month1, year1, hour1, minute1);
    int totalMinutes2 = toTotalMinutes(day2, month2, year2, hour2, minute2);

    // Calculate the difference and convert to hours
    double durationInHours = static_cast<double>(totalMinutes2 - totalMinutes1) / 60.0;
    return durationInHours;
}

time_t convertToTimeT(const std::string& date, const std::string& time) {
    struct tm tm = {};
    std::stringstream ss(date + " " + time);
    ss >> std::get_time(&tm, "%lld-%m-%Y %H:%M");
    return mktime(&tm);
}

// Function to check if two date-time ranges overlap
bool doRangesIntersect(const std::string& start1_date, const std::string& start1_time,
                       const std::string& end1_date, const std::string& end1_time,
                       const std::string& start2_date, const std::string& start2_time,
                       const std::string& end2_date, const std::string& end2_time) {

    // Convert date-time strings to time_t
    time_t start1 = convertToTimeT(start1_date, start1_time);
    time_t end1 = convertToTimeT(end1_date, end1_time);
    time_t start2 = convertToTimeT(start2_date, start2_time);
    time_t end2 = convertToTimeT(end2_date, end2_time);

    // Check if ranges overlap
    return (start1 <= end2 && end1 >= start2);
}

bool overlap_checker(int user_id,int conference_id)
{
    auto myuser=ID_user_mapping[user_id];
    auto myconference=ID_conference_mapping[conference_id];
    int flag=0;
    for(auto it:myuser.myBookings)
    {
        auto already_booked_Conference=ID_conference_mapping[it];
        if(doRangesIntersect(myconference.startDate,myconference.startTime,myconference.endDate,myconference.endTime,already_booked_Conference.startDate,already_booked_Conference.startTime,already_booked_Conference.endDate,already_booked_Conference.endTime))
        {
            flag=1;
        }
    }
    return flag;
}

void Add_Conference_fn(set<string> & conferences)
{
    
    std::string name;
    std::string location;
    std::string topics_str;
    std::string start_date,start_time,end_date,end_time;
    std::string end_timestamp;
    int available_slots;

    // Input Name
    getline(std::cin, name);
    cout << "Enter Name (alphanumeric with spaces allowed): " << std::flush;
    getline(std::cin, name);
    cout << name << "\n";
    // Input Location
    cout << "Enter Location (alphanumeric with spaces allowed): " << std::flush;
    getline(std::cin, location);

    // Input Topics
    cout << "Enter Topics (comma separated, max 10): ";
    getline(std::cin, topics_str);

    // Validate Topics
    std::vector<std::string> topics;
    std::stringstream ss(topics_str);
    std::string topic;
    while (std::getline(ss, topic, ',')) {
        topic.erase(0, topic.find_first_not_of(" ")); // Trim leading spaces
        topic.erase(topic.find_last_not_of(" ") + 1); // Trim trailing spaces
        if (topic.empty()) continue;
        if (topics.size() >= 10) {
            cout << "Error: Too many topics. Maximum allowed is 10." << std::endl;
            continue;
        }
        topics.push_back(topic);
    }

    // Input Start Timestamp
    std::cout << "Enter Start Date(DD-MM-YYYY): ";
    std::getline(std::cin, start_date);
    std::cout << "Enter Start Time(HH:MM): ";
    std::getline(std::cin, start_time);
    if (!isValidTimestamp(start_date,start_time)) {
        cout << "Error: Invalid Start Timestamp format." << std::endl;
        return;
    }

    // Input End Timestamp
    std::cout << "Enter End Date(DD-MM-YYYY): ";
    std::getline(std::cin, end_date);
    std::cout << "Enter End Time(HH:MM): ";
    std::getline(std::cin, end_time);
    if (!isValidTimestamp(end_date,end_time)) {
        cout << "Error: Invalid End Timestamp format." << std::endl;
        return;
    }

    double duration=calculateDurationInHours(start_date,start_time,end_date,end_time);

    std::cout << "Enter Available Slots (integer greater than 0): ";
    std::cin >> available_slots;
    if (available_slots <= 0) {
        cout << "Error: Available Slots must be greater than 0." << std::endl;
        return;
    }
    if(conferences.find(name)==conferences.end())
    {
        // Safe to insert
        vector<int> Waiting_list_empty;
        Conference myConference(name, location, topics, start_date, start_time, end_date, end_time,available_slots,conference_id,Waiting_list_empty);
        ID_conference_mapping[conference_id]=myConference;
        conference_id++;
        conferences.insert(name);
    }
    else
    {
        cout << "Failure - A conference by the same name already exists\n";
    }

}

void Add_User(set<string> & users)
{
    cout << "Enter Username :\n";
    string username;
    cin >> username;
    string str_topics;
    cout << "Enter Topics (comma separated, max 10): ";
    getline(std::cin, str_topics);

    // Validate Topics
    std::vector<std::string> topics;
    std::stringstream ss(str_topics);
    std::string topic;
    while (std::getline(ss, topic, ',')) {
        topic.erase(0, topic.find_first_not_of(" ")); // Trim leading spaces
        topic.erase(topic.find_last_not_of(" ") + 1); // Trim trailing spaces
        if (topic.empty()) continue;
        if (topics.size() >= 10) {
            cout << "Error: Too many topics. Maximum allowed is 10." << std::endl;
            continue;
        }
        topics.push_back(topic);
    }
    if(users.find(username)==users.end())
    {
        int cur_id=user_id;
        set<int> myBookings_empty;
        User myUser = User(username,cur_id,topics,myBookings_empty);
        ID_user_mapping[user_id]=myUser;
        user_id++;
        users.insert(username);
    }
    else
    {
        cout << "Failure - A user by the same name already exists\n";
    }
}

void Add_Booking()
{
    int userid,conferenceid;
    cout << "Enter Username" << "\n";
    string username;
    cin >> username;
    if(!Username_userid_translation[username])
    {
        cout << "Error: Sorry, such a user doesnt exist\n";
        return;
    }
    userid=Username_userid_translation[username];
    string conference_name;
    cout << "Enter Conference name" << "\n";
    cin >> conference_name;
    if(!ConferenceName_ConferenceId_translation[conference_name])
    {
        cout << "Error: Sorry, such a user doesnt exist\n";
        return;
    }
    conferenceid=ConferenceName_ConferenceId_translation[conference_name];
    int flag=overlap_checker(userid,conference_id);
    if(flag)
    {
        cout << "Error: User already has a booking at the same time\n";
        return;
    }
    cout << "Your booking id is: " << booking_id << "\n";
    int status=0;
    if(ID_conference_mapping[conferenceid].available_slots>0 and ID_conference_mapping[conferenceid].Waiting_list.size()==0)
    {
        status=1;
        ID_conference_mapping[conferenceid].available_slots--;
        cout << "Booking was succesful\n";
    }
    else
    {
        status=2;
        ID_conference_mapping[conferenceid].Waiting_list.push_back(booking_id);
        cout << "Booking is placed on hold: Waiting List\n";
    }
    int starttime=0;
    Booking mybooking=Booking(booking_id,user_id,conference_id,status,starttime);
    ID_booking_mapping[booking_id]=mybooking;
    ID_user_mapping[userid].myBookings.insert(booking_id);
    booking_id++;
}

void Get_Status()
{
    int id;
    cout << "Enter the booking id: ";
    cin >> id;
    if(booking_id>id)
    {
        // Booking exists
        auto myBooking=ID_booking_mapping[id];
        int status=myBooking.status;
        if(status==4)
        {
            cout << "Booking does not exist\n";
        }
        else if(status==1)
        {
            cout << "Confirmed Booking\n";
        }
        else if(status==2)
        {
            cout << "In Waiting List\n";
        }
        else if(status==3)
        {
            cout << "Waiting for Confirmation\n";
            cout << myBooking.start_time << "\n";
        }
    }
    else
    {
        cout << "Error:booking does not exist\n";
    }
}

void Confirm_booking()
{
    int id;
    cout << "Enter the booking id: ";
    cin >> id;
    if(booking_id<=id or ID_booking_mapping[id].status!=3)
    {
        cout << "Inaccessible booking id\n";
    }
    else
    {
        auto myBooking=ID_booking_mapping[id];
        Conference myConference=ID_conference_mapping[myBooking.conference_id];
        myBooking.status=1;
        User myUser=ID_user_mapping[myBooking.user_id];
        myConference.Waiting_list.erase(myConference.Waiting_list.begin());
    }
}

void Delete_booking()
{
    int id;
    cout << "Enter the booking id: ";
    cin >> id;
    if(booking_id>id)
    {
        // Booking exists
        auto myBooking=ID_booking_mapping[id];
        int status=myBooking.status;
        if(status==4)
        {
            cout << "Booking does not exist\n";
        }
        else
        {
            Conference myConference=ID_conference_mapping[myBooking.conference_id];
            User myUser=ID_user_mapping[myBooking.user_id];
            myUser.myBookings.erase(booking_id);
            ID_booking_mapping[booking_id].status=4;
            if(status==1)
            {
                myConference.available_slots++;
                if(myConference.Waiting_list.size()!=0)
                {
                    auto waiting_booking_id=myConference.Waiting_list[0];
                    auto waiting_Booking=ID_booking_mapping[waiting_booking_id];
                    myConference.Waiting_list.erase(myConference.Waiting_list.begin());
                    waiting_Booking.status=3;
                    auto now = std::chrono::system_clock::now();

                    // Convert the current time to a time_t object
                    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
                    
                    // Convert the time_t object to a tm struct for local time
                    std::tm* now_tm = std::localtime(&now_c);
                    waiting_Booking.start_time=now_tm->tm_min;
                }
            }
            else if(status==2)
            {
                auto iterator=myConference.Waiting_list.begin();
                while(iterator!=myConference.Waiting_list.end())
                {
                    if(*iterator==booking_id)
                    {
                        myConference.Waiting_list.erase(iterator);
                        break;
                    }
                    iterator++;
                }
            }
            else if(status==3)
            {
                myConference.Waiting_list.erase(myConference.Waiting_list.begin());
                if(myConference.Waiting_list.size()!=0)
                {
                    auto waiting_booking_id=myConference.Waiting_list[0];
                    auto waiting_Booking=ID_booking_mapping[waiting_booking_id];
                    waiting_Booking.status=3;
                    auto now = std::chrono::system_clock::now();

                    // Convert the current time to a time_t object
                    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
                    
                    // Convert the time_t object to a tm struct for local time
                    std::tm* now_tm = std::localtime(&now_c);
                    waiting_Booking.start_time=now_tm->tm_min;
                }
            }
        }
    }
    else
    {
        cout << "Booking does not exist\n";
    }
}

int32_t main()
{
    set<string> conferences;
    set<string> users;
    while(1)
    {
        cout << "1. Add Conference" << "\n";
        cout << "2. Add User\n";
        cout << "3. Book a Conference\n";
        cout << "4. Booking Status\n";
        cout << "5. Confirm Waitlist Booking\n";
        cout << "6. Cancel Booking\n";
        cout << "7. Quit\n";
        cout << "\n\n";
        int choice;
        cin >> choice;
        if(choice==7)
        {
            break;
        }
        else if(choice == 1)
        {
            Add_Conference_fn(conferences);
        }
        else if(choice == 2)
        {
            Add_User(users);
        }
        else if(choice == 3)
        {
            Add_Booking();
        }
        else if(choice == 4)
        {
            Get_Status();
        }
        else if(choice==5)
        {
            Confirm_booking();
        }
        else if(choice==6)
        {
            Delete_booking();
        }
        cout << "Press any key to continue\n";
        char ch;
        cin >> ch;
        system("clear");
    }
}