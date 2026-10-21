#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
typedef SOCKET socket_t;
#define CLOSESOCKET closesocket
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
typedef int socket_t;
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#define CLOSESOCKET close
#endif

#include "include/Student.h"
#include "include/Academic.h"
#include "include/Career.h"
#include "include/Dashboard.h"

using namespace std;

static string trim(const string& s)
{
    size_t a=s.find_first_not_of(" \t\r\n");
    if(a==string::npos) return "";
    size_t b=s.find_last_not_of(" \t\r\n");
    return s.substr(a,b-a+1);
}

static string jsonEscape(const string& s)
{
    string r;
    for(char c:s)
    {
        if(c=='"') r += "\\\"";
        else if(c=='\\') r += "\\\\";
        else if(c=='\n') r += "\\n";
        else if(c=='\r') r += "\\r";
        else r += c;
    }
    return r;
}

static string getJsonField(const string& body, const string& key)
{
    string marker="\""+key+"\"";
    size_t p=body.find(marker);
    if(p==string::npos) return "";
    p=body.find(':',p+marker.size());
    if(p==string::npos) return "";
    ++p;
    while(p<body.size() && isspace((unsigned char)body[p])) ++p;
    if(p<body.size() && body[p]=='"')
    {
        ++p; string r;
        bool esc=false;
        for(;p<body.size();++p)
        {
            char c=body[p];
            if(esc){ r += c; esc=false; continue; }
            if(c=='\\'){ esc=true; continue; }
            if(c=='"') break;
            r += c;
        }
        return r;
    }
    size_t e=p;
    while(e<body.size() && body[e]!=',' && body[e]!='}') ++e;
    return trim(body.substr(p,e-p));
}

static map<string,string> parseQuery(const string& q)
{
    map<string,string> out;
    size_t start=0;
    while(start<q.size())
    {
        size_t amp=q.find('&',start); if(amp==string::npos) amp=q.size();
        string part=q.substr(start,amp-start); size_t eq=part.find('=');
        if(eq!=string::npos) out[part.substr(0,eq)]=part.substr(eq+1);
        start=amp+1;
    }
    return out;
}

