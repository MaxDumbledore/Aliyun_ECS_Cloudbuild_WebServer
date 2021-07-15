#include "HeaderParser.h"

HeaderParser::HeaderParser() : state(METHOD_START) {}

void HeaderParser::reset() {
    state = METHOD_START;
    method.clear();
    uriPath.clear();
    headers.clear();
}

inline bool isChar(char c) {
    return c >= 0;
}

inline bool isCtl(char c) {
    return c >= 0 && c <= 31 || c == 127;
}

inline bool isSpecial(char c) {
    switch (c) {
        case '(':
        case ')':
        case '<':
        case '>':
        case '@':
        case ',':
        case ';':
        case ':':
        case '\\':
        case '"':
        case '/':
        case '[':
        case ']':
        case '?':
        case '=':
        case '{':
        case '}':
        case ' ':
        case '\t':
            return true;
        default:
            return false;
    }
}

inline bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

std::string decode(std::string& src) {
    std::string ret;
    char ch;
    int ii;
    for (int i = 0; i < src.size(); i++) {
        if (src[i] == '%') {
            sscanf(src.substr(i + 1, 2).c_str(), "%x", &ii);
            ret += static_cast<char>(ii);
            i += 2;
        } else
            ret += src[i];
    }
    return (ret);
}

HeaderParser::ResultType HeaderParser::append(char c) {
    switch (state) {
        case METHOD_START:
            if (!isChar(c) || isCtl(c) || isSpecial(c)) {
                return BAD;
            } else {
                state = METHOD;
                method += c;
                return INDETERMINATE;
            }
        case METHOD:
            if (c == ' ') {
                state = URI_PATH;
                return INDETERMINATE;
            } else if (!isChar(c) || isCtl(c) || isSpecial(c)) {
                return BAD;
            } else {
                method += c;
                return INDETERMINATE;
            }
        case URI_PATH:
            if (c == ' ') {
                state = HTTP_VERSION_H;
                uriPath=decode(uriPath); //
                return INDETERMINATE;
            } else if (isCtl(c)) {
                return BAD;
            } else if (c == '?') {
                state = URI_KEY;
                uriPath=decode(uriPath); //
                uriParams.emplace_back();
                return INDETERMINATE;
            } else {
                uriPath += c;
                return INDETERMINATE;
            }
        case URI_KEY:
            if (c == ' ') {
                state = HTTP_VERSION_H;
                return INDETERMINATE;
            } else if (isCtl(c)) {
                return BAD;
            } else if (c == '=') {
                state = URI_VALUE;
                return INDETERMINATE;
            } else {
                uriParams.back().first += c;
                return INDETERMINATE;
            }
        case URI_VALUE:
            if (c == ' ') {
                state = HTTP_VERSION_H;
                uriParams.back().second=decode(uriParams.back().second);//
                return INDETERMINATE;
            } else if (isCtl(c)) {
                return BAD;
            } else if (c == '&') {
                state = URI_KEY;
                uriParams.back().second=decode(uriParams.back().second);//
                uriParams.emplace_back();
                return INDETERMINATE;
            } else {
                uriParams.back().second += c;
                return INDETERMINATE;
            }
        case HTTP_VERSION_H:
            if (c == 'H') {
                state = HTTP_VERSION_T_1;
                return INDETERMINATE;
            } else {
                return BAD;
            }
        case HTTP_VERSION_T_1:
            if (c == 'T') {
                state = HTTP_VERSION_T_2;
                return INDETERMINATE;
            } else {
                return BAD;
            }
        case HTTP_VERSION_T_2:
            if (c == 'T') {
                state = HTTP_VERSION_P;
                return INDETERMINATE;
            } else {
                return BAD;
            }
        case HTTP_VERSION_P:
            if (c == 'P') {
                state = HTTP_VERSION_SLASH;
                return INDETERMINATE;
            } else {
                return BAD;
            }
        case HTTP_VERSION_SLASH:
            if (c == '/') {
                majorVersion = 0;
                minorVersion = 0;
                state = HTTP_VERSION_MAJOR_START;
                return INDETERMINATE;
            } else {
                return BAD;
            }
        case HTTP_VERSION_MAJOR_START:
            if (isDigit(c)) {
                majorVersion = majorVersion * 10 + c - '0';
                state = HTTP_VERSION_MAJOR;
                return INDETERMINATE;
            } else {
                return BAD;
            }
        case HTTP_VERSION_MAJOR:
            if (c == '.') {
                state = HTTP_VERSION_MINOR_START;
                return INDETERMINATE;
            } else if (isDigit(c)) {
                majorVersion = majorVersion * 10 + c - '0';
                return INDETERMINATE;
            } else {
                return BAD;
            }
        case HTTP_VERSION_MINOR_START:
            if (isDigit(c)) {
                minorVersion = minorVersion * 10 + c - '0';
                state = HTTP_VERSION_MINOR;
                return INDETERMINATE;
            } else {
                return BAD;
            }
        case HTTP_VERSION_MINOR:
            if (c == '\r') {
                state = EXPECTING_NEWLINE_1;
                return INDETERMINATE;
            } else if (isDigit(c)) {
                minorVersion = minorVersion * 10 + c - '0';
                return INDETERMINATE;
            } else {
                return BAD;
            }
        case EXPECTING_NEWLINE_1:
            if (c == '\n') {
                state = HEADER_LINE_START;
                return INDETERMINATE;
            } else {
                return BAD;
            }
        case HEADER_LINE_START:
            if (c == '\r') {
                state = EXPECTING_NEWLINE_3;
                return INDETERMINATE;
            } else if (!headers.empty() && (c == ' ' || c == '\t')) {
                state = HEADER_LWS;
                return INDETERMINATE;
            } else if (!isChar(c) || isCtl(c) || isSpecial(c)) {
                return BAD;
            } else {
                headers.emplace_back();
                headers.back().first += c;
                state = HEADER_NAME;
                return INDETERMINATE;
            }
        case HEADER_LWS:
            if (c == '\r') {
                state = EXPECTING_NEWLINE_2;
                return INDETERMINATE;
            } else if (c == ' ' || c == '\t') {
                return INDETERMINATE;
            } else if (isCtl(c)) {
                return BAD;
            } else {
                state = HEADER_VALUE;
                headers.back().second += c;
                return INDETERMINATE;
            }
        case HEADER_NAME:
            if (c == ':') {
                state = SPACE_BEFORE_HEADER_VALUE;
                return INDETERMINATE;
            } else if (!isChar(c) || isCtl(c) || isSpecial(c)) {
                return BAD;
            } else {
                headers.back().first += c;
                return INDETERMINATE;
            }
        case SPACE_BEFORE_HEADER_VALUE:
            if (c == ' ') {
                state = HEADER_VALUE;
                return INDETERMINATE;
            } else {
                return BAD;
            }
        case HEADER_VALUE:
            if (c == '\r') {
                state = EXPECTING_NEWLINE_2;
                return INDETERMINATE;
            } else if (isCtl(c)) {
                return BAD;
            } else {
                headers.back().second += c;
                return INDETERMINATE;
            }
        case EXPECTING_NEWLINE_2:
            if (c == '\n') {
                state = HEADER_LINE_START;
                return INDETERMINATE;
            } else {
                return BAD;
            }
        case EXPECTING_NEWLINE_3:
            return (c == '\n') ? GOOD : BAD;
        default:
            return BAD;
    }
}

std::string HeaderParser::get(const std::string& name) {
    for (auto& i : headers)
        if (i.first == name)
            return i.second;
    return "";
}

const std::string& HeaderParser::getUriPath() {
    return uriPath;
}

const std::vector<std::pair<std::string, std::string>>&
HeaderParser::getUriParams() {
    return uriParams;
}

const std::string& HeaderParser::getMethod() {
    return method;
}
