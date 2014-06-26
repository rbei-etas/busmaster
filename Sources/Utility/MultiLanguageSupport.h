/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
