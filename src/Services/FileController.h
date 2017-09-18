/*
 * FileController.h
 *
 *  Created on: Apr 16, 2013
 *      Author: root
 */

#ifndef FILECONTROLLER_H_
#define FILECONTROLLER_H_

#include <httpserver.hpp>

using namespace std;
using namespace httpserver;

class FileController : public http_resource {
	public:
		FileController();
		const http_response render_GET(const http_request&);
};


#endif /* FILECONTROLLER_H_ */
