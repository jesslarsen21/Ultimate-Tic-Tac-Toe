#include <iostream>
#include <cstring>
#include <string>
#include "Serialization.h"

int TestGetBoard() {
	Serialization::Serializer ser;
	
	const Messaging::GetBoard* const msg = new Messaging::GetBoard(0, "bob");
	
	std::string data, err;
	
	if ((err = ser.Serialize(msg, data)) != "") {
		std::cout << "Serialize Failure => " << err << std::endl;
		return 1;
	}
	
	std::cout << "Serialize success => " << data << std::endl;
	
	Messaging::IMsg* nmsg = NULL;
	if ((err = ser.Deserialize(data, nmsg)) != "") {
		std::cout << "Deserialize Failure => " << err << std::endl;
		return 1;
	}
	
	std::cout << "Deserialize Success." << std::endl;
	
	if (nmsg->GetSender() != msg->GetSender()) {
		std::cout 	<< "Error: Expected { " 
					<< msg->GetSender() 
					<< " }, got { " 
					<< nmsg->GetSender()
					<< std::endl;
		return 1;
	}
	
	std::cout << "TestGetBoard() test passed." << std::endl;
	
	return 0;
}

int TestTakeTurn() {
	Serialization::Serializer ser;
	
	const Messaging::TakeTurn* const msg = new Messaging::TakeTurn(0, "bob", 1, 2, 3);
	
	std::string data, err;
	
	if ((err = ser.Serialize(msg, data)) != "") {
		std::cout << "Serialize Failure => " << err << std::endl;
		return 1;
	}
	
	std::cout << "Serialize Success => " << data << std::endl;
	
	Messaging::IMsg* nmsg = NULL;
	if ((err = ser.Deserialize(data, nmsg)) != "") {
		std::cout << "Deserialize Failure => " << err << std::endl;
		return 1;
	}
	
	std::cout << "Deserialize Success." << std::endl;
	
	return 0;
}

int TestTakeTurnIter(const int iters) {
  return 1;
}

int TestPing() {
	Serialization::Serializer ser;
	
	const Messaging::Ping* const msg = new Messaging::Ping(0, "sender", "message is here");
	
	std::string data, err;
	
	if ((err = ser.Serialize(msg, data)) != "") {
		std::cout << "Serialize Failure => " << err << std::endl;
		return 1;
	}
	
	std::cout << "Serialize success => " << data << std::endl;
	
	Messaging::IMsg* nmsg = NULL;
	if ((err = ser.Deserialize(data, nmsg)) != "") {
		std::cout << "Deserialize Failure => " << err << std::endl;
		return 1;
	}
	
	std::cout << "Deserialize Success." << std::endl;
	
	if (nmsg->GetSender() != msg->GetSender()) {
		std::cout 	<< "Error: Expected { " 
					<< msg->GetSender() 
					<< " }, got { " 
					<< nmsg->GetSender()
					<< std::endl;
		return 1;
	}
  
	Messaging::Ping* nping = dynamic_cast<Messaging::Ping*>(nmsg);
	if (nping->GetMessage() != msg->GetMessage()) {
		std::cout 	<< "Error: Expected { " 
					<< msg->GetMessage() 
					<< " }, got { " 
					<< nping->GetMessage()
					<< std::endl;
		return 1;
  }
  std::cout << "Message: " << nping->GetMessage() << std::endl;
	std::cout << "TestPing() test passed." << std::endl;
	delete msg;
  delete nmsg;
	return 0;
}

int TestPong() {
	Serialization::Serializer ser;
	
	const Messaging::Pong* const msg = new Messaging::Pong(0, "sender", "return message is here");
	
	std::string data, err;
	
	if ((err = ser.Serialize(msg, data)) != "") {
		std::cout << "Serialize Failure => " << err << std::endl;
		return 1;
	}
	
	std::cout << "Serialize success => " << data << std::endl;
	
	Messaging::IMsg* nmsg = NULL;
	if ((err = ser.Deserialize(data, nmsg)) != "") {
		std::cout << "Deserialize Failure => " << err << std::endl;
		return 1;
	}
	
	std::cout << "Deserialize Success." << std::endl;
	
	if (nmsg->GetSender() != msg->GetSender()) {
		std::cout 	<< "Error: Expected { " 
					<< msg->GetSender() 
					<< " }, got { " 
					<< nmsg->GetSender()
					<< std::endl;
		return 1;
	}
  
	Messaging::Pong* npong = dynamic_cast<Messaging::Pong*>(nmsg);
	if (npong->GetMessage() != msg->GetMessage()) {
		std::cout 	<< "Error: Expected { " 
					<< msg->GetMessage() 
					<< " }, got { " 
					<< npong->GetMessage()
					<< std::endl;
		return 1;
  }
  std::cout << "Message: " << npong->GetMessage() << std::endl;
	std::cout << "TestPong() test passed." << std::endl;
	delete msg;
  delete nmsg;
	return 0;
}


int main() {
    int res = 0;
    res = TestGetBoard();
    if (res != 0)
        return 1;

    res = TestTakeTurn();
    if (res != 0) 
        return 1;

    res = TestPing();
    if (res != 0) 
        return 1;
    
    res = TestPong();
    if (res != 0) 
        return 1;

    return 0;
    
	Serialization::Serializer ser;
	
	const Messaging::TakeTurn* const msg = new Messaging::TakeTurn(0, "bob", 1, 1, 2);
	
	std::string data; 
	std::string err = ser.Serialize(msg, data);
	
	if (err != "") {
		std::cout << err << std::endl;
		return 1;
	}
	
	std::cout << data << std::endl;
	
	Messaging::IMsg* nmsg = NULL;
	err = ser.Deserialize(data, nmsg);
	
	if (err != "") {
		std::cout << err << std::endl;
	}

	return 0;
}
