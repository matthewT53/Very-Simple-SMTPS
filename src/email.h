/*
	Code to send an email with attachments using LIBCURL
	Supports SMTPS only.
	Made by: Matthew Ta
*/

#ifndef EMAIL_H
#define EMAIL_H

#include <vector>
#include <string>

#include "curl/curl.h"

// Enable one of the below depending on your operating system
// #define WINDOWS_H
#define LINUX_H

class Email
{
public:
	// default constructor
	Email();

	// sets who the email is going to
	void setTo(const std::string to);

	// sets who the email came from
	void setFrom(const std::string from);

	// sets the cc
	void setCc(const std::string to);

	// sets the subject of the email
	void setSubject(const std::string subject);

	// set the body of the email
	void setBody(const std::string body);

	// sets the smtp username 
	void setSMTP_username(const std::string username);

	// sets the smtp password 
	void setSMTP_password(const std::string password);

	// sets the SMTP HOST
	void setSMTP_host(const std::string host);

	// adds a binary attachment to the email
	void addAttachment(const std::string filepath);

	// removes an attachment from the email (Not implemented yet)
	void removeAttachment(const std::string filepath);

	// removes all attachments
	void removeAllAttachments();

	// contructs the final email
	void constructEmail();

	// clears the contents of the email
	void clearEmailContents();

	
	int send() const; 

	void dump() const;

private:
	// smtp information
	std::string smtp_user;
	std::string smtp_password;
	std::string smtp_host;

	// email data
	std::string to;
	std::string from;
	std::string cc;
	std::string subject;
	std::string body;

	// vector which stores the email data
	std::vector<std::string> email_contents;
	std::vector<std::string> attachments;

	// length of the above vector
	int numberOfLines;
};

#endif


