#pragma once
#include <vector>
#include <exception>
#include <memory>

#include "Message.h"
#include "User.h"


class Chat
{
public:
    std::shared_ptr<User> getCurrentUser() const
    {
        return _currentUser;
    }
    std::string messageProcessing(std::string clientMessage);

private:
    std::vector<User> _users;
    std::vector<Message> _messages;
    std::shared_ptr<User> _currentUser;
    std::vector<std::string> _vectorFromMessage;

    std::string loginUser();
    std::string registerUser();
    std::string showChat();
    std::string showUsers();
    std::string addMessage();

    std::vector<User>& getAllUsers()
    {
        return _users;
    }
    std::vector<Message>& getAllMessages()
    {
        return _messages;
    }
    std::shared_ptr<User> getUserByLogin(const std::string& login) const;
    std::shared_ptr<User> getUserByName(const std::string& name) const;

    std::vector<std::string> messageToVector(std::string message, std::string delimiter);
};
