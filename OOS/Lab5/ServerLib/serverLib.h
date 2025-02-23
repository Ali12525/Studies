#ifndef SERVERLIB_H
#define SERVERLIB_H

#ifdef SERVERLIB_EXPORTS
#define SERVERLIB_API __declspec(dllexport)
#else
#define SERVERLIB_API __declspec(dllimport)
#endif

#define CHECK_INTERVAL 500

SERVERLIB_API void StartServer();
#endif