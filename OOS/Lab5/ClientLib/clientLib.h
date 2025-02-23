#ifndef CLIENTLIB_H
#define CLIENTLIB_H

#ifdef CLIENTLIB_EXPORTS
#define CLIENTLIB_API __declspec(dllexport)
#else
#define CLIENTLIB_API __declspec(dllimport)
#endif

CLIENTLIB_API void StartClient();
#endif