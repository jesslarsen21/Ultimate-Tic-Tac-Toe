#include "ByteBuffer.h"
#include "logging.h"
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>

const size_t MAX_MESSAGE_SIZE = 1024;
//#define DEBUG 1
#if DEBUG
#define DEBUG_PRINT_BUFFER(msg, buffer, size) { std::cerr << msg << "||"; if(size >= 0) { debug_print_buffer(std::cerr, buffer, size); } else { std::cerr << " size: " << size << std::endl; } }

static void debug_print_buffer(std::ostream &os, const char *buffer, const ssize_t size)
{
	ssize_t i;
	os << "BUFFER @" << (void *)buffer << " size: " << size << "::";
	for(i = 0; i < size; i++)
		{
			os << buffer[i];
		}
	os << "::" << std::endl;
}

#else
#define DEBUG_PRINT_BUFFER(msg, buffer, size) { }
#endif



static ssize_t _read(int fd, char *buf, size_t count)
{
	ssize_t tmp_count = ::read(fd, buf, count);
	DEBUG_PRINT_BUFFER("_read()", buf, tmp_count);
	if(tmp_count >= 0)
		{
			return tmp_count;
		}
	else
		{
			switch(errno)
				{
				case EAGAIN:
#if EAGAIN != EWOULDBLOCK
				case EWOULDBLOCK:
#endif
					/* This is OK, return 0 in case this is at the end of valid reads */
					return 0;
					break;
				default:
					perror("Error reading from socket.");
				}
			return tmp_count;
		}
}

static ssize_t _write(int fd, const char *buf, size_t count)
{
	ssize_t tmp_count = ::write(fd, buf, count);
	DEBUG_PRINT_BUFFER("_write()", buf, tmp_count);
	if(tmp_count >= 0)
		{
			return tmp_count;
		}
	else
		{
			switch(errno)
				{
				case EAGAIN:
#if EAGAIN != EWOULDBLOCK
				case EWOULDBLOCK:
#endif
					return 0;
					break;
				default:
					perror("Error writing to socket.");
				}
			return tmp_count;
		}
}

static bool _read_ok(int fd)
{
	static int nfds = getdtablesize();
	fd_set rfds;
	struct timeval time_out;
	int n;
	FD_ZERO(&rfds);
	FD_SET(fd, &rfds);
	/* Poll by not waiting */
	time_out.tv_sec  = 0;
	time_out.tv_usec = 0;

	if((n = select(nfds, &rfds,  0, 0, &time_out)) < 0)
		{
			return false;
		}
	return (n == 1 && FD_ISSET(fd, &rfds));
}

static bool _write_ok(int fd)
{
	static int nfds = getdtablesize();
	fd_set wfds;
	struct timeval time_out;
	int n;
	FD_ZERO(&wfds);
	FD_SET(fd, &wfds);
	/* Poll by not waiting */
	time_out.tv_sec  = 0;
	time_out.tv_usec = 0;

	if((n = select(nfds, 0, &wfds,  0, &time_out)) < 0)
		{
			return false;
		}
	return (n == 1 && FD_ISSET(fd, &wfds));
}


ByteBuffer::ByteBuffer()
	: expected_count(0), bytes_written(0)
{ // default constructors for string and vector work for other data members
}


