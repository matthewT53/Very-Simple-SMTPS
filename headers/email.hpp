/*
	Code to send an email with attachments using LIBCURL
	Supports SMTPS only.
*/

#ifndef SMTP_EMAIL_H
#define SMTP_EMAIL_H

#include <vector>
#include <string>
#include <memory>

#include "mime.hpp"
#include "secure_strings.hpp"

namespace smtp 
{

class Email
{
	public:
		// default constructor
		Email();

		void setTo( const std::string &p_to );
		void setFrom( const std::string &p_from );
		void setCc( const std::string &p_to );
		void setSubject( const std::string &p_subject );
		void setBody( const std::string &p_body );
		void addAttachment( const std::string &p_attachment );
		void removeAttachment( const std::string &p_attachment );
		void setMimeDocument( std::unique_ptr<smtp::IMime> &p_mime );

		void setSmtpUsername( const std::string &p_username );
		void setSmtpPassword( const std::string &p_password );
		void setSmtpHost( const std::string &p_host );

		void clear();
		void send() const;

	private:
		// smtp information
		smtp::secure_string m_smtp_user;
		smtp::secure_string m_smtp_password;
		smtp::secure_string m_smtp_host;

		// email data
		std::string m_to;
		std::string m_from;
		std::string m_cc;
		std::string m_subject;
		std::string m_body;

		// Mime document with possible attachments
		std::unique_ptr<smtp::IMime> m_mime;

		std::vector<std::string> build() const;
		static std::size_t payloadCallback(void *ptr, size_t size, size_t nmemb, void *userp);

	friend std::ostream &operator<<(std::ostream &p_out, Email &p_email)
	{
		const std::vector<std::string> &email_contents = p_email.build();
		std::string result;
		for (const auto &line : email_contents) result += line;
		return p_out << result;
	}
};

}

#endif


