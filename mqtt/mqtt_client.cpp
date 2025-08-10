#include "mqtt_client.h"

MQTT_Client::MQTT_Client(const string& server_addr, const string& client_id, const string& usr, const string& pass)
    : client(server_addr, client_id)   // initializer member client
{
    this->server_addr = server_addr;
    this->client_id = client_id;
    this->usr = usr;
    this->pass = pass;

    client.set_callback(*this);
    connOpts.set_user_name(usr);
    connOpts.set_password(pass);

    mqtt::ssl_options sslopts;
    sslopts.set_trust_store("/etc/ssl/certs/ca-certificates.crt");
    connOpts.set_ssl(sslopts);
}

MQTT_Client::~MQTT_Client() 
{
    disconnect();
}

bool MQTT_Client::is_connected() 
{
    return connect_status;
}

bool MQTT_Client::connect() 
{
    try {
        cout << "[MQTT] Connecting to broker..." << endl;
        client.connect(connOpts)->wait();
        connect_status = true;
        cout << "[MQTT] Connected!" << endl;
        return true;
    }
    catch (const mqtt::exception& e) {
        cerr << "[MQTT] Connect error: " << e.what() << endl;
        return false;
    }
}

bool MQTT_Client::disconnect() 
{
    if (connect_status) {
        try {
            client.disconnect()->wait();
            connect_status = false;
            cout << "[MQTT] Disconnected!" << endl;
            return true;
        }
        catch (const mqtt::exception& e) {
            cerr << "[MQTT] Disconnect error: " << e.what() << endl;
        }
    }
    return false;
}

bool MQTT_Client::publish(const string& topic, int qos, string msg) 
{
    if (!connect_status) return false;
    try {
        auto pubmsg = mqtt::make_message(topic, msg);
        pubmsg->set_qos(qos);
        client.publish(pubmsg)->wait();
        cout << "[MQTT] Published: " << msg << " -> " << topic << endl;
        return true;
    }
    catch (const mqtt::exception& e) {
        cerr << "[MQTT] Publish error: " << e.what() << endl;
        return false;
    }
}

bool MQTT_Client::subscribe(const string& topic, int qos) 
{
    if (!connect_status) return false;
    try {
        client.subscribe(topic, qos)->wait();
        cout << "[MQTT] Subscribed to: " << topic << endl;
        return true;
    }
    catch (const mqtt::exception& e) {
        cerr << "[MQTT] Subscribe error: " << e.what() << endl;
        return false;
    }
}

void MQTT_Client::connected(const string& cause) 
{
    cout << "[MQTT] Connection success. Cause: " << cause << endl;
}

void MQTT_Client::connection_lost(const string& cause) 
{
    cout << "[MQTT] Connection lost. Cause: " << cause << endl;
    connect_status = false;
}

void MQTT_Client::message_arrived(mqtt::const_message_ptr msg) 
{
    // cout << "[MQTT] Message arrived. Topic: " << msg->get_topic()
            //   << " | Payload: " << msg->to_string() << endl;
    if (onMessage_) {
        onMessage_(msg->get_topic(), msg->to_string());
    }
}

void MQTT_Client::setOnMessage(MessageHandler handler) 
{
    onMessage_ = handler; // declare slot
}