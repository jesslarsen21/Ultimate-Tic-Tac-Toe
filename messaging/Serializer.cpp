#include <sstream>
#include <vector>
#include <cstring>
#include <iostream>
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
#include "UpdateFinishedMsg.h"
#include "Serializer.h"

namespace Serialization {
    std::vector<std::string> SplitStr(const std::string str, const std::string separator) {
        char* data = new char[str.length() + 1];
        const char* const sep = separator.c_str();
        strcpy(data, str.c_str());

        char* token = strtok(data, sep);
        std::vector<std::string> tokens;

        while (token != NULL) {
					std::string stoken(token);
            if (stoken != "" && stoken != " ")
                tokens.push_back(stoken); 
            token = strtok(NULL, sep);
        }
        delete [] data;

        return tokens;
    }

    std::string ReplaceAll(const std::string& str, const std::string& what, const std::string& with) {
        size_t pos = 0;
        std::string result = str;
        while ((pos = result.find(what, pos)) != std::string::npos) {
            result.replace(pos, what.length(), with);
            pos += with.length();
        }

        return result;
    }

    std::string ReplaceSpaces(const std::string& str) {
        if (str == "") {
            return "NULL";
        }
        return ReplaceAll(str, " ", "%20");
    }

    std::string UnreplaceSpaces(const std::string& str) {
        if (str == "NULL")
            return "";
        return ReplaceAll(str, "%20", " ");
    }

    std::string SerializeTurnMsg(
            const Messaging::TurnMsg* const msg, 
            const std::string separator, 
            std::string& result ) {
        std::ostringstream out;

        out << msg->GetTypeName() << separator
            << msg->GetPlayer() << separator
            << msg->GetBoard() << separator
            << msg->GetPosition();

        result = out.str();
        return "";
    }

    std::string DeserializeTurnMsg(
            const std::vector<std::string>& parts,
            Messaging::IMsg*& result) {
        std::ostringstream err;

        if (parts.size() != 4) {
            err << "Error: Invalid number of arguments. "
                << "Expected 4, but got " << parts.size() 
                << ".";

            err << "Tokens => [ ";
            for (auto token : parts) 
                err << token << ", ";
            err << std::endl;

            return err.str();
        }

        auto playerStr = parts[1];
        auto boardStr = parts[2];
        auto posStr = parts[3];

        char player;
        int board;
        int pos;
             
        try {
            player = playerStr.c_str()[0];
        }
        catch (const std::exception& exn) {
            err << "Error: Exception when parsing Messaging::TakeTurn::player property from string. "
                << "Expected a char, but got unknown type. "
                << "Exn => " << exn.what();
            return err.str();
        }

        try {
            board = std::stoi(boardStr);
        }
        catch (const std::exception& exn) {
            err << "Error: Exception when parsing Messaging::TakeTurn::board property from string. "
                << "Expected an integer, but got unknown type. "
                << "Exn => " << exn.what();
            return err.str();
        }

        try {
            pos = std::stoi(posStr);
        }
        catch (const std::exception& exn) {
            err << "Error: Exception when parsing Messaging::TakeTurn::position property from string. "
                << "Expected an integer, but got unknown type. "
                << "Exn => " << exn.what();
            return err.str();
        }

        result = (Messaging::IMsg*)(new Messaging::TurnMsg(player, board, pos));
        return "";
    }

    std::string SerializeUpdateMsg(
            const Messaging::UpdateMsg* const msg,
            const std::string separator,
            std::string& result ) { 
        std::ostringstream out;

        out << msg->GetTypeName();

        result = out.str();
        return "";
    }

    std::string DeserializeUpdateMsg(
            const std::vector<std::string>& parts,
            Messaging::IMsg*& result) {
        std::ostringstream err;

        if (parts.size() != 1) {
            err << "Error: Invalid number of arguments. "
                << "Expected 1, but got " << parts.size() 
                << ".";
            return err.str();
        }

        result = (Messaging::IMsg*)(new Messaging::UpdateMsg());        
        return "";
    }

    std::string SerializeMarkerMsg(
            const Messaging::MarkerMsg* const msg,
            const std::string separator,
            std::string& result ) {
        std::ostringstream out;

        out << msg->GetTypeName() << separator
            << msg->GetPlayer() << separator
            << msg->GetBoard() << separator
            << msg->GetPosition();

        result = out.str();
        return "";
    }

