#include <napi.h>
#include <iostream>

#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "netapi32.lib")

#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <lm.h>

/*
 -1 - not found
  0 - guest
  1 - user
  2 - admin
*/
Napi::Number CheckPrivileges(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  std::u16string temp = info[0].As<Napi::String>().Utf16Value();
  std::wstring wideTemp = std::wstring(temp.begin(), temp.end());

  NET_API_STATUS nStatus;
  LPCWSTR username = wideTemp.c_str();
  DWORD level = 1;
  LPUSER_INFO_1 pBuf = NULL;

  nStatus = NetUserGetInfo(NULL, username, level, (LPBYTE *)&pBuf);

  int userPriv = -1;
  if (pBuf != NULL)
  {
    userPriv = (int)pBuf->usri1_priv;
    NetApiBufferFree(pBuf);
  }
  Napi::Number result = Napi::Number::New(env, userPriv);

  return result;
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "checkPrivileges"), Napi::Function::New(env, CheckPrivileges));
  return exports;
}

NODE_API_MODULE(addon, Init)