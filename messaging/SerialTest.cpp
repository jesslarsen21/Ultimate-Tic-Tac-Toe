#include <iostream>
#include <string>
#include <future>
#include "Serializer.h"
#include "TurnMsg.h"
#include "UpdateMsg.h"
#include "MarkerMsg.h"
#include "NextTurnMsg.h"
#include "WinStateMsg.h"
#include "BoardStateMsg.h"
#include "PingMsg.h"
#include "PongMsg.h"
#include "WhoamiMsg.h"
#include "YouAreMsg.h"
#include "TurnFailedMsg.h"
#include "LoginMsg.h"
#include "LoginReplyMsg.h"
#include "SignUpMsg.h"
#include "SignUpReplyMsg.h"
#include "ErrorMsg.h"
#include "OpponentNameMsg.h"
#include "ConcurrentQueue.h"
#include "AsyncSerializer.h"

bool cmpTurnMsg(const Messaging::TurnMsg& a, const Messaging::TurnMsg& b) {
    return  (a.GetPlayer() == b.GetPlayer()) &&
            (a.GetPosition() == b.GetPosition()) &&
            (a.GetBoard() == b.GetBoard());
}

bool testTurnMsg() {
    const Messaging::TurnMsg initMsg ('a', 1, 1);
    const Serialization::Serializer ser;

    std::string result, err;
    if ((err = ser.Serialize((const Messaging::IMsg*)(&initMsg), result)) != "") {
        std::cout << err << std::endl;
        return false;
    } 

    std::cout << result << std::endl;

    Messaging::IMsg* imsg;
    if ((err = ser.Deserialize(result, imsg)) != "") {
        std::cout << err << std::endl;
        return false;
    }

    Messaging::TurnMsg* finMsg = dynamic_cast<Messaging::TurnMsg*>(imsg);
    if (finMsg == NULL) {
        std::cout << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
        return false;
    }

    std::cout << "Deserialize successful." << std::endl;

    if (!cmpTurnMsg(initMsg, *finMsg)) {
        std::cout   
            << "TurnMsg = {" << std::endl
            << "\tplayer = " << finMsg->GetPlayer() << std::endl
            << "\tboard = " << finMsg->GetBoard() << std::endl
            << "\tposition = " << finMsg->GetPosition() << std::endl
            << "}" << std::endl
            << "Expected {" << std::endl
            << "\tplayer = " << initMsg.GetPlayer() << std::endl
            << "\tboard = " << initMsg.GetBoard() << std::endl
            << "\tposition = " << initMsg.GetPosition() << std::endl
            << "}" << std::endl;
             
        std::cout << "Messages not equal." << std::endl;
        return 1;
    }

    std::cout << "Messages are equal." << std::endl;
    return true;
}

bool testUpdateMsg() {
    const Messaging::UpdateMsg initMsg;
    const Serialization::Serializer ser;

    std::string result, err;
    if ((err = ser.Serialize((const Messaging::IMsg*)(&initMsg), result)) != "") {
        std::cout << err << std::endl;
        return false;
    } 

    std::cout << result << std::endl;

    Messaging::IMsg* imsg;
    if ((err = ser.Deserialize(result, imsg)) != "") {
        std::cout << err << std::endl;
        return false;
    }

    Messaging::UpdateMsg* finMsg = dynamic_cast<Messaging::UpdateMsg*>(imsg);
    if (finMsg == NULL) {
        std::cout << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
        return false;
    }

    std::cout << "Deserialize successful." << std::endl;

    std::cout << "Messages are equal." << std::endl;
    return true;
}