    std::string DeserializeMarkerMsg(
            const std::vector<std::string> parts,
            Messaging::IMsg*& result) {
        std::ostringstream err;
        
        if (parts.size() != 4) {
            err << "Error: Invalid number of arguments. "
                << "Expected 4, but got " << parts.size() 
                << ".";
            return err.str();
        }

        auto playerStr = parts[1];
        auto boardStr = parts[2];
        auto positionStr = parts[3];

        char player;
        int board;
        int position;

        try {
            player = playerStr.c_str()[0];
            board = std::stoi(boardStr);
            position = std::stoi(positionStr);
        }
        catch (const std::exception& exn) {
            err << "Error: Unable to parse expected values for Messaging::MarkerMsg. "
                << "Exn => " << exn.what();
            return err.str();
        }

        result = (Messaging::IMsg*)(new Messaging::MarkerMsg(player, board, position));
        return "";
    }

    std::string SerializeNextTurn(
            const Messaging::NextTurnMsg* const msg,
            const std::string separator,
            std::string& result ) { 
        std::ostringstream out;

        out << msg->GetTypeName() << separator
            << msg->GetPlayer() << separator
            << msg->GetNextBoard();

        result = out.str();
        return ""; 
    }

    std::string DeserializeNextTurnMsg(
            const std::vector<std::string>& parts,
            Messaging::IMsg*& result) {
        std::ostringstream err;

        if (parts.size() != 3) {
            err << "Error: Invalid number of arguments. "
                << "Expected 3, but got " << parts.size() 
                << ".";
            return err.str();
        }

        auto playerStr = parts[1];
        auto nextBoardStr = parts[2];
        
        char player;
        int nextBoard;

        try {
            player = playerStr.c_str()[0];
            nextBoard = std::stoi(nextBoardStr);
        }
        catch (const std::exception& exn) {
            err << "Error: Unable to parse expected values for Messaging::NextTurnMsg. "
                << "Exn => " << exn.what();
            return err.str();
        }

        result = (Messaging::IMsg*)(new Messaging::NextTurnMsg(player, nextBoard));
        return "";
    }

    std::string SerializeWinState(
            const Messaging::WinStateMsg* const msg,
            const std::string separator,
            std::string& result ) {
        std::ostringstream out;

        out << msg->GetTypeName() << separator
            << msg->GetPlayer();

        result = out.str();
        return "";
    }

    std::string DeserializeWinStateMsg(
            const std::vector<std::string>& parts,
            Messaging::IMsg*& result) {
        std::ostringstream err;

        if (parts.size() != 2) {
            err << "Error: Invalid number of arguments. "
                << "Expected 2, but got " << parts.size() 
                << ".";
            return err.str();
        }

        auto playerStr = parts[1];
        
        char player;

        try {
            player = playerStr.c_str()[0];
        }
        catch (const std::exception& exn) {
            err << "Error: Unable to parse expected values for Messaging::WinStateMsg. "
                << "Exn => " << exn.what();
            return err.str();
        }

        result = (Messaging::IMsg*)(new Messaging::WinStateMsg(player));
        return "";
    }

    std::string SerializeBoardState(
            const Messaging::BoardStateMsg* const msg,
            const std::string separator,
            std::string& result) { 
        std::ostringstream out;

        out << msg->GetTypeName() << separator
            << msg->GetPlayer() << separator
            << msg->GetBoard();

        result = out.str();
        return "" ;
    }

    std::string DeserializeBoardStateMsg(
            const std::vector<std::string>& parts,
            Messaging::IMsg*& result) {
        std::ostringstream err;

        if (parts.size() != 3) {
            err << "Error: Invalid number of arguments. "
                << "Expected 3, but got " << parts.size() 
                << ".";
            return err.str();
        }

        auto playerStr = parts[1];
        auto boardStr = parts[2];
        
        char player;
        int board;

        try {
            player = playerStr.c_str()[0];
            board = std::stoi(boardStr);
        }
        catch (const std::exception& exn) {
            err << "Error: Unable to parse expected values for Messaging::BoardStateMsg. "
                << "Exn => " << exn.what();
            return err.str();
        }

        
        result = (Messaging::IMsg*)(new Messaging::BoardStateMsg(player, board));
        return "";
    }

    std::string SerializePingMsg(
            const Messaging::PingMsg* const msg,
            const std::string separator,
            std::string& result) {
        std::ostringstream out;

        out << msg->GetTypeName();

        result = out.str();
        return "";
    }

