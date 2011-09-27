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

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include <android/log.h>
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "zz", __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG  , "zz", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO   , "zz", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN   , "zz", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "zz", __VA_ARGS__)


void load (const char *filename, int *width, int *height) {
    lua_State *L = lua_open();
    luaL_openlibs(L);

    if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0))
        LOGD( "cannot run configuration file: %s",
              lua_tostring(L, -1));

    lua_getglobal(L, "width");
    lua_getglobal(L, "height");
    if (!lua_isnumber(L, -2))
        LOGD("`width' should be a number\n");
    if (!lua_isnumber(L, -1))
        LOGD("`height' should be a number\n");
    *width = (int)lua_tonumber(L, -2);
    *height = (int)lua_tonumber(L, -1);

    lua_close(L);
}

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/HelloJni/HelloJni.java
JNIEXPORT jstring JNICALL stringFromJNI( JNIEnv* env,
        jobject thiz )
*/
jstring
Java_com_zzfreedom_luandroid_Luandroid_loadluaconfig( JNIEnv* env,
                                                  jobject thiz, jstring path )
{
    int w,h;
    char s[256];
    const char *pathl = (*env)->GetStringUTFChars(env, path, 0);
    load (pathl, &w, &h);
    sprintf(s,"w=%d\nh=%d\n",w,h);
    return (*env)->NewStringUTF(env, s);
}
