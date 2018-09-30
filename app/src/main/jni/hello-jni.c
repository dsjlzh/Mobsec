/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <string.h>
#include <jni.h>
#include "checker.h"
#include "hot-patch.h"


JNIEXPORT jstring JNICALL
Java_com_gerald_mobsec_MainActivity_resultFromJNI( JNIEnv* env,
                                                  jobject thiz )
{
	if (!g_patched) {
		if (!patch_all()) {
            // debug msg, comment in release
            return (*env)->NewStringUTF(env, "Patch failed!");
        }
	}

    int result = 0;
	result = check_cve_2017_0589();
	if (result ==  0)
		return (*env)->NewStringUTF(env, "secure :) ");
	else if (result == 1)
		return (*env)->NewStringUTF(env, "vulnerable :( ");
	else
		return (*env)->NewStringUTF(env, "error!!! ");
}
