#pragma once
#include <sio_client.h>
#include <Object.h>
class SioClientWrapper{
	//Socket.IO client object
	sio::client client;
	std::string myRoom, dstRoom, enterRoomEvent, enterRoomKey;
	std::vector<std::string> eventList;
	std::map<std::string, sio::message::ptr> latestData;
	std::map<std::string, bool> updateFlag;

	//WebSocket Connection Listener
	void onOpen(void);
	void onFail(void);
	void onClose(sio::client::close_reason const& reason);
	//Socket Namespace Connection Listener
	void onSocketOpen(std::string const& nsp);
	void onSocketClose(std::string const& nsp);
	//On event
	void OnEvent(sio::event &event);
public:
	SioClientWrapper();
	~SioClientWrapper();
        //set event list 
	void setEventList(const std::vector<std::string>& list);
        // set own room name
	void setMyRoom(const std::string& name);
        // set destination room name
	void setDstRoom(const std::string& name);

        // set room event name
        void setRoomEvent(const std::string& eventName, const std::string& key);
        // initialize and start connection
	void start(const std::string& uri);
        // the data of the event is updated or not
	bool isUpdated(const std::string& name);
        // get data of ptr
	sio::message::ptr getData(const std::string& name);
        // send data
	template<typename T>
	void sendData(const std::string& eventName, const T& data){
		client.socket()->emit("transfer", sio::Object()
			.add("event", eventName)
			.add("room", sio::Array().add(dstRoom))
			.add("data", data).pack());
	}
        // send data to destination
	template<typename T>
	void sendData(const std::string& eventName,const std::string& dst, const T& data){
		client.socket()->emit("transfer", sio::Object()
			.add("event", eventName)
			.add("room", sio::Array().add(dst))
			.add("data", data).pack());
	}
	void sendData(const std::string& eventName);
        void sendDataToRoom(const std::string& eventName, const std::string& roomName, sio::Object data);
	void sendString(std::string *eventName, std::string *message);
        void sendString(const std::string& eventName, const std::string& message);
        void sendObject(const std::string& eventName, sio::Object object);
        void sendStringToRoom(const std::string& eventName, const std::string& roomName, const std::string& message);
};
