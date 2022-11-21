/**
* @file FileRW.cpp in InsCubeBsp
* @author linfe
* @comment
* Create on 2022/11/20 12:10
* @version 1.0
**/


#include "FileRW.h"

bool ReaderBase::isOk() const {
    return ok;
}

LogReader::LogReader(const std::string &filename):filename(std::move(filename)),ReaderBase() {
}

bool LogReader::ReadNext(DataType::recorder_data_t &data) {
    return false;
}
LogReader::~LogReader() {

}
