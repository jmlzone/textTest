#Example/test to send text/email through gmail.

Although there are many references listed below.  The example below is 
very simple and can easlily be adapted for a device that sends email or
text/SMS through gmail.
Note: The 'ESP_Mail_Client' library was tried without success,
      too many dependancies and too heavy.  It tried to take over time 
      and file system and also messed up the baud rate.  
      Too hard to figure out.

This example shows just how simple and easy it is to send a email/text.

This is just an example I created to learn.  the learneings are imbeded in my iotfw
Internet of Things Freamwork.

#References:
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

