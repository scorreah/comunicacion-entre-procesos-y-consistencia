/* IPC - Interprocess Communications
 *
 * Trace.h - utility function for debugging
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

#ifndef __TRACE_H_
#define __TRACE_H_

#ifndef __STRING_
#include <string>
#define __STRING_
#endif

#ifdef __TRACE__

#ifndef __IOSTREAM_
#include <iostream>
#define __IOSTREAM_
#endif

#ifndef __UNISTD_H
#include <unistd.h>
#define __UNISTD_H
#endif

using std::cerr;
using std::endl;

static inline void __TraceFn__(const string &msg, const string &file, const int line) {
	  cerr << getpid() << "-" << file << "(" << line << "):" << msg << endl;
};


#define Trace(x) __TraceFn__(x,__FILE__,__LINE__)

#else

#define Trace(x)

#endif

#endif /* __TRACE_H_ */