    std::string DeserializePingMsg(
            const std::vector<std::string>& parts,
            Messaging::IMsg*& result) {
        std::ostringstream err;

        if (parts.size() != 1) {
            err << "Error: Invalid number of arguments. "
                << "Expected 1, but got " << parts.size() 
                << ".";
            return err.str();
        }

        result = (Messaging::IMsg*)(new Messaging::PingMsg());        
        return "";
    }

    std::string SerializePongMsg(
            const Messaging::PongMsg* const msg,
            const std::string separator,
            std::string& result) {
        std::ostringstream out;

        out << msg->GetTypeName();

        result = out.str();
        return "";
    }

    std::string DeserializePongMsg(
            const std::vector<std::string>& parts,
            Messaging::IMsg*& result) {
        std::ostringstream err;

        if (parts.size() != 1) {
            err << "Error: Invalid number of arguments. "
                << "Expected 1, but got " << parts.size() 
                << ".";
            return err.str();
        }

        result = (Messaging::IMsg*)(new Messaging::PongMsg());        
        return "";
    }

    std::string SerializeWhoamiMsg(
            const Messaging::WhoamiMsg* const msg,
            const std::string separator,
            std::string& result) {
        std::ostringstream out;

        out << msg->GetTypeName();

        result = out.str();
        return "";
    }

    std::string DeserializeWhoamiMsg(
            const std::vector<std::string>& parts,
            Messaging::IMsg*& result) {
        std::ostringstream err;

        if (parts.size() != 1) {
            err << "Error: Invalid number of arguments. "
                << "Expected 1, but got " << parts.size() 
                << ".";
            return err.str();
        }

        result = (Messaging::IMsg*)(new Messaging::WhoamiMsg());        
        return "";
    }
    
    std::string SerializeYouAreMsg(
            const Messaging::YouAreMsg* const msg,
            const std::string separator,
            std::string& result ) {
        std::ostringstream out;

        out << msg->GetTypeName() << separator
            << msg->GetPlayer() << separator
			<< msg->GetName();

        result = out.str();
        return "";
    }

    std::string DeserializeYouAreMsg(
            const std::vector<std::string>& parts,
            Messaging::IMsg*& result) {
        std::ostringstream err;

        if (parts.size() != 3) {
            err << "Error: Invalid number of arguments. "
                << "Expected 3, but got " << parts.size() 
                << ".";
            return err.str();
        }

        auto playerStr = parts[1];
		auto name = parts[2];
        
        char player;

        try {
            player = playerStr.c_str()[0];
        }
        catch (const std::exception& exn) {
            err << "Error: Unable to parse expected values for Messaging::YouAreMsg. "
                << "Exn => " << exn.what();
            return err.str();
        }

        result = (Messaging::IMsg*)(new Messaging::YouAreMsg(player, name));
        return "";
    }

    std::string SerializeTurnFailedMsg(
            const Messaging::TurnFailedMsg* const msg,
            const std::string separator,
            std::string& result ) {
        std::ostringstream out;

        out << msg->GetTypeName() << separator
            << msg->GetPlayer() << separator
            << ReplaceSpaces(msg->GetMessage());

        result = out.str();
        return "";
    }

    std::string DeserializeTurnFailedMsg(
            const std::vector<std::string>& parts,
            Messaging::IMsg*& result) {
        std::ostringstream err;

        if (parts.size() != 3) {
            err << "Error: Invalid number of arguments. "
                << "Expected 3, but got " << parts.size() 
                << ".";
            return err.str();
        }

        auto playerStr = parts[1];
        
        char player;
        std::string messageStr = parts[2];

        try {
            player = playerStr.c_str()[0];

            // Try to pull the message string from the payload.
            messageStr = UnreplaceSpaces(messageStr);
        }
        catch (const std::exception& exn) {
            err << "Error: Unable to parse expected values for Messaging::TurnFailed. "
                << "Exn => " << exn.what();
            return err.str();
        }

        result = (Messaging::IMsg*)(new Messaging::TurnFailedMsg(player, messageStr));
        return "";
    }
    
    std::string SerializeOpponentNameMsg(
            const Messaging::OpponentNameMsg* const msg,
            const std::string separator,
            std::string& result ) {
        std::ostringstream out;

        out << msg->GetTypeName() << separator
            << ReplaceSpaces(msg->GetUsername());

        result = out.str();
        return "";
    }

