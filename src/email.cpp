/*
	Implementation of the SMTPS email class
	Written by Matthew Ta
*/

#include <iostream>
#include <ctime>
#include <string.h>

// #include "stdafx.h"
#include "email.h"

Email::Email()
{
	// empty constructor
}

void Email::setTo(const std::string to)
{
	this->to = to;
}

void Email::setFrom(const std::string from)
{
	this->from = from;
}

void Email::setCc(const std::string cc)
{
	this->cc = cc;
}

void Email::setSubject(const std::string subject)
{
	this->subject = subject;
}

void Email::setBody(const std::string body)
{
	this->body = body;
}

void Email::setSMTP_username(const std::string user)
{
	this->smtp_user = user;
}

void Email::setSMTP_password(const std::string pass)
{
	this->smtp_password = pass;
}

void Email::setSMTP_host(const std::string hostname)
{
	this->smtp_host = hostname;
}

void Email::addAttachment(const std::string file_path)
{
	
}

void Email::constructEmail(void)
{
	
}

/*
	This function was taken and modified from:
	https://curl.haxx.se/libcurl/c/smtp-ssl.html
*/
int Email::send(void) const
{
	//TODO:
	return 0;
}

void Email::removeAllAttachments()
{
	this->attachments.clear();
}

void Email::clearEmailContents()
{
	this->email_contents.clear();
	this->attachments.clear();
}

void Email::dump(void) const
{
	int i = 0;

	std::cout << "Email contents: " << std::endl;
	for (i = 0; i < static_cast<int>( this->email_contents.size() ); i++) {
		std::cout << this->email_contents[i];
		if (i == 20) {
			break;
		}
	}

	std::cout << "\n\nEmail attachments: " << std::endl;
	for (i = 0; i < static_cast<int>( this->attachments.size() ); i++) {
		std::cout << this->attachments[i];
		if (i == 5) {
			break;
		}
	}
}