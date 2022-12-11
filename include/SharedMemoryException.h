/* IPC - Interprocess Communications
 *
 * SharedMemoryException.h - Exception definition for shared memory wrapper
 *
 * Copyright (C) 2022  Juan G. Lalinde-Pulido <jlalinde@eafit.edu.co>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef __SHAREDMEMORYEXCEPTION_H_
#define __SHAREDMEMORYEXCEPTION_H_

#ifndef __EXCEPTION__
#include <exception>
#define __EXCEPTION__
#endif

#ifndef __STRING__
#include <string>
#define __STRING__
#endif

using namespace std;

class SharedMemoryException : public exception {

	string message;
public:
	SharedMemoryException(const string &m) throw() {
		message = m;
	}

	virtual ~SharedMemoryException() throw() {};
	
	const char *what() const throw() {
		return message.c_str();
	}
	
};

#endif /*__SHAREDMEMORYEXCEPTION_H_*/
