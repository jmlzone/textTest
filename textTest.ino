/*----------------------------------------------------------------------
Example/test to send text/email through gmail.

Although there are many references listed below.  The example below is 
very simple and can easlily be adapted for a device that sends email or
text/SMS through gmail.

Find this source at:
  https://github.com/jmlzone/textTest

When adapting this code, you can omit all the serial prints.  The serial prints
are there to help you see what is happening and debug if needed.

If you are looking for an IOT framework that makes many IOT foundation items
easy to use such as:
* Automatic Accesspoint/captive server and client switchover.
* LittleFS upload/download
* Authentication and password file
* HTTP/HTTPS servers
* Cron (running things on a calendar/time)
* sending email/sms
* etc.
Please see:
  https://github.com/jmlzone/iotfw

Note: The 'ESP_Mail_Client' library was tried without success,
      too many dependancies and too heavy.  It tried to take over time 
      and file system and also messed up the baud rate.  
      Too hard to figure out.

This example shows just how simple and easy it is to send a email/text.

References:
1) This reference shows just how easy it is to do a ssl session although
    it had nothing to do with sending mail.  This example is 90% of what 
    I used.
http://www.iotsharing.com/2017/08/how-to-use-https-in-arduino-esp32.html

2) The baics of doing a simple exchange for smtp:
https://learn.microsoft.com/en-us/exchange/mail-flow/test-smtp-telnet?view=exchserver-2019

3) The basics of sending gmail from a device:
https://support.google.com/a/answer/176600?hl=en

4) SMTP with authentication:
https://www.samlogic.net/articles/smtp-commands-reference.htm

5) Authentication Details:
https://www.ietf.org/rfc/rfc4954.txt

6) Finally getting past the dreded google 555 error due to non standard
   syntax checking done by google.
https://magento.stackexchange.com/questions/269620/how-to-fix-5-5-2-syntax-error-m189sm22281031qkf-2-gsmtp-in-magento-2-email
----------------------------------------------------------------------*/

#include <WiFiClientSecure.h>

const char* ssid     = "wifi_ssid";     // your network SSID 
const char* password = "wifi_pasword"; // your network password
const char* server = "smtp.gmail.com";  // Server URL
/*----------------------------------------------------------------------
  On a linux machine: Get your email username like this:
 echo "my_username@gmail.com" | base64

  There are also websites which will do base 64 encoding for you
----------------------------------------------------------------------*/
const char* emailUserB64 ="bXlfdXNlcm5hbWVAZ21haWwuY29tCg=="

/*----------------------------------------------------------------------
  On a linux machine: Get your email password like this:
 echo "this_is_my_Password" | base64

  There are also websites which will do base 64 encoding for you
----------------------------------------------------------------------*/
const char* emailPasswordB64 ="dGhpc19pc19teV9QYXNzd29yZAo="

/*----------------------------------------------------------------------
  Google has some non RFC * rules for the from address you may get a 
  syntax error from gmail if you dont specify the from properly.
  use something like "printable_name emailAddress@domain"
  ----------------------------------------------------------------------*/
const char* sender = "my_name my_name@gmail.com";

/*----------------------------------------------------------------------
  Google has some non RFC * rules for the to address you may get a 
  syntax error from gmail if you dont specify the from properly.
  use something like "phone_number@cellprovider.net"
  ----------------------------------------------------------------------*/
const char * to = "8005551212@txt.att.net";

/*----------------------------------------------------------------------
  The subject and message can be what ever you want
  ----------------------------------------------------------------------*/