bool testMarkerMsg() {
    const Messaging::MarkerMsg initMsg ('a', 1, 1);
    const Serialization::Serializer ser;

    std::string result, err;
    if ((err = ser.Serialize(&initMsg, result)) != "") {
        std::cerr << err << std::endl;
        return false;
    } 

    std::cout << result << std::endl;

    Messaging::IMsg* imsg;
    if ((err = ser.Deserialize(result, imsg)) != "") {
        std::cerr << err << std::endl;
        return false;
    }

    Messaging::MarkerMsg* finMsg = dynamic_cast<Messaging::MarkerMsg*>(imsg);
    if (finMsg == NULL) {
        std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
        return false;
    }

    std::cout << "Deserialize successful." << std::endl;

    if (    (initMsg.GetPlayer() != finMsg->GetPlayer()) ||
            (initMsg.GetBoard() != finMsg->GetBoard()) ||
            (initMsg.GetPosition() != finMsg->GetPosition()) ) {
        std::cerr << "Messages are not equal." << std::endl;
        return false;
    }

    std::cout << "Messages are equal." << std::endl;
    return true;
}

bool testNextTurnMsg() {
    const Messaging::NextTurnMsg initMsg ('a', 1);
    const Serialization::Serializer ser;

    std::string result, err;
    if ((err = ser.Serialize(&initMsg, result)) != "") {
        std::cerr << err << std::endl;
        return false;
    } 

    std::cout << result << std::endl;

    Messaging::IMsg* imsg;
    if ((err = ser.Deserialize(result, imsg)) != "") {
        std::cerr << err << std::endl;
        return false;
    }

    Messaging::NextTurnMsg* finMsg = dynamic_cast<Messaging::NextTurnMsg*>(imsg);
    if (finMsg == NULL) {
        std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
        return false;
    }

    std::cout << "Deserialize successful." << std::endl;

    if (    (initMsg.GetPlayer() != finMsg->GetPlayer()) ||
            (initMsg.GetNextBoard() != finMsg->GetNextBoard()) ) {
        std::cerr << "Messages are not equal." << std::endl;
        return false;
    }

    std::cout << "Messages are equal." << std::endl;
    return true;
}

bool testWinStateMsg() {
    const Messaging::WinStateMsg initMsg ('a');
    const Serialization::Serializer ser;

    std::string result, err;
    if ((err = ser.Serialize(&initMsg, result)) != "") {
        std::cerr << err << std::endl;
        return false;
    } 

    std::cout << result << std::endl;

    Messaging::IMsg* imsg;
    if ((err = ser.Deserialize(result, imsg)) != "") {
        std::cerr << err << std::endl;
        return false;
    }

    Messaging::WinStateMsg* finMsg = dynamic_cast<Messaging::WinStateMsg*>(imsg);
    if (finMsg == NULL) {
        std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
        return false;
    }

    std::cout << "Deserialize successful." << std::endl;

    if (    (initMsg.GetPlayer() != finMsg->GetPlayer()) ) {
        std::cerr << "Messages are not equal." << std::endl;
        return false;
    }

    std::cout << "Messages are equal." << std::endl;
    return true;
}

bool testBoardStateMsg() {
    const Messaging::BoardStateMsg initMsg ('a', 1);
    const Serialization::Serializer ser;

    std::string result, err;
    if ((err = ser.Serialize(&initMsg, result)) != "") {
        std::cerr << err << std::endl;
        return false;
    } 

    std::cout << result << std::endl;

    Messaging::IMsg* imsg;
    if ((err = ser.Deserialize(result, imsg)) != "") {
        std::cerr << err << std::endl;
        return false;
    }

    Messaging::BoardStateMsg* finMsg = dynamic_cast<Messaging::BoardStateMsg*>(imsg);
    if (finMsg == NULL) {
        std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
        return false;
    }

    std::cout << "Deserialize successful." << std::endl;

    if (    (initMsg.GetPlayer() != finMsg->GetPlayer()) ||
            (initMsg.GetBoard() != finMsg->GetBoard()) ) {
        std::cerr << "Messages are not equal." << std::endl;
        return false;
    }

    std::cout << "Messages are equal." << std::endl;
    return true;
}

