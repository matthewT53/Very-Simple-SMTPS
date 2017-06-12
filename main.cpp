/*
	Example of how to this SMTP library to send attachments.
*/

#include <iostream>
#include "email.h"

using namespace std;

int main(void)
{
	Email e;
	int curlError = 0;
	// e.dump();

	e.setTo("matthewta45@gmail.com");
	e.setFrom("LiverpoolTest45@gmail.com");
	e.setSubject("Test Email");
	e.setCc("");
	e.setBody("Do not reply to this email");

	e.setSMTP_host("smtps://smtp.gmail.com:465");
	e.setSMTP_username("a.taylor9358@gmail.com");
	e.setSMTP_password("adamflag543");

	e.addAttachment("/mnt/d/Projects/Very-Simple-SMTPS/email.h");

	e.constructEmail();
	e.dump();

	curlError = e.send();

	if (curlError){
		cout << "Error sending email!" << endl;
	}

	else{
		cout << "Email sent successfully!" << endl;
	}

	return 0;
}