const char * subject = "gmail test";
const char * message = "this is a test of sending an email to sms";
/*----------------------------------------------------------------------
  use: 
openssl s_client -showcerts -connect smtp.gmail.com:465 </dev/null 
to get this certificate 
----------------------------------------------------------------------*/
const char gmailCert[] PROGMEM = R"rawliteral(
-----BEGIN CERTIFICATE-----
MIIFYjCCBEqgAwIBAgIQd70NbNs2+RrqIQ/E8FjTDTANBgkqhkiG9w0BAQsFADBX
MQswCQYDVQQGEwJCRTEZMBcGA1UEChMQR2xvYmFsU2lnbiBudi1zYTEQMA4GA1UE
CxMHUm9vdCBDQTEbMBkGA1UEAxMSR2xvYmFsU2lnbiBSb290IENBMB4XDTIwMDYx
OTAwMDA0MloXDTI4MDEyODAwMDA0MlowRzELMAkGA1UEBhMCVVMxIjAgBgNVBAoT
GUdvb2dsZSBUcnVzdCBTZXJ2aWNlcyBMTEMxFDASBgNVBAMTC0dUUyBSb290IFIx
MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAthECix7joXebO9y/lD63
ladAPKH9gvl9MgaCcfb2jH/76Nu8ai6Xl6OMS/kr9rH5zoQdsfnFl97vufKj6bwS
iV6nqlKr+CMny6SxnGPb15l+8Ape62im9MZaRw1NEDPjTrETo8gYbEvs/AmQ351k
KSUjB6G00j0uYODP0gmHu81I8E3CwnqIiru6z1kZ1q+PsAewnjHxgsHA3y6mbWwZ
DrXYfiYaRQM9sHmklCitD38m5agI/pboPGiUU+6DOogrFZYJsuB6jC511pzrp1Zk
j5ZPaK49l8KEj8C8QMALXL32h7M1bKwYUH+E4EzNktMg6TO8UpmvMrUpsyUqtEj5
cuHKZPfmghCN6J3Cioj6OGaK/GP5Afl4/Xtcd/p2h/rs37EOeZVXtL0m79YB0esW
CruOC7XFxYpVq9Os6pFLKcwZpDIlTirxZUTQAs6qzkm06p98g7BAe+dDq6dso499
iYH6TKX/1Y7DzkvgtdizjkXPdsDtQCv9Uw+wp9U7DbGKogPeMa3Md+pvez7W35Ei
Eua++tgy/BBjFFFy3l3WFpO9KWgz7zpm7AeKJt8T11dleCfeXkkUAKIAf5qoIbap
sZWwpbkNFhHax2xIPEDgfg1azVY80ZcFuctL7TlLnMQ/0lUTbiSw1nH69MG6zO0b
9f6BQdgAmD06yK56mDcYBZUCAwEAAaOCATgwggE0MA4GA1UdDwEB/wQEAwIBhjAP
BgNVHRMBAf8EBTADAQH/MB0GA1UdDgQWBBTkrysmcRorSCeFL1JmLO/wiRNxPjAf
BgNVHSMEGDAWgBRge2YaRQ2XyolQL30EzTSo//z9SzBgBggrBgEFBQcBAQRUMFIw
JQYIKwYBBQUHMAGGGWh0dHA6Ly9vY3NwLnBraS5nb29nL2dzcjEwKQYIKwYBBQUH
MAKGHWh0dHA6Ly9wa2kuZ29vZy9nc3IxL2dzcjEuY3J0MDIGA1UdHwQrMCkwJ6Al
oCOGIWh0dHA6Ly9jcmwucGtpLmdvb2cvZ3NyMS9nc3IxLmNybDA7BgNVHSAENDAy
MAgGBmeBDAECATAIBgZngQwBAgIwDQYLKwYBBAHWeQIFAwIwDQYLKwYBBAHWeQIF
AwMwDQYJKoZIhvcNAQELBQADggEBADSkHrEoo9C0dhemMXoh6dFSPsjbdBZBiLg9
NR3t5P+T4Vxfq7vqfM/b5A3Ri1fyJm9bvhdGaJQ3b2t6yMAYN/olUazsaL+yyEn9
WprKASOshIArAoyZl+tJaox118fessmXn1hIVw41oeQa1v1vg4Fv74zPl6/AhSrw
9U5pCZEt4Wi4wStz6dTZ/CLANx8LZh1J7QJVj2fhMtfTJr9w4z30Z209fOU0iOMy
+qduBmpvvYuR7hZL6Dupszfnw0Skfths18dG9ZKb59UhvmaSGZRVbNQpsg3BZlvi
d0lIKO2d1xozclOzgjXPYovJJIultzkMu34qQb9Sz/yilrbCgj8=
-----END CERTIFICATE-----
)rawliteral";