bool testPingMsg() {
    const Messaging::PingMsg initMsg;
    const Serialization::Serializer ser;

    std::string result, err;
    if ((err = ser.Serialize(&initMsg, result)) != "") {
        std::cerr << err << std::endl;
        return false;
    } 

    std::cout << result << std::endl;

    Messaging::IMsg* imsg;
    if ((err = ser.Deserialize(result, imsg)) != "") {
        std::cerr << err << std::endl;
        return false;
    }

    Messaging::PingMsg* finMsg = dynamic_cast<Messaging::PingMsg*>(imsg);
    if (finMsg == NULL) {
        std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
        return false;
    }

    std::cout << "Deserialize successful." << std::endl;

    std::cout << "Messages are equal." << std::endl;
    return true;
}

bool testPongMsg() {
    const Messaging::PongMsg initMsg;
    const Serialization::Serializer ser;

    std::string result, err;
    if ((err = ser.Serialize(&initMsg, result)) != "") {
        std::cerr << err << std::endl;
        return false;
    } 

    std::cout << result << std::endl;

    Messaging::IMsg* imsg;
    if ((err = ser.Deserialize(result, imsg)) != "") {
        std::cerr << err << std::endl;
        return false;
    }

    Messaging::PongMsg* finMsg = dynamic_cast<Messaging::PongMsg*>(imsg);
    if (finMsg == NULL) {
        std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
        return false;
    }

    std::cout << "Deserialize successful." << std::endl;

    std::cout << "Messages are equal." << std::endl;
    return true;
}

bool testWhoamiMsg() {
    const Messaging::WhoamiMsg initMsg;
    const Serialization::Serializer ser;

    std::string result, err;
    if ((err = ser.Serialize(&initMsg, result)) != "") {
        std::cerr << err << std::endl;
        return false;
    } 

    std::cout << result << std::endl;

    Messaging::IMsg* imsg;
    if ((err = ser.Deserialize(result, imsg)) != "") {
        std::cerr << err << std::endl;
        return false;
    }

    auto finMsg = dynamic_cast<Messaging::WhoamiMsg*>(imsg);
    if (finMsg == NULL) {
        std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
        return false;
    }

    std::cout << "Deserialize successful." << std::endl;

    std::cout << "Messages are equal." << std::endl;
    return true;
}

bool testYouAreMsg() {
	const Messaging::YouAreMsg initMsg ('O', "TonyTiger");
    const Serialization::Serializer ser;

    std::string result, err;
    if ((err = ser.Serialize(&initMsg, result)) != "") {
        std::cerr << err << std::endl;
        return false;
    } 

    std::cout << result << std::endl;

    Messaging::IMsg* imsg;
    if ((err = ser.Deserialize(result, imsg)) != "") {
        std::cerr << err << std::endl;
        return false;
    }

    auto finMsg = dynamic_cast<Messaging::YouAreMsg*>(imsg);
    if (finMsg == NULL) {
        std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
        return false;
    }


    if (finMsg->GetPlayer() != initMsg.GetPlayer()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::TurnFailedMsg::GetPlayer."
            << "\t Expected => " << initMsg.GetPlayer()
            << "\t Got => " << finMsg->GetPlayer()
            << std::endl;

        std::cerr << "Deserialization failed." << std::endl;
        return false;
    }

    std::cout << "Deserialize successful." << std::endl;

    std::cout << "Messages are equal." << std::endl;
    return true;
}

