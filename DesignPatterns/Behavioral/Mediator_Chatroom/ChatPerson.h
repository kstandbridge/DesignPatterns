﻿#pragma once
#include <string>
#include <iostream>
#include <vector>
using namespace std;

namespace Mediator_Chatroom
{
    struct ChatRoom;

    struct Person
    {
        string name;
        ChatRoom* room = nullptr;
        vector<string> chat_log;

        explicit Person(const string& name);
        void say(const string& message) const;
        void receive(const string& origin, const string& message);
        void pm(const string& who, const string& message);
    };
}
