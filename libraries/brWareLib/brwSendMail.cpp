

#include "brwSendMail.h"


void brwSendMail::mailSend (){

	smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);	

	smtpData.setSender("Playade02", emailSenderAccount);

	smtpData.setPriority("High");

	smtpData.setSubject(emailSubject);

	smtpData.setMessage("Ola! - Envio de email - Playade02", false); // true para texto em HTML

	smtpData.addRecipient(emailRecipient); // ea

	smtpData.setSendCallback(sendCallback);

	if (!MailClient.sendMail(smtpData))
  		Serial.println("Error sending Email, " + MailClient.smtpErrorReason());

}



void brwSendMail::sendCallback(SendStatus msg) {
  // Print the current status
  Serial.println(msg.info());

  // Do something when complete
  if (msg.success()) {
    Serial.println("----------------");
  }
}


bool brwSendMail::setEmailSenderAccount (char* var) {

	if (sizeof (var) > 127)
		return (false);

	strncpy (emailSenderAccount, var, sizeof (emailSenderAccount));
	return (true);

}



bool brwSendMail::setEmailSenderPassword (char* var) {

	if (sizeof (var) > 64)
		return (false);

	strncpy (emailSenderPassword, var, sizeof (emailSenderPassword));
	return (true);

}


bool brwSendMail::setEmailSenderRecipient (char* var) {

	if (sizeof (var) > 128)
		return (false);

	strncpy (emailRecipient, var, sizeof (emailRecipient));
	return (true);

}


bool brwSendMail::setEmailSenderServer (char* var) {

	if (sizeof (var) > 64)
		return (false);

	strncpy (smtpServer, var, sizeof (smtpServer));
	return (true);

}


bool brwSendMail::setEmailSenderServerPort (int var) {

	if (var > 1024)
		return (false);

	smtpServerPort = var;
	return (true);	

} 

bool brwSendMail::setEmailSubject (char* var) {

	if (sizeof (var) > 254)
		return (false);

	strncpy (emailSubject, var, sizeof (emailSubject));
	return (true);


}


bool brwSendMail::setEmailMessage (char* var) {

	if (sizeof (var) > 512)
		return (false);

	strncpy (emailMessage, var, sizeof (emailMessage));
	return (true);


}


char* brwSendMail::getEmailSenderAccount () {
 
	return (emailSenderAccount);

}

char* brwSendMail::getEmailSenderPassword () {

	return (emailSenderPassword);

}


char* brwSendMail::getEmailSenderRecipient () {

	return (emailRecipient);

}

char* brwSendMail::getEmailSenderServer () {

	return (smtpServer);

}

int brwSendMail::getEmailSenderServerPort () {

	return (smtpServerPort);

}

char* brwSendMail::getEmailSubject () {

	return (emailSubject);

}


char* brwSendMail::getEmailMessage () {

	return (emailMessage);

}

