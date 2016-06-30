/*
 * HttpRequest.h
 *
 *  Created on: 2016年6月28日
 *      Author: Hery Long
 *	A simple lib for http get/post usage;
 */

#ifndef HTTPREQUEST_H_
#define HTTPREQUEST_H_

#define __LINUX__
//#define __WIN32__

//#define __DEBUG__

#ifdef __LINUX__
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#ifdef __WIN32__
#include <winsock2.h>
#endif

#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
#include <map>

#define BUFFER_SIZE 1024

// exception to indicate the failed reason
class HttpRequestException {
private:
	std::string message;

public:
	HttpRequestException() { }
	HttpRequestException(std::string msg) : message(msg) { }

	const char* what() {
		return this->message.c_str();
	}
};

class HttpRequest {
private:
	int socket_fd;
	sockaddr_in sock_info;

public:
	HttpRequest();
	~HttpRequest();

	// close the socket connection
	int close_socket();

	// store the request line detail
	std::string Request_Line;
	// store the request header info
	std::map<std::string, std::string> Request_Header;

	// store the response line detail
	std::string Response;
	// store the response header info
	//std::map<std::string, std::string> Response_Header;
	// store the response body
	//std::string Response_Body;

	// inline function to transfer [int] to [string]
	inline std::string to_string(int value) {
		char tmp[8] = { '\0' };
		sprintf(tmp, "%d", value);
		return std::string(tmp);
	}

	// connect to ip:port
	HttpRequest& connect_server(const std::string ip, const int port);
	// set the request uri
	HttpRequest& set_request_uri(const std::string uri);
	// set optional header info
	HttpRequest& set_header(const std::string key, const std::string value);
	// get msg with current info
	HttpRequest& get();
	// post msg with current info
	HttpRequest& post(const std::string data);
};

#endif /* HTTPREQUEST_H_ */
