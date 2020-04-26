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

namespace smtp 
{

class Email
{
public:
	// default constructor
	Email();

	void setTo(const std::string p_to);
	void setFrom(const std::string p_from);
	void setCc(const std::string p_to);
	void setSubject(const std::string p_subject);
	void setBody(const std::string p_body);
	void setSmtpUsername(const std::string p_username);
	void setSmtpPassword(const std::string p_password);
	void setSmtpHost(const std::string p_host);
	void addAttachment(const std::string p_filepath);
	void removeAttachment(const std::string p_filepath);
	void removeAllAttachments();
	void clear();

	int send() const; 

	friend std::ostream &operator<<(std::ostream &p_out, const Email &p_email);

private:
	// smtp information
	std::string m_smtp_user;
	std::string m_smtp_password;
	std::string m_smtp_host;

	// email data
	std::string m_to;
	std::string m_from;
	std::string m_cc;
	std::string m_subject;
	std::string m_body;

	// vector which stores the email data
	std::vector<std::string> m_attachments;

	// contructs the final email
	void constructEmail();
};

}

#endif