// read data from file descriptor
// return number of bytes read [ > 0: ok; == 0: close; < 0: error ]
// should only be called if select() says there is data to be read
ssize_t ByteBuffer::read(int fd)
{
	/* Possible states:
	 * - num_data empty and expected_count is 0 -> read digits
	 * - num_data not empty, digits only and expected_count is 0 -> read digits
	 * - num_data not empty, ends with space and expected_count is 0 -> convert num_data to number, set expected_count, wipe num_data
	 * - num_data empty, expected_count > 0 -> try to read expected_count, add to data, decrement expected_count, if 0 push data to msgs, clear data
	 */
	if(!_read_ok(fd))
		{
			std::cerr << "Should not call ByteBuffer::read() if fd has not been selected!" << std::endl;
			return -1;
		}
	ssize_t number_read = 0;
	ssize_t tmp_count = 0;
	const size_t BUFSIZE = 1024;
	char buf[BUFSIZE];
	while(true)
		{
			if(expected_count == 0 &&
				 (num_data.size() == 0 ||
					isdigit(num_data[num_data.size()-1])))
				{ // read a digit
					tmp_count = _read(fd, buf, 1);
					if(tmp_count == 1 && (isdigit(buf[0]) || isspace(buf[0]) || num_data.size() > 0))
						{
							num_data += buf[0];
							number_read += tmp_count;
						}
					else if(tmp_count == 1)
						{ 
							/* non-digit in first digit field */
                                                  std::cerr << "Unexpected character in ByteBuffer::read() digits'" << buf[0] << "'" << std::endl;
							return -1;
						}
					else if(tmp_count == 0)
						{
							/* no more data right now, */
							return number_read;
						}
					else
						{
							/* < 0, must be error */
							return tmp_count;
						}
				}
			else if(expected_count == 0 &&
							num_data.size() > 0 &&
							!isdigit(num_data[num_data.size()-1]))
				{ // set expected_count, wipe num_data
					expected_count = atoi(num_data.c_str());
					num_data.clear();
                                        if(expected_count > MAX_MESSAGE_SIZE)
                                          {
                                            /* non-digit in first digit field */
                                            std::cerr << "Message size is too big: " << expected_count << std::endl;
                                            return -1;
                                          }
				}
			else if(expected_count > 0)
				{ // read some bytes
					tmp_count = _read(fd, buf, std::min(expected_count, BUFSIZE));
					if(tmp_count > 0)
						{
							data.insert(data.end(), buf, buf+tmp_count);
							number_read += tmp_count;
							expected_count -= tmp_count;
							if(expected_count == 0)
								{
									msgs.push(data);
									data.clear();
									// Got a full message.
								}
						}
					else if(tmp_count == 0)
						{
							/* no more data right now, */
							return number_read;
						}
					else
						{
							/* < 0, must be error */
							return tmp_count;
						}
				}
			else
				{
					std::cerr << "Unexpected state in ByteBuffer::read()" << std::endl;
					return number_read;
				}
		}	
	return number_read;
}

// need to read until we get \r\n\r\n at end of message
ssize_t ByteBuffer::readWebSocketHandshake(int fd)
{
	const size_t BUFSIZE = 1;
	char buf[BUFSIZE];
	ssize_t number_read = 0;
	ssize_t tmp_count = 0;
	while(data.size() < 4 ||
				strncmp(data.data()+data.size()-4, "\r\n\r\n", 4) != 0)
		{
			tmp_count = _read(fd, buf, BUFSIZE);
			if(tmp_count > 0)
				{
					data.insert(data.end(), buf, buf + tmp_count);
					number_read += tmp_count;
				}
			else if(tmp_count == 0)
				{
					return number_read;
				}
			else
				{
					return tmp_count;
				}
		}
	// Only get here if successfully completed message.
	msgs.push(data);
	data.clear();
	return number_read;
}

