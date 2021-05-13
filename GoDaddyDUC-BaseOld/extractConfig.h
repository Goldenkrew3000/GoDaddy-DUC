#ifndef EXTRACTCONFIG_H_INCLUDED
#define EXTRACTCONFIG_H_INCLUDED

char* extract_godaddy_domain(char* jsonString);
char* extract_godaddy_a_name(char* jsonString);
char* extract_godaddy_ttl(char* jsonString);
char* extract_godaddy_type(char* jsonString);
char* extract_api_key(char* jsonString);
char* extract_api_secret(char* jsonString);

#endif
