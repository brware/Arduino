#ifndef brwSendMail_h
#define brwSendMail_h

#include "ESP32_MailClient.h"

class brwSendMail 
{

	public :

		brwSendMail  ();	

		void mailSend ();


		bool setEmailSenderAccount (char* var);
		bool setEmailSenderPassword (char* var);
		bool setEmailSenderRecipient (char* var);
		bool setEmailSenderServer (char* var);
		bool setEmailSenderServerPort (int);
		bool setEmailSubject (char* var);
		bool setEmailMessage (char* var);

		char* getEmailSenderAccount ();
		char* getEmailSenderPassword ();
		char* getEmailSenderRecipient ();
		char* getEmailSenderServer ();
		int getEmailSenderServerPort ();
		char* getEmailSubject ();
		char* getEmailMessage ();

	private :

		static void sendCallback(SendStatus info);

		SMTPData smtpData;

		char emailSenderAccount [128];   //  Email do remetente
		char emailSenderPassword [64];  //  senha do Email do remetente
		char emailRecipient [128];       //  Email do destinatario
		char smtpServer [64];           //  smtp.gmail.com
		int smtpServerPort;        		 //  465
		char emailSubject  [256];        //  Assunto
		char emailMessage  [512];        //  Menssagem

};




#endif