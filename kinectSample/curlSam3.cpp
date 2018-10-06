/***************************************************************************
*                                  _   _ ____  _
*  Project                     ___| | | |  _ \| |
*                             / __| | | | |_) | |
*                            | (__| |_| |  _ <| |___
*                             \___|\___/|_| \_\_____|
*
* Copyright (C) 1998 - 2015, Daniel Stenberg, <daniel@haxx.se>, et al.
*
* This software is licensed as described in the file COPYING, which
* you should have received as part of this distribution. The terms
* are also available at https://curl.haxx.se/docs/copyright.html.
*
* You may opt to use, copy, modify, merge, publish, distribute and/or sell
* copies of the Software, and permit persons to whom the Software is
* furnished to do so, under the terms of the COPYING file.
*
* This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
* KIND, either express or implied.
*
***************************************************************************/
/* <DESC>
* simple HTTP POST using the easy interface
* </DESC>
*/

/* curl �̿��ؼ� http�� ������ ������ �ҽ� ����(5/28) */

#include <stdio.h> 
#include <stdlib.h>
#include <curl/curl.h> 


int main(void)
{
	CURL *curl;
	CURLcode res;

	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_SSL);

	/* get a curl handle */
	curl = curl_easy_init();
	if (curl) {
		/* First set the URL that is about to receive our POST. This URL can
		just as well be a https:// URL if that is what should receive the
		data. */
		curl_easy_setopt(curl, CURLOPT_URL, "https://we-fit.co.kr:8080/joints");
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // ������ üũ������ true �� �ȵǴ� ��찡 ����.(https ���ӽÿ� �ʿ�)
		curl_easy_setopt(curl, CURLOPT_SSLVERSION, 0); // SSL ���� (https ���ӽÿ� �ʿ�)
													  
		//curl_easy_setopt(curl, CURLOPT_POST, 1); // Post Get ���� ����
		/* Now specify the POST data */
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "userId=white&backAngle=123&rightArm=123&leftArm=123&timestamp=2018-12-12 12:31:23");

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));

		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	return 0;
}

