#include <iostream>
#include <string>
#include "name.hpp"

const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

std::string Name::incrementBase62(std::string s) {
	int i = s.size() - 1;

	while (i >= 0) {
		/* find current char pos in chars */
        	size_t pos = chars.find(s[i]);
        	if (pos == std::string::npos) {
			/* if char not found, reset to '0' */
			s[i] = '0';
			i--;
			continue;
        	}
        	if (pos == chars.size() - 1) {
            		/* if last char in base62, wrap around to '0' and carry */
            		s[i] = '0';
            		i--;
        	} else {
            		/* just increment char to next in base62 and done */
			s[i] = chars[pos + 1];
	    		return s;
		}
	}
    	/* if all digits wrapped around, add a new '0' at front */
	s = '0' + s;

	return s;
}
