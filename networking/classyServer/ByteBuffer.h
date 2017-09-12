#ifndef _BYTE_BUFFER_H_
#define _BYTE_BUFFER_H_
#include <vector>
#include <queue>
#include <string>

class ByteBuffer
{
public:
	ByteBuffer();
	
	// read data from file descriptor
	// return number of bytes read [ > 0: ok; == 0: close; < 0: error ]
	// should only be called if select() says there is data to be read
	ssize_t read(int fd);
	ssize_t readWebSocketHandshake(int fd);
	ssize_t readWebSocketFrame(int fd);
	
	// fill msg with a single message, return true
	// unless no full message is available, then return false
	bool getMessage(std::vector<char> &msg);
	
	// true if > 0 messages are in hand
	bool haveMessage() const;

	// write data to file descriptor
	// return the number of bytes written [ > 0: ok; == 0: ??; < 0: error ]
	// uses select to poll file for ability to write
	ssize_t write_aux(int fd);
	ssize_t write(int fd, const std::vector<char> &msg);
	ssize_t writeWebSocketHandshake(int fd, const std::vector<char> &msg);
	ssize_t writeWebSocketFrame(int fd, const std::vector<char> &msg);

	// true if > 0 messages need to be written
	bool haveWriteMessage() const;
	
protected:
	// INCOMING
	// data to hold the number digits until all are read
	// emptied when expected_count is set
	std::string num_data;
	
	// total number of bytes expected for this message
	size_t expected_count;
	
	// current message
	std::vector<char> data;
	
	// previous messages not yet returned.
	std::queue<std::vector<char>> msgs;

	// OUTGOING
	// number of bytes written for current message
	size_t bytes_written;
	// current outgoing message
	std::vector<char> out_msg;
	// messages queued to send.
	std::queue<std::vector<char>> out_msgs;
	
private:
};



#endif // _BYTE_BUFFER_H_
