#ifndef _SOCKETMESSAGE
#define _SOCKETMESSAGE

#include <cstdio>
#include <cstdlib>
#include <cstring>

class SocketMessage
{
public:
	enum { header_length = 4 };

	enum { max_body_length = 8192 };

	SocketMessage()
		: _bodyLength(0)
	{
	}

	const char* data() const
	{
		return _data;
	}

	char* data()
	{
		return _data;
	}

	size_t length() const
	{
		return header_length + _bodyLength;
	}

	const char* body() const
	{
		return _data + header_length;
	}

	char* body()
	{
		return _data + header_length;
	}

	size_t body_length() const
	{
		return _bodyLength;
	}

	void body_length(size_t new_length)
	{
		_bodyLength = new_length;
		if (_bodyLength > max_body_length)
			_bodyLength = max_body_length;
	}

	bool decode_header()
	{
		using namespace std; // For strncat and atoi.
		char header[header_length + 1] = "";
		strncat(header, _data, header_length);
		_bodyLength = atoi(header);
		if (_bodyLength > max_body_length)
		{
			_bodyLength = 0;
			return false;
		}
		return true;
	}

	void encode_header()
	{
		using namespace std; // For sprintf and memcpy.
		char header[header_length + 1] = "";
		sprintf(header, "%4d", static_cast<int>(_bodyLength));
		memcpy(_data, header, header_length);
	}

private:
	char _data[header_length + max_body_length];
	size_t _bodyLength;
};

#endif // _SOCKETMESSAGE

