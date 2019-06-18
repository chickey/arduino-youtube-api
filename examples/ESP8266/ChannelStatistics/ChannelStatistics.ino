/*******************************************************************
 *  Read YouTube Channel statistics from the YouTube API           *
 *                                                                 *
 *  By Brian Lough                                                 *
 *  https://www.youtube.com/channel/UCezJOfu7OtqGzd5xrP3q6WA       *
 *******************************************************************/

#include <YoutubeApi.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <ArduinoJson.h> // This Sketch doesn't technically need this, but the library does so it must be installed.

//------- Replace the following! ------
char ssid[] = "xxx";       // your network SSID (name)
char password[] = "yyyy";  // your network key
#define API_KEY "zzzz"  // your google apps API Token
#define CHANNEL_ID "UCezJOfu7OtqGzd5xrP3q6WA" // makes up the url of channel

WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

unsigned long api_mtbs = 600000; //mean time between api requests
unsigned long api_lasttime = 600001;   //last time api request has been done

long subs = 0;

String channelstatus;

void setup() {

  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  client.setInsecure();
}

void loop() {

  if (millis() - api_lasttime > api_mtbs)  {
    if(api.getChannelStatistics(CHANNEL_ID))
    {
      Serial.println("---------Stats---------");
      Serial.print("Subscriber Count: ");
      Serial.println(api.channelStats.subscriberCount);
      Serial.print("View Count: ");
      Serial.println(api.channelStats.viewCount);
      Serial.print("Comment Count: ");
      Serial.println(api.channelStats.commentCount);
      Serial.print("Video Count: ");
      Serial.println(api.channelStats.videoCount);
      // Probably not needed :)
      //Serial.print("hiddenSubscriberCount: ");
      //Serial.println(api.channelStats.hiddenSubscriberCount);
      Serial.println("------------------------");

    }
	/*Serial.println("Checking Live Status");
	//This has a cost of 100 for every call so commented out by default
	if(api.getChannelLive(CHANNEL_ID))
    {
      Serial.println("---------Live---------");
      Serial.print("Live Status: ");
      if (api.channelStats.channellive) {
        channelstatus="Live";
      } else channelstatus="Offline";
      Serial.println(channelstatus);      
    }*/
    api_lasttime = millis();
  }
}
