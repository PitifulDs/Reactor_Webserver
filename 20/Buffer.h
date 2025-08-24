#pragma once
#include <string>
#include <iostream>

class Buffer
{ 
private:
    std::string buf_;   // 用于存放数据 
public:
    Buffer();
    ~Buffer();

    void append(const char* data, size_t size);  // 添加数据
    void erase(size_t pose, size_t nn);         // 从buf_的pos开始，删除nn个数据，pos从0开始
    size_t size();                              // 获取数据大小         
    const char* data();                         // 返回buf_的首地址
    void clear();                               // 清空buf_

};



