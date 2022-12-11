/* IPC - Interprocess Communications
 *
 * ToString.h - Utility template to convert any type to string
 *              The type T must overload the << operator
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

#ifndef __TOSTRING_H_
#define __TOSTRING_H_

#ifndef __STRING_
#include <string>
#define __STRING_
#endif

#ifndef __SSTREAM_
#include <sstream>
#define __SSTREAM_
#endif

using std::string;
using std::stringstream;

template<class T>
const string ToString(const T &val) {

	stringstream out;
	out << val;
	return out.str();
}

#endif /* TOSTRING_H_ */