bool testTurnFailedMsg() {
    const Messaging::TurnFailedMsg initMsg ('O', "You're a bad player!");
    const Serialization::Serializer ser;

    std::string result, err;
    if ((err = ser.Serialize(&initMsg, result)) != "") {
        std::cerr << err << std::endl;
        return false;
    } 

    std::cout << result << std::endl;

    Messaging::IMsg* imsg;
    if ((err = ser.Deserialize(result, imsg)) != "") {
        std::cerr << err << std::endl;
        return false;
    }

    auto finMsg = dynamic_cast<Messaging::TurnFailedMsg*>(imsg);
    if (finMsg == NULL) {
        std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
        return false;
    }

    bool isGood = true;

    if (finMsg->GetPlayer() != initMsg.GetPlayer()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::TurnFailedMsg::GetPlayer."
            << "\t Expected => " << initMsg.GetPlayer()
            << "\t Got => " << finMsg->GetPlayer()
            << std::endl;

        isGood = false;
    }
    if (finMsg->GetMessage() != initMsg.GetMessage()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::TurnFailedMsg::GetMessage." 
            << "\t Expected => " << initMsg.GetMessage()
            << "\t Got => " << finMsg->GetMessage()
            << std::endl;

        isGood = false;
    }

    if (!isGood) {
        std::cerr << "Deserialization failed." << std::endl;
        return false;
    }

    std::cout << "Deserialize successful." << std::endl;

    std::cout << "Messages are equal." << std::endl;
    return true;
}

bool testLoginMsg() {
    const Messaging::LoginMsg initMsg ("test@gmail.com", "1234");
    const Serialization::Serializer ser;

    std::string result, err;
    if ((err = ser.Serialize(&initMsg, result)) != "") {
        std::cerr << err << std::endl;
        return false;
    } 

    std::cout << result << std::endl;

    Messaging::IMsg* imsg;
    if ((err = ser.Deserialize(result, imsg)) != "") {
        std::cerr << err << std::endl;
        return false;
    }

    auto finMsg = dynamic_cast<Messaging::LoginMsg*>(imsg);
    if (finMsg == NULL) {
        std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
        return false;
    }

    bool isGood = true;

    if (finMsg->GetEmail() != initMsg.GetEmail()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::LoginMsg::GetEmail."
            << "\t Expected => " << initMsg.GetEmail()
            << "\t Got => " << finMsg->GetEmail()
            << std::endl;

        isGood = false;
    }
    if (finMsg->GetPassword() != initMsg.GetPassword()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::LoginMsg::GetPassword." 
            << "\t Expected => " << initMsg.GetPassword()
            << "\t Got => " << finMsg->GetPassword()
            << std::endl;

        isGood = false;
    }

    if (!isGood) {
        std::cerr << "Deserialization failed." << std::endl;
        return false;
    }

    std::cout << "Deserialize successful." << std::endl;

    std::cout << "Messages are equal." << std::endl;
    return true;
}

bool testLoginReplyMsg() {
    const Messaging::LoginReplyMsg initMsg (true, "1!==234");
    const Serialization::Serializer ser;

    std::string result, err;
    if ((err = ser.Serialize(&initMsg, result)) != "") {
        std::cerr << err << std::endl;
        return false;
    } 

    std::cout << result << std::endl;

    Messaging::IMsg* imsg;
    if ((err = ser.Deserialize(result, imsg)) != "") {
        std::cerr << err << std::endl;
        return false;
    }

    auto finMsg = dynamic_cast<Messaging::LoginReplyMsg*>(imsg);
    if (finMsg == NULL) {
        std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
        return false;
    }

    bool isGood = true;

    if (finMsg->IsValid() != initMsg.IsValid()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::LoginReplyMsg::IsValid."
            << "\t Expected => " << initMsg.IsValid()
            << "\t Got => " << finMsg->IsValid()
            << std::endl;

        isGood = false;
    }
    if (finMsg->GetToken() != initMsg.GetToken()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::LoginReplyMsg::GetToken." 
            << "\t Expected => " << initMsg.GetToken()
            << "\t Got => " << finMsg->GetToken()
            << std::endl;

        isGood = false;
    }

    if (!isGood) {
        std::cerr << "Deserialization failed." << std::endl;
        return false;
    }

    std::cout << "Deserialize successful." << std::endl;

    std::cout << "Messages are equal." << std::endl;
    return true;
}

