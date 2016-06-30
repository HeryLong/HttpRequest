# HttpRequest
A simple c++ http request library base on socket, without any trouble about dependency.
Now, it just supports get/post request, but i'll continue to add more function. 

## Usage
It's very simple to create a get/post request, after include the header `#include "HttpRequest.h"`, such as:
a get request:
```C++
HttpRequest httpRequest;
httpRequest.connect_server(string server_ip, int port)
	.set_request_uri(string url)
	.set_header(string key, string value)
	.get();
```

a post request:
```C++
HttpRequest httpRequest;
httpRequest.connect_server(string server_ip, int port)
  	.set_request_uri(string url)
	.set_header(string key, string value)
	.post(string post_data);
```