    std::string DeserializeOpponentNameMsg(
            const std::vector<std::string>& parts,
            Messaging::IMsg*& result) {
        std::ostringstream err;

        if (parts.size() != 2) {
            err << "Error: Invalid number of arguments. "
                << "Expected 2, but got " << parts.size() 
                << ".";
            return err.str();
        }

        const std::string username = UnreplaceSpaces(parts[1]);

        result = (Messaging::IMsg*)(new Messaging::OpponentNameMsg(username));
        return "";
    }
    
    std::string SerializeUpdateFinishedMsg(
            const Messaging::UpdateFinishedMsg* const msg,
            const std::string separator,
            std::string& result) {
        std::ostringstream out;

        out << msg->GetTypeName();

        result = out.str();
        return "";
    }

    std::string DeserializeUpdateFinishedMsg(
            const std::vector<std::string>& parts,
            Messaging::IMsg*& result) {
        std::ostringstream err;

        if (parts.size() != 1) {
            err << "Error: Invalid number of arguments. "
                << "Expected 1, but got " << parts.size() 
                << ".";
            return err.str();
        }

        result = (Messaging::IMsg*)(new Messaging::UpdateFinishedMsg());        
        return "";
    }

    std::string SerializeErrorMsg(
            const Messaging::ErrorMsg* const msg,
            const std::string separator,
            std::string& result ) {
        std::ostringstream out;

		out << msg->GetTypeName() << separator
			<< ReplaceSpaces(msg->GetWhen()) << separator
			<< msg->GetSeverity() << separator
			<< ReplaceSpaces(msg->GetSender()) << separator
			<< ReplaceSpaces(msg->GetWhat()) << separator
			<< ReplaceSpaces(msg->GetReason());

		const Messaging::ErrorMsg* const nextError = msg->GetNextError();

		if (nextError) {
			std::string errorMsgResult;
			std::string err = SerializeErrorMsg(nextError, separator, errorMsgResult);

			if (err != "") {
				return err;
			}

			out << separator << errorMsgResult;
		}

        result = out.str();
        return "";
    }

	std::string DeserializeErrorMsgSegement(
		std::vector<std::basic_string<char>>::const_iterator start,
		Messaging::IMsg*& result,
		const Messaging::ErrorMsg* const nextError) {

		std::ostringstream err;

		auto when = UnreplaceSpaces(*start);
		int severity;
		auto severityStr = *(++start);
		auto sender = UnreplaceSpaces(*(++start));
		auto what = UnreplaceSpaces(*(++start));
		auto reason = UnreplaceSpaces(*(++start));

        std::cout
            << "When = " << when << std::endl
            << "Severity = " << severityStr << std::endl
            << "Sender = " << sender << std::endl
            << "What = " << what << std::endl
            << "Reason = " << reason << std::endl;


		try {
			severity = std::stoi(severityStr);
		}
		catch (const std::exception& exn) {
			err << "Error: Unable to parse expected values for Messaging::Error. "
				<< "Exn => " << exn.what();
			return err.str();
		}

		result = (Messaging::IMsg*)(new Messaging::ErrorMsg(
			when,
			severity,
			sender,
			what,
			reason,
			nextError
			));
        return "";
	}

    std::string DeserializeErrorMsg(
            const std::vector<std::string>& parts,
            Messaging::IMsg*& result) {
        std::ostringstream err;

		// Errors arrive in parts of 6.
		
		if (parts.size() % 6 != 0) {
            err << "Error: Invalid number of arguments. "
                << "Expected the number of segements to be on an increment of 6 per message."
				<< "Total number of parts equals " << parts.size() << ", this is an invalid number of arguments."
                << ".";
            return err.str();
        }

		// Get the number of messages that needs to be parsed.

		// Walk through each segments, starting from the end for the `nextError` part of the ctor.
	
		Messaging::ErrorMsg* curMsg = 0;
		std::string error;

        std::cout << "Pulling data from segments." << std::endl;

		for (int i = parts.size() - 6; i >= 0; i -= 6) {
		    std::vector<std::string>::const_iterator cur = parts.begin() + i + 1;

            std::cout << "Starting at index " << i << std::endl;

            const Messaging::ErrorMsg* const nextError = curMsg;            
			error = DeserializeErrorMsgSegement(cur, (Messaging::IMsg*&)curMsg, nextError);
			if (error != "") {
				return error;
			}
		}	

		result = (Messaging::IMsg*)(curMsg);
        return "";
    }