bool testSignUpMsg() {
    const Messaging::SignUpMsg initMsg (
        "A guy with a really really really long name Smith",
        "TheBeastOfGore@ThePearlyGates.com",
        "Long pass",
        "Long pass"
    );
    const Serialization::Serializer ser;

    std::string result, err;
    if ((err = ser.Serialize(&initMsg, result)) != "") {
        std::cerr << err << std::endl;
        return false;
    } 

    std::cout << result << std::endl;

    Messaging::IMsg* imsg;
    if ((err = ser.Deserialize(result, imsg)) != "") {
        std::cerr << err << std::endl;
        return false;
    }

    auto finMsg = dynamic_cast<Messaging::SignUpMsg*>(imsg);
    if (finMsg == NULL) {
        std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
        return false;
    }

    bool isGood = true;

    if (finMsg->GetUsername() != initMsg.GetUsername()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::SignUpMsg::GetUsername."
            << "\t Expected => " << initMsg.GetUsername()
            << "\t Got => " << finMsg->GetUsername()
            << std::endl;

        isGood = false;
    }
    if (finMsg->GetEmail() != initMsg.GetEmail()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::SignUpMsg::GetEmail." 
            << "\t Expected => " << initMsg.GetEmail()
            << "\t Got => " << finMsg->GetEmail()
            << std::endl;

        isGood = false;
    }
    if (finMsg->GetPassword() != initMsg.GetPassword()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::SignUpMsg::GetPassword." 
            << "\t Expected => " << initMsg.GetPassword()
            << "\t Got => " << finMsg->GetPassword()
            << std::endl;

        isGood = false;
    }
    if (finMsg->GetPasswordConfirm() != initMsg.GetPasswordConfirm()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::SignUpMsg::GetPasswordConfirm." 
            << "\t Expected => " << initMsg.GetPasswordConfirm()
            << "\t Got => " << finMsg->GetPasswordConfirm()
            << std::endl;

        isGood = false;
    }

    if (!isGood) {
        std::cerr << "Deserialization failed." << std::endl;
        return false;
    }

    std::cout << "Deserialize successful." << std::endl;

    std::cout << "Messages are equal." << std::endl;
    return true;
}

bool testSignUpReplyMsg() {
    const Messaging::SignUpReplyMsg initMsg (
        true, true, Messaging::SignUpReplyMsg::Unmatched
    );
    const Serialization::Serializer ser;

    std::string result, err;
    if ((err = ser.Serialize(&initMsg, result)) != "") {
        std::cerr << err << std::endl;
        return false;
    } 

    std::cout << result << std::endl;

    Messaging::IMsg* imsg;
    if ((err = ser.Deserialize(result, imsg)) != "") {
        std::cerr << err << std::endl;
        return false;
    }

    auto finMsg = dynamic_cast<Messaging::SignUpReplyMsg*>(imsg);
    if (finMsg == NULL) {
        std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
        return false;
    }

    bool isGood = true;

    if (finMsg->IsUsernameValid() != initMsg.IsUsernameValid()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::SignUpReplyMsg::IsUsernameValid."
            << "\t Expected => " << initMsg.IsUsernameValid()
            << "\t Got => " << finMsg->IsUsernameValid()
            << std::endl;

        isGood = false;
    }
    if (finMsg->IsEmailValid() != initMsg.IsEmailValid()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::SignUpReplyMsg::IsEmailValid." 
            << "\t Expected => " << initMsg.IsEmailValid()
            << "\t Got => " << finMsg->IsEmailValid()
            << std::endl;

        isGood = false;
    }
    if (finMsg->GetPasswordState() != initMsg.GetPasswordState()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::SignUpReplyMsg::IsPasswordValid." 
            << "\t Expected => " << initMsg.GetPasswordState()
            << "\t Got => " << finMsg->GetPasswordState()
            << std::endl;

        isGood = false;
    }

    if (!isGood) {
        std::cerr << "Deserialization failed." << std::endl;
        return false;
    }

    std::cout << "Deserialize successful." << std::endl;

    std::cout << "Messages are equal." << std::endl;
    return true;
}

