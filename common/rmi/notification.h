/*
 *  Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 */

#ifndef __RMI_NOTIFICATION_H__
#define __RMI_NOTIFICATION_H__

#include <string>
#include <vector>
#include <mutex>
#include <unordered_map>

#include "socket.h"
#include "message.h"

namespace rmi {

class Notification {
public:
    Notification();
	Notification(const std::string& name);
    Notification(const Notification&) = default;
    Notification(Notification&&);

	int createSubscriber();
    void removeSubscriber(const int id);

	template<typename... Args>
	void notify(Args&&... args);

private:
	std::string signalName;
	std::vector<Socket> subscribers;
    std::mutex subscriberLock;
};

template<typename... Args>
void Notification::notify(Args&&... args)
{
	Message msg(Message::Signal, signalName);
	msg.packParameters(std::forward<Args>(args)...);

    std::lock_guard<std::mutex> lock(subscriberLock);

	for (Socket& subscriber : subscribers) {
        msg.encode(subscriber);
	}
}

} // namespae rmi
#endif //__RMI_NOTIFICATION_H__