// Must read the WebSocket header to decipher the
// size, and read the whole size.
#define BAD_SIZE (((size_t)1) << (sizeof(size_t)*8 - 1))
#define HASMASK(v) (((v) & 0x80) == 0x80)
#define IS8(v) (((v) & 0x7F) < 126)
#define GET8(d) ((*(reinterpret_cast<const uint8_t *>(d)))& 0x7F)
#define SET8(d,v) ((*(reinterpret_cast<uint8_t *>(d))) = static_cast<const uint8_t>(v) & 0x7F)
#define IS16(v) (((v) & 0x7F) == 126)
#define GET16(d) (be16toh(*(reinterpret_cast<const uint16_t *>(d))))
#define SET16(d,v) (*(reinterpret_cast<uint16_t *>(d)) = htobe16(static_cast<const uint16_t>(v)))
#define IS64(v) (((v) & 0x7F) == 127)
#define GET64(d) (be64toh(*(reinterpret_cast<const uint64_t *>(d))))
#define SET64(d,v) (*(reinterpret_cast<uint64_t *>(d)) = htobe64(static_cast<const uint64_t>(v)))
#define PAYLOAD_SIZE(data) ((data.size() >= 2 && IS8(data[1])) ?			\
														GET8(&data[1]) :													\
														((data.size() >= 4 && IS16(data[1])) ?		\
														 GET16(&data[2]) :												\
														 ((data.size() >= 10 && IS64(data[1])) ?	\
															GET64(&data[2]) :												\
															BAD_SIZE																\
															)																				\
														 )																				\
														)
#define HEADER_SIZE(data) (data.size() >= 2 ? \
													 (2 + ) \
													 : BAD_SIZE)

static size_t calc_payload_size(const std::vector<char> &data)
{
	size_t payload_size = BAD_SIZE;
	if(data.size() >= 2 && IS8(data[1]))
		{
			payload_size = GET8(&data[1]);
		}
	else if(data.size() >= 4 && IS16(data[1]))
		{
			payload_size = GET16(&data[2]);
		}
	else if(data.size() >= 10 && IS64(data[1]))
		{
			payload_size = GET64(&data[2]);
		}
	// otherwise don't know
	return payload_size;
}

static size_t calc_header_size(const std::vector<char> &data)
{
	size_t header_size = BAD_SIZE;
	if(data.size() >= 2)
		{
			if(IS8(data[1]))
				{
					header_size = 2;
				}
			else if(IS16(data[1]))
				{
					header_size = 4;
				}
			else if(IS64(data[1]))
				{
					header_size = 10;
				}
			if(header_size != BAD_SIZE && HASMASK(data[1]))
				{
					header_size += 4;
				}
		}
	// otherwise don't know
	return header_size;
}

ssize_t ByteBuffer::readWebSocketFrame(int fd)
{
	size_t payload_size = BAD_SIZE, header_size = BAD_SIZE;
	
	const size_t BUFSIZE = 1024;
	char buf[BUFSIZE];
	ssize_t number_read = 0;
	ssize_t tmp_count = 0;

	while(header_size == BAD_SIZE || payload_size == BAD_SIZE ||
				data.size() < header_size + payload_size)
		{
			if(data.size() < 2)
				{
					tmp_count = _read(fd, buf, 2);
					if(tmp_count > 0)
						{
							data.insert(data.end(), buf, buf + tmp_count);
							number_read += tmp_count;
						}
					else if(tmp_count == 0)
						{
							return number_read;
						}
					else
						{
							return tmp_count;
						}
				}
			else if(data.size() < header_size)
				{
					tmp_count = std::min(header_size - data.size(), BUFSIZE);
					tmp_count = _read(fd, buf, tmp_count);
					if(tmp_count > 0)
						{
							data.insert(data.end(), buf, buf + tmp_count);
							number_read += tmp_count;
						}
					else if(tmp_count == 0)
						{
							return number_read;
						}
					else
						{
							return tmp_count;
						}
				}
			else if(data.size() < header_size + payload_size)
				{
					tmp_count = std::min(header_size + payload_size - data.size(), BUFSIZE);
					tmp_count = _read(fd, buf, tmp_count);
					if(tmp_count > 0)
						{
							data.insert(data.end(), buf, buf + tmp_count);
							number_read += tmp_count;
							if(data.size() == header_size + payload_size)
								{
									if(HASMASK(data[1]))
										{
											uint32_t maskingkey = *(reinterpret_cast<const uint32_t *>(&data[header_size-4]));
											size_t i;

											std::vector<char>::iterator it;
											for(it = data.begin() + header_size, i = 0; it != data.end(); it++, i = (i+1)%4)
												{
													*it ^= (0xFF & (maskingkey >> (8*(i))));
												}
										}
									std::vector<char> msg(data.size()-header_size);
									std::copy(data.begin() + header_size, data.end(), msg.begin());
									msgs.push(msg);
									data.clear();
								}
						}
					else if(tmp_count == 0)
						{
							return number_read;
						}
					else
						{
							return tmp_count;
						}
				}
			
			payload_size = calc_payload_size(data);
			header_size = calc_header_size(data);
		}
	return number_read;
}
	
