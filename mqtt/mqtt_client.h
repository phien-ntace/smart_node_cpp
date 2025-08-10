#pragma once

#include <iostream>
#include <string>
#include <functional>
#include "mqtt/async_client.h"

using namespace std;

class MQTT_Client : public virtual mqtt::callback {
public:
    using MessageHandler = function<void(const string&, const string&)>;
    MQTT_Client(const string& server_addr, const string& client_id, const string& usr, const string& pass);
    ~MQTT_Client();
    bool is_connected();
    bool connect();
    bool disconnect();
    bool publish(const string& topic, int qos, string msg);
    bool subscribe(const string& topic, int qos);

    void setOnMessage(MessageHandler handler);

private:
    void connected(const string& cause) override;
    void connection_lost(const string& cause) override;
    void message_arrived(mqtt::const_message_ptr msg) override;
    MessageHandler onMessage_;

    string server_addr;
    string client_id;
    string usr;
    string pass;
    bool connect_status = false;

    mqtt::async_client client;          // MQTT client instance
    mqtt::connect_options connOpts;     // Connection options
};