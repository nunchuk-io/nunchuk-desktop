#ifndef MATRIXLISTENER_H
#define MATRIXLISTENER_H

#include "matrixbrigde.h"
// event id
std::string SendFunction(const string &roomid,
                         const std::string& eventtype,
                         const std::string& eventContent,
                         bool ignore_error);

bool consume_sync_event_listener(int percent);

Q_DECLARE_METATYPE(const char*);
std::string UploadFileFunc (const std::string& file_name,
                            const std::string&  mine_type,
                            const std::string& json_info ,
                            const char* data ,
                            size_t data_length);

std::vector<unsigned char>  DownloadFileFunc  (const std::string& file_name ,
                                               const std::string& mine_type,
                                               const std::string& json_info ,
                                               const std::string& mxc_uri);

bool DownloadFileProgress(int percent);

#endif // MATRIXLISTENER_H