// fill msg with a single message, return true
// unless no full message is available, then return false
bool ByteBuffer::getMessage(std::vector<char> &msg)
{
	if(haveMessage())
		{
			msg = msgs.front();
			msgs.pop();
			return true;
		}
	return false;
}
	
// true if > 0 messages are in hand
bool ByteBuffer::haveMessage() const
{
	return msgs.size() > 0;
}

ssize_t ByteBuffer::write_aux(int fd)
{
	ssize_t number_written = 0;
	
	while(_write_ok(fd) && ((out_msgs.size() > 0) || (bytes_written < out_msg.size())) )
		{
			if(out_msg.size() == 0 && out_msgs.size() > 0)
				{
					bytes_written = 0;
					out_msg = out_msgs.front();
					out_msgs.pop();
				}
			if(bytes_written < out_msg.size())
				{
					ssize_t tmp_count;
					tmp_count = _write(fd, out_msg.data() + bytes_written, out_msg.size() - bytes_written);
					if(tmp_count > 0)
						{
							number_written += tmp_count;
							bytes_written += tmp_count;
							if(bytes_written >= out_msg.size())
								{
									bytes_written = 0;
									out_msg.clear();
								}
						}
					else
						{
							return tmp_count;
						}
				}
		}
	return number_written;
}

ssize_t ByteBuffer::write(int fd, const std::vector<char> &msg)
{
	if(msg.size() > 0)
		{
			char buf[32];
			std::sprintf(buf, "%ld ", msg.size());
			std::vector<char> full_msg(std::strlen(buf) + msg.size());
			std::copy(buf, buf+strlen(buf), full_msg.begin());
			std::copy(msg.begin(), msg.end(), full_msg.begin() + strlen(buf));
			out_msgs.push(full_msg);
		}
	return write_aux(fd);
}

ssize_t ByteBuffer::writeWebSocketHandshake(int fd, const std::vector<char> &msg)
{
	if(msg.size() > 0)
		{
			out_msgs.push(msg);
		}
	return write_aux(fd);
}

ssize_t ByteBuffer::writeWebSocketFrame(int fd, const std::vector<char> &msg)
{
	if(msg.size() > 0)
		{
			std::vector<char> full_msg;
			size_t header_size = BAD_SIZE;
			size_t payload_size = msg.size();
			if(payload_size < 126)
				{
					header_size = 2;
				}
			else if(payload_size < 1 << 16)
				{
					header_size = 4;
				}
			else
				{
					header_size = 10;
				}
			full_msg.resize(header_size + payload_size);
			full_msg[0] = 0x81; // FIN | TextFrame
			if(payload_size < 126)
				{
					SET8(&full_msg[1], payload_size);
				}
			else if(payload_size < 1 << 16)
				{
					SET8(&full_msg[1], 126);
					SET16(&full_msg[2], payload_size);
				}
			else
				{
					SET8(&full_msg[1], 126);
					SET64(&full_msg[2], payload_size);
				}
			// no mask
			
			// set data
			std::copy(msg.begin(), msg.end(), full_msg.begin() + header_size);
			out_msgs.push(full_msg);
		}
	
	return write_aux(fd);
}

	
bool ByteBuffer::haveWriteMessage() const
{
	return bytes_written < out_msg.size() || out_msgs.size() > 0;
}