bool testOpponentNameMsg() {
    const Messaging::OpponentNameMsg initMsg (
        "Billy bob!"
    );
    const Serialization::Serializer ser;

    std::string result, err;
    if ((err = ser.Serialize(&initMsg, result)) != "") {
        std::cerr << err << std::endl;
        return false;
    } 

    std::cout << result << std::endl;

    Messaging::IMsg* imsg;
    if ((err = ser.Deserialize(result, imsg)) != "") {
        std::cerr << err << std::endl;
        return false;
    }

    auto finMsg = dynamic_cast<Messaging::OpponentNameMsg*>(imsg);
    if (finMsg == NULL) {
        std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
        return false;
    }

    bool isGood = true;

    if (finMsg->GetUsername() != initMsg.GetUsername()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::OpponentNameMsg::GetUsername."
            << "\t Expected => " << initMsg.GetUsername()
            << "\t Got => " << finMsg->GetUsername()
            << std::endl;

        isGood = false;
    }

    if (!isGood) {
        std::cerr << "Deserialization failed." << std::endl;
        return false;
    }

    std::cout << "Deserialize successful." << std::endl;
    std::cout << "Messages are equal." << std::endl;
    return true;
}

bool testErrorMsg() {
    const Messaging::ErrorMsg* const parentMsg = new Messaging::ErrorMsg (
        "10 Apr 2015 10:00pm",
        0,
        "Server:auth0",
        "Log msg",
        "Logging",
        NULL
    );
    const Messaging::ErrorMsg initMsg (
        "",
        0,
        "",
        "",
        "",
        parentMsg
    );
    //const Messaging::ErrorMsg initMsg (
    //    "10 Apr 2015 10:00pm",
    //    0,
    //    "Server:auth0",
    //    "Login attempt",
    //    "Logging",
    //    parentMsg
    //);
    const Serialization::Serializer ser;

    std::string result, err;
    if ((err = ser.Serialize(&initMsg, result)) != "") {
        std::cerr << err << std::endl;
        return false;
    } 

    std::cout << result << std::endl;

    Messaging::IMsg* imsg;
    if ((err = ser.Deserialize(result, imsg)) != "") {
        std::cerr << err << std::endl;
        return false;
    }

    auto finMsg = dynamic_cast<Messaging::ErrorMsg*>(imsg);
    if (finMsg == NULL) {
        std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
                    
        return false;
    }
    auto parentFinMsg = finMsg->GetNextError();
    if (parentFinMsg == NULL) {
        std::cerr << "Deserialize error, nextError() was expected, but got NULL." << std::endl;
        return false;
    }

    bool isGood = true;

    if (finMsg->GetWhen() != initMsg.GetWhen()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::ErrorMsg::GetWhen."
            << "\t Expected => " << initMsg.GetWhen()
            << "\t Got => " << finMsg->GetWhen()
            << std::endl;

        isGood = false;
    }
    if (finMsg->GetSeverity() != initMsg.GetSeverity()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::ErrorMsg::GetSeverity()." 
            << "\t Expected => " << initMsg.GetSeverity()
            << "\t Got => " << finMsg->GetSeverity()
            << std::endl;

        isGood = false;
    }
    if (finMsg->GetSender() != initMsg.GetSender()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::ErrorMsg::GetSender." 
            << "\t Expected => " << initMsg.GetSender()
            << "\t Got => " << finMsg->GetSender()
            << std::endl;

        isGood = false;
    }
    if (finMsg->GetWhat() != initMsg.GetWhat()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::ErrorMsg::GetWhat." 
            << "\t Expected => " << initMsg.GetWhat()
            << "\t Got => " << finMsg->GetWhat()
            << std::endl;

        isGood = false;
    }
    if (finMsg->GetReason() != initMsg.GetReason()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::ErrorMsg::GetReason." 
            << "\t Expected => " << initMsg.GetReason()
            << "\t Got => " << finMsg->GetReason()
            << std::endl;

        isGood = false;
    }


    if (finMsg->GetWhen() != initMsg.GetWhen()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::ErrorMsg::GetWhen."
            << "\t Expected => " << initMsg.GetWhen()
            << "\t Got => " << finMsg->GetWhen()
            << std::endl;

        isGood = false;
    }
    if (finMsg->GetSeverity() != initMsg.GetSeverity()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::ErrorMsg::GetSeverity()." 
            << "\t Expected => " << initMsg.GetSeverity()
            << "\t Got => " << finMsg->GetSeverity()
            << std::endl;

        isGood = false;
    }
    if (finMsg->GetSender() != initMsg.GetSender()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::ErrorMsg::GetSender." 
            << "\t Expected => " << initMsg.GetSender()
            << "\t Got => " << finMsg->GetSender()
            << std::endl;

        isGood = false;
    }
    if (finMsg->GetWhat() != initMsg.GetWhat()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::ErrorMsg::GetWhat." 
            << "\t Expected => " << initMsg.GetWhat()
            << "\t Got => " << finMsg->GetWhat()
            << std::endl;

        isGood = false;
    }
    if (finMsg->GetReason() != initMsg.GetReason()) {
        std::cerr 
            << "Deserializer failed to deserialize Messaging::ErrorMsg::GetReason." 
            << "\t Expected => " << initMsg.GetReason()
            << "\t Got => " << finMsg->GetReason()
            << std::endl;

        isGood = false;
    }

    if (!isGood) {
        std::cerr << "Deserialization failed." << std::endl;
        return false;
    }

    std::cout 
        << "Stack trace => " << std::endl
        << finMsg->GetStackTrace(" | ") << std::endl;

    std::cout << "Deserialize successful." << std::endl;

    std::cout << "Messages are equal." << std::endl;
    return true;
}