static string readFile(const string& path)
{
    ifstream f(path,ios::binary); if(!f) return "";
    return string((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
}

static string mimeType(const string& path)
{
    if(path.size()>=5 && path.substr(path.size()-5)==".html") return "text/html; charset=utf-8";
    if(path.size()>=3 && path.substr(path.size()-3)==".js") return "application/javascript; charset=utf-8";
    if(path.size()>=4 && path.substr(path.size()-4)==".css") return "text/css; charset=utf-8";
    return "text/plain; charset=utf-8";
}

static string httpResponse(const string& body, const string& type="application/json; charset=utf-8", int code=200, const string& status="OK")
{
    ostringstream out;
    out << "HTTP/1.1 " << code << ' ' << status << "\r\n"
        << "Content-Type: " << type << "\r\n"
        << "Access-Control-Allow-Origin: *\r\n"
        << "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
        << "Access-Control-Allow-Headers: Content-Type\r\n"
        << "Cache-Control: no-store\r\n"
        << "Content-Length: " << body.size() << "\r\n\r\n" << body;
    return out.str();
}

static string okJson(const string& extra="") { return "{\"success\":true" + extra + "}"; }
static string errorJson(const string& message) { return "{\"success\":false,\"message\":\""+jsonEscape(message)+"\"}"; }

static string profileJson(Student& s)
{
    ostringstream o;
    o << "{\"name\":\""<<jsonEscape(s.getName())<<"\",\"age\":"<<s.getAge()
      << ",\"department\":\""<<jsonEscape(s.getDepartment())<<"\",\"username\":\""<<jsonEscape(s.getUsername())
      << "\",\"gpa\":"<<s.getGPA()<<",\"attendance\":"<<s.getAttendance()<<"}";
    return o.str();
}

static string dashboardJson(const string& username)
{
    Student s; if(!s.loginStudent(username, s.getPassword())) { /* password is not known here */ }
    // Load the student record without exposing password through the API.
    ifstream f("data/users.txt"); string n;
    bool found=false;
    while(getline(f,n))
    {
        string age,dep,user,pass,gpa,att;
        if(!getline(f,age)||!getline(f,dep)||!getline(f,user)||!getline(f,pass)||!getline(f,gpa)||!getline(f,att)) break;
        if(user==username)
        {
            s.loginStudent(user,pass); found=true; break;
        }
    }
    if(!found) return "";

    Academic a; auto assignments=a.getAssignments(username); int pending=a.getPendingAssignments(username);
    Career c; string skill=c.getSkill(username); string career=skill.empty()?"":c.getCareer(skill); int score=career.empty()?0:c.getMatchScore(career);
    Dashboard d; int perf=d.performanceScore(s.getAttendance(),s.getGPA(),pending);
    string status=d.performanceStatus(s.getAttendance(),s.getGPA(),pending);
    string level=d.performanceLevelText(s.getAttendance(),s.getGPA());
    auto alerts=d.alertMessages(s.getAttendance(),s.getGPA(),pending);

    ostringstream o;
    o<<"{\"success\":true,\"user\":"<<profileJson(s)<<",\"skill\":\""<<jsonEscape(skill)
     <<"\",\"career\":\""<<jsonEscape(career)<<"\",\"matchScore\":"<<score
     <<",\"pending\":"<<pending<<",\"performanceScore\":"<<perf
     <<",\"performanceStatus\":\""<<jsonEscape(status)<<"\",\"performanceLevel\":\""<<jsonEscape(level)<<"\",\"alerts\":[";
    for(size_t i=0;i<alerts.size();++i){ if(i) o<<','; o<<"\""<<jsonEscape(alerts[i])<<"\""; }
    o<<"],\"assignments\":[";
    for(size_t i=0;i<assignments.size();++i)
    {
        if(i) o<<',';
        o<<"{\"index\":"<<i<<",\"name\":\""<<jsonEscape(assignments[i].name)<<"\",\"status\":\""<<jsonEscape(assignments[i].status)<<"\"}";
    }
    o<<"]}";
    return o.str();
}

static string handleRequest(const string& method, const string& target, const string& body)
{
    string path=target, query=""; size_t qpos=target.find('?');
    if(qpos!=string::npos){path=target.substr(0,qpos);query=target.substr(qpos+1);}

    if(method=="OPTIONS") return httpResponse("", "text/plain; charset=utf-8", 204, "No Content");

    if(method=="GET" && path=="/")
    {
        string html=readFile("frontend/index.html");
        if(html.empty()) return httpResponse("Frontend not found. Run the server from the project root.","text/plain; charset=utf-8",404,"Not Found");
        return httpResponse(html,"text/html; charset=utf-8");
    }
    if(method=="GET" && (path=="/style.css" || path=="/script.js"))
    {
        string data=readFile("frontend"+path);
        if(data.empty()) return httpResponse("Not Found","text/plain; charset=utf-8",404,"Not Found");
        return httpResponse(data,mimeType(path));
    }

    if(method=="GET" && path=="/api/profile")
    {
        auto p=parseQuery(query); string username=p["username"]; if(username.empty()) return httpResponse(errorJson("Username is required"));
        ifstream f("data/users.txt"); string n;
        while(getline(f,n)){
            string age,dep,user,pass,gpa,att;
            if(!getline(f,age)||!getline(f,dep)||!getline(f,user)||!getline(f,pass)||!getline(f,gpa)||!getline(f,att)) break;
            if(user==username){ Student s; s.loginStudent(user,pass); return httpResponse(okJson(",\"user\":"+profileJson(s))); }
        }
        return httpResponse(errorJson("Student not found"),"application/json; charset=utf-8",404,"Not Found");
    }

    if(method=="GET" && path=="/api/dashboard")
    {
        auto p=parseQuery(query); string username=p["username"]; string result=dashboardJson(username);
        if(result.empty()) return httpResponse(errorJson("Student not found"),"application/json; charset=utf-8",404,"Not Found");
        return httpResponse(result);
    }

    if(method!="POST") return httpResponse(errorJson("Route not found"),"application/json; charset=utf-8",404,"Not Found");

    if(path=="/api/register")
    {
        Student s;
        string name=getJsonField(body,"name"), department=getJsonField(body,"department"), username=getJsonField(body,"username"), password=getJsonField(body,"password");
        int age=stoi(getJsonField(body,"age")); float gpa=stof(getJsonField(body,"gpa")); int attendance=stoi(getJsonField(body,"attendance"));
        if(!s.registerStudent(name,age,department,username,password,gpa,attendance)) return httpResponse(errorJson("Registration failed. Username may already exist or values are invalid."));
        return httpResponse(okJson(",\"user\":"+profileJson(s)));
    }

    if(path=="/api/login")
    {
        Student s; string username=getJsonField(body,"username"), password=getJsonField(body,"password");
        if(!s.loginStudent(username,password)) return httpResponse(errorJson("Invalid username or password."));
        string result=dashboardJson(username);
        return httpResponse(result);
    }

    if(path=="/api/academic/update")
    {
        string username=getJsonField(body,"username"); float gpa=stof(getJsonField(body,"gpa")); int attendance=stoi(getJsonField(body,"attendance"));
        Student s; if(!s.loginStudent(username,s.getPassword())) { /* locate password below */ }
        ifstream f("data/users.txt"); string n; bool ok=false;
        while(getline(f,n)){
            string age,dep,user,pass,oldGpa,att;
            if(!getline(f,age)||!getline(f,dep)||!getline(f,user)||!getline(f,pass)||!getline(f,oldGpa)||!getline(f,att)) break;
            if(user==username){ s.loginStudent(user,pass); ok=s.updateAcademicInfo(gpa,attendance); break; }
        }
        if(!ok) return httpResponse(errorJson("Academic information could not be updated."));
        return httpResponse(dashboardJson(username));
    }

    if(path=="/api/assignments/add")
    {
        string username=getJsonField(body,"username"), name=getJsonField(body,"name"); Academic a;
        if(!a.addAssignment(username,name)) return httpResponse(errorJson("Assignment could not be added."));
        return httpResponse(dashboardJson(username));
    }

    if(path=="/api/assignments/status")
    {
        string username=getJsonField(body,"username"), status=getJsonField(body,"status"); int index=stoi(getJsonField(body,"index")); Academic a;
        if(!a.setAssignmentStatus(username,index,status)) return httpResponse(errorJson("Assignment could not be updated."));
        return httpResponse(dashboardJson(username));
    }

    if(path=="/api/career")
    {
        string username=getJsonField(body,"username"), skill=getJsonField(body,"skill"); Career c;
        if(!c.setSkill(username,skill)) return httpResponse(errorJson("Invalid skill."));
        return httpResponse(dashboardJson(username));
    }

    if(path=="/api/gpa")
    {
        float percentage=stof(getJsonField(body,"percentage")); Academic a; float g=a.calculateGPA(percentage);
        if(g<0) return httpResponse(errorJson("Percentage must be between 0 and 100."));
        ostringstream o; o << "{\"success\":true,\"gpa\":" << g << "}"; return httpResponse(o.str());
    }

    return httpResponse(errorJson("Route not found"),"application/json; charset=utf-8",404,"Not Found");
}

int main()
{
#ifdef _WIN32
    WSADATA wsa; if(WSAStartup(MAKEWORD(2,2),&wsa)!=0){cerr<<"WSAStartup failed.\n";return 1;}
#endif

    socket_t serverSocket=socket(AF_INET,SOCK_STREAM,0);
    if(serverSocket==INVALID_SOCKET){cerr<<"Could not create server socket.\n";return 1;}

    int opt=1;
#ifdef _WIN32
    setsockopt(serverSocket,SOL_SOCKET,SO_REUSEADDR,(const char*)&opt,sizeof(opt));
#else
    setsockopt(serverSocket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
#endif

    sockaddr_in address{}; address.sin_family=AF_INET; address.sin_addr.s_addr=INADDR_ANY; address.sin_port=htons(18080);
    if(bind(serverSocket,(sockaddr*)&address,sizeof(address))==SOCKET_ERROR){cerr<<"Could not bind port 18080.\n";CLOSESOCKET(serverSocket);return 1;}
    if(listen(serverSocket,10)==SOCKET_ERROR){cerr<<"Could not listen.\n";CLOSESOCKET(serverSocket);return 1;}

    cout<<"Smart Academic & Career Decision Intelligence System server started.\n";
    cout<<"Open http://localhost:18080 in your browser.\n";

    while(true)
    {
        sockaddr_in client{};
#ifdef _WIN32
        int len=sizeof(client);
#else
        socklen_t len=sizeof(client);
#endif
        socket_t clientSocket=accept(serverSocket,(sockaddr*)&client,&len);
        if(clientSocket==INVALID_SOCKET) continue;

        string request; char buffer[8192]; int received=0;
        do { received=(int)recv(clientSocket,buffer,sizeof(buffer),0); if(received>0) request.append(buffer,received); } while(received==(int)sizeof(buffer) && request.size()<1024*1024);

        size_t lineEnd=request.find("\r\n");
        if(lineEnd==string::npos){CLOSESOCKET(clientSocket);continue;}
        string requestLine=request.substr(0,lineEnd); string method,target,version; stringstream rl(requestLine); rl>>method>>target>>version;
        size_t headerEnd=request.find("\r\n\r\n"); string body=headerEnd==string::npos?"":request.substr(headerEnd+4);
        size_t contentPos=request.find("Content-Length:");
        if(contentPos!=string::npos)
        {
            size_t e=request.find("\r\n",contentPos); string val=request.substr(contentPos+15,e-contentPos-15); int contentLength=stoi(trim(val));
            while((int)body.size()<contentLength){ int n=recv(clientSocket,buffer,sizeof(buffer),0); if(n<=0) break; body.append(buffer,n); }
        }
        string response=handleRequest(method,target,body);
        send(clientSocket,response.c_str(),(int)response.size(),0);
        CLOSESOCKET(clientSocket);
    }

    CLOSESOCKET(serverSocket);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