    std::string SerializeLoginMsg(
            const Messaging::LoginMsg* const msg,
            const std::string separator,
            std::string& result ) {
        std::ostringstream out;

        const std::string email = ReplaceSpaces(msg->GetEmail());
        const std::string password = ReplaceSpaces(msg->GetPassword());

        out << msg->GetTypeName() << separator
            << email << separator 
            << password;

        result = out.str();
        return "";
    }

    std::string DeserializeLoginMsg(
            const std::vector<std::string>& parts,
            Messaging::IMsg*& result) {
        std::ostringstream err;

        if (parts.size() != 3) {
            err << "Error: Invalid number of arguments. "
                << "Expected 3, but got " << parts.size() 
                << ".";
            return err.str();
        }

        const std::string email = UnreplaceSpaces(parts[1]);
        const std::string password = UnreplaceSpaces(parts[2]);

        result = (Messaging::IMsg*)(new Messaging::LoginMsg(email, password));
        return "";
    }
    
    std::string SerializeLoginReplyMsg(
            const Messaging::LoginReplyMsg* const msg,
            const std::string separator,
            std::string& result ) {
        std::ostringstream out;

        const std::string valid = 
            (msg->IsValid()) ?
                "valid" :
                "invalid";

        out << msg->GetTypeName() << separator
            << valid << separator
            << msg->GetToken();

        result = out.str();
        return "";
    }

    std::string DeserializeLoginReplyMsg(
            const std::vector<std::string>& parts,
            Messaging::IMsg*& result) {
        std::ostringstream err;

        if (parts.size() != 3) {
            err << "Error: Invalid number of arguments. "
                << "Expected 3, but got " << parts.size() 
                << ".";
            return err.str();
        }

        const std::string validStr = parts[1];
        const std::string token = parts[2]; // This should never have spaces in it.
        bool valid;

        try {
            if (validStr == "valid")
                valid = true;
            else if (validStr == "invalid")
                valid = false;
            else {
                err << "Error: Expected keywords { 'valid' | 'invalid' }, but found {"
                    << validStr << " }";

                return err.str();
            }
        }
        catch (const std::exception& exn) {
            err << "Error: Unable to parse expected values for Messaging::TurnFailed. "
                << "Exn => " << exn.what();
            return err.str();
        }

        result = (Messaging::IMsg*)(new Messaging::LoginReplyMsg(valid, token));
        return "";
    }

    std::string SerializeSignUpMsg(
            const Messaging::SignUpMsg* const msg,
            const std::string separator,
            std::string& result ) {
        std::ostringstream out;

        out << msg->GetTypeName() << separator
            << ReplaceSpaces(msg->GetUsername()) << separator
            << ReplaceSpaces(msg->GetEmail()) << separator
            << ReplaceSpaces(msg->GetPassword()) << separator
            << ReplaceSpaces(msg->GetPasswordConfirm());

        result = out.str();
        return "";
    }

    std::string DeserializeSignUpMsg(
            const std::vector<std::string>& parts,
            Messaging::IMsg*& result) {
        std::ostringstream err;

        if (parts.size() != 5) {
            err << "Error: Invalid number of arguments. "
                << "Expected 5, but got " << parts.size() 
                << ".";
            return err.str();
        }

        const std::string username = UnreplaceSpaces(parts[1]);
        const std::string email = UnreplaceSpaces(parts[2]);
        const std::string password = UnreplaceSpaces(parts[3]);
        const std::string passwordConfirm = UnreplaceSpaces(parts[4]);

        result = (Messaging::IMsg*)(new Messaging::SignUpMsg(
            username,
            email,
            password,
            passwordConfirm
        ));
        return "";
    }

    std::string SerializeSignUpReplyMsg(
            const Messaging::SignUpReplyMsg* const msg,
            const std::string separator,
            std::string& result ) {
        std::ostringstream out;

        const std::string usernameValid =
            (msg->IsUsernameValid()) ?
                "valid" : "invalid";

        const std::string emailValid =
            (msg->IsEmailValid()) ?
                "valid" : "invalid";

        std::string passwordState;

        switch (msg->GetPasswordState()) {
            case Messaging::SignUpReplyMsg::Invalid:
            passwordState = "invalid";
            break;
            case Messaging::SignUpReplyMsg::Valid:
            passwordState = "valid";
            break;
        case Messaging::SignUpReplyMsg::Unmatched:
            passwordState = "unmatched";
            break;
        default:
            return "Error: Got unmatched enumerated value for Messaging::SignUpReplyMsg::PasswordState.";
        };

        out << msg->GetTypeName() << separator
            << usernameValid << separator
            << emailValid << separator
            << passwordState;

        result = out.str();
        return "";
    }

