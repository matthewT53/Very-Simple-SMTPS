/*
	Implementation of the SMTPS email class
	Written by Matthew Ta
*/

#include <iostream>
#include <ctime>
#include <string.h>

// #include "stdafx.h"
#include "email.h"

smtp::Email::Email()
{
	// empty constructor
}

void smtp::Email::setTo(const std::string p_to)
{
	this->m_to = p_to;
}

void smtp::Email::setFrom(const std::string p_from)
{
	this->m_from = p_from;
}

void smtp::Email::setCc(const std::string p_cc)
{
	this->m_cc = p_cc;
}

void smtp::Email::setSubject(const std::string p_subject)
{
	this->m_subject = p_subject;
}

void smtp::Email::setBody(const std::string p_body)
{
	this->m_body = p_body;
}

void smtp::Email::setSmtpUsername(const std::string p_user)
{
	this->m_smtp_user = p_user;
}

void smtp::Email::setSmtpPassword(const std::string p_password)
{
	this->m_smtp_password = p_password;
}

void smtp::Email::setSmtpHost(const std::string p_hostname)
{
	this->m_smtp_host = p_hostname;
}

void smtp::Email::addAttachment(const std::string p_file_path)
{
}

/*
	This function was taken and modified from:
	https://curl.haxx.se/libcurl/c/smtp-ssl.html
*/
int smtp::Email::send(void) const
{
	//TODO:
	return 0;
}

void smtp::Email::removeAllAttachments()
{
	this->m_attachments.clear();
}