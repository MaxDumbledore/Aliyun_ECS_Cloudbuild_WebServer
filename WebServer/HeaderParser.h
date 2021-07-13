#ifndef WEBSERVER_HEADERPARSER_H
#define WEBSERVER_HEADERPARSER_H

#include <string>
#include <vector>

class HeaderParser {
   public:
    enum State {
        METHOD_START,
        METHOD,
        URI_PATH,
        URI_KEY,
        URI_VALUE,
        HTTP_VERSION_H,
        HTTP_VERSION_T_1,
        HTTP_VERSION_T_2,
        HTTP_VERSION_P,
        HTTP_VERSION_SLASH,
        HTTP_VERSION_MAJOR_START,
        HTTP_VERSION_MAJOR,
        HTTP_VERSION_MINOR_START,
        HTTP_VERSION_MINOR,
        EXPECTING_NEWLINE_1,
        HEADER_LINE_START,
        HEADER_LWS,
        HEADER_NAME,
        SPACE_BEFORE_HEADER_VALUE,
        HEADER_VALUE,
        EXPECTING_NEWLINE_2,
        EXPECTING_NEWLINE_3
    };

    enum ResultType { GOOD, BAD, INDETERMINATE };

    HeaderParser();

    void reset();

    ResultType append(char c);

    std::string get(const std::string &name);

    const std::string & getUriPath();

    const std::vector<std::pair<std::string, std::string>> & getUriParams();

    const std::string & getMethod();

   private:
    State state;

    std::string method,uriPath;
    int majorVersion, minorVersion;
    std::vector<std::pair<std::string, std::string>> uriParams,headers;
};

#endif