    std::string DeserializeSignUpReplyMsg(
            const std::vector<std::string>& parts,
            Messaging::IMsg*& result) {
        std::ostringstream err;

        if (parts.size() != 4) {
            err << "Error: Invalid number of arguments. "
                << "Expected 4, but got " << parts.size() 
                << ".";
            return err.str();
        }

        const std::string& usernameStr = parts[1];
        const std::string& emailStr = parts[2];
        const std::string& passwordStateStr = parts[3];

        bool usernameValid, emailValid;
        Messaging::SignUpReplyMsg::PasswordState passwordState;

        if (usernameStr == "valid")
            usernameValid = true;
        else if (usernameStr == "invalid")
            usernameValid = false;
        else {
            err << "Error: Expected keywords { 'valid' | 'invalid' }, but found {"
                << usernameStr << " }";

            return err.str();
        }

        if (emailStr == "valid")
            emailValid = true;
        else if (emailStr == "invalid")
            emailValid = false;
        else {
            err << "Error: Expected keywords { 'valid' | 'invalid' }, but found {"
                << emailStr << " }";

            return err.str();
        }

        if (passwordStateStr == "invalid") {
            passwordState = Messaging::SignUpReplyMsg::Invalid;
        }
        else if (passwordStateStr == "valid") {
            passwordState = Messaging::SignUpReplyMsg::Valid;
        }
        else if (passwordStateStr == "unmatched") {
            passwordState = Messaging::SignUpReplyMsg::Unmatched;
        }
        else {
            err << "Error Expceted { `invalid` | `valid` | `unmatched` }, but found { "
                << passwordStateStr << " }";
            return err.str();
        }
        
        result = (Messaging::IMsg*)(new Messaging::SignUpReplyMsg(
            usernameValid,
            emailValid,
            passwordState
        ));
        return "";
    }


    const std::string Serializer::GSeparator = " ";

    std::string Serializer::Deserialize(const std::string data, Messaging::IMsg*& result) const {
        std::ostringstream err;

        auto parts = SplitStr(data, GSeparator);
        if (parts.size() == 0) {
            return "Error: Invalid number of arguments to form a message.";
        } 

        //std::cout << "Deserializing. . . " << std::endl;

        auto typeName = parts[0];
        if (typeName == Messaging::TurnMsg::GTypeName) {
            return DeserializeTurnMsg(parts, result);
        }
        else if (typeName == Messaging::UpdateMsg::GTypeName) {
            return DeserializeUpdateMsg(parts, result);
        }
        else if (typeName == Messaging::MarkerMsg::GTypeName) {
            return DeserializeMarkerMsg(parts, result);
        }
        else if (typeName == Messaging::NextTurnMsg::GTypeName) {
            return DeserializeNextTurnMsg(parts, result);
        }
        else if (typeName == Messaging::WinStateMsg::GTypeName) {
            return DeserializeWinStateMsg(parts, result);
        }
        else if (typeName == Messaging::BoardStateMsg::GTypeName) {
            return DeserializeBoardStateMsg(parts, result);
        }
        else if (typeName == Messaging::PingMsg::GTypeName) {
            return DeserializePingMsg(parts, result);
        }
        else if (typeName == Messaging::PongMsg::GTypeName) {
            return DeserializePongMsg(parts, result);
        }
        else if (typeName == Messaging::WhoamiMsg::GTypeName) {
            return DeserializeWhoamiMsg(parts, result);
        }
        else if (typeName == Messaging::YouAreMsg::GTypeName) {
            return DeserializeYouAreMsg(parts, result);
        }
        else if (typeName == Messaging::TurnFailedMsg::GTypeName) {
            return DeserializeTurnFailedMsg(parts, result);
        }
        else if (typeName == Messaging::LoginMsg::GTypeName) {
            return DeserializeLoginMsg(parts, result);
        }
        else if (typeName == Messaging::LoginReplyMsg::GTypeName) {
            return DeserializeLoginReplyMsg(parts, result);
        }
        else if (typeName == Messaging::SignUpMsg::GTypeName) {
            return DeserializeSignUpMsg(parts, result);
        }
        else if (typeName == Messaging::SignUpReplyMsg::GTypeName) {
            return DeserializeSignUpReplyMsg(parts, result);
        }
        else if (typeName == Messaging::ErrorMsg::GTypeName) {
            return DeserializeErrorMsg(parts, result);
        }
        else if (typeName == Messaging::OpponentNameMsg::GTypeName) {
            return DeserializeOpponentNameMsg(parts, result);
        }
        else if (typeName == Messaging::UpdateFinishedMsg::GTypeName) {
            return DeserializeUpdateFinishedMsg(parts, result);
        }
        else {
            err << "Error: Invalid TypeName { "
                << typeName
                << " }";
            return err.str();
        }
    }

