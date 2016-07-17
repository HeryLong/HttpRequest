/*
 * HttpRequest.cpp
 *
 *  Created on: 2016年6月28日
 *      Author: Hery Long
 */

#include "HttpRequest.h"

HttpRequest::HttpRequest() {
	// TODO Auto-generated constructor stub
	this->socket_fd = 0;
	memset(&this->sock_info, 0, sizeof(sock_info));

	this->Request_Uri = "";
	this->Request_Line = "";
	this->Request_Header.clear();

	this->Response = "";
	//this->Response_Line = "";
	//this->Response_Header.clear();
	//this->Response_Body = "";
}

HttpRequest::~HttpRequest() {
	// TODO Auto-generated destructor stub
	close(socket_fd);

	this->Request_Uri = "";
	this->Request_Line = "";
	this->Request_Header.clear();

	this->Response = "";
	//this->Response_Line = "";
	//this->Response_Header.clear();
	//this->Response_Body = "";
}

int HttpRequest::close_socket() {
	close(socket_fd);

	this->Request_Uri = "";
	this->Request_Line = "";
	this->Request_Header.clear();

	this->Response = "";
	//this->Response_Line = "";
	//this->Response_Header.clear();
	//this->Response_Body = "";
	return 0;
}

HttpRequest& HttpRequest::connect_server(const std::string ip, const int port) {
	// create socket_fd
	this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(this->socket_fd == 0) {
		std::cout<<"HttpClient: failed to create a new socket\n";
		throw HttpRequestException("Failed to create a new socket.\n");
	}
	
#ifdef __DEBUG__
	std::cout<<"success to create socket.\n";
#endif

	// configure the sockaddr_in
	this->sock_info.sin_addr.s_addr = inet_addr(ip.c_str());
	this->sock_info.sin_family = AF_INET;
	this->sock_info.sin_port = htons(port);

	// connect to server with ip:port
	if(connect(socket_fd, (sockaddr*)&sock_info, sizeof(sock_info)) < 0) {
		std::cout<<"HttpClient: failed to connect to the server with" + ip + ":" + to_string(port) + ".\n";
		throw HttpRequestException("Failed to connect to the server.\n");
	}
	
#ifdef __DEBUG__
	std::cout<<"success to connect.\n";
#endif

	return *this;
}

HttpRequest& HttpRequest::set_request_uri(const std::string uri) {
	this->Request_Uri = uri;
	return *this;
}

HttpRequest& HttpRequest::set_header(const std::string key, const std::string value) {
	// if [this->Request_Header] has the key, then edit the value
	// else add the new pair of [key-value]
	this->Request_Header[key] = value;
	return *this;
}

HttpRequest& HttpRequest::get() {
	// check socket is_open();

	// prepare the http message
	std::string message("");
	this->Request_Line = "GET " + this->Request_Uri + " HTTP/1.1\r\n";
	message += this->Request_Line;
	for(std::map<std::string, std::string>::iterator iter = this->Request_Header.begin();
			iter != this->Request_Header.end();
			iter++ ) {
		message += (iter->first + ":" + iter->second + "\r\n");
	}
	message += "\r\n";
#ifdef __DEBUG__
	std::cout<<message<<std::endl;
#endif

	// send message to server
	if(send(this->socket_fd, message.c_str(), strlen(message.c_str()), 0) < 0) {
		std::cout<<"HttpClient: failed to send message to server.\n";
		throw HttpRequestException("Failed to send message to server.\n");
	}
	
#ifdef __DEBUG__
	std::cout<<"success to send message.\n";
#endif

	// receive message from server
	message = "";
	char buffer[BUFFER_SIZE] = { '\0' };
	while(recv(this->socket_fd, buffer, BUFFER_SIZE, 0) > 0) {
		message += std::string(buffer);
		memset(buffer, '\0', BUFFER_SIZE);
	}
	this->Response = message;

	return *this;
}

HttpRequest& HttpRequest::post(std::string data) {
	// check socket is_open();

	// prepare the http message
	std::string message("");
	this->Request_Line = "POST " + this->Request_Uri + " HTTP/1.1\r\n";
	message += this->Request_Line;
	for(std::map<std::string, std::string>::iterator iter = this->Request_Header.begin();
			iter != this->Request_Header.end();
			iter++ ) {
		message += (iter->first + ":" + iter->second + "\r\n");
	}
	message += "Content-Length:" + to_string(data.length()) + "\r\n";
	message += "Content-Type: application/x-www-form-urlencoded\r\n";
	message += "\r\n";
	message += data.c_str();
	
#ifdef __DEBUG__
	std::cout<<message<<std::endl;
#endif

	// send message to server
	if(send(this->socket_fd, message.c_str(), strlen(message.c_str()), 0) < 0) {
		std::cout<<"HttpClient: failed to send message to server.\n";
		throw HttpRequestException("Failed to send message to server.\n");
	}

	// receive message from server
	message = "";
	char buffer[BUFFER_SIZE] = { '\0' };
	while(recv(this->socket_fd, buffer, BUFFER_SIZE, 0) > 0) {
		message += std::string(buffer);
		memset(buffer, '\0', BUFFER_SIZE);
	}
	this->Response = message;

	return *this;
}

