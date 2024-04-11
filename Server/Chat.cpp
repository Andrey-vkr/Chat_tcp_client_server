#include <iostream>
#include <string>
#include "Chat.h"

std::string Chat::messageProcessing(std::string clientMessage)
{
    std::string outMessage;

    _vectorFromMessage = messageToVector(clientMessage, ":");

    if (_vectorFromMessage[0] == "loginUser")
    {
        outMessage = loginUser();
    }
    else if (_vectorFromMessage[0] == "registerUser")
    {
        outMessage = registerUser();
    }
    else if (_vectorFromMessage[0] == "showChat")
    {
        outMessage = showChat();
    }
    else if (_vectorFromMessage[0] == "addMessage")
    {
        outMessage = addMessage();
    }
    else if (_vectorFromMessage[0] == "showUsers")
    {
        outMessage = showUsers();
    }

    return outMessage;
}

std::vector<std::string> Chat::messageToVector(std::string message, std::string delimiter)
{
    std::vector<std::string> resultVector;

    if (!message.empty())
    {
        int start = 0;
        do
        {
            int idx = message.find(delimiter, start);
            if (idx == std::string::npos)
            {
                break;
            }

            int length = idx - start;
            resultVector.push_back(message.substr(start, length));
            start += (length + delimiter.size());
        } while (true);

        resultVector.push_back(message.substr(start));
    }

    return resultVector;
}

std::shared_ptr<User> Chat::getUserByLogin(const std::string& login) const
{
    for (auto& user : _users)
    {
        if (login == user.getUserLogin())
            return std::make_shared<User>(user);
    }

    return nullptr;
}

std::shared_ptr<User> Chat::getUserByName(const std::string& name) const
{
    for (auto& user : _users)
    {
        if (name == user.getUserName())
            return std::make_shared<User>(user);
    }

    return nullptr;
}

std::string Chat::loginUser()
{
    std::string login, password, name, outMessage;
    login = _vectorFromMessage[1];
    password = _vectorFromMessage[2];
 
    if (_currentUser == nullptr || (password != _currentUser->getUserPassword()))
    {
        _currentUser = nullptr;
        outMessage = "loginUser:error";
    }

    if (_currentUser != nullptr && (password == _currentUser->getUserPassword())) 
    {
        name = _currentUser->getUserName();
        outMessage = "loginUser:ok:" + name;
    }

    return outMessage;
}

std::string Chat::registerUser()
{
    std::string login, password, name, outMessage;
    login = _vectorFromMessage[1];
    password = _vectorFromMessage[2];
    name = _vectorFromMessage[3];

    if (getUserByLogin(login) || login == "all")
    {
        outMessage = "registerUser:error:login";
    }

    if (getUserByName(name) || name == "all")
    {
        outMessage = "registerUser:error:name";
    }

    if ((!getUserByName(name) || name != "all") && (!getUserByLogin(login) || login != "all"))
    {
        outMessage = "registerUser:ok:" + name;
    }

    User user = User(login, password, name);
    _users.push_back(user);
    _currentUser = std::make_shared<User>(user);

    return outMessage;
}

std::string Chat::addMessage()
{
    std::string to, text, outMessage;
    to = _vectorFromMessage[1];
    text = _vectorFromMessage[2];

    if (!(to == "all" || getUserByName(to)))
    {
        outMessage = "addMessage:error:" + to;
    }
    else
    {
        outMessage = "addMessage:ok";

        if (to == "all")
            _messages.push_back(Message{ _currentUser->getUserLogin(), "all", text });
        else
            _messages.push_back(Message{ _currentUser->getUserLogin(), getUserByName(to)->getUserLogin(), text });
    }

    return outMessage;
}

std::string Chat::showChat()
{
    std::string from, to, text, outMessage;

    outMessage = "showChat:";

    if (_messages.empty())
    {
        outMessage += "empty:";
    }
    else
    {
        for (auto& mess : _messages)
        {
            if (_currentUser->getUserLogin() == mess.getFrom() || _currentUser->getUserLogin() == mess.getTo() || mess.getTo() == "all")
            {
                from = (_currentUser->getUserLogin() == mess.getFrom()) ? "(me)" : getUserByLogin(mess.getFrom())->getUserName();

                if (mess.getTo() == "all")
                {
                    to = "all";
                }
                else
                {
                    to = (_currentUser->getUserLogin() == mess.getTo()) ? "(me)" : getUserByLogin(mess.getTo())->getUserName();
                }

                text = mess.getText();
            }

            outMessage += from + ":" + to + ":" + text + ":";
        }
    }

    outMessage += "showChat";

    return outMessage;
}

std::string Chat::showUsers()
{
    std::string name, outMessage;

    outMessage = "showUsers:";

    for (auto& user : _users)
    {
        if (_currentUser->getUserLogin() == user.getUserLogin())
        {
            name = "(me)";
        }
        else
        {
            name = user.getUserName();
        }

        outMessage += name + ":";
    }

    outMessage += "showUsers";

    return outMessage;
}