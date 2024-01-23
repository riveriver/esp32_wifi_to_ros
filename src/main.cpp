/*
 *  Code initializes and connects to a WiFi network using given SSID and password, 
 *  then publishes a "Hello World!" message to a ROS topic "chatter" at regular intervals. 
 *  Make sure to update the SSID, password, IP and server details as per your network.
 * 
 *  Create by Stan Fu on 2023/08/07
 */
#include <arduino.h>
#include "WiFi.h"
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float32.h>
IPAddress ip(192, 168, 178, 47);
IPAddress server(172, 17, 0, 2);
uint16_t serverPort = 11411;
// const char*  ssid = "FRITZ!Box 7530 JQ";
// const char*  password = "27635171366830739521";
// const char*  ssid = "GH034";
// const char*  password = "chenlaoshiniubi";
const char*  ssid = "TP-Link_21B5";
const char*  password = "26796666";
// Be polite and say hello
char hello[13] = "hello world!";
uint16_t period = 1000;
uint32_t last_time = 0;

ros::NodeHandle  nh;
double rx_temp;
// Make a chatter publisher
void ListenerCallback(const std_msgs::Float32 &rx_data){
  char rx[8];
  rx_temp = rx_data.data;
  dtostrf(rx_temp, 2, 4, rx);
  Serial.printf("rev: %s\n",rx); 
}
std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);
ros::Subscriber<std_msgs::Float32> listener("top_motor", ListenerCallback);


void setupWiFi();

void setup(){
    Serial.begin(115200);
    setupWiFi();

    nh.getHardware()->setConnection(server, serverPort);
    nh.initNode();

    // Another way to get IP
    Serial.print("ROS IP = ");
    Serial.println(nh.getHardware()->getLocalIP());

    // Start to be polite
    nh.advertise(chatter);
    nh.subscribe(listener);

}

void loop(){
  if(millis() - last_time >= period)
  {
    last_time = millis();
    if (nh.connected())
    {
      Serial.println("Connected");
      // Say hello
      str_msg.data = hello;
      chatter.publish( &str_msg );
    } else {
      Serial.println("Not Connected");
    }
  }
  nh.spinOnce();
  delay(1);
}


void setupWiFi()
{  
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) { delay(500);Serial.print("."); }
   Serial.print("SSID: ");
   Serial.println(WiFi.SSID());
   Serial.print("IP:   ");
   Serial.println(WiFi.localIP());
}

