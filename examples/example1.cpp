/*
        Example of how to this SMTP library to send attachments.
*/

#include "email.hpp"
#include <iostream>

using namespace smtp;

int main(void) {
  Email e;

  e.setTo("blurred@gmail.com");
  e.setFrom("Big boss");
  e.setSubject("Testing sending attachments");
  e.setBody("Hey listen bro here are some attachments for you to play with!");

  // NOTE: Full path to the file must be provided.
  e.addAttachment("/mnt/d/Projects/end2end/smtp/attachments/image.png");
  e.addAttachment("/mnt/d/Projects/end2end/smtp/mime.cpp");

  e.setSmtpUsername("some_email@gmail.com");
  e.setSmtpPassword("Password");
  e.setSmtpHost("smtps://smtp.gmail.com:465");

  e.send();

  return 0;
}
