#pragma once

/** loads the internationalization library */
void loadInternationalizationLibrary();

/** unloads the internationalization library */
void unloadInternationalizationLibrary();

/* Function pointers for multi language support */
char * gettext(char * __msgid);
char * textdomain(const char * __domainname);
char * bindtextdomain(const char * __domainname, const char * __dirname);

/* convenience function for string translation */
#define _(string) gettext(string)