    std::string Serializer::Serialize(const Messaging::IMsg* const msg, std::string& result) const {
        std::ostringstream out, err;
        
        const std::string typeName = msg->GetTypeName();
        if (typeName == Messaging::TurnMsg::GTypeName) {
            auto nMsg = dynamic_cast<const Messaging::TurnMsg* const>(msg);
            if (nMsg == NULL) {
                return "Error: Cannot cast Messaging::IMsg* to Messaging::TakeTurnMsg*, evne though TypeName matched up.";
            }

            return SerializeTurnMsg(nMsg, GSeparator, result);
        }
        else if (typeName == Messaging::UpdateMsg::GTypeName) {
            auto nMsg = dynamic_cast<const Messaging::UpdateMsg* const>(msg);
            if (nMsg == NULL) {
                return "Error: Cannot cast Messaging::IMsg* to Messaging::UpdateMsg*, even though TypeName mached up.";                
            }

            return SerializeUpdateMsg(nMsg, GSeparator, result);
        }
        else if (typeName == Messaging::MarkerMsg::GTypeName) {
            auto nMsg = dynamic_cast<const Messaging::MarkerMsg* const>(msg);
            if (nMsg == NULL) {
                return "Error: Cannot cast Messaging::IMsg* to Messaging::MarkerMsg*, even though TypeName matched up.";
            }

            return SerializeMarkerMsg(nMsg, GSeparator, result);    
        }   
        else if (typeName == Messaging::NextTurnMsg::GTypeName) {
            auto nMsg = dynamic_cast<const Messaging::NextTurnMsg* const>(msg);
            if (nMsg == NULL) {
                return "Error: Cannot cast Messaging::IMsg* to Messaging::NextTurnMsg*, even though TypeName matched up.";
            }

            return SerializeNextTurn(nMsg, GSeparator, result);
        }
        else if (typeName == Messaging::WinStateMsg::GTypeName) {
            auto nMsg = dynamic_cast<const Messaging::WinStateMsg* const>(msg);
            if (nMsg == NULL) {
                return "Error: Cannot cast Messaging::IMsg* to Messaging::WinStateMsg*, even though TypeName matched up.";
            }

            return SerializeWinState(nMsg, GSeparator, result);
        }
        else if (typeName == Messaging::BoardStateMsg::GTypeName) {
            auto nMsg = dynamic_cast<const Messaging::BoardStateMsg* const>(msg);
            if (nMsg == NULL) {
                return "Error: Cannot cast Messaging::IMsg* to Messaging::BoardStateMsg*, even though TypeName matched up.";
            }

            return SerializeBoardState(nMsg, GSeparator, result);
        }
        else if (typeName == Messaging::PingMsg::GTypeName) {
            auto nMsg = dynamic_cast<const Messaging::PingMsg* const>(msg);
            if (nMsg == NULL) {
                return "Error: Cannot cast Messaging::IMsg* to Messaging::PingMsg*, even though TypeName matched up.";
            }

            return SerializePingMsg(nMsg, GSeparator, result);
        }
        else if (typeName == Messaging::PongMsg::GTypeName) {
            auto nMsg = dynamic_cast<const Messaging::PongMsg* const>(msg);
            if (nMsg == NULL) {
                return "Error: Cannot cast Messaging::IMsg* to Messaging::PongMsg*, even though TypeName matched up.";
            }

            return SerializePongMsg(nMsg, GSeparator, result);
        }
        else if (typeName == Messaging::WhoamiMsg::GTypeName) {
            auto nMsg = dynamic_cast<const Messaging::WhoamiMsg* const>(msg);
            if (nMsg == NULL) {
                return "Error: Cannot cast Messaging::IMsg* to Messaging::WhoamiMsg*, even though TypeName matched up.";
            }

            return SerializeWhoamiMsg(nMsg, GSeparator, result);
        } 
        else if (typeName == Messaging::YouAreMsg::GTypeName) {
            auto nMsg = dynamic_cast<const Messaging::YouAreMsg* const>(msg);
            if (nMsg == NULL) {
                return "Error: Cannot cast Messaging::IMsg* to Messaging::YouAreMsg*, even though TypeName matched up.";
            }

            return SerializeYouAreMsg(nMsg, GSeparator, result);
        }
        else if (typeName == Messaging::TurnFailedMsg::GTypeName) {
            auto nMsg = dynamic_cast<const Messaging::TurnFailedMsg* const>(msg);
            if (nMsg == NULL) {
                return "Error: Cannot cast Messaging::IMsg* to Messaging::TurnFailedMsg*, even though TypeName matched up.";
            }

            return SerializeTurnFailedMsg(nMsg, GSeparator, result);
        }
        else if (typeName == Messaging::LoginMsg::GTypeName) {
            auto nMsg = dynamic_cast<const Messaging::LoginMsg* const>(msg);
            if (nMsg == NULL) {
                return "Error: Cannot cast Messaging::IMsg* to Messaging::LoginMsg*, even though TypeName matched up.";
            }

            return SerializeLoginMsg(nMsg, GSeparator, result);
        }
        else if (typeName == Messaging::LoginReplyMsg::GTypeName) {
            auto nMsg = dynamic_cast<const Messaging::LoginReplyMsg* const>(msg);
            if (nMsg == NULL) {
                return "Error: Cannot cast Messaging::IMsg* to Messaging::LoginReplyMsg*, even though TypeName matched up.";
            }

            return SerializeLoginReplyMsg(nMsg, GSeparator, result);
        }
        else if (typeName == Messaging::SignUpMsg::GTypeName) {
            auto nMsg = dynamic_cast<const Messaging::SignUpMsg* const>(msg);
            if (nMsg == NULL) {
                return "Error: Cannot cast Messaging::IMsg* to Messaging::SignUpMsg*, even though TypeName matched up.";
            }

            return SerializeSignUpMsg(nMsg, GSeparator, result);
        }
        else if (typeName == Messaging::SignUpReplyMsg::GTypeName) {
            auto nMsg = dynamic_cast<const Messaging::SignUpReplyMsg* const>(msg);
            if (nMsg == NULL) {
                return "Error: Cannot cast Messaging::IMsg* to Messaging::SignUpReplyMsg*, even though TypeName matched up.";
            }

            return SerializeSignUpReplyMsg(nMsg, GSeparator, result);
        }
        else if (typeName == Messaging::ErrorMsg::GTypeName) {
            auto nMsg = dynamic_cast<const Messaging::ErrorMsg* const>(msg);
            if (nMsg == NULL) {
                return "Error: Cannot cast Messaging::IMsg* to Messaging::ErrorMsg*, even though TypeName matched up.";
            }

            return SerializeErrorMsg(nMsg, GSeparator, result);
        }
        else if (typeName == Messaging::OpponentNameMsg::GTypeName) {
            auto nMsg = dynamic_cast<const Messaging::OpponentNameMsg* const>(msg);
            if (nMsg == NULL) {
                return "OpponentName: Cannot cast Messaging::IMsg* to Messaging::OpponentNameMsg*, even though TypeName matched up.";
            }

            return SerializeOpponentNameMsg(nMsg, GSeparator, result);
        }
        else if (typeName == Messaging::UpdateFinishedMsg::GTypeName) {
            auto nMsg = dynamic_cast<const Messaging::UpdateFinishedMsg* const>(msg);
            if (nMsg == NULL) {
                return "Error: Cannot cast Messaging::IMsg* to Messaging::UpdateFinishedMsg*, even though TypeName matched up.";
            }

            return SerializeUpdateFinishedMsg(nMsg, GSeparator, result);
        }
            
        err << "Error: Unable to match Messaging::IMsg::GetTypeName() => { "
            << typeName << " } to correct serialization.";
        return err.str(); 
    }
};