WiFiClientSecure client;

void printClientLines(int ms, char * exp=NULL, int timeout=0) {
  long currentTime, giveupTime;
  bool gotExp = false;
  int wt;
  char recstr[20];
  int ptr = 0;
  int l;
  if(exp==NULL) {
    gotExp = true;
    wt=ms;
    l=0;
  } else {
    wt = timeout;
    l = strlen(exp);
  }
  currentTime=millis();
  giveupTime=currentTime + wt;
  while (client.available() || (currentTime < giveupTime)) {
    /* if data is available then receive and print to Terminal */
    if(client.available()) {
      char c = client.read();
      Serial.write(c);
      if(!gotExp) {
	      if(ptr<=l) {
	        recstr[ptr]=c;
	        if(ptr==l) {
	          if(!strncmp(exp,recstr,l)) {
	            gotExp = true;
	            Serial.print("Got exp ");
	            Serial.println(exp);
	            wt=ms;
	          }
	        }
	      }
	     if(c=='\n' || c=='\r') {
	       ptr=0;
	     } else if(ptr<19) {ptr++;}
        }
        currentTime=millis();
        giveupTime=currentTime + wt;
      } else {
        delay(10);
        currentTime=millis();
      }
  }
}

void setup() {
    Serial.begin(115200);

    WiFi.begin(ssid, password);

    /* waiting for WiFi connect */
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }

    Serial.print("Connected to ");
    Serial.println(ssid);

    /* set SSL/TLS certificate */
    client.setCACert(gmailCert);

    Serial.println("Connect to server via port 465");
    if (!client.connect(server, 465)){
        Serial.println("Connection failed!");
    } else {
        Serial.println("Connected to server!\n Starting Dialog");
        /* create email dialog */
        Serial.println("EHLO mydomain.net");
        client.println("EHLO mydomain.net");
	printClientLines(100,"220",1000);
        Serial.println("AUTH LOGIN");
        client.println("AUTH LOGIN");
        printClientLines(100,"334 VXNlcm5hbWU6",1000);
        Serial.println("my user name (base 64 encoded)");
        client.println(emailUserB64);
	printClientLines(100,"334 UGFzc3dvcmQ6",1000);
        Serial.println("<password> (base64 encoded)");
        client.println(emailPasswordB64);
	printClientLines(100,"235",1000); // password OK
	Serial.printf("MAIL FROM: <%s>\n",sender);
	client.printf("MAIL FROM: <%s>\n", sender);
	printClientLines(100,"250",1000);
        Serial.printf("RCPT TO: <%s><%s>\n",to,to); // yes it should be duplicated like this for gmail
        client.printf("RCPT TO: <%s><%s>\n",to,to);
	printClientLines(100,"250",1000);
        Serial.println("DATA");
        client.println("DATA");
	printClientLines(100);
        Serial.printf("From: %s\n", sender); // the From: and To: are not stricly needed
        client.printf("From: %s\n", sender);
	printClientLines(100);
        Serial.printf("To: %s\n", to);
        client.printf("To: %s\n", to);
	printClientLines(100);
        Serial.printf("Subject: %s\n\n", subject); // need a blank line between the subject and body
        client.printf("Subject: %s\n\n", subject);
	printClientLines(100);
        Serial.println(message);
        client.println(message);
	printClientLines(100);
        Serial.println(".");
        client.println(".");
	printClientLines(100);
        Serial.println("QUIT");
        client.println("QUIT");
	printClientLines(100);

        /* if the server disconnected, stop the client */
        if (!client.connected()) {
	  Serial.println();
	  Serial.println("Server disconnected");
	  client.stop();
        }
    }
}

void loop() {
}