int main(int argc, char** argv) {

    auto ser = new Serialization::AsyncSerializer();
    auto msg = new Messaging::PingMsg();
    ser->Serialize(msg, false);

    Serialization::SerializeResult result;
    while( !ser->GetSerializeResult(result) ) ;

    std::cout << result.result << std::endl;
    std::cout << result.error << std::endl;

    return 0;

    ConcurrentQueue<int> queue;

    const int threads = 100;
    const int msgs = 1000;

    std::future<void> tasks [threads];

    int i;
    for ( i = 0; i < threads; i++) {
        tasks[i] = std::async(std::launch::async, [threads, msgs, i, &queue]() {
            for (int j = 0; j < msgs; j++) 
                queue.Push(i * threads + j);
        });
    }

    while( queue.Pop(i) ) {
        std::cout << i << std::endl;
    }

    return 0;

    int n;
    try {
        if (argc > 1) 
            n = atoi(argv[1]);
        else 
            n = 1;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (!testTurnMsg()) 
            return 1;
        if (!testUpdateMsg())
            return 1;
        if (!testMarkerMsg())
            return 1;
        if (!testNextTurnMsg())
            return 1;
        if (!testWinStateMsg())
            return 1;
        if (!testBoardStateMsg())
            return 1;
        if (!testPingMsg())
            return 1;
        if (!testPongMsg())
            return 1;
        if (!testWhoamiMsg())
            return 1;
        if (!testYouAreMsg())
            return 1;
        if (!testTurnFailedMsg())
            return 1;
        if (!testLoginMsg())
            return 1;
        if (!testLoginReplyMsg())
            return 1;
        if (!testSignUpMsg())
            return 1;
        if (!testSignUpReplyMsg())
            return 1;
        if (!testErrorMsg())
            return 1;
        if (!testOpponentNameMsg())
            return 1;
    }
    return 0;
}
